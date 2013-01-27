/*
 * gptPartition.h
 *
 *  Created on: 17.01.2013
 *      Author: admin
 */

#ifndef GPTPARTITION_H_
#define GPTPARTITION_H_

#include "../disk/diskRecord/diskRecord.h"

#include "diskPartition.h"

typedef struct {
  diskDevice_t* device;
  uint64_t      sector;
  uint64_t      bytesPerSector;
} diskPartition_t;

diskReturn_t ntfsOpenPartition(const diskDevice_t* device, diskPartition_t* volume);


#endif /* GPTPARTITION_H_ */
