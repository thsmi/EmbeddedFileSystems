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

/*#ifdef __CYGWIN32__
  #define WIN32
#endif*/

#include "diskRecordWin32.h"

/**
 * The win32 api has some strange implementation pitfalls. One of them is
 * you cannot trust the return values of win32 api function. A function
 * might return success but failed in reality.
 *
 * The only way to check for success is to call GetLastError() after any
 * api call. The function succeeded only when the function's return value
 * and GetLastError indicate success.
 *
 * This a wrapper around window's getLastError() function, to make
 * simplify debugging.
 *
 * @return
 */
static diskReturn_t diskGetLastError()
{
  uint32_t error = GetLastError();

  if (error == 0)
    return DISK_SUCCESS;

  ntfsDebug(L"\n!!! Last API call failed, GetLastError Returned %u\n",error);

  return DISK_ERROR_WIN32;
}

diskReturn_t diskOpenDevice(LPCWSTR lpFileName, diskDevice_t* device)
{

  device->handle = CreateFileW(lpFileName,
                  GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                  OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);

  if (device->handle != INVALID_HANDLE_VALUE)
    return DISK_SUCCESS;

  return diskGetLastError();
}

diskReturn_t diskCloseDevice(diskDevice_t *device)
{
  if (!device || (device->handle == INVALID_HANDLE_VALUE))
    return DISK_ERROR_DEVICE_INVALID;

  if (!CloseHandle(device->handle))
    return diskGetLastError();

  if (diskGetLastError() != DISK_SUCCESS)
    return DISK_ERROR_WIN32;

  device->handle = INVALID_HANDLE_VALUE;

  return DISK_SUCCESS;
}

diskReturn_t diskGetGeometry(const diskDevice_t* device, ntfsDiskGeometry_t* geometry)
{
  DWORD unused = 0;
  DISK_GEOMETRY pdg;
  BOOL rv = FALSE;

  if (!device || (device->handle == INVALID_HANDLE_VALUE))
    return DISK_ERROR_DEVICE_INVALID;

  rv = DeviceIoControl(device->handle, IOCTL_DISK_GET_DRIVE_GEOMETRY,
               NULL, 0, &pdg, sizeof(pdg), &unused, (LPOVERLAPPED) NULL);

  if (FALSE == rv)
  {
    // GetLastError == ERROR_NOT_READY == 21 -> Device not ready, insert memory stick/card
    ntfsDebug(L"Getting drive geometry failed: %u\n",GetLastError());
    return DISK_ERROR;
  }

  geometry->BytesPerSector    = pdg.BytesPerSector;
  geometry->Cylinders         = pdg.Cylinders.QuadPart;
  geometry->SectorsPerTrack   = pdg.SectorsPerTrack;
  geometry->TracksPerCylinder = pdg.TracksPerCylinder;

  return DISK_SUCCESS;
}

/**
 * Initializes a buffer object. The given buffer has to be a multiple
 * of the sector size. Because disks use sectors as atomic size.
 * It is not possible to read only half a sector.
 *
 * @param buffer
 * @param bytes
 * @param size
 * @return
 */
diskReturn_t diskInitBuffer(diskBuffer_t* buffer, uint8_t* bytes, uint16_t size)
{
  buffer->bytes = bytes;

  // Sectors are atomic units for disks. Thus only full multiples of the sector...
  // ...size can be read.
  buffer->sizeInSectors = size / NTFS_BYTES_PER_SECTOR;
  return DISK_SUCCESS;
}

