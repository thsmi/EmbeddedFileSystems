/*
 * The content of this file is licensed. You may obtain a copy of the license
 * at http://sourceforge.net/projects/ntfs/ or request it via email from the author.
 *
 * Do not remove or change this comment.
 *
 * The initial author of the code is:
 *   Thomas Schmid <schmid-thomas@gmx.net>
 */

#ifndef _INC_GTP_RECORD_H
  #define _INC_GTP_RECORD_H

  #include "./../../disk/diskRecord/diskRecord.h"

  // Partition Type definitions...
  // Global
  #define GTP_GUID_EMPTY      {0x00000000, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}
  #define GTP_GUID_MBR        {0x024DEE41, 0x33E7, 0x11D3, { 0x9D, 0x69, 0x00, 0x08, 0xC7, 0x81, 0xF3, 0x9F}}
  #define GTP_GUID_EFI        {0xC12A7328, 0xF81F, 0x11D2, { 0xBA, 0x4B, 0x00, 0xA0, 0xC9, 0x3E, 0xC9, 0x3B}}
  #define GTP_GUID_BIOS_BOOT  {0x21686148, 0x6449, 0x6E6F, { 0x74, 0x4E, 0x65, 0x65, 0x64, 0x45, 0x46, 0x49}}

  // Microsoft Specific
  #define GTP_GUID_RESERVED   {0xE3C9E316, 0x0B5C, 0x4DB8, { 0x81, 0x7D, 0xF9, 0x2D, 0xF0, 0x02, 0x15, 0xAE}}
  #define GTP_GUID_BASIC_DATA {0xEBD0A0A2, 0xB9E5, 0x4433, { 0x87, 0xC0, 0x68, 0xB6, 0xB7, 0x26, 0x99, 0xC7}}
  // LDM = Logical Disk manager
  #define GTP_GUID_LDM_META   {0x5808C8AA, 0x7E8F, 0x42E0, { 0x85, 0xD2, 0xE1, 0xE9, 0x04, 0x34, 0xCF, 0xB3}}
  #define GTP_GUID_LDM_DATA   {0xAF9B60A0, 0x1431, 0x4F62, { 0xBC, 0x68, 0x33, 0x11, 0x71, 0x4A, 0x69, 0xAD}}
  #define GTP_GUID_RECOVERY   {0xDE94BBA4, 0x06D1, 0x4D40, { 0xA1, 0x6A, 0xBF, 0xD5, 0x01, 0x79, 0xD6, 0xAC}}


  typedef struct {
    uint64_t signature; //"EFI PART" 0x45 0x46 0x49 0x20 0x50 0x41 0x52 0x54
    uint32_t revision; // 0x00000100
    struct {
      uint32_t size;
      uint32_t crc32;
    }header;
    uint32_t reserved;
    struct {
      uint64_t current;
      uint64_t backup;
      uint64_t firstUsable;
      uint64_t lastUsable;
    } lba;
    uint8_t guid[16];
 
    uint64_t arrayStartLBA; // Starting LBA of array of partition entries
    uint32_t arrayCount;
    uint32_t entrySize;
    uint32_t crc32;
    uint8_t  reserved2[0];

  } gptHeader_t;
 
  #define GPT_ATTRIBUTE_SYSTEM         0
  #define GPT_ATTRIBUTE_LEGACY         2
  #define GPT_ATTRIBUTE_READONLY      60
  #define GPT_ATTRIBUTE_HIDDEN        62
  #define GPT_ATTRIBUTE_NO_AUTOMOUNT  63

  typedef struct {
    uint8_t type[16];
    uint8_t guid[16];
    struct {
      uint64_t first;
      uint64_t last;
    } lba;
    uint64_t attributes;
    uint16_t name[36]; //UTF-16;
  } gptEntry_t;

#endif
