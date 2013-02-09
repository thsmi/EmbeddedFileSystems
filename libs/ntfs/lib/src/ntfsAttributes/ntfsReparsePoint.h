/*
 * ntfsReparsePointAttr.h
 *
 *  Created on: 07.02.2013
 *      Author: admin
 */

#ifndef NTFSREPARSEPOINT_H_
#define NTFSREPARSEPOINT_H_

typedef struct {
  /*0x00-0x03*/ uint32_t type; // Reparse type
  /*0x04-0x05*/ uint16_t size; //SizeOfReparseData
  /*0x06-0x07*/ uint16_t reserved1;
  struct {
    /*0x08-0x09*/ uint16_t offset; //  to target name
    /*0x0A-0x0B*/ uint16_t length; // of target name
  } targetName;
  struct {
    /*0x0C-0x0D*/ uint16_t offset; // to print name of target
    /*0x0E-0x0F*/ uint16_t length; // of print name
  } printName;
} ntfsAttrReparsePoint_t;


#endif /* NTFSREPARSEPOINT_H_ */
