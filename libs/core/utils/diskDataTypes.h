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

#ifndef _INC_DISK_TYPES_H
  #define _INC_DISK_TYPES_H

  #include "diskStatus.h"
  #include "stdint.h"

  // types
  #define NTFS_BYTES_PER_SECTOR 512LL

/*// 1 Byte
#define uint8_t unsigned char
#define int8_t signed char
// 2 Bytes
#define uint16_t unsigned short
#define int16_t signed short
// 4 Bytes
#define uint32_t unsigned int
// 8 Bytes
#define uint64_t unsigned long long
#define int64_t signed long long*/

/*#ifndef FALSE
  #define FALSE  0
#endif*/

// C treats any non-zero value as true
/*#ifndef TRUE
  #define TRUE    1
#endif*/

  #ifndef NULL
    #define NULL    0
  #endif

  #pragma pack(push, 1)

  #ifndef GUID_DEFINED
    #define GUID_DEFINED
    
    typedef struct _GUID {
      uint32_t Data1;
      uint16_t Data2;
      uint16_t Data3;
      uint8_t  Data4[ 8 ];
    } GUID;

  #endif

  #pragma pack(pop)


  typedef struct {
    uint8_t* bytes;
    uint16_t sizeInSectors;
  } diskBuffer_t;


#endif
