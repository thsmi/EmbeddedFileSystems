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

#ifndef FAT_SFN_ENTRY_H_
  #define FAT_SFN_ENTRY_H_

  #define FAT_ATTRIBUTE_READONLY       (1<<0)
  #define FAT_ATTRIBUTE_HIDDEN         (1<<1)
  #define FAT_ATTRIBUTE_SYSTEM         (1<<2)
  #define FAT_ATTRIBUTE_VOLUMELABEL    (1<<3)
  #define FAT_ATTRIBUTE_SUBDIRECTORY   (1<<4)
  #define FAT_ATTRIBUTE_ARCHIVE        (1<<5)
  #define FAT_ATTRIBUTE_DEVICE         (1<<6)
  #define FAT_ATTRIBUTE_LONGFILENAME   (1<<7)

  // standard file name entry
  struct {
    /*0x00*/ uint8_t filename[8]; // padded with space
    /*0x08*/ uint8_t extension[3]; // padded with space
    /*0x0B*/ uint8_t attributes;
    /*0x0C*/ uint8_t reserved;
    uint8_t creationtime[3];
    uint16_t creationdate;
    uint16_t accessdate;
    uint16_t highbytesofclusternumber;
    uint16_t modifiedtime;
    uint16_t modifieddate;
    uint16_t lowbytesclusternumber;
    uint32_t filesize;
  } fatSfnEntry_t;

#endif
