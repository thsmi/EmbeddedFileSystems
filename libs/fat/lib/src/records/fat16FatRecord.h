/*
 * fat16FatRecord.h
 *
 *  Created on: 17.01.2013
 *      Author: admin
 */

#ifndef FAT16FATRECORD_H_
#define FAT16FATRECORD_H_

typedef struct {
  uint16_t entry[256];
} fat16FatRecord_t;

#endif /* FAT16FATRECORD_H_ */
