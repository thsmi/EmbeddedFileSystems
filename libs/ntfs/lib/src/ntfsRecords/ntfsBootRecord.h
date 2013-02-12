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

#ifndef NTFS_BOOT_RECORD_H_
  #define NTFS_BOOT_RECORD_H_

  #include "disk/diskRecord/diskRecord.h"

  #pragma pack(push, 1)

  typedef struct {
    uint8_t jump[3];
    uint64_t OEM_ID; // 0x4e544653202020 "NTFS    "
    /*0x0B*/ uint16_t bytesPerSector; 
    /*0x0D*/ uint8_t  sectorsPerCluster;
    uint8_t padding[7];
    /*0x15*/ uint16_t mediatype; //0xF = Fixed disk
    uint8_t padding2[25];
    /*0x30*/ uint64_t lcnMFT; //locgial Cluster number for $MFT
    uint8_t padding3[190];
    uint8_t padding4[264];
    /*0x1FE*/uint16_t endOfSector; //0x55 0xAA
  } ntfsBootRecord_t;

  #pragma pack(pop)

  diskReturn_t ntfsReadBootRecord(const diskDevice_t* hDevice, diskBuffer_t* buffer, ntfsBootRecord_t** record);

#endif
