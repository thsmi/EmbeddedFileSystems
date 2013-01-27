/*
 * exfatDirectoryEntry.h
 *
 *  Created on: 15.01.2013
 *      Author: admin
 */

#ifndef EXFATDIRECTORYENTRY_H_
#define EXFATDIRECTORYENTRY_H_

#define EXFAT_ATTRIBUTE_READONLY  (1<<0)
#define EXFAT_ATTRIBUTE_HIDDEN    (1<<2)
#define EXFAT_ATTRIBUTE_SYSTEM    (1<<2)
#define EXFAT_ATTRIBUTE_DIRECTORY (1<<4)
#define EXFAT_ATTRIBUTE_ARCHIVE   (1<<5)

#define EXFAT_TYPE_DIRECTORY_ENTRY         0x85
#define EXFAT_TYPE_DIRECTORY_ENTRY_UNUSED  0x05

typedef struct {
  /*0x00*/ uint8_t type;
  /*0x01*/ uint8_t entries; // number of secondary entries that follow, additional 32 byte records(exxtensions) for this entry. must be from 2 to 18
  /*0x02*/ uint16_t checksum;
  /*0x04*/ uint16_t attributes; // Hidden, Archive...
  /*0x06*/ uint8_t reserved1[2];
  struct {
    struct {
      /*0x08*/ uint16_t time;
      /*0x10*/ uint16_t date;
    } created;
    struct {
      /*0x0C*/ uint16_t time;
      /*    */ uint16_t date;
    } modified;
    struct {
      /*0x10*/  uint16_t time;
      /*    */  uint16_t date;
    } accessed;
    struct {
      /*0x14*/ uint8_t created; //10 ms increments added ot created and modified times (increases time 0 upto 1990 ms)
      /*0x15*/ uint8_t modified; // 10ms increment on modified (0...199)
      struct {
        /*0x16*/ uint8_t created;
        /*0x16*/ uint8_t modified; // Timezone offset from UTC applied to file time in 15 minute increments
        /*0x17*/ uint8_t accessed;
      } timezone;
    } offest;
    /*0x18*/ uint8_t reserved2[7];
  } timestamps;
} exfatDirectoryEntry_t;


#endif /* EXFATDIRECTORYENTRY_H_ */
