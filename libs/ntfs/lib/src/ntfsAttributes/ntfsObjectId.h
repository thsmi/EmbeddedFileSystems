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

#ifndef NTFS_OBJECT_ID_ATTTRIBUTE_H_
  #define NTFS_OBJECT_ID_ATTTRIBUTE_H_

  #include "utils/diskUtils.h"

  typedef struct {
    /*0x00-0x0F*/ GUID objectId;
    struct {
      /*0x10-0x1F*/ GUID volumeId; /*optional*/
      /*0x20-0x2F*/ GUID objectId; /*optional*/
      /*0x30-0x3F*/ GUID domainId; /*optional*/
    } birth;
  } ntfsAttrObjectId_t;

#endif
