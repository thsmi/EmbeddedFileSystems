/*
 * ntfsVolumeName.h
 *
 *  Created on: 07.02.2013
 *      Author: admin
 */

#ifndef NTFSVOLUMENAME_H_
#define NTFSVOLUMENAME_H_

  #include "utils/diskUtils.h"
  #include "ntfsAttributes.h"

  #pragma pack(push, 1)

  typedef struct {
    /*0x00*/ uint8_t name[0];
  } ntfsAttrVolumeName_t;

  #pragma pack(pop)

  diskReturn_t ntfsGetVolumeNameAttr(const ntfsAttrHdr_t* header, ntfsAttrVolumeName_t** VolumeName);

#endif /* NTFSVOLUMENAME_H_ */
