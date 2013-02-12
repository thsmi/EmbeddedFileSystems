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

#ifndef NTFS_INDEX_RECORD_H_
  #define NTFS_INDEX_RECORD_H_

  #include "disk/diskRecord/diskRecord.h"
  #include "./../ntfsIndexNodes/ntfsIndexNode.h"
  #include "./../ntfsDataRuns/ntfsDataRuns.h"

  #pragma pack(push, 1)

  typedef struct {
    ntfsRecordHeader_t header;
    uint64_t updateSequenceNumber;
    uint64_t vcnIndexAllocation;
    ntfsIndexNodeHeader_t nodeInfo;
  } ntfsIndexRecord_t;

  #pragma pack(pop)

  diskReturn_t ntfsReadIndexRecord(const diskDevice_t* device, diskBuffer_t* buffer, ntfsIndexRecord_t** record);

#endif
