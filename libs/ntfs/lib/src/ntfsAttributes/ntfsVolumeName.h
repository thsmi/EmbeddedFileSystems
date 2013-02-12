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

#ifndef NTFS_VOLUMENAME_ATTRIBUTE_H_
  #define NTFS_VOLUMENAME_ATTRIBUTE_H_

  #include "utils/diskUtils.h"
  #include "ntfsAttributes.h"

  #pragma pack(push, 1)

  typedef struct {
    /*0x00*/ uint8_t name[0];
  } ntfsAttrVolumeName_t;

  #pragma pack(pop)

  diskReturn_t ntfsGetVolumeNameAttr(const ntfsAttrHdr_t* header, ntfsAttrVolumeName_t** VolumeName);

#endif
