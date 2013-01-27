/*
 * ntfsVolume.h
 *
 *  Created on: 17.01.2013
 *      Author: admin
 */

#ifndef NTFSVOLUME_H_
#define NTFSVOLUME_H_

#include "../disk/diskRecord/diskRecord.h"

#include "../partition/diskPartition.h"

typedef struct {
  const diskPartition_t* partition;
  uint64_t lcnMFT;

  uint16_t bytesPerSector;
  uint8_t  sectorsPerCluster;

  uint64_t volumeOffset;
} ntfsVolume_t;

// rename to volumeOpenNtfs
diskReturn_t ntfsOpenVolume(const diskPartition_t* device, diskBuffer_t* buffer, ntfsVolume_t* volume);

diskReturn_t volumeSeekCluster(const ntfsVolume_t* volume, uint64_t cluster, diskSeekMethod_t method);

#endif /* NTFSVOLUME_H_ */
