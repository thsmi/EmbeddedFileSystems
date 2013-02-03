/*
 * exfatVolumeLabelEntry.h
 *
 *  Created on: 15.01.2013
 *      Author: admin
 */

#ifndef EXFATVOLUMELABELENTRY_H_
#define EXFATVOLUMELABELENTRY_H_

#define EXFAT_TYPE_VOLUMELABEL_ENTRY         0x83
#define EXFAT_TYPE_VOLUMELABEL_ENTRY_UNUSED  0x03

typedef struct {
  /*0x00*/ uint8_t type;
  /*0x01*/ uint8_t length; // length in unicode characters (max 11);
  /*0x02*/ uint16_t name[11]; // volume label;
  /*0x18*/ uint8_t reserved1[8];
} exfatVolumeLabel_t;


#endif /* EXFATVOLUMELABELENTRY_H_ */
