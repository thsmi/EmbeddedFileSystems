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

#ifndef _INC_MBR_RECORD_H
  #define _INC_MBR_RECORD_H

  #include "disk/diskRecord/diskRecord.h"

  #define MBR_BOOTABLE         0x80
  #define MBR_PARTITION_NTFS   0x07
  #define MBR_PARTITION_FAT16B 0x06

  #pragma pack(push,1)

  typedef struct {
    uint8_t bootable;

    uint8_t startCHS[3];

    uint8_t type;

    uint8_t endCHS[3];

    struct {
      uint32_t offset;  // preceding sectors
      uint32_t length; // partition length
    } size; // length in sectors
  } mbrPartition_t;

  typedef struct {
    uint8_t bootstrap[446];
    mbrPartition_t partitions[4];
    /*0x1FE*/uint16_t signature; //0x55 0xAA
  } mbrRecord_t;

  #pragma pack(pop)

  uint8_t mbrSectorFromCHS(const uint8_t* chs);
  uint8_t mbrHeadFromCHS(const uint8_t* chs);
  uint16_t mbrCylinderFromCHS(const uint8_t* chs);

  diskReturn_t mbrReadRecord(diskDevice_t* hDevice, diskBuffer_t* buffer, mbrRecord_t** record);

  diskReturn_t mbrNextPartition(const mbrRecord_t* record, mbrPartition_t** partition);

  diskReturn_t mbrDumpPartitionInfo(mbrPartition_t* partInfo);

#endif
