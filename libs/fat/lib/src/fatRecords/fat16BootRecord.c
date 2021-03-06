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

#include "fat16BootRecord.h"

diskReturn_t fat16ReadBootRecord(diskDevice_t* hDevice, diskBuffer_t* buffer, fat16BootRecord_t** record)
{
  if (diskReadRecord(hDevice, buffer,1) != DISK_SUCCESS)
    return DISK_ERROR_READ_RECORD;

  (*record) = (fat16BootRecord_t*)(buffer->bytes);

  
  if ((*record)->endOfSector != 0xAA55)
    return DISK_ERROR_SIGNATURE_BOOT;
  
  // Are the clusters inconsistent?
  /*if (!ntfsMemcmp16(ntfsBufferPos(buffer,510), ntfsBufferPos(buffer,(*record)->fixUpArray.offset)))
    return DISK_ERROR;

  if (!ntfsMemcmp16(ntfsBufferPos(buffer,1022), ntfsBufferPos(buffer,(*record)->fixUpArray.offset)))
    return DISK_ERROR;*/

  // copy the fixup values into the right spots
/*  ntfsMemCpy16(ntfsOffset(buffer->buffer,510), ntfsOffset(buffer->buffer,(*record)->fixUpArray.offset+2));
  ntfsMemCpy16(ntfsOffset(buffer->buffer,1022), ntfsOffset(buffer->buffer,(*record)->fixUpArray.offset+4));
  */
/*  ntfsMemcpy16(ntfsBufferPos(buffer,510),ntfsBufferPos(buffer,(*record)->fixUpArray.offset+2));
  ntfsMemcpy16(ntfsBufferPos(buffer,1022),ntfsBufferPos(buffer,(*record)->fixUpArray.offset+4));*/

//  buffer->idx += (*record)->attribute.offset;

  return DISK_SUCCESS;
}
