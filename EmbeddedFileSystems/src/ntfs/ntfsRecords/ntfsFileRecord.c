/*
 * The content of this file is licensed. You may obtain a copy of the license
 * at http://sourceforge.net/projects/ntfs/ or request it via email from the author.
 *
 * Do not remove or change this comment.
 *
 * The initial author of the code is:
 *   Thomas Schmid <schmid-thomas@gmx.net>
 */

#include "ntfsFileRecord.h"

diskReturn_t ntfsReadFileRecord(diskDevice_t* hDevice, diskBuffer_t* buffer, ntfsFileRecord_t** record)
{
  if (buffer->sizeInSectors < NTFS_SECTORS_PER_FILERECORD)
    return DISK_ERROR;

  if (diskReadRecord(hDevice, buffer, NTFS_SECTORS_PER_FILERECORD) != DISK_SUCCESS)
    return DISK_ERROR_READ_RECORD;

  (*record) = (ntfsFileRecord_t*)(buffer->bytes);

  if ((*record)->header.signature != NTFS_RECORD_SIGNATURE_FILE)
    return DISK_ERROR_SIGNATURE_FILE;

  if ( ntfsFixUpRecord(&((*record)->header), NTFS_SECTORS_PER_FILERECORD) != DISK_SUCCESS)
    return DISK_ERROR;

//  buffer->idx += (*record)->attribute.offset;

  return DISK_SUCCESS;
}
