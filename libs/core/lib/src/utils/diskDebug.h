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

#ifndef DISK_DEBUG_H_
  #define DISK_DEBUG_H_

  #include "diskStatus.h"
  #include "diskDataTypes.h"


//  #ifndef _DEBUG
//    #define _DEBUG
//  #endif


  #ifdef _DEBUG
    #include <wchar.h>
    #include <stdio.h>
    #define ntfsDebug(...) wprintf(__VA_ARGS__)

  #else
    #define ntfsDebug(...) while (0) {}
  #endif

  void ntfsHexDump(const uint8_t* bytes, uint8_t length);

  void ntfsStrDump(const uint16_t* bytes, uint8_t length);

#endif
