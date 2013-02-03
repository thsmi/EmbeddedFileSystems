/*
 * exfatUpCaseEntry.h
 *
 *  Created on: 15.01.2013
 *      Author: admin
 */

#ifndef EXFATUPCASEENTRY_H_
#define EXFATUPCASEENTRY_H_

#define EXFAT_TYPE_UPCASE_ENTRY         0x81
#define EXFAT_TYPE_UPCASE_ENTRY_UNUSED  0x02

// locates the upcase table 0x82
typedef struct {
  /*0x00*/ uint8_t  type;
  /*0x01*/ uint8_t  reserved1[3];
  /*0x04*/ uint32_t checksum;
  /*0x08*/ uint8_t  reserved2[12];
  /*0x14*/ uint32_t cluster;
  /*0x18*/ uint64_t size;
} exfatUpCaseEntry_t;

uint32_t exfatCalcUpCaseChecksum(const uint8_t* data, uint32_t length)
{
  uint32_t checksum = 0;
  uint32_t i = 0;

  for (i = 0; i < length; i++)
    checksum = (checksum << 31) | (checksum >> 1) + data[i];

  return checksum;
}


#endif /* EXFATUPCASEENTRY_H_ */
