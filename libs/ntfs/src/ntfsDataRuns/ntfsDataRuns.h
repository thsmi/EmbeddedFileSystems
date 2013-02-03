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

#ifndef _INC_NTFS_DATARUNS_H
  #define _INC_NTFS_DATARUNS_H

  #include "./../../utils/diskUtils.h"
  #include "./../../utils/diskStatus.h"

  #include "./../ntfsAttributes/ntfsAttributes.h"

  #pragma pack(push, 1)

  typedef struct ntfsDataRunTag_t ntfsDataRun_t;

  /*struct ntfsDataRunTag_t {
    uint8_t header;
    uint8_t content[0];
  };*/

  /*typedef struct {
    uint8_t header;
    uint8_t content[0];
  } ntfsDataRun_t;*/

  #pragma pack(pop) 

  diskReturn_t ntfsNextDataRun(const ntfsAttrHdr_t* attr, ntfsDataRun_t** dataRun, diskSeekMethod_t* method);
  diskReturn_t ntfsGetDataRun(const ntfsDataRun_t* dataRun, uint64_t* length, uint64_t* offset, diskSeekMethod_t* method);

#endif
