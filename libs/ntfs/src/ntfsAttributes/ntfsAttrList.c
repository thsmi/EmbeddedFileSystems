/*
 * The content of this file is licensed. You may obtain a copy of the license
 * at http://sourceforge.net/projects/ntfs/ or request it via email from the author.
 *
 * Do not remove or change this comment.
 *
 * The initial author of the code is:
 *   Thomas Schmid <schmid-thomas@gmx.net>
 */

#include "ntfsAttrList.h"

diskReturn_t ntfsNextListAttr(const ntfsAttrHdr_t* header, ntfsAttrList_t** list)
{
  if (header->type != ((uint32_t)NTFS_ATTR_ATTRIBUTE_LIST) )
    return DISK_ERROR_ATTRIBUTE_TYPE;

  if (header->isNonResident)
    return DISK_ERROR_ATTRIBUTE_NONRESIDENT;

  if ((*list) && ((*list)->type == ((uint32_t)NTFS_ATTR_END_OF_LIST)))
     return DISK_ERROR_ATTRIBUTE_INVAILD;

  if (*list == NULL)
    (*list) = (ntfsAttrList_t*) ntfsOffset(header, header->attrHeader.resident.attribute.offset);
  else
    (*list) = (ntfsAttrList_t*) ntfsOffset((*list),(*list)->length);

  if ((*list)->type == ((uint32_t)NTFS_ATTR_END_OF_LIST) )
    return DISK_ERROR_ATTRIBUTE_INVAILD;

  return DISK_SUCCESS;
}
