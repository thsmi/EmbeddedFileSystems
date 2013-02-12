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

#ifndef EXFAT_VOLUME_GUID_ENTRY_H_
  #define EXFAT_VOLUME_GUID_ENTRY_H_

  #define EXFAT_TYPE_VOLUME_GUID_ENTRY         0xA0

  /* Flags:
   *   (1<<0) Alloc Possible, must be null
   *   (1<<1) NoFatChain, must be null
   */

  typedef struct {
    /*0x00*/ uint8_t type;
    /*0x01*/ uint8_t entries;
    /*0x02*/ uint16_t checksum;
    /*0x04*/ uint16_t flags;
    /*0x06*/ GUID guid;
    /*0x16*/ uint8_t reserved1[10];
  } extfatVolumeGuid_t;

#endif
