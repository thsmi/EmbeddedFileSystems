/*
 * The content of this file is licensed. You may obtain a copy of the license
 * at http://sourceforge.net/projects/ntfs/ or request it via email from the author.
 *
 * Do not remove or change this comment.
 *
 * The initial author of the code is:
 *   Thomas Schmid <schmid-thomas@gmx.net>
 */

#include "ntfsIndexAllocation.h"

diskReturn_t ntfsIsIndexAllocationAttr(const ntfsAttrHdr_t* header)
{
  if (header->type != NTFS_ATTR_INDEX_ALLOCATION)
    return DISK_ERROR_ATTRIBUTE_TYPE;

  if( !header->isNonResident )
  {
    ntfsDebug(L"WARNING: Resident Index Allocation Attribute");
    return DISK_ERROR_ATTRIBUTE_RESIDENT;
  }

  if (header->name.lenght != 4)
    return DISK_ERROR_ATTRIBUTE_NAME;
  
  // The name has to be $I30
  if (*( (uint32_t*) ntfsOffset(header,header->name.offset) ) != 0x00490024)
    return DISK_ERROR_ATTRIBUTE_NAME;


  return DISK_SUCCESS;
}
