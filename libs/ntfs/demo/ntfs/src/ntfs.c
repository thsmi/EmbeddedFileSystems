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

#ifndef UNICODE
  #define UNICODE
#endif

#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

#include "./utils/diskStatus.h"
#include "./utils/diskUtils.h"

#include "disk/diskRecord/diskRecord.h"
#include "disk/diskRecord/diskRecordWin32.h"

#include "ntfsRecords/ntfsFileRecord.h"
#include "ntfsRecords/ntfsIndexRecord.h"
#include "ntfsRecords/ntfsBootRecord.h"
#include "ntfsRecords/ntfsRecord.h"

#include "mbr/mbrRecord.h"

#include "ntfsAttributes/ntfsAttributes.h"
#include "ntfsAttributes/ntfsFileName.h"
#include "ntfsAttributes/ntfsIndexAllocation.h"
#include "ntfsAttributes/ntfsIndexRoot.h"
#include "ntfsAttributes/ntfsAttrList.h"

#include "ntfsIndexNodes/ntfsIndexNode.h"

#include "ntfsDataRuns/ntfsDataRuns.h"

#include "ntfsVolume/ntfsVolume.h"
#include "ntfsFiles/ntfsFiles.h"
#include "ntfsFiles/ntfsAttrDefFile.h"


// [ indexrecord | node header ] [node entry] [node entry] [node entry]
// [ indexattr | node Header] [node entry]

// Parses the $root file

