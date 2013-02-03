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

#ifndef _INC_DISK_UTILS_H
  #define _INC_DISK_UTILS_H

  #include "diskStatus.h"
  #include "diskDebug.h"
  #include "diskDataTypes.h"

  #define ntfsMemcmp32(val1,val2) (((*((uint32_t*) &(val1))) == (*((uint32_t*) &(val2))))?1:0)
  #define ntfsMemcpy32(val1,val2) ((*((uint32_t*) &(val1))) = (*((uint32_t*) &(val2))))

  #define ntfsMemcmp16(val1,val2) (((*((uint16_t*) &(val1))) == (*((uint16_t*) &(val2))))?1:0)
  #define ntfsMemcpy16(val1,val2) ((*((uint16_t*) &(val1))) = (*((uint16_t*) &(val2))))

  #define ntfsMemCmp16(val1,val2) (((*((uint16_t*) (val1))) == (*((uint16_t*) (val2))))?1:0)
  #define ntfsMemCpy16(val1,val2) ((*((uint16_t*) (val1))) = (*((uint16_t*) (val2))))

  #define ntfsOffset(start,offset) (((uint8_t*)(start)) + offset)

  // Forward declaration, needs to be replaced with platform specific code
  extern diskReturn_t ntfsMemCpy(diskBuffer_t* buffer, uint8_t* data, uint32_t length);

#endif
