/*
 * The content of this file is licensed. You may obtain a copy of the license
 * at http://sourceforge.net/projects/ntfs/ or request it via email from the author.
 *
 * Do not remove or change this comment.
 * 
 * The initial author of the code is:
 *   Thomas Schmid <schmid-thomas@gmx.net>
 */

#ifndef _INC_NTFS_FILE_RECORD_H
  #define _INC_NTFS_FILE_RECORD_H

  #include "ntfsRecord.h"
  #include "./../../disk/diskRecord/diskRecord.h"

  // Jeder MFT Record ist mindestens 1024 Bytes groß
  // Sofern die Cluster(sector?) Size größer ist zieht diese.
  // Instead of the double sector size we should compute the MFT Record size
  // and replace the constant by a function call...
  #define NTFS_SECTORS_PER_FILERECORD 2LL

  #pragma pack(push, 1)

  typedef struct {
    uint32_t SegmentNumberLowPart;
    uint16_t SegmentNumberHighPart;
    uint16_t SequenceNumber;
  } ntfsFileReference_t;

  typedef struct {
    ntfsRecordHeader_t header; // Signature: 0x46 0x49 0x4C 0x45 FILE
    struct {
      uint64_t SequenceNumber;
    } logFile;
    uint16_t sequenceNumber;
    uint16_t hardLinkCount;
    struct {
      uint16_t offset;
    } attribute;
    
    uint16_t Flags;
    struct {
      uint32_t used;      // MFT Record's Bytes actually used 
      uint32_t allocated; // MTF Record's length, usually 1024
    } size;

    ntfsFileReference_t baseRecord; // reference to base MFT, incase daratruns don't fit into the record

    uint16_t nextAttributeId; // It wich will be used for next Attribute which will be created
    uint8_t padding[2];

    uint16_t mftRecordNumber; // Index within this Mft
  } ntfsFileRecord_t;

  #pragma pack(pop)

  diskReturn_t ntfsReadFileRecord(diskDevice_t* hDevice, diskBuffer_t* buffer, ntfsFileRecord_t** record);

#endif
