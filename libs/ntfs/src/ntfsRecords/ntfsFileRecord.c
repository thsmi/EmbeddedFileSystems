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

#include "ntfsFileRecord.h"

/* TODO: Replace the constant by a function, which calculates the sector size...

  Currently we double the sector size but we should do something like :

  if (disk->bytesPerSector > 1024)
    NTFS_SECTORS_PER_FILERECORD = 1;
  else
    NTFS_SECTORS_PER_FILERECORD = 1024 / disk->bytesPerSector;
*/

// Every MFT Record is minimal 1024 bytes in size. In case the sector size...
// ... is larger than the minimal record size, the sector size is used. Thus...
// ... it's perfectly fine for a MFT Record to be bigger than 1024 bytes.

#define NTFS_SECTORS_PER_FILERECORD 2LL

diskReturn_t ntfsGetFileReferenceOffset(const ntfsFileReference_t* reference, uint64_t* offset)
{
  if (!reference)
    return DISK_ERROR;

  if (!offset)
    return DISK_ERROR;

  // The file reference is 48 Bits in size. It's a bit sub optimal, as it's neither a native...
  // type nor a primitive. So we convert it into a 64bit integer.
  (*offset) = ((((uint64_t)reference->SegmentNumberHighPart) << 32) + (reference->SegmentNumberLowPart));

  // now we convert the entries to sectors.
  (*offset) = (*offset) * NTFS_SECTORS_PER_FILERECORD;

  return DISK_SUCCESS;
}

diskReturn_t ntfsReadFileRecord(const diskDevice_t* hDevice, diskBuffer_t* buffer, ntfsFileRecord_t** record)
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

  return DISK_SUCCESS;
}
