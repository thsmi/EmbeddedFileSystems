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

#ifndef FAT_BOOT_RECORD_H_
  #define FAT_BOOT_RECORD_H_

  #include "utils/diskUtils.h"
  
  #pragma pack(push, 1)

  typedef struct {
    /*0x00*/ uint8_t jump[3]; //Jump& NOP instruction
    /*0x03*/ uint64_t OEM_ID; // "MSDOS5.0"
    /*0x0B*/ uint16_t bytesPerSector; //usually 512
    /*0x0D*/ uint8_t  sectorsPerCluster;
    /*0x0E*/ uint16_t bootSectorLength; // Number of sectors that make up the Boot Record
    /*0x10*/ uint8_t  fileAllocationTableCount; // original +copy typically 2
    /*0x11*/ uint16_t RootDirectoryCount; // number of root directories
    /*0x13*/ uint16_t SectorCount; // number of sectors 
    /*0x15*/ uint16_t mediatype; //0xF8 = Fixed disk
    uint8_t padding2[15];
  } fatBootRecordHeader_t;

  #pragma pack(pop)

#endif
