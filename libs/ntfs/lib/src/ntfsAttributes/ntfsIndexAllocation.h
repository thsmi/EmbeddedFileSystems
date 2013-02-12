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

#ifndef NTFS_INDEX_ALLOCATION_ATTRIBUTE_H_
  #define NTFS_INDEX_ALLOCATION_ATTRIBUTE_H_

  #include "utils/diskUtils.h"
  #include "ntfsAttributes.h"

  diskReturn_t ntfsIsIndexAllocationAttr(const ntfsAttrHdr_t* header);

#endif
