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

#ifndef FAT32_FSINFO_RECORD_H_
  #define FAT32_FSINFO_RECORD_H_

  typedef struct {
    /*0x0000*/ uint32_t signature; // 0x52 0x52 0x61 0x41  "RRaA"
    /*0x0004*/ uint8_t reserved1[480];
    struct {
      /*0x01E4*/ uint32_t signature; // 0x72 0x72 0x41 0x61 "rrAa"
      /*0x01E8*/ uint32_t freeClusters; // 0xFF FF FF FF if unknown
      /*0x01EC*/ uint32_t mostRecentlyAllocatedCluster; // 0xFF FF FFFF if unknown
    } FSInfo;
    /*0x01F0*/ uint8_t reserved2[14];
    /*0x01FE*/ uint16_t endOfSector; /*0x55 0xAA*/
  } fat32FSInfoSector_t;

#endif