// Each file is 1024 Bytes long. If its longer the it is moved to non resident areas
static diskReturn_t listMFT(const ntfsVolume_t* volume)
{

  uint32_t i = 0;
  ntfsFileRecord_t* record = NULL;

  uint8_t dosName = 0;
  uint8_t windowsName = 0;

  uint8_t wait = 0;

  volumeSeekCluster(volume,volume->lcnMFT,DISK_SEEK_ABSOLUTE);

  uint8_t mftBuff[NTFS_BYTES_PER_SECTOR*8];
  diskBuffer_t mftBuffer;
  diskInitBuffer(&mftBuffer, &mftBuff[0], sizeof(mftBuff));

  while (ntfsReadFileRecord(volume->device, &mftBuffer,&record) == DISK_SUCCESS)
  {
    uint8_t dataSegments = 0;
    ntfsAttrHdr_t* header = NULL;

    i++;
    dosName = 0;
    windowsName = 0;
    wait = 0;

    while (ntfsNextAttr(record,&header) == DISK_SUCCESS)
    {
      ntfsDebug(L"[%#x] %#x \n",i*1024,header->type);

      if (header->type == NTFS_ATTR_FILE_NAME)
      {
        ntfsAttrFileName_t* fileAttr = NULL;
        ntfsGetFileNameAttr(header,&fileAttr);

        if(fileAttr->name.length)
          ntfsDebug(L"[%#x] \"%.*ls\" %#x\n",i*1024,fileAttr->name.length, &fileAttr->name.value[0],fileAttr->name.type);
         
        continue;
      }

      if (header->type == NTFS_ATTR_DATA)
      {
        dataSegments++;
          
        if (dataSegments > 1)
          ntfsDebug(L"ADS in %d \n",i);
        if(header->name.lenght)
          ntfsDebug(L"Name Data Segment in %d: %.*ls \n",i,header->name.lenght, ntfsOffset(header,header->name.offset));
          
        continue;
      }

      if (header->type == NTFS_ATTR_INDEX_ROOT)
      {
        ntfsAttrIndexRoot_t* indexRootAttr = NULL;
        //ntfsGetIndexRootAttr(&buffer,header,&indexRootAttr);

        //ntfsDebug(L"Index root attr");
      }

     /* if (header->type == NTFS_ATTR_INDEX_ALLOCATION)
      {
        ntfsDebug(L"index allocation list\n");

        // create here a new buffer for parsing dataruns...
        dataRun = NULL;

        while (ntfsNextDataRun(buffer,header,&dataRun,NULL) == DISK_SUCCESS)
        {
          length,offset = 0;

          if (ntfsGetDataRun(buffer,dataRun,&length,&offset,NULL) == DISK_SUCCESS)
            ntfsDebug(L"[%#x] %llu %llu\n",i*1024,length,offset);
        }        
      }*/

      if (header->type == NTFS_ATTR_ATTRIBUTE_LIST)
      {
        ntfsAttrList_t* listAttr = NULL;

        ntfsDebug(L"[%#x : attr list] MFT record %#x \n",i*1024,i-1);

        /*while (ntfsNextListAttr(buffer,header,&listAttr) == DISK_SUCCESS)
        {
          ntfsDebug(L"[%#x : attr list] type: %#x length: %#x MFT record: %#x %#x . %#x \n",
            i*1024,listAttr->type,listAttr->length,listAttr->baseRecord.SegmentNumberHighPart,listAttr->baseRecord.SegmentNumberLowPart,listAttr->baseRecord.SequenceNumber);

          if (listAttr->name.length)
            ntfsDebug(L"[%#x] \"%.*s\" \n",i*1024,listAttr->name.length, &listAttr->name2[0]);

          wait = 1;
        }*/

        if (ntfsNextListAttr(header,&listAttr) == DISK_ERROR_ATTRIBUTE_NONRESIDENT)
        {
          uint64_t pos = 0;
          ntfsDataRun_t* dataRun = NULL;
          diskSeekMethod_t method = DISK_SEEK_FORWARD;

          //backup old position...
          diskCurrentRecord(volume->device,&pos);

          while (ntfsNextDataRun(header,&dataRun,&method) == DISK_SUCCESS)
          {
            uint64_t length,offset = 0;          

            diskBuffer_t buffer2;
            uint8_t  buff2[NTFS_BYTES_PER_SECTOR * 2];

            ntfsFileRecord_t* record2= NULL; 
            ntfsAttrHdr_t* hdr2 = NULL;

            length = 0;
            offset = 0;

            diskInitBuffer(&buffer2,buff2,sizeof(buff2));

            if (ntfsGetDataRun(dataRun,&length,&offset,&method) != DISK_SUCCESS)
              continue;
       
            ntfsDebug(L" %llu %llu %d \n",length,offset);

           if (length == 0)
             continue;
    
            if (length > 1)
              ntfsDebug(L"Warning more than one record");

            volumeSeekCluster(volume,offset,method);
           //diskSeekRecordEx(device,((uint64_t)offset)*((uint64_t)sectorsPerCluster),method,volumeOffset);

           ntfsDebug(L"[%#llx] Offset Cluster \n",offset);
           ntfsDebug(L"[%#llx] Offset2 \n",((uint64_t)offset)*volume->sectorsPerCluster);

           ntfsDebug(L"[%#x] DataRun %#x %#x \n",i*1024,length,offset);

           while (length--)
           {
             if (diskReadRecord(volume->device, &buffer2,2) != DISK_SUCCESS)
               return DISK_ERROR_READ_RECORD;

             ntfsHexDump(buffer2.bytes,12);
             break;
             /*if (ntfsReadFileRecord(hDevice,&buffer2,&record2) != DISK_SUCCESS)
               break;*/

              while (ntfsNextAttr(record2,&hdr2) == DISK_SUCCESS) 
                ntfsDebug(L"[%#x] AA %#x %#x\n",i*1024,header->type,header->isNonResident);
           }
          }

          // restore old position
          diskSeekRecordEx(volume->device,pos,DISK_SEEK_ABSOLUTE,0x0LL);
          wait = 0;
        }
      }
    }

    if ( wait == 1)
      getchar();
    else 
      ntfsDebug(L"\n");

    //if (dosName & !windowsName)
    //  ntfsDebug(L"[%#x] WARNING: Only DOS Name\n",i*1024);
  }

  //GetNTFSAttrHeader(&buff[firstAttribute]);

  //6 bytes!  0x30 location of updateSeq
  return DISK_SUCCESS;
}

