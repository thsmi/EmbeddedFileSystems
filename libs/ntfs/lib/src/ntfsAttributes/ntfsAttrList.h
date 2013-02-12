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

#ifndef NTFS_ATTR_LIST_ATTRIBUTE_H_
  #define NTFS_ATTR_LIST_ATTRIBUTE_H_

  #include "utils/diskUtils.h"
  #include "ntfsAttributes.h"

  #pragma pack(push, 1)

  typedef struct {
    /*0x00*/ uint32_t type;
    /*0x04*/ uint16_t length;
    struct {
      /*0x06*/ uint8_t length;
      /*0x07*/ uint8_t offset; 
    } name;
    /*0x08*/ uint64_t lowestVCN;
    /*0x10*/ ntfsFileReference_t baseRecord;
    /*0x18*/ uint16_t id;
    /*0x1A*/ uint8_t name2[0];
  } ntfsAttrList_t;


  #pragma pack(pop)

  diskReturn_t ntfsNextListAttr(const ntfsAttrHdr_t* header, ntfsAttrList_t** list);

#endif
