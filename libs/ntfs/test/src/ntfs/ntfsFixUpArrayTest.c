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

#include "../fct/fct.h"

#include "utils/diskUtils.h"
#include "ntfsRecords/ntfsRecord.h"
#include "ntfsRecords/ntfsIndexRecord.h"

FCTMF_SUITE_BGN(test_ntfsSectorFixUp)
{
  FCT_TEST_BGN([ntfsRecord:fixUpArray] Detect iconsistent Sector)
  {
    // Setup test
    uint8_t buffer[16*NTFS_BYTES_PER_SECTOR];  //4k in size
    uint32_t i = 0;

    // clear buffer
    for (i=0; i<16*NTFS_BYTES_PER_SECTOR; i++)
      buffer[i] = 0xFF;

    // initialize fixup
    for (i=0; i<8; i++) //8 sectors
    {
      buffer[510+i*NTFS_BYTES_PER_SECTOR] = 0xCA;
      buffer[511+i*NTFS_BYTES_PER_SECTOR] = 0xFE;
    }

    // make sector inconsistent
    buffer[510+4*NTFS_BYTES_PER_SECTOR] = 0x00;

    // Initialize Sectors/record
    buffer[4] = 0x30;
    buffer[5] = 0x00;

    buffer[6] = 0x09;
    buffer[7] = 0x00;

    buffer[0x30] = 0xCA;
    buffer[0x31] = 0xFE;

    for (i=0; i<8; i++)
    {
      buffer[0x32+i*2] = 0x01+i;
      buffer[0x33+i*2] = 0x02+i;
    }

    // Perform test
    ntfsIndexRecord_t* record = (ntfsIndexRecord_t*) &buffer[0];

    fct_chk(ntfsFixUpRecord(&(record->header),8) == DISK_ERROR);

  }
  FCT_TEST_END();


    FCT_TEST_BGN([ntfsRecord:fixUpArray] Fix up Sectors)
    {
      // Setup test
      uint8_t buffer[16*NTFS_BYTES_PER_SECTOR];  //4k in size
      uint32_t i = 0;

      // clear buffer
      for (i=0; i<16*NTFS_BYTES_PER_SECTOR; i++)
        buffer[i] = 0xFF;

      // initialize fixup
      for (i=0; i<8; i++) //8 sectors
      {
        buffer[510+i*NTFS_BYTES_PER_SECTOR] = 0xCA;
        buffer[511+i*NTFS_BYTES_PER_SECTOR] = 0xFE;
      }

      // Initialize Sectors/record
      buffer[4] = 0x30;
      buffer[5] = 0x00;

      buffer[6] = 0x09;
      buffer[7] = 0x00;

      buffer[0x30] = 0xCA;
      buffer[0x31] = 0xFE;

      for (i=0; i<8; i++)
      {
        buffer[0x32+i*2] = 0x01+i;
        buffer[0x33+i*2] = 0x02+i;
      }

      // Perform test
      ntfsIndexRecord_t* record = (ntfsIndexRecord_t*) &buffer[0];

      fct_chk(ntfsFixUpRecord(&(record->header),8) == DISK_SUCCESS);

      // Tear down
      for (i=0; i<8; i++)
      {
        fct_chk(buffer[510+i*NTFS_BYTES_PER_SECTOR] == 0x01+i);
        buffer[510+i*NTFS_BYTES_PER_SECTOR] = 0xFF;

        fct_chk(buffer[511+i*NTFS_BYTES_PER_SECTOR] == 0x02+i);
        buffer[511+i*NTFS_BYTES_PER_SECTOR] = 0xFF;
      }


      buffer[4] = 0xFF;
      buffer[5] = 0xFF;

      buffer[6] = 0xFF;
      buffer[7] = 0xFF;

      buffer[0x30] = 0xFF;
      buffer[0x31] = 0xFF;

      for (i=0; i<8; i++)
      {
        buffer[0x32+i*2] = 0xFF;
        buffer[0x33+i*2] = 0xFF;
      }

      // after complete teardown all memory should be 0xFF, otherwise we leak...
      for (i=0; i<16*NTFS_BYTES_PER_SECTOR; i++)
        fct_chk(buffer[i] == 0xFF);
    }
    FCT_TEST_END();

}
FCTMF_SUITE_END()