static diskReturn_t listAttrDef(const ntfsVolume_t* volume)
{
  ntfsDebug(L"Attribute:\n");  

  uint64_t slack = 0;

  uint8_t mftBuff[NTFS_BYTES_PER_SECTOR*8];
  diskBuffer_t mftBuffer;

  uint8_t fileBuff[NTFS_BYTES_PER_SECTOR*8];
  diskBuffer_t fileBuffer;

  diskInitBuffer(&mftBuffer,&mftBuff[0],sizeof(mftBuff));
  diskInitBuffer(&fileBuffer,&fileBuff[0],sizeof(fileBuff));

  ntfsFileHandle_t file = { .mftRecord = {NTFS_$ATTRDEF_RECORD,0,0}, .volume = volume };

  ntfsReadFile(volume, &file, &mftBuffer, &fileBuffer,8,0,&slack);

  ntfsAttrDef_t* attr = NULL;
  while ( ntfsNextAttrDef(&fileBuffer, &attr) == DISK_SUCCESS)
  {
    
    ntfsDebug(L"Attribute........: (0x%02x) %.*ls \n",  attr->type, 64,  &(attr->name[0]));
    ntfsDebug(L"Min Size.........: %llu \n", attr->size.minumum);
    
    // 18446744073709551615ULL = 16 Exabytes ntfs's physical maximum...
    if (attr->size.maximum == 18446744073709551615ULL)
       { ntfsDebug(L"Max Size.........: 16 exabytes \n", attr->size.maximum); }
    else
       { ntfsDebug(L"Max Size.........: %llu \n", attr->size.maximum); }
    
    ntfsDebug(L"Storage..........:");
    
    
    if (attr->flags & NTFS_ATTRDEF_FLAGS_RESIDENT)
      { ntfsDebug(L" Resident"); }

    if (attr->flags & NTFS_ATTRDEF_FLAGS_NONRESIDENT)
      { ntfsDebug(L" Non-Resident"); }
    
    if (attr->flags & NTFS_ATTRDEF_FLAGS_INDEXED)
      { ntfsDebug(L" Indexed"); }
    
    ntfsDebug(L"\r\n\r\n");
  }

  return DISK_SUCCESS;
}


static diskReturn_t dumpCluster(const ntfsVolume_t* volume, uint64_t cluster)
{
  // clusters a typically 4kb
  uint8_t bytes[NTFS_BYTES_PER_SECTOR*8];
  diskBuffer_t buffer;

  diskInitBuffer(&buffer, &bytes[0], sizeof(bytes));

  volumeSeekCluster(volume,cluster,DISK_SEEK_ABSOLUTE);

  diskReadRecord(volume->device, &buffer, 8);

  FILE* file = fopen("d:\\dump.h","w+");

  fprintf(file, "static const uint8_t cluster[] = {\r\n");

  for (uint32_t i = 0; i < NTFS_BYTES_PER_SECTOR*8; ++i)
  {

    if (i != 0)
      fprintf(file, ", ");

    if ((i) % 8 == 0)
      fprintf(file, " \r\n  ");

    fprintf(file, "0x%02x", bytes[i]);
  }

  fprintf(file, "\r\n};");

  fclose(file);

  return DISK_SUCCESS;
}

static diskReturn_t onListFiles(ntfsFileReference_t mftReference, const ntfsAttrFileName_t* file, uint8_t* data)
{
  // To read file jump to the mftRecord referenced...

  if (file->flags & 0x10000000)
  {
    ntfsStrDump(L"Directory: ",11);
    if (data)
      (*((ntfsFileReference_t*) data)) = mftReference;

  }
  else
    ntfsStrDump(L"Filename: ",10);

  ntfsStrDump(&(file->name.value[0]), file->name.length);
  
  ntfsStrDump(L" \n",2);

  return DISK_ERROR;
}

