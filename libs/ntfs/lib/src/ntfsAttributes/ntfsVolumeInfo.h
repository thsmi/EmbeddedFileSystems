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

#ifndef NTFS_VOLUMEINFO_ATTRIBUTE_H_
  #define NTFS_VOLUMEINFO_ATTRIBUTE_H_

  #include "utils/diskUtils.h"
  #include "ntfsAttributes.h"

  #define NTFS_VOLUMEINFO_FLAGS_DIRTY              (1 << 0x0001)
  #define NTFS_VOLUMEINFO_FLAGS_RESIZE_LOGFILE     (1 << 0x0002)
  #define NTFS_VOLUMEINFO_FLAGS_UPGRADE_ON_MOUNT   (1 << 0x0004)
  #define NTFS_VOLUMEINFO_FLAGS_MOUNTED_ON_NT4     (1 << 0x0008)
  #define NTFS_VOLUMEINFO_FLAGS_DELETE_USN         (1 << 0x0010)
  #define NTFS_VOLUMEINFO_FLAGS_REPAIR_OBJECT_IDS  (1 << 0x0020)
  #define NTFS_VOLUMEINFO_FLAGS_MODIFIED_BY_CHKDSK (1 << 0x8000)

  #pragma pack(push, 1)

  typedef struct {
    /*0x00*/ uint8_t reserved1[8];
    struct {
      /*0x08*/ uint8_t major;
      /*0x09*/ uint8_t minor;
    } version;
    /*0x0A*/ uint16_t flags;
    /*0x0C*/ uint8_t reserved2[4];
  } ntfsAttrVolumeInfo_t;

  #pragma pack(pop)

  diskReturn_t ntfsGetVolumeInfoAttr(const ntfsAttrHdr_t* header, ntfsAttrVolumeInfo_t** VolumeInfo);

#endif
