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

#include "ntfsVolumeInfo.h"

diskReturn_t ntfsGetVolumeInfoAttr(const ntfsAttrHdr_t* header, ntfsAttrVolumeInfo_t** VolumeInfo)
{
  if (header->type != NTFS_ATTR_VOLUME_INFORMATION)
    return DISK_ERROR_ATTRIBUTE_TYPE;

  if (header->isNonResident)
  {
    ntfsDebug(L"WARNING: Non Resident Filename");
    return DISK_ERROR_ATTRIBUTE_NONRESIDENT;
  }

  // calculate the offset
  (*VolumeInfo) = (ntfsAttrVolumeInfo_t*) ntfsOffset(header, header-> attrHeader.resident.attribute.offset);

  return DISK_SUCCESS;
}
