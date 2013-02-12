/*
 * The content of this file is licensed. You may obtain a copy of
 * the license at https://github.com/thsmi/EmbeddedFileSystems/ or
 * request it via email from the author.
 *
 * Do not remove or change this comment.
 *
 * The initial author of the code is:
 *   Thomas Schmid <schmid-thomas@gmx.net>
 */

#include "ntfsFiles.h"
#include "../ntfsRecords/ntfsIndexRecord.h"
#include "../ntfsRecords/ntfsRecord.h"
#include "../ntfsAttributes/ntfsIndexRoot.h"
#include "../ntfsAttributes/ntfsIndexAllocation.h"


static diskReturn_t ntfsListIndexEntries(
    const ntfsIndexNodeHeader_t* header, ntfsNextFileCallback_t callback, uint8_t* data)
{
  ntfsIndexNodeEntry_t* node = NULL;

  while (ntfsNextIndexNode(header,&node) == DISK_SUCCESS)
    if (callback(node->mftReference,&(node->file),data) == DISK_SUCCESS)
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
  const diskDevice_t* device = volume->device;

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
  const diskDevice_t* device = volume->device;

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
    ntfsGetFileReferenceOffset(&(file->mftRecord),&sector);

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
    const ntfsAttrHdr_t* header, diskBuffer_t* buffer2, uint64_t length, uint64_t offset, uint64_t* slack)
{
  uint8_t* data = NULL;
  data = (uint8_t*) ntfsOffset(header,header->attrHeader.resident.attribute.offset);

  //TODO there is no num read feedback....
  if (buffer2->sizeInSectors < length)
    return DISK_ERROR;

  // convert length from sectors to bytes
  length *= NTFS_BYTES_PER_SECTOR;
  offset *= NTFS_BYTES_PER_SECTOR;

  if(slack)
    (*slack) = length + offset;

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

  (*slack) -= header->attrHeader.resident.attribute.lenght;

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
 * @param slack [in,out]
 *   tracks the slack while reading the file. This means how many bytes within the buffer were left unused.
 *   A slack bigger than 0 indicates you did not need the full buffer and most likely reached the end of file.
 * @return
 */

/* TODO a 64bit file length and offset does not make any sense here especially for an embedded system.
 * NTFS's physical maximal file length limit is 16 exabytes = 64bit. But windows 7 supports only 16TB.
 * An windows 8 (((2^32)-1) * 64K per Cluster) = 256 TB. We do support in worst case with 512B Sectors
 * 64bit+8bit and with 4K Sectors 64bit+32bit, which is way more than the physical limits...
 *
 * a readlength of 32bit (equals with 512 byte sectors 1024TB) should be more than enough
 */

static diskReturn_t ntfsReadNonResidentData(
    const ntfsVolume_t* volume,
    const ntfsAttrHdr_t* header,  diskBuffer_t* buffer2, uint64_t readLength, uint64_t readOffset, uint64_t* slack)
{
  const diskDevice_t* device = volume->device;
  diskBuffer_t worker;

   ntfsDataRun_t* dataRun = NULL;
   diskSeekMethod_t method = DISK_SEEK_FORWARD;

   // check for sufficient buffers
   if (buffer2->sizeInSectors < readLength)
     return DISK_ERROR;

   // calculate the slack, we can do this a priori with the values stored in the MFT.
   // if the MFT would be inconsistent we are lost anyhow.

   if (slack)
   {
     (*slack) = (readOffset + readLength) * NTFS_BYTES_PER_SECTOR;

     if (*slack > header->attrHeader.nonResident.size.real)
       (*slack) -= header->attrHeader.nonResident.size.real;
     else
       (*slack) = 0;
   }

   // we can skip right here if the offset is bigger than the file
   if (readOffset * NTFS_BYTES_PER_SECTOR > header->attrHeader.nonResident.size.real)
     return DISK_SUCCESS;

   // We create a temporary worker from our buffer. We crop the length to readLength...
   // ... because it would be a fatal error to read more data than specified in readLength....
   // ... readLength is guaranteed to be smaller than the buffer size.
   diskInitBuffer(&worker,&buffer2->bytes[0],readLength *NTFS_BYTES_PER_SECTOR);

   while (ntfsNextDataRun(header,&dataRun,&method) == DISK_SUCCESS)
   {
     // all dimensions are clusters...
     uint64_t pos = 0;
     uint64_t runLength = 0;
     uint64_t runOffset = 0;

     if (worker.sizeInSectors == 0)
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

     // run length can't be bigger than the worker, otherwise we would run out of bounds.
     if (runLength > worker.sizeInSectors)
       runLength = worker.sizeInSectors;

     diskReturn_t rv = diskReadRecord(device,&worker,runLength);

     diskSeekRecordEx(device,pos,DISK_SEEK_ABSOLUTE,0);

     if ( rv != DISK_SUCCESS)
       return DISK_ERROR;

     // shrink our worker, as it represents only the free bytes
     worker.sizeInSectors -= runLength;
     worker.bytes += runLength*NTFS_BYTES_PER_SECTOR;
   }

   return DISK_SUCCESS;
}

diskReturn_t ntfsReadFile(
    const ntfsVolume_t* volume, const ntfsFileHandle_t* file,  diskBuffer_t* mftBuffer,
    diskBuffer_t* fileBuffer, uint64_t length, uint64_t offset, uint64_t* slack)
{
  // Helpers, compiler should optimize them out.
  const diskDevice_t* device = volume->device;

  uint64_t sectors = 0;
  ntfsFileRecord_t* record = NULL;
  ntfsAttrHdr_t* header = NULL;

  if(file == NULL)
    return DISK_ERROR;
  
  // Move Read Pointer to disk's Master File Table...
  volumeSeekCluster(volume,volume->lcnMFT,DISK_SEEK_ABSOLUTE);

  ntfsGetFileReferenceOffset(&(file->mftRecord),&(sectors));

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

    // TODO we should test if bit 1 is set as the other bits are currently reserved...
    if ( !(header->isNonResident) )
      return ntfsReadResidentData(header,fileBuffer,length,offset,slack);

    return ntfsReadNonResidentData(volume, header, fileBuffer, length, offset, slack);
  }

  // No valid entry found...
  return DISK_ERROR;
}
