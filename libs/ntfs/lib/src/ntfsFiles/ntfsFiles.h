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

#ifndef NTFS_FILES_H_
  #define NTFS_FILES_H_

  #include "disk/diskRecord/diskRecord.h"

  #include "../ntfsVolume/ntfsVolume.h"
  #include "ntfsFiles.h"

  #include "../ntfsRecords/ntfsFileRecord.h"
  #include "../ntfsDataRuns/ntfsDataRuns.h"
  #include "../ntfsAttributes/ntfsAttributes.h"
  #include "../ntfsAttributes/ntfsFileName.h"

  /**
   * Notes on NTFS Files:
   * All directories are files. Thus it's perfectly fine having data and alternative data streams.
   */

  typedef struct ntfsFileHandleTag_t {
    ntfsFileReference_t mftRecord;
    const ntfsVolume_t* volume;
  } ntfsFileHandle_t;


  /**
   * Upon every file found the callback is invoked.
   *
   * Callbacks are invoked until either all child nodes are enumerated or a callback succeeds (returns DISK_SUCCESS).
   *
   * Do not never invoke any other functions from this library within the callback.
   * Do not store references to any of the parameters. The parameters will be reused and overwritten.
   *
   * To store information use the data pointer and process it after ntfsListFiles succeeded.
   *
   * @param[in] mftReference
   *   A reference to the MFT Record, for this search result
   *
   * @param[in] file
   *   Contains information like the filename, parent folder etc..
   *
   * @param[in,out] data
   *   An user defined pointer to retrieve and store data during the callback.
   *
   * @return
   *   If you found the child you were looking for return DISK_ERROR to stop any further enumeration.
   *   Returning DISK_SUCCESS is equivalent to "Child not found and continue enumeration".
   */
  typedef diskReturn_t (*ntfsNextFileCallback_t)(ntfsFileReference_t mftReference, const ntfsAttrFileName_t* file, uint8_t* data);

  /**
   * Lists all children (files and directories) bound to this file handle.
   * For every child a callback is invoked.
   *
   * @param volume
   * @param file
   *   a handle to the parent directory or file. The root directory has no parent, thus pass null to list it.
   * @param mftBuffer
   *   a buffer for parsing the MFT, typically 1KB
   * @param idxBuffer
   *   buffer for parsing the index files, typically 4KB
   * @param callback
   *   the callback function which should be invoked for every child found.
   * @param data
   *   an user defined pointer which will be passed to the callback function upon callback.
   *   It can be used to store and persist data.
   * @return
   */
  diskReturn_t ntfsListFiles(const ntfsVolume_t* volume, const ntfsFileHandle_t* file,
      diskBuffer_t* mftBuffer, diskBuffer_t* idxBuffer, ntfsNextFileCallback_t callback, uint8_t* data);

  /**
   * Reads a file form disk into memory.
   *
   * You have to read always full sectors. It sounds awkward and like wasting memory, but
   * a hard drive's minimum read chunk is a sector. Thus reading just one byte or a full
   * sector requires in both cases exactly the same amount of memory. That's why this read
   * function works with sectors instead of bytes
   *
   * @param volume
   *   a reference to the volume on which the file is located
   * @param file
   *   a reference to the file which should be read
   * @param ioBuffer
   *   internal cache for reading the MFT and processing data runs, should be 2 Sectors in size.
   * @param dataBuffer
   *   the buffer where the data read should be stored.
   * @param length
   *   how many sectors should be read
   * @param offset
   *   an offset in sectors, where to start reading.
   * @param slack[in,out]
   *   Tracks the slack while reading the file. Slack is the amount of unused bytes within the buffer.
   *   A slack bigger than 0 indicates less data could be read than requested and most likely the end
   *   of file was reached.
   *
   *   In case you don't need this information pass null.
   * @return
   *
   */
  diskReturn_t ntfsReadFile(
      const ntfsVolume_t* volume, const ntfsFileHandle_t* file,  diskBuffer_t* ioBuffer,
      diskBuffer_t* buffer2, uint64_t length, uint64_t offset, uint64_t* slack);


#endif
