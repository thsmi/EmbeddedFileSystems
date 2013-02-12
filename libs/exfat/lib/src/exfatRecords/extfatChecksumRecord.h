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

#ifndef EXFAT_CHECKSUM_RECORD_H_
  #define EXFAT_CHECKSUM_RECORD_H_

  struct {
    uint32_t checksum[128];
  } exfatChecksumRecord_t;

  // Bootcheksum
  // 12 Sector of boot an backup boot

  // Sectors 0-10 : Boot Record
  // Sector    11 : Contains 4 Repetive bytes (checksum of the boot record, flags and no percentage is ignored)
  // Sectord 12-22 : Backup Boot record
  // Sector   23 : Checksum ...


  /* boot sectors Volume flags byte 106,107 as well as percent used (112) are skipped */

  uint32_t exfatCalcBootAreaChecksum(const uint8_t data[], uint32_t length)
  {
    uint32_t checksum = 0;
    uint32_t i = 0;

    for (uint32_t = 0; i < length; i++)
    {
      if (i == 106 || i == 107 || i == 112)
        continue;

      checksum = (checksum<<31) | (checksum>> 1) + data[i];
    }

    return checksum;
  }

#endif
