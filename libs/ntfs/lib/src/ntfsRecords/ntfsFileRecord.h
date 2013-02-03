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

#ifndef _INC_NTFS_FILE_RECORD_H
  #define _INC_NTFS_FILE_RECORD_H

  #include "ntfsRecord.h"
  #include "disk/diskRecord/diskRecord.h"

  #pragma pack(push, 1)

  typedef struct {
    uint32_t SegmentNumberLowPart;
    uint16_t SegmentNumberHighPart;
    uint16_t SequenceNumber;
  } ntfsFileReference_t;

  #pragma pack(pop)

  /**
   * File References are 48-bit wide identifiers. They are used to access and index MFT entries.
   *
   * @param reference
   *   the FileReference which should be converted
   * @param offset
   *   the offset in sectors relative to the first MFT entry
   * @return
   *   Indicates whether the call has failed or was completed
   */
  diskReturn_t ntfsGetFileReferenceOffset(const ntfsFileReference_t* reference, uint64_t* offset);

  #pragma pack(push, 1)

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
    
    uint16_t flags;
    struct {
      uint32_t used;      // MFT Record's bytes actually used
      uint32_t allocated; // MTF Record's length, usually 1024
    } size;

    ntfsFileReference_t baseRecord; // reference to base MFT, in case data runs don't fit into the record

    uint16_t nextAttributeId; // It which will be used for next Attribute which will be created
    uint8_t padding[2];

    uint16_t mftRecordNumber; // Index within this MFT
  } ntfsFileRecord_t;

  #pragma pack(pop)

  diskReturn_t ntfsReadFileRecord(const diskDevice_t* hDevice, diskBuffer_t* buffer, ntfsFileRecord_t** record);

#endif
