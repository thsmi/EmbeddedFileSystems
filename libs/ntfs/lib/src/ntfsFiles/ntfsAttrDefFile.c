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
#include "ntfsAttrDefFile.h"


 // TODO the bufferlength is in full sectors, but the data stream containing the attrdef might be much smaller...
diskReturn_t ntfsNextAttrDef(const diskBuffer_t* buffer, ntfsAttrDef_t** attr)
{
  uint8_t* endOfBuffer = buffer->bytes + (buffer->sizeInSectors * NTFS_BYTES_PER_SECTOR);

  //0x00 flags do not exist they can appear when  the buffersize is wrong
  /*if ((*attr) && ((*attr)->type == 0x00))
    return DISK_ERROR;*/

  // return an error if we reached the buffer's end
  if ((*attr) && (((uint8_t*)(*attr)) >= endOfBuffer))
     return DISK_ERROR;

  if (*attr == NULL)
    (*attr) = (ntfsAttrDef_t*) buffer->bytes;
  else
    (*attr) = (ntfsAttrDef_t*) ntfsOffset((*attr), sizeof(ntfsAttrDef_t));

  //0x00 flags do not exist they can appear when  the buffersize is wrong
  /*if ((*attr) && ((*attr)->type == 0x00))
    return DISK_ERROR;*/

  endOfBuffer = buffer->bytes + (buffer->sizeInSectors * NTFS_BYTES_PER_SECTOR);
  if ((*attr) && (((uint8_t*)(*attr)) >= endOfBuffer))
     return DISK_ERROR;

  return DISK_SUCCESS;
}
