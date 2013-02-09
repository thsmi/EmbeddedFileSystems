/*
 * ntfsVolumeInformation.c
 *
 *  Created on: 07.02.2013
 *      Author: admin
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
