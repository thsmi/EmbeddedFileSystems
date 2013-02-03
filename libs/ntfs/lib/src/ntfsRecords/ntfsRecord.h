/*
 * The content of this file is licensed. You may obtain a copy of the license
 * at http://sourceforge.net/projects/ntfs/ or request it via email from the author.
 *
 * Do not remove or change this comment.
 *
 * The initial author of the code is:
 *   Thomas Schmid <schmid-thomas@gmx.net>
 */

#ifndef _INC_NTFS_RECORD_H
  #define _INC_NTFS_RECORD_H

  #include "utils/diskDataTypes.h"
  #include "disk/diskRecord/diskRecord.h"

  /**
   *  The first MFT Records are well defined
   */
  typedef enum {
    NTFS_$MFT_RECORD      =  0,//!< NTFS_$MFT_RECORD
    NTFS_$MFTMIRR_RECORD  =  1,//!< NTFS_$MFTMIRR_RECORD
    NTFS_$LOGFILE_RECORD  =  2,//!< NTFS_$LOGFILE_RECORD
    NTFS_$VOLUME_RECORD   =  3,//!< NTFS_$VOLUME_RECORD
    NTFS_$ATTRDEF_RECORD  =  4,//!< NTFS_$ATTRDEF_RECORD
    NTFS_$ROOT_RECORD     =  5,//!< NTFS_$ROOT_RECORD
    NTFS_$BITMAP_RECORD   =  6,//!< NTFS_$BITMAP_RECORD
    NTFS_$BOOT_RECORD     =  7,//!< NTFS_$BOOT_RECORD
    NTFS_$BADCLUS_RECORD  =  8,//!< NTFS_$BADCLUS_RECORD
    NTFS_$BAD_RECORD      =  9,//!< NTFS_$BAD_RECORD
    NTFS_$SECURE_RECORD   = 10,//!< NTFS_$SECURE_RECORD
    NTFS_$UPCASE_RECORD   = 11,//!< NTFS_$UPCASE_RECORD
    NTFS_$EXTEND_RECORD   = 12,//!< NTFS_$EXTEND_RECORD

    NTFS_$QUOTA_RECORD    = 24,//!< NTFS_$QUOTA_RECORD
    NTFS_$OBJID_RECORD    = 25 //!< NTFS_$OBJID_RECORD
  } ntfsRecordsTypes_t;

  #define NTFS_RECORD_SIGNATURE_FILE   0x454C4946
  #define NTFS_RECORD_SIGNATURE_INDEX  0x58444e49

  #pragma pack(push, 1)

  typedef struct {
    uint32_t signature; // 0x46 0x49 0x4C 0x45 FILE or "IDXN"
    struct  {
      uint16_t offset; // fixup array location
      uint16_t size;   //Number of array entries
    } fixUpArray;
  } ntfsRecordHeader_t;

  typedef struct {
    uint16_t data;
  } ntfsFixUpEntry_t;

  #pragma pack(pop)

/**
 *  NTFS records typically span multiple sectors. In order to ensure file system consistency, it has
 *  to be guaranteed, when the first sector is written all succeeding sectors are also written. If the
 *  first sector would be written an then stopped there would be an inconsistency in the file system.
 *
 *  NTFS used fixup array to ensure file system consistency and detect if sectors where written correctly.
 *  The last bit of each sector is replaced with a fixup entry. This entry has to be identical for all
 *  consecutive sectors. The original values are stored in the fixup array.
 *
 *  Before using data in a sector you need to restore the fixup data with the original values.
 *
 * @param hDevice
 * @param record
 * @param record
 * @return
 */
diskReturn_t ntfsNextFixUpEntry(const ntfsRecordHeader_t* record, ntfsFixUpEntry_t** entry);


/**
 *
 * @param hDevice
 * @param record
 * @param size
 *   The number sectors, where the fixup entry should replaced. Must be smaller or equal to the fixup array size.
 * @return
 */
diskReturn_t ntfsFixUpRecord(const ntfsRecordHeader_t* record, const uint16_t sectors);


#endif
