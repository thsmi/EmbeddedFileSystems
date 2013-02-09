/*
 * exfatEntry.h
 *
 *  Created on: 15.01.2013
 *      Author: admin
 */

#ifndef EXFATENTRY_H_
#define EXFATENTRY_H_

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

#endif /* EXFATENTRY_H_ */
