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

#include "ntfsDataRuns.h"

// Define datarun's internal structure internally, we don't need it somewhere else
struct ntfsDataRunTag_t {
  uint8_t header;
  uint8_t content[0];
};

#define ntfsDataRunHeaderLenght(val) ((uint8_t)(((val)->header >> 4) & 0x0F) + (uint8_t) ((val)->header & 0x0F) +1)

static diskReturn_t ntfsGetDataRunLength(const ntfsDataRun_t* dataRun, uint64_t* length)
{
  uint8_t len = 0;
  uint8_t* x = NULL;

  if (!length)
    return DISK_ERROR;

  if (!dataRun->header)
    return DISK_ERROR;

  len = (uint8_t) (dataRun->header & 0x0F);
  // we can't handle more than 64 Bits
  if (len > 8)
    return DISK_ERROR;

  // Skip header with length info
  (*length) = 0x0000000000000000;

  x = (uint8_t*)length+len-1;

  while (len > 0)
  {
    x[0] = dataRun->content[--len];
    x--;
  } 

  /*x = (uint8_t*)length;
  while (len > 0)
  {
    x[0] = dataRun->content[--len];
    x++;
  }*/
  return DISK_SUCCESS;
}

static diskReturn_t ntfsGetDataRunOffset(const ntfsDataRun_t* dataRun, uint64_t* offset, diskSeekMethod_t* method)
{
  const uint8_t* buff;
  uint8_t len = 0;
  uint8_t* x = 0;

  if (!offset)
    return DISK_ERROR;

  if (!method)
    return DISK_ERROR;

  len = (uint8_t)((dataRun->header >> 4) & 0x0F);

  // we can't handle more than 64 Bits
  if (len > 8)
    return DISK_ERROR;

  // Skip DataRun header and the length information...
  //buff = ((uint8_t*)&dataRun->content[0]) + (dataRun->header & 0x0F);
  buff = dataRun->content + (dataRun->header & 0x0F);
 
  ntfsHexDump(buff,len);

  // prepare everything
  (*offset) = 0x0000000000000000LL;
  // clear backward flag

  // To make things easier we initialize negative offset with Fs and positives with Zeros
  if (buff[len-1] > 0x7F)
  {
    // Correct the seek method...
    (*method) = DISK_SEEK_BACKWARD;
    (*offset) = 0xFFFFFFFFFFFFFFFFLL;
  }

  // then swap the byte order
  /*x = (uint8_t*)offset;
  while (len > 0)
  {
    x[0] = buff[--len];
    x++;
  }*/


  // little Endian to big endian
  // now swap the byte order. A direct conversion is 
  // not possible as we have different lengths
  //
  //Big endian: first byte = most significant bit

  x = (uint8_t*)offset+len-1;

  while (len > 0)
  {
    x[0] = buff[--len];
    x--;
  }
  
  ntfsHexDump((uint8_t*)offset,8);

  // we can skip here if it's an absolute or forward move...
  if ( (*method) != DISK_SEEK_BACKWARD )
    return DISK_SUCCESS;

  // ... negative offset means more work, we need to calculate the twos complement
  (*offset) = ((*offset) ^ 0xFFFFFFFFFFFFFFFFLL) +1;

  ntfsHexDump((uint8_t*)offset,8);

  return DISK_SUCCESS;
}

diskReturn_t ntfsGetDataRun(const ntfsDataRun_t* dataRun, uint64_t* length, uint64_t* offset, diskSeekMethod_t* method)
{
  if ( !dataRun )
    return DISK_ERROR;

  if ( !dataRun->header )
    return DISK_ERROR;

  if ( !method )
    return DISK_ERROR;

  ntfsHexDump((uint8_t*)dataRun,10);

  // first start with the length calculation 
  if (length && (ntfsGetDataRunLength(dataRun,length) != DISK_SUCCESS))
    return DISK_ERROR;

  if (offset && (ntfsGetDataRunOffset(dataRun,offset,method) != DISK_SUCCESS))
    return DISK_ERROR;

  return DISK_SUCCESS;
}

diskReturn_t ntfsNextDataRun(const ntfsAttrHdr_t* attr, ntfsDataRun_t** dataRun, diskSeekMethod_t* method)
{
  if ( !attr->isNonResident )
    return DISK_ERROR_ATTRIBUTE_RESIDENT;

  if (method)
    (*method) = (*dataRun) ? DISK_SEEK_FORWARD : DISK_SEEK_ABSOLUTE;

  if (*dataRun == NULL)
    (*dataRun) = (ntfsDataRun_t*) ntfsOffset(attr,attr->attrHeader.nonResident.dataRunOffset);
  else
    (*dataRun) = (ntfsDataRun_t*) ntfsOffset((*dataRun),ntfsDataRunHeaderLenght(*dataRun));
 
  if (!(*dataRun)->header)
    return DISK_ERROR;

  return DISK_SUCCESS;
}
