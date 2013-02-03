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

// http://inform.pucp.edu.pe/~inf232/Ntfs/ntfs_doc_v0.5/concepts/data_runs.html

FCTMF_SUITE_BGN(test_ntfsDataRun)
{

  /*
   *
   */
  FCT_TEST_BGN([ntfsRecord:dataRun] Absolute offest)
  {
    uint8_t dataRun[] = { 0x11, 0x30, 0x60 };
    uint64_t length = 0;
    uint64_t offset = 0;
    diskSeekMethod_t method = DISK_SEEK_ABSOLUTE;

    diskReturn_t rv = DISK_ERROR;

    rv = ntfsGetDataRun((ntfsDataRun_t*) &dataRun, &length, &offset, &method);

    fct_chk(rv == DISK_SUCCESS);
    fct_chk(length == 0x30);
    fct_chk(offset == 0x60);
    fct_chk(method == DISK_SEEK_ABSOLUTE);
  }
  FCT_TEST_END();

  /*
   *
   */
  FCT_TEST_BGN([ntfsRecord:dataRun] Relative offest)
  {
    uint8_t dataRun[] =  {0x21, 0x10, 0x00, 0x01};
    uint64_t length = 0;
    uint64_t offset = 0;
    diskSeekMethod_t method = DISK_SEEK_FORWARD;

    diskReturn_t rv = DISK_ERROR;

    rv = ntfsGetDataRun((ntfsDataRun_t*) &dataRun, &length, &offset, &method);

    fct_chk(rv == DISK_SUCCESS);
    fct_chk(length == 0x10);
    fct_chk(offset == 0x100);
    fct_chk(method == DISK_SEEK_FORWARD);

  }
  FCT_TEST_END();

  /*
   *
   */
  FCT_TEST_BGN([ntfsRecord:dataRun] relative negative offest)
  {
    uint8_t dataRun[] =  {0x11, 0x20, 0xE0};
    uint64_t length = 0;
    uint64_t offset = 0;
    diskSeekMethod_t method = DISK_SEEK_FORWARD;

    diskReturn_t rv = DISK_ERROR;

    rv = ntfsGetDataRun((ntfsDataRun_t*) &dataRun, &length, &offset, &method);

    fct_chk(rv == DISK_SUCCESS);
    fct_chk(length == 0x20);
    fct_chk(offset == 0x20);
    fct_chk(method == DISK_SEEK_BACKWARD);

  }
  FCT_TEST_END();

  /* Example 1 - Normal, Unfragmented File
   *
   * Data runs:
   *    21 18 34 56 00
   *    21 18 34 56 - 00 (regrouped)
   *
   * Run 1:
   *    Header = 0x21 - 1 byte length, 2 byte offset
   *    Length = 0x18 (1 byte)
   *    Offset = 0x5634 (2 bytes)
   *
   * Run 2:
   *    Header = 0x00 - the end
   *
   * Summary:
   *    0x18 Clusters @ LCN 0x5634
   *
   * Data is a unfragmented file, of size 0x18 clusters, starting at LCN 0x5634.
   *
   */
  FCT_TEST_BGN([ntfsRecord:dataRun] Normal and Unfragmented File)
  {

    //diskReturn_t ntfsNextDataRun(const ntfsAttrHdr_t* attr, ntfsDataRun_t** dataRun, diskSeekMethod_t* method);

    uint8_t dataRun[] = { 0x21, 0x18, 0x34, 0x56, 0x00 };
    uint64_t length = 0;
    uint64_t offset = 0;
    diskSeekMethod_t method = DISK_SEEK_FORWARD;

    diskReturn_t rv = DISK_ERROR;

    rv = ntfsGetDataRun((ntfsDataRun_t*) &dataRun, &length, &offset, &method);

    fct_chk(rv == DISK_SUCCESS);
    fct_chk(length == 0x18);
    fct_chk(offset == 0x5634);
    fct_chk(method == DISK_SEEK_FORWARD);

  }
  FCT_TEST_END();

  /* Example 2 - Normal, Fragmented File
   *
   * Data runs:
   *    31 38 73 25 34 32 14 01 E5 11 02 31 42 AA 00 03 00
   *    31 38 73 25 34 - 32 14 01 E5 11 02 - 31 42 AA 00 03 - 00 (regrouped)
   *
   * Run 1:
   *    Header = 0x31 - 1 byte length, 3 byte offset
   *    Length = 0x38
   *    Offset = 0x342573
   *
   * Run 2:
   *    Header = 0x32 - 2 byte length, 3 byte offset
   *    Length = 0x114
   *    Offset = 0x363758 (0x211E5 relative to 0x342573)
   * Run 3:
   *    Header = 0x31 - 1 byte length, 3 byte offset
   *    Length = 0x42
   *    Offset = 0x393802 (0x300AA relative to 0x363758)
   * Run 4:
   *    Header = 0x00 - the end
   *
   * Summary:
   *    0x38 Clusters @ LCN 0x342573
   *    0x114 Clusters @ LCN 0x363758
   *    0x42 Clusters @ LCN 0x393802
   *
   *  Data is a fragmented file, of size 0x18E clusters, with data blocks at
   *  LCNs 0x342573, 0x363758 and 0x393802.
   */

  FCT_TEST_BGN([ntfsRecord:dataRun] Normal and Fragmented File)
  {
    uint8_t dataRun[] = { 0x31, 0x38, 0x73, 0x25, 0x34, 0x32, 0x14, 0x01,
                          0xE5, 0x11, 0x02, 0x31, 0x42, 0xAA, 0x00, 0x03, 0x00 };
    uint64_t length = 0;
    uint64_t offset = 0;
    diskSeekMethod_t method = DISK_SEEK_FORWARD;


    fct_chk(1 == 0xFF);
  }
  FCT_TEST_END();

  /*
   Unfragmented file:
   0x32 0xB0 0x01 0x18 0x7B 0x01 0x00

   Fragmented file
   0x31 0x38 0x73 0x25 0x34 0x32 0x14 0x01 0xE5 0x11 0xF3 0x31 0x42 0xAA 0x00 0x03 0x00

   Sparse file
   0x31 0x38 0x73 0x25 0x34 0x01 0x60 0x31 042 0xAA 0x00 0x03 0x00

   Compressed File
   0x31 0x10 0x73 0x25 0x34 0x11 0x09 0x10 0x01 0x07 0x11 0x10 0x08 0x00
   */
}
FCTMF_SUITE_END()
