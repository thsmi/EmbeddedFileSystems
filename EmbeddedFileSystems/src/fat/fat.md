== FAT

FAT stands for File allocation table. 

A FAT Volume contains of 3 basic areas. 
    [ BOOT Area | FAT Area | DATA Area ]
  
    
== Boot Area

Boot Area defines the offset and length of the data and fat area.

=== FAT12, FAT16

  The Boot are consist of just one sector, that contains information on the volume and filessystem
  as well als boot code for the operating system.
  
  The boot sector is always stored on sector 0. the last byte is always 55 AA.

=== FAT32

A fat32 boot record is 6 sectors in size. 

Boot Sector 0 1
FS Info Sector 1 1
Reserved Sector 2 1

Sectors 3-5 are copies of sector 0-2

=== exFat

exFat uses a more complex Boot Area, it is 22 sectors in size.

    Boot Area Layout:    
    [Main Boot Area | Backup Boot Area ]    

As with fat32 it is internally split into two separated areas. The main 
boot area and a backup boot area. The backup boot area is only used when the
main boot area is destroyed (e.g. due to a failed write) and is usually
bit identical. 
  
                          [ Offset | Size ]
  [=======================+========+======]
  [ Boot Sector           |      0 | 0x01 ]
  [ Extended Boot Sector  |      1 | 0x08 ] 
  [ OEM Parameters        |      9 | 0x01 ]
  [ Reserved              |     10 | 0x01 ]
  [ Checksum              |     11 | 0x01 ]  
  
       (Main and backup area layout for exFat)
       
