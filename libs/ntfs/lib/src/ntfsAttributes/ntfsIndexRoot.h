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

#ifndef NTFS_INDEX_ROOT_ATTRIBUTE_H_
  #define NTFS_INDEX_ROOT_ATTRIBUTE_H_

  #include "utils/diskUtils.h"
  #include "./../ntfsIndexNodes/ntfsIndexNode.h"
  #include "ntfsAttributes.h"

  #pragma pack(push, 1)

  typedef struct {
    uint32_t type;
    uint32_t sortingRule;
    struct {
      // size of each index record in bytes, typically 4096
      uint32_t bytes;
      // size of each index record in clusters, typically 4
      uint8_t clusters;
    } size;
    uint8_t padding[3];
    ntfsIndexNodeHeader_t nodeInfo;
  } ntfsAttrIndexRoot_t;
  
  #pragma pack(pop)

  diskReturn_t ntfsGetIndexRootAttr(const ntfsAttrHdr_t* header, ntfsAttrIndexRoot_t** indexRoot);

#endif
