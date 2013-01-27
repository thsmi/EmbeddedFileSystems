/*
 * file.h
 *
 *  Created on: 13.01.2013
 *      Author: admin
 */

#ifndef FILE_H_
#define FILE_H_

#include "../disk/diskRecord/diskRecord.h"

#include "../volume/ntfsVolume.h"
#include "../file/ntfsFiles.h"

#include "../ntfs/ntfsRecords/ntfsFileRecord.h"
#include "../ntfs/ntfsDataRuns/ntfsDataRuns.h"
#include "../ntfs/ntfsAttributes/ntfsAttributes.h"
#include "../ntfs/ntfsAttributes/ntfsFileName.h"

// directorys and files are files
// -> ntfs alternate datastreams are files that contain files...

  typedef struct {
    ntfsFileReference_t mftRecord;
  } ntfsFileHandle_t;

  // Read the MBR and parses it important infos, secors size MFT location etc are
  // cached in the colume handle;


  diskReturn_t ntfsInitHandle(const ntfsVolume_t* volume, uint8_t* buffer, ntfsFileHandle_t* file);


  /**
   * Upon every file found the callback is invoked.
   * Do not never invoke any operations from this library within the callback.
   *
   * Use instead the data pointer to store the information you need.
   *
   * @param[in]     mftReference
   * A reference to the MFT Record, for this search result
   *
   * @param[in]     file
   * Contains information like the filename, parent folder etc..
   *
   * @param[in,out] data
   * An user defined pointer to retrieve and store data during the callback.
   *
   * @return
   */
  typedef diskReturn_t (*ntfsNextFileCallback_t)(ntfsFileReference_t mftReference, const ntfsAttrFileName_t* file, uint8_t* data);

  diskReturn_t ntfsListFiles(const ntfsVolume_t* volume, const ntfsFileHandle_t* file, diskBuffer_t* buffer, ntfsNextFileCallback_t callback, uint8_t* data);

  /**
   *
   * Reads a file. You have to read full sector.
   * The minimum read chunk of a Harddrive is a sector.
   *
   * @param volume
   * @param file
   * @param ioBuffer
   *   internal cache for reading the MFT and processing Dataruns, should be 2 Sectors in size.
   * @param dataBuffer
   *   the buffer where the data read should be stored.
   * @param length
   *   how many sectors should be read
   * @param offset
   *   an offset in sectors, where to start reading.
   * @return
   */
  diskReturn_t ntfsReadFile(
      const ntfsVolume_t* volume, const ntfsFileHandle_t* file,  diskBuffer_t* ioBuffer,
      diskBuffer_t* buffer2, uint64_t length, uint64_t offset);

/*
.\
.\file1.txt
.\file2.txt
.\folder
.\folder\abc
.\file3.txt


handle1
handle2
handle3

fsInitHandle(buffer,&handle);
fsInitHandle(buffer2,&childhandle);

ntfsGetRoot(device, &handle1);

ntfsNextFile(&handle1, &handle2); // -> file1.txt
ntfsNextFile(&handle1, &handle2); // -> file2.txt
ntfsNextFile(&handle1, &handle2); // -> folder
  ntfsNextFile(&handle2, &handle3); // -> abc
  ntfsReadFile(&handle3, &buffer,  size);
  ntfsNextFile(&Handle2, &handle3); // return DISK_ERROR_INVALID_FILE
ntfsNextFile(&handle1, &handle2); // -> file3.txt
ntfsNextFile(&handle1, &handle2); // return DISK_ERROR_


fsGetFile(null, &handle); -> .
  fsNextFile(handle); -> file1.txt
  fsNextFile(handle); -> file2.txt
  fsNextFile(handle); -> folder
    fsGetFile(handle, &ChildHandle);
      fsNextFile(childhandle); -> abc
      fsReadFile(chuldHandle, &buffer3, size)
      fsNextFile(childHandle2);  DISK_ERROR_INVALID
  fsNextFile(handle); -> file3.txt
  fsNextFile(handle); DISK_ERROR_INVALID
*/

#endif /* FILE_H_ */