The first 9 sectors (0x00 to 0x08) have a signature of 0x55 0xAA
Sector 11 contains the boot checksum. The 4 byte checksum is reapeaded from the beginning
to the end of the sector. (This value is created during formation the sick ans is kopied to the backup
record.

Sectors 12-22 are a complete backup of the first sectors.        
  
ActiveFat field in Boot Sector defines which FAT and Allocation Bitmap are active.
  
== Fat Area

FAT area stores primary and backup file tables. The file allocation table is large 
list/array. It keeps track which data sector belong to eachother. Thus every data 
sector has a separate entry within the FAT Area. An entry is 12Bit for Fat12, 16Bits 
for Fat16 and 32Bit for FAT32 and exFAT. 

fat32 uses 28bits, exfat all 32bits

    [ Cluster | 0x000  | 0x001 | 0x002 | 0x003 | 0x004 | ... | 0x072 | ... | 0x0AA | ... ]
    [ Value   |  ...   |  ...  | 0x000 | 0x004 | 0x072 | ... | 0xFFF | ... | 0xFF7 | ... ]

The first entry corresponds to the first data cluster, the second one to the second cluster 
and so far so forth. It's build like an extended array. 

In the FAT12 example above, the second (0x002) entry is set to 0x00. This means the second
sector in the data area is unused and can be allocated. 

Let's assume a file starts at entry/data cluster 0x003. Entry 0x003 point to entry 0x004
so the file starts in cluster 0x003 and continues in cluster 0x004. Again the information
for data area cluster 0x004 is stored in the forth entry in the fat. This entry points to 
sector 0x072. So the file continues here. Finally the fat entry for sector 0x072 contains an
end of file mark. This means the file ends in sector 0x072. 

Cluster 0x0AA is marked as bad, it should not be allocated.

The first addressable cluster is cluster 2. 

The entry for Cluster 0 stores the mediatype, it duplicates byte 21 of the FAT Boot Record 
(0xF0 -> removable,  0xF8 -> non removable). The second entry is always set to 0xFF if the disk was ejected sfely.
 
Cluster 1 is used to indicate the dirty status of the filesystem.

                        [  FAT12 | FAT16  | FAT32 & exFAT ]
   [====================+========+========+===============]
   [ unused Cluster     | 0x000  | 0x0000 | 0x?000 0000   ]
   [ reserved Cluster   | 0x001  | 0x0001 | 0x?000 0001   ]
   [            
   [ reserved ...       | 0xFF0
   [     ...  reserved  | 0xFF6     
   [ bad cluster        | 0xFF7  | 0xFFF7 | 0x?FFF FFF7   ]
   [ end of file ...    | 0xFF8  | 0xFFF8 | 0x?FFF FFF8   ]
   [    ... end of file | 0xFFF  | 0xFFFF | 0x?FFF FFFF   ]
   
Note: FAT32 uses only 28 of 32 possible bits, the upper 4 bits should be left alone, the are reserved. 
Typically these bits are zero, and are represented above by a question mark (?). 

0xFF0 - 0xFF6 (Reserved values)
0xFF7 (Bad cluster)
0xFF9 - 0xFFE (Not defined)
0xFFF (Last cluster in file)

The first two sectors are not addressable, as well as the highest 9 sectors.
These values are used to indecate an End Of File or  a bad sector. So FAT12 
can address in total 0xFF5 cluster at maximum, FAT16 is capable of 0xFFF5 
sectors, while FAT32 can address 0xFFFF FFF5 clusters.     

With transactional fat the fat region contains two fat table.
FAT32,16,12 have two fat tables.

With fat12,16 the fat is always mirrored. Any modifications to the primary fat is seamlessly applied 
to the secondary fat.

in FAT32 mirroring can be disabled by clearing bit 0x0080 in extdpb_flas in de Dat32 drive parameter block

== Data Area

The data area stores directory and file content.
 
Data Area:
[ ROOT Directory | DATA    ]
    
Typically it start with ROOT Directory. The Root Directory has fixed length of 32sectors 
in fat12/16 with no more than 512 entries. 

With FAT32 the Root Directory length is no more limited. But each entry still can't be longer 
than 32 bytes. 

The ROOT Directory starts in exFAT and FAT32 at cluster 2, while with FAT12/16 before cluster 2.

=== Allocate Bitmap

exFat use an allocation bitmap to track the used state of all clusters in the data area. 
In FAT12/16/32 the FAT is used as bitmap.

The first bit in the bitmap is the lowest order bit of the first byte.
Bitmaps first bit is cluster2

! entry i root directory!
0x81 entry
texfat contains 2 of these records

Bitmap size in bytes must be a number of clusters in the volume divided by 8 and rounded up.

==== Up-Case Table
0x82 entry in root directory 

contains instructions how to fold/compare
filenames to uppercase. 

Conversions instructions for first 128 Bits are mandatory.

array of unicode characters, 
index = unicoode codepoint which should be upcase
value is the upcase character

[ Index      | 0x0000 | ... | 0x0041 | 0x0042 | ... | 0x0061 | 0x0062 | ... ]
[ Codepoint  |  (\0)  | ... |   A    |   B    | ... |   a    |   b    | ... ]
[ Value(Hex) | 0x0000 | ... | 0x0041 | 0x0042 | ... | 0x0041 | 0x0042 | ... ]
[ value      |  (\0)  | ... |   A    |   B    | ... |   A    |   B    | ... ]

case insensitive filenames plus checksum


Compressed format skips entries which do not need conversion...
Skipped blicks are indecated by 0xFFFF Mark.

[ Value  |                                                   ]
[--------+---------------------------------------------------]
[ 0xFFFF | Compression List Begin Marker                     ]
[ 0x0061 | Starting codepoint (index)                        ]
[ 0x0041 | Codepoint (0x0061) is converted to (0x0041)       ]
[ 0x0042 | Codepoint (0x0061 + 1) is converted to (0x0042)   ]
[ ...    | ...                                               ]
[ 0x005A | Codepoint (0x005A + n) is converted to (0x005A)   ]
[ 0xFFFF | Compression List End Marker                       ]
[ 0x0005 | Checksum                                          ]

0x0000 - 0x0061 -> no conversion

[http://www.ntfs.com/exfat-upcase-table.htm]

=== Directory 
Directories have a one to many relationship between parent and hcild in the directory tree.
The root directory is referenced in the boot record.
Each directory entry is a linked list, which points to other elements.

The can lookup the first cluster o the root directory.
We know the number of sectors per cluster
We know that the bitmaps always starts as data area cluster 2

Example:

RootDirectory Cluster : 0x04
Bitmaps first cluster : 0x02

0x04 - 0x02 = 0x02 -> the bitmap is two cluster long

// Directory:
// a series of 32 byte record, type flag is the first byte
//
// Direcotry Entry -> tracks attributes created, access and modified
// Stream -> tracks size and offset if file, also size of filename
// Filename -> contains unicode characters
//
//  0x85/05 -> Directory Entry Record
// 0x83/03 Volume name Record
//0x82 Up-Case table logical Location and Size
//0x81 Bitmap Locigal location an size
//0xc0 stream extension
// 0xc1 filename extension
//
// first bit is toogled to indecate that entry is unused
// this means a type of 0x85 (allocated directory entry record)
// turns into 0x05 (unallocated directory entry record). A type
// of 0x00 means unused entry. Same applies to extensions
// 0xC0 -> 0x40 and 0xc1 -> 0x41
//
// entry type:
// bit 7: in used ( 0 unused, 1- used)
// bit 6: category (0 -Primary, 1-scondary)
// bit 5: importance (0-critical, 1 benign)
// bit 0-4: type code

/*
 [             Bits                ]
 [ 7 | 6 | 5 ] [ 4 | 3 | 2 | 1 | 0 ]  [ Hex  | Name                               ]
 [================================================================================]
 [ 1 | 0 | 0 ] [ 0 | 0 | 0 | 0 | 1 ]  [ 0x81 | Allocation Bitmap Directroy Entry  ]
 [ 1 | 0 | 0 ] [ 0 | 0 | 0 | 1 | 0 ]  [ 0x82 | Upcase Table / Hashmap             ]
 [ 1 | 0 | 0 ] [ 0 | 0 | 0 | 1 | 1 ]  [ 0x83 | Volume Label                       ]
 [ 1 | 0 | 0 ] [ 0 | 0 | 1 | 0 | 1 ]  [ 0x85 | File                               ]

 [ 1 | 0 | 1 ] [ 0 | 0 | 0 | 0 | 0 ]  [ 0xA0 | Volume GUID                        ]
 [ 1 | 0 | 1 ] [ 0 | 0 | 0 | 0 | 1 ]  [ 0xA1 | TexFAT Padding                     ]
 [ 1 | 0 | 1 ] [ 0 | 0 | 0 | 1 | 0 ]  [ 0xA2 | Windows CE Access Control          ]

 [ 1 | 1 | 0 ] [ 0 | 0 | 0 | 0 | 0 ]  [ 0xC0 | Stream Extension                   ]
 [ 1 | 1 | 0 ] [ 0 | 0 | 0 | 0 | 1 ]  [ 0xC1 | Filename                           ]

*/




== Createing a File

==Deleting a File (dir1\file.txt)

1. Read Fat Boot Sector (sector 0 of the volume) to understand structure and location
of Reserved, FAT, and Data areas

2. Locate "dir1" in Root Directory; determine its starting cluster

3. Go to dir1 cluster; determine starting cluster for file1.txt

4. Set FAT entries for file1.txt to 0

5. Change filename to σile1.txt in dir1 directory. First character becomes 0xE5


directory:

[ file1.txt  |  0x02c ]
[ file2.txt  |   ...  ]

file info can be found in the fat area at cluster 0x2c

Fat:

[ Cluster | 0x000  | 0x001 | ... | 0x02C | 0x02D | 0x02E | ... | ... ]
[ Entry   |  ...   |  ...  | ... | 0x02D | 0x02E | 0xFFF | ... | ... ]


file extends from data cluster 0x02D to 0x02D and ends at 0x02E


== exFAT

Pros:
Free space bitmap
faster deletes fast lookup for free sectors see ntfs

Larger files
file size filed is 8bytes -> 2^64 bytes

Larger Clusters
clusters can be upto 2^255 bytes. current limit is 32 MB

Name hashes
exFAT uses hashing mechanisms in order to speed up file access. 
2 byte checksum of upcased filenames. Allows fast filename cheks.

Supports Access control lists
no implementation exists

Support for trsaction-safe fat 
currently windowsce only

Cons:
exFat is not backwart compatible to FAT32
no support for short names


Default cluster size in windows:

Volume size   Clustersize
7MB-256MB     4KB
256MB-32GB    32KB
32GB-256TB    128KB
>256TB        Not supported



