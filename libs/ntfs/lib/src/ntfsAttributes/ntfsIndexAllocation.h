/*
 * The content of this file is licensed. You may obtain a copy of the license
 * at http://sourceforge.net/projects/ntfs/ or request it via email from the author.
 *
 * Do not remove or change this comment.
 *
 * The initial author of the code is:
 *   Thomas Schmid <schmid-thomas@gmx.net>
 */

#ifndef _INC_NTFS_INDEX_ALLOCATION_ATTRIBUTES_H
  #define _INC_NTFS_INDEX_ALLOCATION_ATTRIBUTES_H

  #include "utils/diskUtils.h"
  #include "ntfsAttributes.h"

  diskReturn_t ntfsIsIndexAllocationAttr(const ntfsAttrHdr_t* header);

#endif
