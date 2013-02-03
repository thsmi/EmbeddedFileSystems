NTFS

New Technology file system

== alternate data streams

history mac: executable and resources...

To create an ADS named foo to go with
the file.txt file, use the following DOS
command

echo "Hello There" > file.txt:foo 


start calc.exe:notepad.exe


=== 

concepts every thing are files
files have attributes

$FILE_NAME
$DATA (the actual data)
...


Filenames starting with $ are administrative and hidden.
$MFT the master file table contains a list with all files.

== Master File Table

Mft is a database containing information about all files and
directories. Every file and directory has at least one
entry in the table
 
Each entry is simple max 1 KB in size. Entry header is first 42 bytes.
Remaining bytes store attributes. 

Small files >500 KB are typically stored directly in the $MFT. This means
their attributes are stored as resident attributes. In case an $MFT entry 
grows bigger tha 1kb. Resident attributes are converted into non-resident.
This means they store their content in externals clusters. So called
dataruns are used to reference non-resident attributes form the MFT. 
Dataruns itself are always resident, they are tiny and store only the cluster
address and the run length. 

Typical non resident $Data attibutes
  $DATA attribute for files > 1 KB
  $DATA attribute for $Boot
  $DATA attribute for $MFTMirr
  $DATA attribute for $LogFile 
  
  
Boot sector is a file and contains the location of the master file table as well as the Master file table mirror.
The mirror of the master file table contains only the first special entries.

The MTFs first entries are reserverd for $LogFile $Bitmap $Boot...


== Creating an NTFS File
Creating dir1\file1.txt

1. Read volume boot sector to locate MFT.

2. Read first entry in MFT to determine layout of MFT.

3. Allocate a MFT entry for the new file.

4. Initialize MFT entry with $STANDARD_INFORMATION, In-Use Flag,  etc.

5. If data does not fit into MFT, Check MFT $Bitmap to find free clusters, using best-fit algorithm.
   Depending on the file size and on the disk's fragmentation level, it might not be possible to
   store the file contiguous. Create for every fragment a datarun.

6. Set corresponding $Bitmap bits to 1.

7. Write file content to clusters and add dataruns to $DATA attribute. Every datarun consists of  
  a starting address and a run length.

8. Read root directory (MFT entry 5, $ROOT or . ), traverse index, and find dir1.

9. Read $INDEX_ROOT attribute for dir1 and determine where file1.txt should go.

10. Create new index entry; resort index tree. If you index entry grows too big you might
    move attributes into dataruns.
    
11. Enter steps in $LogFile (as each step is taken).  

== Deleting an NTFS File
Deleting dir1\file1.txt

1. Read volume boot sector to locate MFT.

2. Read first entry in MFT to determine layout of MFT.

3. Read root directory (MFT entry 5), traverse index, and find dir1.

4. Read $INDEX_ROOT for dir1 entry and find file1.txt entry.

5. Remove filename entry from index; move other entries over.
   If possible use free space in MFT entry and make other non-resident attributes resident.

6. Unallocate MFT entry and clean In-Use Flag.

7. Set MFT $Bitmap entries to 0.

8. Enter steps in $LogFile (as each step is taken). 


== creating / deleting files

 * Creation:
   * Bitmap file in MFTupdated.
   * Index entry created to point to file.
   
 * Deletion:
   * Bitmap file changed.
   * File remains on disk until overwritten.
   * Allows for reconstruction.
   
 ---
 
 NTFS has three size fields
    
   [<-- logical size              -->|
   |<-- initialized size -->|
   
   [XXXXXXXXXXXXXXXXXXXXXXXX|xxxxxxxx|.......]
   
   |<--             Physical size         -->|
   
 --- jsournals USN_RECORD
 
 typedef struct {
   DWORD RecordLength
   WORD  MajorVersion;
   DWORD MinorVersion;
   DWORDLONG FileReferenceNmber;
   DWORDLONG ParentFileReferenceNumber;
   USN usn;
   LARGE_INTEGER Timestamp;
   DWORD reason;
   DWORD SourceInfo;
   DWORD SecurityId;
   DWORD FileAttributes;
   WORD FileNameLength;
   WORD FileNameOffst;
   WCHAR Filename[1];
 } ntfsUsnRecord_t
 
 http://www.slideshare.net/null0x00/ntfs-forensics
 
 
 
 
 
 
