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

#ifndef _INC_DISK_STATUS_H
  #define _INC_DISK_STATUS_H

  typedef enum {
    DISK_SUCCESS,
    DISK_ERROR,
    DISK_ERROR_READ_RECORD,
    DISK_ERROR_READ_FILE,
    DISK_ERROR_SIGNATURE_FILE,
    DISK_ERROR_SIGNATURE_INDX,
    DISK_ERROR_SIGNATURE_BOOT,
    // wrong attribute type
    DISK_ERROR_ATTRIBUTE_TYPE,
    // attribute is nonresident but should be resident
    DISK_ERROR_ATTRIBUTE_NONRESIDENT,
    DISK_ERROR_ATTRIBUTE_RESIDENT,
    // attribute's name is wrong
    DISK_ERROR_ATTRIBUTE_NAME,
    DISK_ERROR_ATTRIBUTE_INVAILD,

    // Device handle is invalid
    DISK_ERROR_DEVICE_INVALID,
    DISK_ERROR_WIN32 /** a Windows API call failed */
  } diskReturn_t;

#endif
