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

#ifndef _INC_DISK_RECORDS_WIN32_H
  #define _INC_DISK_RECORDS_WIN32_H

  #include "disk/diskRecord/diskRecord.h"

  #include "./utils/diskUtils.h"
  #include "./utils/diskStatus.h"

  #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
  #endif

  #ifdef _MSC_VER
    #pragma comment( lib, "Setupapi.lib" )
  #endif

  #include <windows.h>
  #include <winnt.h>
  #include <SetupAPI.h>
  #include <WinIoCtl.h>

  struct diskDeviceTag_t {
    HANDLE handle;
  };

  // a simple wrapper ...
  typedef struct {
    uint64_t Cylinders;
    uint32_t TracksPerCylinder;
    uint32_t SectorsPerTrack;
    uint32_t BytesPerSector;
  } ntfsDiskGeometry_t;

  diskReturn_t diskInitBuffer(diskBuffer_t* buffer, uint8_t* buff, uint16_t size);

  diskReturn_t diskOpenDevice(LPCWSTR lpFileName, diskDevice_t* device);
  diskReturn_t diskCloseDevice(diskDevice_t *device);

  diskReturn_t diskGetGeometry(const diskDevice_t* device, ntfsDiskGeometry_t *pdg);

  diskReturn_t diskReadRecord(const diskDevice_t* hDevice, diskBuffer_t* buffer,uint8_t sectors);
  diskReturn_t diskSeekRecord(const diskDevice_t* hDevice, uint64_t sectors);
  diskReturn_t diskSeekRecordEx(const diskDevice_t* device, uint64_t offset, diskSeekMethod_t method, uint64_t volumeOffset);

  diskReturn_t diskCurrentRecord(const diskDevice_t* hDevice, uint64_t* position);

  /*
  
prinzip flexible buffer
  entweder speed dann jede eene einen eigenen buffer
  oder minimize mem, dann reread der sektoren anstatt buffers

  */

#endif
