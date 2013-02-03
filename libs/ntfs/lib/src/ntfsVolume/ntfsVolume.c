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

#include "ntfsVolume.h"

#include "../ntfsRecords/ntfsBootRecord.h"

diskReturn_t ntfsOpenVolume(const diskDevice_t* device, const mbrPartition_t* partition,  diskBuffer_t* buffer, ntfsVolume_t* volume)
{
  ntfsBootRecord_t* bootRecord = NULL;

  // Check if volume handle is already in use...
  if (volume->device != NULL)
    return DISK_ERROR;

  volume->volumeOffset =  partition->size.offset;

  diskSeekRecordEx(device,0x0LL,
      DISK_SEEK_ABSOLUTE, (uint64_t)(volume->volumeOffset));

  if (ntfsReadBootRecord(device,buffer,&bootRecord) != DISK_SUCCESS)
  {
    ntfsDebug(L" Invalid Handle\n");
    //ntfsDebug(L" Invalid Handle %u\n",GetLastError());
    return DISK_ERROR;
  }

  // Partition sectors have to align with file system sectors
  /*if (bootRecord->bytesPerSector != partition->bytesPerSector)
    return DISK_ERROR;*/

  //TODO idealerweise sollte man nur mit sektoren arbeiten...
  // Volume offset
  //li.QuadPart =  sector*bytesPerSector;

  // LCN * bytes per cluster
  //li.QuadPart = li.QuadPart + bootRecord->lcnMFT*bootRecord->bytesPerSector * bootRecord->sectorsPerCluster;

  // Save all information needed otherwise it will be gone as the buffer will be reused...
  volume->lcnMFT = bootRecord->lcnMFT;
  volume->bytesPerSector = bootRecord->bytesPerSector;
  volume->sectorsPerCluster = bootRecord->sectorsPerCluster;

  volume->device = device;

  return DISK_SUCCESS;
}

diskReturn_t volumeSeekCluster(const ntfsVolume_t* volume, uint64_t cluster, diskSeekMethod_t method)
{
  return diskSeekRecordEx(volume->device,
      cluster * volume->sectorsPerCluster,
      method,
      volume->volumeOffset);
}
