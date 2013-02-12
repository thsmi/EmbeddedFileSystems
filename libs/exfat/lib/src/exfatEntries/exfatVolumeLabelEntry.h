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

#ifndef EXFAT_VOLUMELABEL_ENTRY_H_
  #define EXFAT_VOLUMELABEL_ENTRY_H_

  #define EXFAT_TYPE_VOLUMELABEL_ENTRY         0x83
  #define EXFAT_TYPE_VOLUMELABEL_ENTRY_UNUSED  0x03

  typedef struct {
    /*0x00*/ uint8_t type;
    /*0x01*/ uint8_t length; // length in unicode characters (max 11);
    /*0x02*/ uint16_t name[11]; // volume label;
    /*0x18*/ uint8_t reserved1[8];
  } exfatVolumeLabel_t;

#endif
