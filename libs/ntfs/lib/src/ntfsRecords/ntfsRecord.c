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

#include "ntfsRecord.h"
#include "disk/diskRecord/diskRecord.h"


diskReturn_t ntfsNextFixUpEntry(const ntfsRecordHeader_t* record, ntfsFixUpEntry_t** entry)
{
  ntfsFixUpEntry_t* lastEntry = NULL;

  if (*entry == NULL)
    (*entry) = (ntfsFixUpEntry_t*) ntfsOffset(record, record->fixUpArray.offset);
  else
    (*entry) = (ntfsFixUpEntry_t*) ntfsOffset((*entry), sizeof(ntfsFixUpEntry_t)); // fixup entries are 2 bytes

  lastEntry = (ntfsFixUpEntry_t*)ntfsOffset(record,
      record->fixUpArray.offset + record->fixUpArray.size * sizeof(ntfsFixUpEntry_t));

  // Check if array is out of bounds...
  if ( (*entry) >= lastEntry)
  {
    (*entry) = NULL;
    return DISK_ERROR;
  }

  return DISK_SUCCESS;
}

diskReturn_t ntfsFixUpRecord(const ntfsRecordHeader_t* record, const uint16_t sectors)
{
  ntfsFixUpEntry_t* fixUpEntry = NULL;
  ntfsFixUpEntry_t* originalEntry = NULL;
  ntfsFixUpEntry_t* currentEntry = NULL;

  //The fixup array length is the number of sectors plus one...
  if (record->fixUpArray.size != sectors+1)
    return DISK_ERROR;

  if (ntfsNextFixUpEntry(record, &fixUpEntry) != DISK_SUCCESS)
    return DISK_ERROR;

  // Initialize pointer for original Entries
  originalEntry = fixUpEntry;

  // Jump to last sector minus the fixup array.
  currentEntry = (ntfsFixUpEntry_t*) ntfsOffset(record, NTFS_BYTES_PER_SECTOR - 2);

  while (ntfsNextFixUpEntry(record, &originalEntry) == DISK_SUCCESS)
  {
    if (currentEntry->data != fixUpEntry->data)
      return DISK_ERROR;

    //ntfsDebug(L"  [FIXUP] Replace 0x%04x fix up entry with 0x%04x \n", currentEntry->data, originalEntry->data);
    currentEntry->data = originalEntry->data;

    // Advance to the end of the next sector...
    currentEntry = (ntfsFixUpEntry_t*) ntfsOffset(currentEntry,NTFS_BYTES_PER_SECTOR);
  }

  return DISK_SUCCESS;
}
