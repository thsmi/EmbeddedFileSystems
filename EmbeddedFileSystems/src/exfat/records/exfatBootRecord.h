/*
 * exfatVolumeBootRecord.h
 *
 *  Created on: 13.01.2013
 *      Author: admin
 */

#ifndef EXFATBOOTRECORD_H_
#define EXFATBOOTRECORD_H_

#include "./../../utils/diskUtils.h"


typedef struct {
  uint8_t jump[3];  //0xEB7690
  uint64_t OEM_ID;  //"EXFAT  " 0x45 58 46 41 54 20 20 20
  uint8_t reserved[53]; // must be zeroed, fat12/16/32 Boot Records would start here. Helps to prevent FAT12/16/32 implementations from mistakenly mounting an exFAT volume
  struct {
    uint64_t offset; // partition's first sector (e.g. 02 00 = 8192)
    uint64_t size; // partition size in sector  (Fe.g. 4 40 00 = 16007168 *512 = 8195670016 = 7.6GB
  } partition;
  struct {
    uint32_t offset; // Sector address of first FAT
    uint32_t size; //Size of FAT in Sectors;
  } fat;
  struct {
    uint32_t offset; // Sector address of Data Region (Data Area)
    uint32_t size; // Number of clusters in Cluster Heap (allocated clusters)
  } cluster;
  uint32_t rootDirectory; // Cluster address of the Root Directory
  uint32_t serialNumber; // Volume serial number
  struct {
    uint8_t major; // 1
    uint8_t minor; // 0
  } version;
  uint16_t flag;             // volume flags see below
  uint8_t bytesPerSector;    // is a power of 2 (9 =2^9 =512, 12 = 2^12 = 4096)
  uint8_t sectorsPerCluster; // power of 2 (bytes per sector + sectors per cluster < 25, as max clustersize = 32MiB)
  uint8_t numberOfFAT;       // Usually 1, 2 for texFAT (trasactional fat)
  uint8_t driveselect; //Used by int13 for bootstrapping  0x80
  uint8_t heapusage; //percentage of heap used / allocated space 0x01 = 1%, 0xff not available
  uint8_t reserved2[7];
  uint8_t bootCode[390];
  uint16_t endOfSector; // 0x55 0xAA
  // padding is sector > 512bytes
} exFatBootRecord_t;


// 2nd FAT and Bitmap is active , only possible when NumberOfFats is 2
#define EXFAT_BOOTFLAG_SECOND_FAT_ACTIVE (1<<0)
// Set if volume is inconsistent/dirty
#define EXFAT_BOOTFLAG_VOLUME_DIRTY      (1<<1)
// Not set if all failures are reported as bad sectors. Set if failures
// are detected but not fixed, e.g. by marking sector as bad
#define EXFAT_BOOTFLAG_MEDIA_FAILURE     (1<<2)
// No usefull Meaning
#define EXFAT_BOOTFLAG_CLEAR_TO_ZERO     (1<<3)

// FAT Entry:
// List of 4 byte entries. Terminated by a  0xFFFFFFFF (EOF). Unused entries are marked by 0x00000000
// 0xFFFFFFF7 - bad block
// 0xFFFFFFF8 - media descriptor
// 0xFFFFFFF9- =xFFFFFFFE undefined
//
//  a value of 0x000000FC means the next fragment can be found at the 252nd allocation unit;
//

// Bitmap:
// Determines if cluster is empty or not. One bit per cluster
//
// If only first cluster is used bitmap value would be 0x01. If first and eight is used value would be 0x81



#endif /* EXFATVOLUMEBOOTRECORD_H_ */
