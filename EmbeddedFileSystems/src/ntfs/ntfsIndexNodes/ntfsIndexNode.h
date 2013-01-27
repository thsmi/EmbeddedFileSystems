/*
 * The content of this file is licensed. You may obtain a copy of the license
 * at http://sourceforge.net/projects/ntfs/ or request it via email from the author.
 *
 * Do not remove or change this comment.
 *
 * The initial author of the code is:
 *   Thomas Schmid <schmid-thomas@gmx.net>
 */

#ifndef _INC_NTFS_INDEX_NODE
  #define _INC_NTFS_INDEX_NODE

  #include "./../../utils/diskUtils.h"
  #include "./../ntfsAttributes/ntfsFileName.h"

  #pragma pack(push,1)

  typedef struct {
    uint32_t offset; //relative to index Node
    struct {
      uint32_t used;      
      uint32_t allocated; 
    } size; // index nodes lenght's
    uint32_t flags; // 0x00 information is stored in the mtf
    //0x001 information stored in runs, details in the index allocation attribute
  } ntfsIndexNodeHeader_t;


  typedef struct {
    ntfsFileReference_t mftReference; //should be ntfsFileRefernce_t
    uint16_t indexEntyLength;
    uint16_t indexDataLength;// (Filenameattr attr);
    uint16_t flags; //0x1 childnode 0x2 listterminator
    uint16_t padding;
    ntfsAttrFileName_t file;
  } ntfsIndexNodeEntry_t;

  #pragma pack(pop)

  diskReturn_t ntfsNextIndexNode(const ntfsIndexNodeHeader_t* header, ntfsIndexNodeEntry_t** node);
#endif
