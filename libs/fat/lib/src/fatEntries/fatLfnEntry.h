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

#ifndef FAT_LFN_ENTRY_H_
  #define FAT_LFN_ENTRY_H_

  /*
  === long file names (lfn)

  backward sequence prior to short filename:

  [ entry type | sequence | name entry]
  [ lfn | name.txt]
  [ lfn  | y lonf file n]
  [ lfn | This is a ver]
  [ sfn | THISIS~1.TXT]


  unused bytes are filled with ffff

  first character is set to 0xe5 when file is deleted
  */

  typedef struct {
    uint8_t sequence; //0x4? - last entry
    uint16_t characters1[5]; // first 5 characters;
    uint8_t attribute; // always 0x0f
    uint8_t reserved;
    uint8_t checksum;
    uint16_t characters2[6]; // next 6 characters;
    uint16_t firstCluster; // always 0x00
    uint16_t characters3[2]; // next 2 characters;
  } fatLfnEntry_t;

  diskReturn_t fatIsLastLfnEntry(fatLfnEntry_t* entry)
  {
    if ((entry->sequence & 0xF0) != 0x40)
      return DISK_ERROR;

    return DISK_SUCCESS;
  }

#endif
