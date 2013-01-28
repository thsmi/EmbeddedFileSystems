/*
 * ntfsIO.c
 *
 *  Created on: 13.01.2013
 *      Author: admin
 */

#include "ntfsFiles.h"
#include "../ntfs/ntfsRecords/ntfsIndexRecord.h"
#include "../ntfs/ntfsRecords/ntfsRecord.h"
#include "../ntfs/ntfsAttributes/ntfsIndexRoot.h"
#include "../ntfs/ntfsAttributes/ntfsIndexAllocation.h"

static inline diskReturn_t ntfsListIndexEntries(
    const ntfsIndexNodeHeader_t* header, ntfsNextFileCallback_t callback, uint8_t* data)
{
  ntfsIndexNodeEntry_t* node = NULL;

  while (ntfsNextIndexNode(header,&node) == DISK_SUCCESS)
    if (callback(node->mftReference,&(node->file),data) != DISK_SUCCESS)
      return DISK_SUCCESS;

  return DISK_SUCCESS;
}


static diskReturn_t ntfsListNonResidentIndexEntries(
    const ntfsVolume_t* volume,
    const ntfsFileRecord_t* record, ntfsAttrHdr_t* header,
    diskBuffer_t* buffer,
    ntfsNextFileCallback_t callback, uint8_t* data)
{
  // Helpers, compiler should optimize them out.
  diskDevice_t* device = volume->partition->device;

  ntfsDataRun_t* dataRun = NULL;
  diskSeekMethod_t method = DISK_SEEK_FORWARD;

  ntfsDebug(L"Index stored as data run/ non resident\n");

  if (ntfsFindAttr(record, &header,NTFS_ATTR_INDEX_ALLOCATION) != DISK_SUCCESS)
    return DISK_ERROR;

  ntfsIsIndexAllocationAttr(header);

  while (ntfsNextDataRun(header,&dataRun,&method) == DISK_SUCCESS)
  {
    uint64_t pos = 0;
    uint64_t length = 0;
    uint64_t offset = 0;

    if (ntfsGetDataRun(dataRun,&length,&offset,&method) != DISK_SUCCESS)
      continue;

    //ntfsDebug(L" Datarun: %llu %llu %d \n",length,offset);

    if (length == 0)
      continue;

    diskCurrentRecord(device,&pos);
    volumeSeekCluster(volume,offset,method);

    while (length--)
    {
      ntfsIndexRecord_t* idxRecord = NULL;

      // index files are always 1 Cluster in size, which is usually 4K. Their size is defined in the MBR
      ntfsReadIndexRecord(device, buffer,&idxRecord);
      ntfsListIndexEntries(&(idxRecord->nodeInfo), callback, data);
    }

    diskSeekRecordEx(device,pos,DISK_SEEK_ABSOLUTE,0);
  }

  return DISK_SUCCESS;

}

diskReturn_t ntfsListFiles(
    const ntfsVolume_t* volume, const ntfsFileHandle_t* file, diskBuffer_t* mftBuffer,
    diskBuffer_t* idxBuffer, ntfsNextFileCallback_t callback, uint8_t* data)
{

  // Helpers, compiler should optimize them out.
  diskDevice_t* device = volume->partition->device;

  ntfsFileRecord_t* record = NULL;
  ntfsAttrHdr_t* header = NULL;
  ntfsAttrFileName_t* fileAttr;
  ntfsAttrIndexRoot_t* indexRoot;

  // FIXME: Instead of the double sector size we should compute the MFT Record size...
  uint64_t sector = NTFS_$ROOT_RECORD*2;

  // Jeder MFT Record ist mindestens 1024 Bytes groß
  // Sofern die Cluster Size größer ist zieht diese.

  // Move Read Pointer to disk's Master File Table...
  volumeSeekCluster(volume,volume->lcnMFT,DISK_SEEK_ABSOLUTE);

  if (file != NULL)
    sector = ((((uint64_t)file->mftRecord.SegmentNumberHighPart) << 32)+(file->mftRecord.SegmentNumberLowPart))*2LL;

  diskSeekRecord(device,sector);

  ntfsReadFileRecord(device, mftBuffer,&record);

  // Header IDs are in ascending order...
  if (ntfsFindAttr(record, &header,NTFS_ATTR_FILE_NAME) != DISK_SUCCESS)
    return DISK_ERROR;

  if (ntfsGetFileNameAttr(header,&fileAttr) != DISK_SUCCESS)
    return DISK_ERROR;

  // root filename is a "."
  if (file == NULL) {
    if (fileAttr->name.length != 1)
      return DISK_ERROR;

    if ((*((uint16_t*)(&(fileAttr->name.value)))) != 0x002E )
      return DISK_ERROR;
  }

  ntfsDebug(L"Parent Filename: %.*ls \n", fileAttr->name.length,  &(fileAttr->name.value));

  if (ntfsFindAttr(record,&header,NTFS_ATTR_INDEX_ROOT) != DISK_SUCCESS)
    return DISK_ERROR;

  if (ntfsGetIndexRootAttr(header,&indexRoot) != DISK_SUCCESS)
    return DISK_ERROR;

  // check if we have a data run
  if (indexRoot->nodeInfo.flags == 0x01)
    return ntfsListNonResidentIndexEntries(volume,record,header,idxBuffer,callback,data);

  ntfsDebug(L"Index stored resident\n");
  
  // ok it is stored resident...
  // ... we have to find those records...
  return ntfsListIndexEntries(&(indexRoot->nodeInfo),callback,data);
}


