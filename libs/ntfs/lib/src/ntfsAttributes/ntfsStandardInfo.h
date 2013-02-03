/*
 * The content of this file is licensed. You may obtain a copy of the license
 * at http://sourceforge.net/projects/ntfs/ or request it via email from the author.
 *
 * Do not remove or change this comment.
 *
 * The initial author of the code is:
 *   Thomas Schmid <schmid-thomas@gmx.net>
 */

#ifndef _INC_NTFS_STANDARD_ATTRIBUTES_H
  #define _INC_NTFS_STANDARD_ATTRIBUTES_H

  #include "utils/diskUtils.h"
  #include "utils/diskStatus.h"
  #include "ntfsAttributes.h"

  #define NTFS_STANDARD_ATTR_READ_ONLY      0x0001
  #define NTFS_STANDARD_ATTR_HIDDEN         0x0002
  #define NTFS_STANDARD_ATTR_SYSTEM         0x0004
  #define NTFS_STANDARD_ATTR_ARCHIVE        0x0020
  #define NTFS_STANDARD_ATTR_DEVICE         0x0040
  #define NTFS_STANDARD_ATTR_NORMAL         0x0080
  #define NTFS_STANDARD_ATTR_TEMPORARY      0x0100
  #define NTFS_STANDARD_ATTR_SPARSE         0x0200
  #define NTFS_STANDARD_ATTR_REPARSE_POINT  0x0400
  #define NTFS_STANDARD_ATTR_COMPRESSED     0x0800
  #define NTFS_STANDARD_ATTR_OFFLINE        0x1000
  #define NTFS_STANDARD_ATTR_NOT_INDEXED    0x2000
  #define NTFS_STANDARD_ATTR_ENCRYPTED      0x4000
  #define NTFS_STANDARD_ATTR_VIRTUAL        0x8000

  #pragma pack(push, 1)

  typedef struct {
    struct {
      /*0x00*/ uint64_t created; 
      /*0x08*/ uint64_t modified;
      /*0x10*/ uint64_t modifiedMTF;
      /*0x16*/ uint64_t accessed;
    } date;
    /*0x20*/ uint32_t flags;
    /*0x24*/ uint32_t maxNumberOfVersions;
    /*0x28*/ uint32_t VersionNumber;
    /*0x2C*/ uint32_t classId;
    /*0x30*/ uint32_t ownerId;
    /*0x34*/ uint32_t securityId;
    /*0x38*/ uint32_t quota;
    /*0x40*/ uint32_t updateSequenceNumber;
  } ntfsAttrStandard_t;


#pragma pack(pop)

  diskReturn_t ntfsGetStandardAttr(const ntfsAttrHdr_t* header, ntfsAttrStandard_t** timestamp);

#endif
