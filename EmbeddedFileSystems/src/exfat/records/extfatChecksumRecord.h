/*
 * extfatChecksumRecord.h
 *
 *  Created on: 15.01.2013
 *      Author: admin
 */

#ifndef EXTFATCHECKSUMRECORD_H_
#define EXTFATCHECKSUMRECORD_H_

struct {
  uint32_t checksum[128];
} exfatChecksumRecord_t;

// Bootcheksum
// 12 Sector of boot an backup boot

// Sectors 0-10 : Boot Record
// Sector    11 : Contains 4 Repetive bytes (checksum of the boot record, flags and no percentage is ignored)
// Sectord 12-22 : Backup Boot record
// Sector   23 : Checksum ...


/* boot sectors Volume flags byte 106,107 as well as percent used (112) are skipped */

uint32_t exfatCalcBootAreaChecksum(const uint8_t data[], uint32_t length)
{
  uint32_t checksum = 0;
  uint32_t i = 0;

  for (uint32_t = 0; i < length; i++)
  {
    if (i == 106 || i == 107 || i == 112)
      continue;

    checksum = (checksum<<31) | (checksum>> 1) + data[i];
  }

  return checksum;
}

#endif /* EXTFATCHECKSUMRECORD_H_ */
