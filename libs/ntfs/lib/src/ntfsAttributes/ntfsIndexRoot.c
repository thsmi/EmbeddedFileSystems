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

#include "ntfsIndexRoot.h"

diskReturn_t ntfsGetIndexRootAttr(const ntfsAttrHdr_t* header, ntfsAttrIndexRoot_t** indexRoot)
{
  if (header->type != NTFS_ATTR_INDEX_ROOT)
    return DISK_ERROR_ATTRIBUTE_TYPE;

  // Index Roots are always resident...
  if(header->isNonResident)
  {
    ntfsDebug(L"WARNING: Non Resident Index");
    return DISK_ERROR_ATTRIBUTE_NONRESIDENT;
  }

  // the name has to be $I30
  if (header->name.lenght != 4)
    return DISK_ERROR_ATTRIBUTE_NAME;

  if ( *( (uint32_t*) ntfsOffset(header,header->name.offset) ) != 0x00490024 )
    return DISK_ERROR_ATTRIBUTE_NAME;

  (*indexRoot) = (ntfsAttrIndexRoot_t*) ntfsOffset(header,header->attrHeader.resident.attribute.offset);

  return DISK_SUCCESS;
}
