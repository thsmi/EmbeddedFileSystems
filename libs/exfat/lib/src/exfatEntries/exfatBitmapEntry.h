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

#ifndef EXFAT_BITMAP_ENTRY_H_
  #define EXFAT_BITMAP_ENTRY_H_

  #define EXFAT_TYPE_BITMAP_ENTRY         0x81
  #define EXFAT_TYPE_BITMAP_ENTRY_UNUSED  0x01

  // first Bit is the BitmapIdentifier
  //   0 - 1st Bitmap
  //   1 - 2nd Bitmap (only used with second fat texfat)
  #define EXFAT_FLAGS_SECOND_BITMAP (1<<0)

  typedef struct {
    /*0x00*/ uint8_t type;
    /*0x01*/ uint8_t flags;
    /*0x02*/ uint8_t reserved1[18];
    /*0x14*/ uint32_t cluster; //cluster adderssess
    /*0x18*/ uint64_t size;    // size in bytes
  } exfatBitmapEntry_t;

#endif
