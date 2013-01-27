/*
 * ntfsVolume.c
 *
 *  Created on: 17.01.2013
 *      Author: admin
 */

#include "ntfsVolume.h"

#include "../ntfs/ntfsRecords/ntfsBootRecord.h"

diskReturn_t ntfsOpenVolume(const diskPartition_t* partition,  diskBuffer_t* buffer, ntfsVolume_t* volume)
{
  ntfsBootRecord_t* bootRecord = NULL;

  // Check if volume handle is already in use...
  if (volume->partition != NULL)
    return DISK_ERROR;

  diskSeekRecordEx(partition->device,0x0LL,
      DISK_SEEK_ABSOLUTE, (uint64_t)(partition->sector));

  if (ntfsReadBootRecord(partition->device,buffer,&bootRecord) != DISK_SUCCESS)
  {
    ntfsDebug(L" Invalid Handle\n");
    //ntfsDebug(L" Invalid Handle %u\n",GetLastError());
    return DISK_ERROR;
  }

  // Partition sectors have to align with file system sectors
  if (bootRecord->bytesPerSector != partition->bytesPerSector)
    return DISK_ERROR;

  //TODO idealerweise sollte man nur mit sektoren arbeiten...
  // Volume offset
  //li.QuadPart =  sector*bytesPerSector;

  // LCN * bytes per cluster
  //li.QuadPart = li.QuadPart + bootRecord->lcnMFT*bootRecord->bytesPerSector * bootRecord->sectorsPerCluster;

  // Save all information needed otherwise it will be gone as the buffer will be reused...
  volume->lcnMFT = bootRecord->lcnMFT;
  volume->bytesPerSector = bootRecord->bytesPerSector;
  volume->sectorsPerCluster = bootRecord->sectorsPerCluster;

  volume->volumeOffset =  partition->sector;

  volume->partition = partition;

  return DISK_SUCCESS;
}

inline diskReturn_t volumeSeekCluster(const ntfsVolume_t* volume, uint64_t cluster, diskSeekMethod_t method)
{
  return diskSeekRecordEx(volume->partition->device,
      cluster * volume->sectorsPerCluster,
      method,
      volume->volumeOffset);
}