diskReturn_t diskReadRecord(const diskDevice_t* device, diskBuffer_t* buffer, uint8_t sectors)
{
  DWORD  dwBytesRead;
  uint8_t* bytes = buffer->bytes;

  if (!device || (device->handle == INVALID_HANDLE_VALUE))
    return DISK_ERROR_DEVICE_INVALID;
  
  // ensure that we have sufficient buffer memory
  if (sectors > buffer->sizeInSectors)
    return DISK_ERROR;

  // read clusters
  while(sectors--)
  {
    
    if (!ReadFile(device->handle, bytes, NTFS_BYTES_PER_SECTOR ,&dwBytesRead,NULL))
      return diskGetLastError();

    if (diskGetLastError() != DISK_SUCCESS)
      return DISK_ERROR_WIN32;
    
    // as sectors are atoms it's a fatal error if we did not read the full sector
    if (dwBytesRead != NTFS_BYTES_PER_SECTOR)
      return DISK_ERROR_READ_FILE;

    // Advance read/write pointers
    diskSeekRecord(device,1);
    //SetFilePointer(device->handle, NTFS_BYTES_PER_SECTOR, 0, FILE_CURRENT);
    bytes += 512;
  }

  return DISK_SUCCESS;
}

/**
 * Seeks a sector relative to the current position
 * Seek only forward if you need to seek backward, use @see diskSeekRecordEx()
 *
 * @param[in] device  The device which should be used
 * @param[in] sectors The number of sectors to move
 *
 * @return
 */
diskReturn_t diskSeekRecord(const diskDevice_t* device, uint64_t sectors)
{
  LARGE_INTEGER distance;
  distance.QuadPart =  NTFS_BYTES_PER_SECTOR*((uint64_t)sectors);

  if (!device || (device->handle == INVALID_HANDLE_VALUE))
    return DISK_ERROR_DEVICE_INVALID;

  if (!SetFilePointerEx(device->handle, distance, 0, FILE_CURRENT))
    return diskGetLastError();

  if (diskGetLastError() != DISK_SUCCESS)
    return DISK_ERROR_WIN32;

  return DISK_SUCCESS;
}

diskReturn_t diskSeekRecordEx(const diskDevice_t* device, uint64_t offset, diskSeekMethod_t method, uint64_t volumeOffset)
{
  LARGE_INTEGER distance;
  distance.QuadPart = offset * NTFS_BYTES_PER_SECTOR;

  if (!device || (device->handle == INVALID_HANDLE_VALUE))
    return DISK_ERROR_DEVICE_INVALID;

  if (DISK_SEEK_ABSOLUTE == method)
  {
    distance.QuadPart+=(volumeOffset * NTFS_BYTES_PER_SECTOR);
    if (!SetFilePointerEx(device->handle, distance, 0, FILE_BEGIN))
      return diskGetLastError();

    if (diskGetLastError() != DISK_SUCCESS)
      return DISK_ERROR_WIN32;

    return DISK_SUCCESS;
  }
  
  if (DISK_SEEK_BACKWARD == method)
    distance.QuadPart = -1 * distance.QuadPart; 

  if (!SetFilePointerEx(device->handle, distance, 0, FILE_CURRENT))
    return diskGetLastError();

  if (diskGetLastError() != DISK_SUCCESS)
    return DISK_ERROR_WIN32;

  return DISK_SUCCESS;
}

/**
 *
 * @param[in]  device
 * @param[out] position
 *   The current position in sectors
 * @return
 */
diskReturn_t diskCurrentRecord(const diskDevice_t* device, uint64_t* position)
{
  LARGE_INTEGER current;
  LARGE_INTEGER offset;
  offset.QuadPart = 0L;

  if (!device || (device->handle == INVALID_HANDLE_VALUE))
    return DISK_ERROR_DEVICE_INVALID;

  if (!SetFilePointerEx(device->handle,offset,&current,FILE_CURRENT))
    return diskGetLastError();

  if (diskGetLastError() != DISK_SUCCESS)
    return DISK_ERROR_WIN32;

  // we can address and read only full sectors. Thus modulo should always return 0.
  if ((current.QuadPart % NTFS_BYTES_PER_SECTOR) == 0)
    return DISK_ERROR;

  (*position) = (current.QuadPart / NTFS_BYTES_PER_SECTOR);

  return DISK_SUCCESS;
}

