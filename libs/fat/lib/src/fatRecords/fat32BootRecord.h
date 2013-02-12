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

#ifndef FAT32_BOOT_RECORD_H_
  #define FAT32_BOOT_RECORD_H_

  #include "disk/diskRecord/diskRecord.h"
  #include "fatBootRecord.h"

  #pragma pack(push, 1)

/*Für Fat32
  0x24 Number of Sectors  4Byte
  0x28 Flags              2Bytes
  0x2A Version            2Bytes (Hi Byte = Major, Low Byte = Minor)
  0x2C Cluster of the Rootdirectory 4 Bytes
  0x30 SectorNumberOfFileSystemInformation 2 Bytes
  0x32 Sector Bumber of backup boot sector 2 Bytes
  0x34 Reserved           12 Bytes
  0x40 Logical Byte Number of Partition 1Byte
  0x41 ??? 1Byte
  0x42 Extebded Signature (0x29) 1 Byte
  0x43 SerialNumberOfPartition 4 Bytes
  0x47 VolumeNameOfPartiiton 11 Byte
  0x53 FatName (FAT32) 8 Bytes
  0x5A Executabe Code 420 Bytes
  0x01FE Signature/End of boot record 2 Bytes*/

  typedef struct {
    fatBootRecordHeader_t header;
    /*0x0024*/ uint32_t sectors; // number of sectors per fat
    /*0x0028*/ uint16_t flags;   // flags
    struct {
      /*0x002A*/ uint16_t major; 
      /*0x002B*/ uint16_t minor;
    } version;
    /*0x002C*/ uint32_t rootDirectory; // Cluster of the Root Directory
    /*0x0030*/ uint16_t fileSystemInformation; // Number of File System Information Sectors
    /*0x0032*/ uint16_t backupBootSectors; // Number of backup sectors
    /*0x0034*/ uint8_t reserverd[12];
    /*0x0040*/ uint8_t logicalByteNumber; // logical Byte Number of this partition 
    /*0x0041*/ uint8_t XXX; 
    /*0x0042*/ uint8_t signature; // Embedded Signature (0x29)
    /*0x0043*/ uint32_t serialNumber;
    /*0x0047*/ uint8_t volumeName[11];
    /*0x0053*/ uint64_t fatName; // FAT32
    /*0x005A*/ uint8_t bootCode[420];
    /*0x01FE*/ uint16_t endOfSector;
  } fat32BootRecord_t;


/*FSInfo Sector (Fat32) 
  direkt nach dem ersten Sektor (BIOS Parameter Block)
 
  0x00 Signature for FSIno Sector 0x52 0x52 0x61 0x41 4 Bytes
  0x04 Reserved 480 Bytes
  0x1E4 Signature for Start of FSINFo Data 0x73 0x73 0x41 0x41) 4 Bytes
  0x1E8 Number of free Clusters (-1 = FF FF FF FF= unkonwn) 
  0x1EC ClusterMostRecently Allocated 4 Bytes
  0x1F0 Reserved 14 Bytes
  0x01FE End of FSInfoSector (0x55 0xAA) 2 bytes*/
  typedef struct {
    /*0x0000*/ uint32_t signature; //0x52 0x52 0x61 0x41
    /*0x0004*/ uint8_t reserved[480];
    /*0x01E4*/ uint32_t signature2; //0x73 0x73 0x41 0x41
    struct {
      /*0x01E8*/ uint32_t free;
      /*0x01EC*/ uint32_t mostRecentlyAllocated;
    } cluster;
    /*0x01F0*/ uint8_t reserved2[14];
    /*0x01FE*/ uint16_t endOfSector;
  } fat32FSInfoRecord_t;

  #pragma pack(pop)

  diskReturn_t fat32ReadBootRecord(diskDevice_t* hDevice, diskBuffer_t* buffer, fat32BootRecord_t** record);
  diskReturn_t fat32ReadFSInfoRecord(diskDevice_t* hDevice, diskBuffer_t* buffer, fat32FSInfoRecord_t** record);

#endif
