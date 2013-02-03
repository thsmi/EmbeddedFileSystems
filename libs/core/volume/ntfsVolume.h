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

#ifndef NTFSVOLUME_H_
#define NTFSVOLUME_H_

#include "../disk/diskRecord/diskRecord.h"
#include "../partition/mbr/mbrRecords/mbrRecord.h"

typedef struct {
  const diskDevice_t* device;
  uint64_t lcnMFT;

  uint16_t bytesPerSector;
  uint8_t  sectorsPerCluster;

  uint64_t volumeOffset;
} ntfsVolume_t;

// rename to volumeOpenNtfs
diskReturn_t ntfsOpenVolume(const diskDevice_t* device, const mbrPartition_t* partition,  diskBuffer_t* buffer, ntfsVolume_t* volume);

diskReturn_t volumeSeekCluster(const ntfsVolume_t* volume, uint64_t cluster, diskSeekMethod_t method);

#endif /* NTFSVOLUME_H_ */
