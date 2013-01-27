/*
 * The content of this file is licensed. You may obtain a copy of the license
 * at http://sourceforge.net/projects/ntfs/ or request it via email from the author.
 *
 * Do not remove or change this comment.
 *
 * The initial author of the code is:
 *   Thomas Schmid <schmid-thomas@gmx.net>
 */

#ifndef _INC_NTFS_FILENAME_ATTRIBUTES_H
  #define _INC_NTFS_FILENAME_ATTRIBUTES_H

  #include "./../../utils/diskUtils.h"
  #include "ntfsAttributes.h"


  enum NTFSAttrFileNameType {
    NTFS_ATTR_FILENAME_POSIX = 0,    //UNICODE case sensitive
    NTFS_ATTR_FILENAME_WIN32 = 1,    //UNICODE case insensitive
    NTFS_ATTR_FILENAME_DOS = 2      //8+3 ASCII case insensitive
  };

  #define NTFS_ATTR_FILENAME_DIRECTORY = 0x10000000

  #pragma pack(push, 1)

  typedef struct {
    ntfsFileReference_t parentDirectory;  // 6bytes MTF record number, 2bytes sequence number
    struct {
      uint64_t created;
      uint64_t modified;
      uint64_t modifiedMTF;
      uint64_t accessed;
    } date;
    struct {
      uint64_t logical; // logical size
      uint64_t actual; // size on disk
    } size;
    uint32_t flags;
    uint32_t reparseValue;
    struct {
      uint8_t length;
      uint8_t type; 
      uint16_t value[1];
    } name;
  } ntfsAttrFileName_t;

  #pragma pack(pop)

  diskReturn_t ntfsGetFileNameAttr(const ntfsAttrHdr_t* header, ntfsAttrFileName_t** fileName);

#endif