static diskReturn_t listFiles(const ntfsVolume_t* volume)
{
  ntfsFileReference_t mftReference = {0,0,0};

  uint8_t mftBuff[NTFS_BYTES_PER_SECTOR*2LL];
  diskBuffer_t mftBuffer;

  uint8_t fileBuff[NTFS_BYTES_PER_SECTOR*8LL];
  diskBuffer_t fileBuffer;

  diskInitBuffer(&mftBuffer,&mftBuff[0],sizeof(mftBuff));
  diskInitBuffer(&fileBuffer,&fileBuff[0],sizeof(fileBuff));

  ntfsDebug(L"=== Listing directory\n");
  ntfsListFiles(volume, NULL,  &mftBuffer, &fileBuffer, &onListFiles, (uint8_t*)(&mftReference));

  if ((mftReference.SegmentNumberLowPart == 0) && (mftReference.SegmentNumberHighPart == 0))
    return DISK_SUCCESS;

  ntfsFileHandle_t file = { .mftRecord = mftReference, .volume = volume };

  ntfsDebug(L"=== Listing directory\n");
  return ntfsListFiles(volume, &file, &fileBuffer, &fileBuffer,&onListFiles, NULL);
}

static diskReturn_t onListFiles2(ntfsFileReference_t mftReference, const ntfsAttrFileName_t* file, uint8_t* data)
{
  if (file->flags & 0x10000000)
    return DISK_ERROR;

  if (ntfsMemCmp((uint8_t*)(L"lpc.txt"), (uint8_t*) (&(file->name.value[0])) , 14 ) == DISK_ERROR)
    return DISK_ERROR;

  // we found the file, we can skip...
  if (data)
    (*((ntfsFileReference_t*) data)) = mftReference;

  ntfsDebug(L" File found... \n");

  return DISK_SUCCESS;
}

static diskReturn_t readFile(const ntfsVolume_t* volume)
{
  ntfsFileReference_t mftReference = {0,0,0};

  uint8_t mftBuff[NTFS_BYTES_PER_SECTOR*2LL];
  diskBuffer_t mftBuffer;

  uint8_t fileBuff[NTFS_BYTES_PER_SECTOR*8LL];
  diskBuffer_t fileBuffer;

  diskInitBuffer(&mftBuffer,&mftBuff[0],sizeof(mftBuff));
  diskInitBuffer(&fileBuffer,&fileBuff[0],sizeof(fileBuff));

  ntfsListFiles(volume, NULL,  &mftBuffer, &fileBuffer, &onListFiles2, (uint8_t*)(&mftReference));

  if ((mftReference.SegmentNumberLowPart == 0) && (mftReference.SegmentNumberHighPart == 0))
    return DISK_SUCCESS;

  ntfsFileHandle_t file = { .mftRecord = mftReference, .volume = volume };

  uint64_t slack = 0;
  do {

    if (ntfsReadFile(volume,&file,&mftBuffer,&fileBuffer,2,0,&slack) != DISK_SUCCESS)
      { ntfsDebug(L"Reading clusters failed"); }

    ntfsStrDump((uint16_t*)&(fileBuffer.bytes[0]),128);
    ntfsStrDump(L"\n",1);
  } while (!slack);

  return DISK_SUCCESS;
}


diskReturn_t ReadDisk(LPCWSTR lpFileName)
{
  diskDevice_t device;
  ntfsDiskGeometry_t geometry;

  mbrRecord_t* record = NULL;

  uint8_t  buff[NTFS_BYTES_PER_SECTOR*2];
  diskBuffer_t buffer;
  diskInitBuffer(&buffer,buff,sizeof(buff));

  if (diskOpenDevice(lpFileName, &device) != DISK_SUCCESS)
    return DISK_ERROR;

  if (diskGetGeometry(&device, &geometry) != DISK_SUCCESS)
  {
    diskCloseDevice(&device);
    return DISK_ERROR;
  }

  ntfsDebug(L" Cylinders = %I64d\n", geometry.Cylinders);
  ntfsDebug(L" Tracks per Cylinder = %ld\n", geometry.TracksPerCylinder);
  ntfsDebug(L" Sectors per Track = %ld\n", geometry.SectorsPerTrack);
  ntfsDebug(L" Bytes per Sector = %ld\n", geometry.BytesPerSector);

  // Ensure we start at the disks first sector...
  diskSeekRecordEx(&device,0,DISK_SEEK_ABSOLUTE,0x0LL);

  if (mbrReadRecord(&device,&buffer,&record) != DISK_SUCCESS)
    return DISK_ERROR;

  mbrPartition_t* partition = NULL;

  while (mbrNextPartition(record,&partition) == DISK_SUCCESS)
    mbrDumpPartitionInfo(partition);

  partition = NULL;
  mbrNextPartition(record,&partition);
  mbrNextPartition(record,&partition);
  mbrNextPartition(record,&partition);
  
  /*partition.sector = record->partitions[0].size.offset;
  partition.bytesPerSector = geometry.BytesPerSector;*/

  ntfsVolume_t volume = { 0,0,0,0,0 };


  if (ntfsOpenVolume(&device, partition,&buffer, &volume) != DISK_SUCCESS)
    return DISK_ERROR;

  uint8_t demo = 2;

  switch (demo)
  {
    case 1:
      return listAttrDef(&volume);

    case 2:
      return listFiles(&volume);

    case 3:
      return readFile(&volume);

    case 4:
      return dumpCluster(&volume,0xbcd3e);

    case 5:
      return listMFT(&volume);
  }

  diskCloseDevice(&device);

  return DISK_SUCCESS;
}


