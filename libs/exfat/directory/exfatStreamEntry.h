/*
 * exfatStreamEntry.h
 *
 *  Created on: 15.01.2013
 *      Author: admin
 */

#ifndef EXFATSTREAMENTRY_H_
#define EXFATSTREAMENTRY_H_

// Stream Extension

/* Flags:
 *   (1<<0) Alloc Possible, must be 1
 *   (1<<1) NoFatChain
 */

typedef struct {
  /*0x00*/ uint8_t type; // 0xC0 or 0x40
  /*0x01*/ uint8_t flags; //alloc possible/fat invalid
  /*0x02*/ uint8_t reserved1;
  struct {
    /*0x03*/ uint8_t lenght; // numbe of unicode characters in filenname;
    /*0x04*/ uint16_t hash; // filenam hash
  } name;
  /*0x06*/ uint8_t reserved2[2];
  /*0x08*/ uint64_t size; // initialized File size in Bytes >0 and < logical size
  /*0x10*/ uint8_t resereved3[8];
  /*0x14*/ uint32_t cluster; // Starting cluster of file;
  /*0x18*/ uint64_t logicalSize; // logical size of file;
} exfatStream_t;

/*
Stream etension entry must be immediately followed by th efile director entry.
There can't be more than one stream extenion  entry in the tset.


If NofatChain flag is set all allocated clusters are contignous*/
/*size determines how much data is written to the disk. You need to
update this on every write. Any data beyond size is replaced by zeros*/

// fileName points to up-cased file name

uint16_t exfatCalcFileNameHash(uint16_t *fileName, uint8_t nameLength)
{
  uint16_t hash = 0;
  uint8_t *data = (uint8_t*) fileName;

  for ( uint16_t i = 0; i<nameLength*2; i++)
    hash = (hash << 15) | (hash >> 1) + data[i];

  return hash;
}

#endif /* EXFATSTREAMENTRY_H_ */
