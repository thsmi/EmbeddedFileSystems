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

#include "ntfsIndexRecord.h"

// Index files are typically one cluster in size, which is usually 4k or 8 Sectors
#define NTFS_SECTORS_PER_INDEXRECORD 8

diskReturn_t ntfsReadIndexRecord(const diskDevice_t* hDevice, diskBuffer_t* buffer, ntfsIndexRecord_t** record)
{
  if (buffer->sizeInSectors < NTFS_SECTORS_PER_INDEXRECORD)
    return DISK_ERROR;

  if (diskReadRecord(hDevice, buffer, NTFS_SECTORS_PER_INDEXRECORD) != DISK_SUCCESS)
    return DISK_ERROR_READ_RECORD;

  (*record) = (ntfsIndexRecord_t*)(&(buffer->bytes[0]));

  //test for indx
  if ((*record)->header.signature != NTFS_RECORD_SIGNATURE_INDEX) // INDX
    return DISK_ERROR_SIGNATURE_INDX;

  if (ntfsFixUpRecord(&((*record)->header), NTFS_SECTORS_PER_INDEXRECORD) != DISK_SUCCESS)
    return DISK_ERROR;

  return DISK_SUCCESS;
}

