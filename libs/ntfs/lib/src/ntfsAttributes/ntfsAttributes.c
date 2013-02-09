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

#include "ntfsAttributes.h"

diskReturn_t ntfsNextAttr(const ntfsFileRecord_t* record, ntfsAttrHdr_t** attr)
{

  if ((*attr) && ((*attr)->type == ((uint32_t)NTFS_ATTR_END_OF_LIST)))
     return DISK_ERROR_ATTRIBUTE_INVAILD;

  if (*attr == NULL)
    (*attr) = (ntfsAttrHdr_t*) ntfsOffset(record,record->attribute.offset);
  else
    (*attr) = (ntfsAttrHdr_t*) ntfsOffset((*attr),(*attr)->length);

  if ( (*attr)->type ==  ((uint32_t)NTFS_ATTR_END_OF_LIST) )
    return DISK_ERROR_ATTRIBUTE_INVAILD;

  // TODO check if boundaries are broken
  return DISK_SUCCESS;
}

diskReturn_t ntfsFindAttr(const ntfsFileRecord_t* record, ntfsAttrHdr_t** header, const ntfsAttrType_t type)
{
  while (ntfsNextAttr(record, header) == DISK_SUCCESS)
    if ((*header)->type == ((uint32_t)type))
      return DISK_SUCCESS;

  ntfsDebug(L"WARNING: invalid idx \n");
  return DISK_ERROR;
}
