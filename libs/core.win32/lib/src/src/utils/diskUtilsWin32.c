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

#include "utils/diskUtils.h"
#include <string.h>


diskReturn_t ntfsMemCmp(const uint8_t* buffer1, const uint8_t* buffer2, uint32_t length)
{
  if (memcmp(buffer1, buffer2, length)!=0)
    return DISK_ERROR;

  return DISK_SUCCESS;
}

diskReturn_t ntfsMemCpy(diskBuffer_t* buffer, uint8_t* data, uint32_t length)
{
  if (memcpy(&(buffer->bytes[0]),data,length) == NULL)
    return DISK_ERROR;

  return DISK_SUCCESS;
}

