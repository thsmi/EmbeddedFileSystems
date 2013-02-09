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

#ifndef _INC_NTFS_ATTRIBUTES_H
  #define _INC_NTFS_ATTRIBUTES_H

  #include "utils/diskUtils.h"
  #include "./../ntfsRecords/ntfsFileRecord.h"

  typedef enum  {
    NTFS_ATTR_STANDARD_INFORMATION  = 0x00000010,
    NTFS_ATTR_ATTRIBUTE_LIST        = 0x00000020,
    NTFS_ATTR_FILE_NAME             = 0x00000030,
    NTFS_ATTR_OBJECT_ID             = 0x00000040,
    NTFS_ATTR_SECURITY_DESCRIPTOR   = 0x00000050,
    NTFS_ATTR_VOLUME_NAME           = 0x00000060,
    NTFS_ATTR_VOLUME_INFORMATION    = 0x00000070,
    NTFS_ATTR_DATA                  = 0x00000080,
    NTFS_ATTR_INDEX_ROOT            = 0x00000090,
    NTFS_ATTR_INDEX_ALLOCATION      = 0x000000A0,
    NTFS_ATTR_BITMAP                = 0x000000B0,
    NTFS_ATTR_REPARSE_POINT         = 0x000000C0,
    NTFS_ATTR_EA_INFORMATION        = 0x000000D0,
    NTFS_ATTR_EA                    = 0x000000E0,
    NTFS_ATTR_PROPERTY_SET          = 0x000000F0,
    NTFS_ATTR_LOGGED_UTILITY_STREAM = 0x00000100,
    NTFS_ATTR_END_OF_LIST           = 0xFFFFFFFF
  } ntfsAttrType_t;

  #define NTFS_ATTR_FLAG_COMPRESSED  (0x00FF)
  #define NTFS_ATTR_FLAG_SPARSE      (0x8000)
  #define NTFS_ATTR_FLAG_ENCRYPTED   (0x4000)

  #pragma pack(push, 1)

  typedef struct {
    struct {
      /*0x10*/ uint32_t lenght;  
      /*0x14*/ uint16_t offset;   // 2*namelength+0x18, bei unnamed 0x18;
    } attribute;
    /*0x16*/ uint8_t indexed; // Directory?
    /*0x17*/ uint8_t padding; // Unused...
    uint8_t name[0];
    
    // 0x18: unicode name: 2*namelength, not nullterminated
    // 2*namelength+0x18 -> Attribute;
  } ntfsResidentAttrHdr_t;

  typedef struct {
    // virtual cluster number
    struct { 
      /*0x10*/ uint64_t lowest;
      /*0x18*/ uint64_t highest;
    } VCN;
    /*0x20*/ uint16_t dataRunOffset; // 0x40
    /*0x22*/ uint16_t CompressionUnitSize;
    /*0x24*/ uint8_t padding[4];

    struct {
      /*0x28*/ uint64_t allocated; // Size on disk
      /*0x??*/ uint64_t real;  // real size...
      /*0x??*/ uint64_t initialized;
    } size;

    uint8_t name[0];
    // 0x40 Datatruns... (dataRunOffSet);
  } ntfsNonResidentAttrHdr_t;

  typedef struct {
    /*0x00*/ uint32_t type;
    /*0x04*/ uint32_t length;
//    /*0x06*/ uint16_t padding; // used on some dir bitmaps
    /*0x08*/ uint8_t isNonResident;
    // Shorthands to make code more readable
    struct {
      /*0x09*/ uint8_t lenght;    // wenn unnamed 0x00
      /*0x0A*/ uint16_t offset;   // wenn unnamed 0x00 sonst 0x18     
    } name;
    /*0x0C*/ uint16_t flags;          
    /*0x0E*/ uint16_t id; // attribute's id
    union {
      ntfsResidentAttrHdr_t resident;
      ntfsNonResidentAttrHdr_t nonResident; // 0x01 : Non Resident; 0x00 : Resident
    } attrHeader;
  } ntfsAttrHdr_t;

  #pragma pack(pop)

  diskReturn_t ntfsNextAttr(const ntfsFileRecord_t* record, ntfsAttrHdr_t** attr);
  diskReturn_t ntfsFindAttr(const ntfsFileRecord_t* record, ntfsAttrHdr_t** header, const ntfsAttrType_t type);

#endif
