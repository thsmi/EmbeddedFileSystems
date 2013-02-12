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

#ifndef FAT16_FAT_RECORD_H_
  #define FAT16_FAT_RECORD_H_

  typedef struct {
    uint16_t entry[256];
  } fat16FatRecord_t;

#endif
