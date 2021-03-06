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

#include "ntfsVolumeName.h"

/**
 * The volume name length is equal to the attribute's size
 *
 * @param header
 * @param VolumeName
 * @return
 */
diskReturn_t ntfsGetVolumeNameAttr(const ntfsAttrHdr_t* header, ntfsAttrVolumeName_t** VolumeName)
{
  if (header->type != NTFS_ATTR_VOLUME_NAME)
    return DISK_ERROR_ATTRIBUTE_TYPE;

  if (header->isNonResident)
  {
    ntfsDebug(L"WARNING: Non Resident Filename");
    return DISK_ERROR_ATTRIBUTE_NONRESIDENT;
  }

  // calculate the offset
  (*VolumeName) = (ntfsAttrVolumeName_t*) ntfsOffset(header, header-> attrHeader.resident.attribute.offset);//(((uint8_t*)header) +

  return DISK_SUCCESS;
}
