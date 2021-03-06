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

#include "ntfsStandardInfo.h"

diskReturn_t ntfsGetStandardAttr(const ntfsAttrHdr_t* header, ntfsAttrStandard_t** timestamp)
{
  if (header->type != NTFS_ATTR_FILE_NAME)
    return DISK_ERROR_ATTRIBUTE_TYPE;

  if (header->isNonResident)
  {
    ntfsDebug(L"WARNING: Non Resident Timestamp");
    return DISK_ERROR_ATTRIBUTE_NONRESIDENT;
  }
  
  // calculate the offset
  (*timestamp) = (ntfsAttrStandard_t*) ntfsOffset(header, header->attrHeader.resident.attribute.offset);//(((uint8_t*)header) +

  return DISK_SUCCESS;
}