int main()
{
  // eclipse voodoo
  // (http://wiki.eclipse.org/CDT/User/FAQ#Eclipse_console_does_not_show_output_on_Windows)
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  //GUID for GUID_DEVICEINTERFACE_DISK
  GUID guid = {0x53F56307L, 0xB6BF, 0x11D0, {0x94, 0xF2, 0x00, 0xA0, 0xC9, 0x1E, 0xFB, 0x8B}};

  HDEVINFO hDevInfo = SetupDiGetClassDevs(&guid,NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

  DWORD i =0;
  BOOL rv =0;

  SP_DEVINFO_DATA deviceInfoData;
  SP_DEVICE_INTERFACE_DATA deviceInterfaceData;

  DWORD ulSize = 0;

  PSP_DEVICE_INTERFACE_DETAIL_DATA_W pDeviceInterfaceDetailData = 0;

  if (hDevInfo == INVALID_HANDLE_VALUE)
    return 1;

  // Start Enumerating...
  while (1)
  {
    deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    rv = SetupDiEnumDeviceInfo(hDevInfo,i,&deviceInfoData);

    if (!rv)
    {
      // Exit successful;
      if (GetLastError() == ERROR_NO_MORE_ITEMS)
        break;

      ntfsDebug(L"Enumerating Device Interfaces Failed %u \n", GetLastError());
    }

    deviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

    rv = SetupDiEnumDeviceInterfaces(hDevInfo, NULL, &guid, i, &deviceInterfaceData);

    if (!rv)
    {
      ntfsDebug(L"Enumerating Device Interfaces failed %u \n", GetLastError());
      break;
    }

    ulSize = 0;
    rv = SetupDiGetDeviceInterfaceDetail(hDevInfo, &deviceInterfaceData, NULL, 0, &ulSize, NULL);

    if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
    {
      ntfsDebug(L"Enumerating Device Interfaces failed %u \n", GetLastError());
      break;
    }

    pDeviceInterfaceDetailData
        = (PSP_DEVICE_INTERFACE_DETAIL_DATA_W) malloc(ulSize);

    pDeviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

    rv=SetupDiGetDeviceInterfaceDetailW(hDevInfo, &deviceInterfaceData, pDeviceInterfaceDetailData, ulSize, &ulSize, &deviceInfoData);

    if (!rv)
    {
      ntfsDebug(L"Enumerating Device Interfaces failed %u \n", GetLastError());
      free(pDeviceInterfaceDetailData);
      break;
    }

   
    ntfsDebug(L" Device %ls\n",pDeviceInterfaceDetailData->DevicePath);
    
    //if (i==1)
      ReadDisk(pDeviceInterfaceDetailData->DevicePath);

    
    free(pDeviceInterfaceDetailData);
    i++;
  }

  ntfsDebug(L"Closed Successfully");
  SetupDiDestroyDeviceInfoList(hDevInfo);

  //getchar();

  return 0;
}


