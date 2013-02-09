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

#include "ntfsIndexNode.h"

diskReturn_t ntfsNextIndexNode(const ntfsIndexNodeHeader_t* header, ntfsIndexNodeEntry_t** node)
{
  if (*node == NULL)
    (*node) = (ntfsIndexNodeEntry_t*) ntfsOffset(header,header->offset);
  else
    (*node) = (ntfsIndexNodeEntry_t*) ntfsOffset((*node), (*node)->indexEntyLength );

  //wprintf(L"%d %d %d %d", (*node)->file.flags,(*node)->indexDataLength, (*node)->indexEntyLength, (*node)->file.name.length);
  // offset 0xaa / 170

  // we reached the end of the this list, we have to continue 
  // in the next sector...
  if ((*node)->indexDataLength == 0)
    return DISK_ERROR;

  if ((*node)->flags & 0x2)
    return DISK_ERROR;

  return DISK_SUCCESS;
}
