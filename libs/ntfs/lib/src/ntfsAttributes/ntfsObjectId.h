/*
 * ntfsObject.h
 *
 *  Created on: 08.02.2013
 *      Author: admin
 */

#ifndef NTFSOBJECTID_H_
#define NTFSOBJECTID_H_

  #include "utils/diskUtils.h"

  typedef struct {
    /*0x00-0x0F*/ GUID objectId;
    struct {
      /*0x10-0x1F*/ GUID volumeId; /*optional*/
      /*0x20-0x2F*/ GUID objectId; /*optional*/
      /*0x30-0x3F*/ GUID domainId; /*optional*/
    } birth;
  } ntfsAttrObjectId_t;

#endif /* NTFSOBJECTID_H_ */
