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

#ifndef FAT16_BOOT_RECORD_H_
  #define FAT16_BOOT_RECORD_H_

  #include "disk/diskRecord/diskRecord.h"
  #include "fatBootRecord.h"

  #pragma pack(push, 1)

  #define FAT12_16_SIGNATURE 0x29;
  #define FAT12_16_FILE_SYSTEM_TYPE "FAT12";

/*Für FAT12/16
  0x24 Drive number       1Byte
  0x25 reserved           1Byte
  0x26 Signature          1Byte   0x29
  0x27 Volume id          4Bytes
  0x2B Volume Label       11Bytes
  0x34 File System Type   8Bytes  FAT16
  0x3E Bootcode           448
  0x1FE signature         2Bytes  0x55 0xAA*/

  typedef struct {
    fatBootRecordHeader_t header;
    /*0x0024*/ uint8_t drive; // Drive Number
    /*0x0025*/ uint8_t reserved;
    /*0x0026*/ uint8_t signature; //0x29
    struct {
      /*0x0027*/ uint32_t id;
      /*0x002B*/ uint8_t label[11];
    } volume;
    /*0x0034*/ uint64_t type; //FAT16
    /*0x003E*/ uint8_t bootCode[448];
    /*0x01FE*/ uint16_t endOfSector; //0x55 0xAA
  } fat16BootRecord_t;

  #pragma pack(pop)

  diskReturn_t fat16ReadBootRecord(diskDevice_t* hDevice, diskBuffer_t* buffer, fat16BootRecord_t** record);

#endif
