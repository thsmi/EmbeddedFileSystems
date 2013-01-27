/*
 * exfatVolumeGUID.h
 *
 *  Created on: 15.01.2013
 *      Author: admin
 */

#ifndef EXFATVOLUMEGUID_H_
#define EXFATVOLUMEGUID_H_

#define EXFAT_TYPE_VOLUME_GUID_ENTRY         0xA0

/* Flags:
 *   (1<<0) Alloc Possible, must be null
 *   (1<<1) NoFatChain, must be null
 */

typedef struct {
  /*0x00*/ uint8_t type;
  /*0x01*/ uint8_t entries;
  /*0x02*/ uint16_t checksum;
  /*0x04*/ uint16_t flags;
  /*0x06*/ uint8_t guid[16];
  /*0x16*/ uint8_t reserved1[10];
} extfatVolumeGuid_t;

#endif /* EXFATVOLUMEGUID_H_ */
