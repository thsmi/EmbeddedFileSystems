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

#ifndef _INC_NTFS_ATTRIBUTE_LIST_RECORD_H
  #define _INC_NTFS_ATTRIBUTE_LIST_RECORD_H

  #include "utils/diskUtils.h"
  #include "utils/diskStatus.h"

  #include "../ntfsAttributes/ntfsAttrList.h"

  typedef struct {
    ntfsAttrList_t attribute;
  } ntfsAttrListRecord_t;

  diskReturn_t ntfsReadAttrListRecord(diskDevice_t* hDevice, diskBuffer_t* buffer,uint8_t sectors);

  // struct ntfsListItem_t = ntfsAttrList_t
  // uint8_t ntfsNextListItem(const diskBuffer_t* buffer, const ntfsListRecord_t* header, ntfsListItem_t** node);
#endif
