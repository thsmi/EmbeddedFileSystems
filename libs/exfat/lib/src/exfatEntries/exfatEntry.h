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

#ifndef EXFAT_ENTRY_H_
  #define EXFAT_ENTRY_H_

  #include "utils/diskDataTypes.h"

  // The Checksum is computed across all entries in the set, excluding the checksum itself.

  uint16_t exfatCalcEntrySetChecksum(const uint8_t octets[], uint32_t numberOfBytes)
  {
    uint16_t checksum = 0;
    uint32_t index;

    for (index = 0; index < numberOfBytes; index++)
    {
      if (index == 2 || index == 3)
        continue;

      checksum = ((checksum <<15) | (checksum>> 1)) + (uint16_t) octets[index];
    }

    return checksum;
  }

#endif
