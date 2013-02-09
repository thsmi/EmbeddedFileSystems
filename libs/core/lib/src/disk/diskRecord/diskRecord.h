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

#ifndef _INC_DISK_RECORDS_H
  #define _INC_DISK_RECORDS_H

  #include "./../../utils/diskUtils.h"
  #include "./../../utils/diskStatus.h"


  typedef enum {
    DISK_SEEK_ABSOLUTE,
    DISK_SEEK_BACKWARD,
    DISK_SEEK_FORWARD
  } diskSeekMethod_t;


  // The following declarations are forward declarations...
  // ... you need to link these against a platform specific...
  // ... implementation
  typedef struct diskDeviceTag_t diskDevice_t;

  extern diskReturn_t diskInitBuffer(diskBuffer_t* buffer, uint8_t* buff, uint16_t size);

  extern diskReturn_t diskReadRecord(const diskDevice_t* hDevice, diskBuffer_t* buffer,uint8_t sectors);
  extern diskReturn_t diskSeekRecord(const diskDevice_t* hDevice, uint64_t sectors);
  extern diskReturn_t diskSeekRecordEx(const diskDevice_t* hDevice, uint64_t offset, diskSeekMethod_t method, uint64_t volumeOffset);

  extern diskReturn_t diskCurrentRecord(const diskDevice_t* hDevice, uint64_t* position);

#endif
