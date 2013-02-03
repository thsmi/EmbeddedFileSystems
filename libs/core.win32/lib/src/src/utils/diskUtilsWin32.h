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

#ifndef DISKUTILSWIN32_H_
#define DISKUTILSWIN32_H_

#include "utils/diskUtils.h"

diskReturn_t ntfsMemCpy(diskBuffer_t* buffer, uint8_t* data, uint32_t* length);

#endif /* DISKUTILSWIN32_H_ */
