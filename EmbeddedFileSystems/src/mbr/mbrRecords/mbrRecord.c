/*
 * The content of this file is licensed. You may obtain a copy of the license
 * at http://sourceforge.net/projects/ntfs/ or request it via email from the author.
 *
 * Do not remove or change this comment.
 *
 * The initial author of the code is:
 *   Thomas Schmid <schmid-thomas@gmx.net>
 */

#include "mbrRecord.h"

diskReturn_t mbrReadRecord(diskDevice_t* hDevice, diskBuffer_t* buffer, mbrRecord_t** record)
{
  if (diskReadRecord(hDevice, buffer,1) != DISK_SUCCESS)
    return DISK_ERROR_READ_RECORD;

  (*record) = (mbrRecord_t*)(buffer->bytes);

/*  if ((*record)->OEM_ID != 0x202020205346544e)
    return DISK_ERROR_SIGNATURE_BOOT;*/
  
/*  uint16_t* tmp = &((*record)->signature);
  if ((*record)->signature != 0xAA55)
    return DISK_ERROR_SIGNATURE_BOOT;*/

  return DISK_SUCCESS;
}

uint8_t mbrSectorFromCHS(const uint8_t* chs)
{
  return (chs[1] & 0x3F);
}

uint8_t mbrHeadFromCHS(const uint8_t* chs)
{
  return (chs[0]);
}

uint16_t mbrCylinderFromCHS(const uint8_t* chs)
{
  return (((chs[1]) << 2) & 0x300) + (chs)[2];
}

diskReturn_t mbrDumpPartitionInfo(mbrPartition_t* partInfo)
{
  if (partInfo->bootable == MBR_BOOTABLE)
    ntfsDebug(L" Bootable!\n");

  ntfsDebug(L" Start Head.............: %u \n", mbrHeadFromCHS(partInfo->startCHS));
  ntfsDebug(L" Start Sectors..........: %u \n", mbrSectorFromCHS(partInfo->startCHS));

  ntfsDebug(L" Start Cylinder.........: %d \n", mbrCylinderFromCHS(partInfo->startCHS));

  switch (partInfo->type)
  {
    case MBR_PARTITION_NTFS:
      ntfsDebug(L" Type...................: NTFS \n");
      break;

    case MBR_PARTITION_FAT16B:
      ntfsDebug(L" Type...................: FAT16B \n");
      break;

    default:
      ntfsDebug(L" Type...................: Unknown\n");
      break;
  }

  ntfsDebug(L" End Head...............: %u \n", mbrHeadFromCHS(partInfo->endCHS));
  ntfsDebug(L" End Sectors............: %u \n", mbrSectorFromCHS(partInfo->endCHS));

  ntfsDebug(L" End Cylinder...........: %d \n", mbrCylinderFromCHS(partInfo->endCHS));

  ntfsDebug(L" Sectors Proceeding.....: %d \n", partInfo->size.offset);

  ntfsDebug(L" Sectors in Partition...: %d \n", partInfo->size.length);
  ntfsDebug(L"\n");

  return DISK_SUCCESS;
}
