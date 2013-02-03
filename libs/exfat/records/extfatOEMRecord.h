/*
 * extfatOEMRecord.h
 *
 *  Created on: 15.01.2013
 *      Author: admin
 */

#ifndef EXTFATOEMRECORD_H_
#define EXTFATOEMRECORD_H_


/* OEM Parameter Record

 a list of oem parameters, 48 bytes long, staring with a 16byte guid

 0x00 / 48 bytes / Parameter [0]
...
 0x1B0 / 48 byte / Parameter [9]
 0x01E / 2^BytesPerSectorshift - 480 / reserved


  /*0x00 / 16 bytes long / OEM Defined GUID, GUID_NULL indecates that parameter value is not used*/
  /*0x10 / 32 bytes long / OEM Specific

Example:

#define OEM_FLASH_PARAMETER_GUID 0A0C7E46-3399-4021-90C8-FA6D389C4BA2
struct
{
GUID OemParameterType;     //Value is OEM_FLASH_PARAMETER_GUID
UINT32 EraseBlockSize;     //Erase block size in bytes
UNIT32 PageSize;
UNIT32 NumberOfSpareBlocks;
UNIT32 tRandomAccess;     //Random Access Time in nanoseconds
UNIT32 tProgram;     //Program time in nanoseconds
UNIT32 tReadCycle;     //Serial read circle time in nanoseconds
UNIT32 tWriteCycle;     //Write Cycle time in nanoseconds
UCHAR Reserved[4];
}
FlashParameters;
*/

typedef struct {
  struct {
    GUID guid;
    uint8_t undefined[32]; // OEM Specified content
  } parameter[10];
  uint8_t reserved1;
} exfatOEMRecord_t;

#endif /* EXTFATOEMRECORD_H_ */