static diskReturn_t ntfsReadResidentData(
    const ntfsAttrHdr_t* header, diskBuffer_t* buffer2, uint64_t length, uint64_t offset)
{
  uint8_t* data = NULL;
  data = (uint8_t*) ntfsOffset(header,header->attrHeader.resident.attribute.offset);

  //TODO there is no num read feedback....
  if (buffer2->sizeInSectors < length)
    return DISK_ERROR;

  // convert length from sectors to bytes
  length *= NTFS_BYTES_PER_SECTOR;
  offset *= NTFS_BYTES_PER_SECTOR;

  // truncate length, so that only data is read...
  if (length > header->attrHeader.resident.attribute.lenght)
    length = header->attrHeader.resident.attribute.lenght;

  // check if we run out of bounds
  if (offset > length)
    return DISK_ERROR;

  // apply offset
  if (offset > 0)
  {
    data += offset;
    length -= offset;
  }

  // finally copy data into our buffer
  if (ntfsMemCpy(buffer2,data,length) != DISK_SUCCESS)
    return DISK_ERROR;

  return DISK_SUCCESS;
}


/**
 *
 *
 * @param volume
 * @param header
 * @param buffer2
 *   The buffer where to read the data
 * @param sectors[in]
 *   The number of sectors to read (in sectors)
 * @param offset2
 *   defines at which sector reading should start (in sectors)
 * @return
 */
static diskReturn_t ntfsReadNonResidentData(
    const ntfsVolume_t* volume,
    const ntfsAttrHdr_t* header,  diskBuffer_t* buffer2, uint64_t readLength, uint64_t readOffset)
{
  diskDevice_t* device = volume->partition->device;
  diskBuffer_t worker;

  /* numread:
   if (length2 > header->attrHeader.nonResident.size.real)
     length2 = header->attrHeader.nonResident.size.real;*/

   ntfsDataRun_t* dataRun = NULL;
   diskSeekMethod_t method = DISK_SEEK_FORWARD;

   if (buffer2->sizeInSectors < readLength)
     return DISK_ERROR;

   // clone the buffer. It is used to keep track of the free space
   diskInitBuffer(&worker,&buffer2->bytes[0],buffer2->sizeInSectors*NTFS_BYTES_PER_SECTOR);

   while (ntfsNextDataRun(header,&dataRun,&method) == DISK_SUCCESS)
   {
     // all dimensions are clusters...
     uint64_t pos = 0;
     uint64_t runLength = 0;
     uint64_t runOffset = 0;

     if (readLength == 0)
       break;

     if (ntfsGetDataRun(dataRun,&runLength,&runOffset,&method) != DISK_SUCCESS)
       return DISK_ERROR;

     if (runLength == 0)
       continue;

     // convert everything into sectors...
     runLength *= volume->sectorsPerCluster;
     runOffset *= volume->sectorsPerCluster;

     // If the offset is be larger than the datarun, we can safely skip this run.
     // Otherwise we would run out of bounds.
     if (readOffset >= runLength) {
       readOffset -= runLength;
       continue;
     }

     // otherwise we simply increase the datarun's offset by readoffset and decrease the size by
     // the offset
     if (readOffset) {
       runOffset += readOffset;
       runLength -= readOffset;

       // finally we can set the offset to 0 as we fully processed it.
       readOffset = 0;
     }

     // Save the current read pointer, we will restore it later
     diskCurrentRecord(device,&pos);
     diskSeekRecordEx(device, runOffset, method, volume->volumeOffset);

     // run length can't be bigger than the buffer, otherwise we would run out of bounds.
     if (runLength > readLength)
       runLength = readLength;

     diskReturn_t rv = diskReadRecord(device,&worker,runLength);

     diskSeekRecordEx(device,pos,DISK_SEEK_ABSOLUTE,0);

     if ( rv != DISK_SUCCESS)
       return DISK_ERROR;

     readLength -= runLength;

     // shrink our worker, as it represents only the free bytes
     worker.bytes += runLength;
     worker.sizeInSectors -= runLength;
   }

   // TODO return num read...

   return DISK_SUCCESS;
}

diskReturn_t ntfsReadFile(
    const ntfsVolume_t* volume, const ntfsFileHandle_t* file,  diskBuffer_t* mftBuffer,
    diskBuffer_t* fileBuffer, uint64_t length, uint64_t offset)
{
  // Helpers, compiler should optimize them out.
  diskDevice_t* device = volume->partition->device;

  uint64_t sectors = 0;
  ntfsFileRecord_t* record = NULL;
  ntfsAttrHdr_t* header = NULL;

  if(file == NULL)
    return DISK_ERROR;
  
  // Move Read Pointer to disk's Master File Table...
  volumeSeekCluster(volume,volume->lcnMFT,DISK_SEEK_ABSOLUTE);

  sectors = ((((uint64_t)file->mftRecord.SegmentNumberHighPart) << 32)+(file->mftRecord.SegmentNumberLowPart))*NTFS_SECTORS_PER_FILERECORD;
  diskSeekRecord(device,sectors);

  if (ntfsReadFileRecord(device, mftBuffer,&record) != DISK_SUCCESS)
    return DISK_ERROR;

  // NTFS supports alternative data stream, this means there might be more than one $data entry...
  // ... alternative Data streams are named, while the "main" entry is usually unnamed.
  while (ntfsFindAttr(record, &header,NTFS_ATTR_DATA) == DISK_SUCCESS)
  {
    if (header->name.lenght != 0)
      continue;

    // ok we have an unnamed data attribute, so this is the main entry...
    // ... it might be resident or non resident.

    // TODO implement bytes read...
    // TODO we should test if bit 1 is set as the other bits are currently reserved...
    if ( !(header->isNonResident) )
      return ntfsReadResidentData(header,fileBuffer,length,offset);

    return ntfsReadNonResidentData(volume, header, fileBuffer, length, offset);
  }

  // No valid entry found...
  return DISK_ERROR;
}
