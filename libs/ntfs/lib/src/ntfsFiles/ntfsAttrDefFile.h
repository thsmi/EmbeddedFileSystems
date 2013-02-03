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

#ifndef NTFSATTRDEFFILE_H_
#define NTFSATTRDEFFILE_H_

/**
 * Attribute definition contains a list containing a description for every attribute
 * supported by this volume.
 *
 * The attribute list is stored in the unnamed $Data stream. In theory it could be
 * resident, but as even old ntfs implementations rely on numerous list of attributes
 * it is always to big to be stored resident.
 *
 * Ever AttDef entry is 120Bytes in size, thus $data length has to be always a multiple
 * There is no End Of list mark, you need to calculate the number of entries on you own.
 *
 */

  #include "utils/diskDataTypes.h"
  #include "utils/diskUtils.h"

/*$ATTRDEF

0x00-0x7F Name of Attribute
0x80-0x83 Type
0x84-0x87 Display Rule
0x88-0x8B Collation Rule (How to be sorted when attribut is an index)
0x8C-0x8F Flags
  0x02 = can be used in index records
  0x40 = always resident
  0x80 = can be non resident
0x90-0x97 Minimum Size
0x98-0x9F Maximum Size*/

/*
 * $AttrDef file contains
 *   $Standard_Information
 *   $Filename is always $AttrDef
 *   $Security_descriptor
 *   $Data [unnamed]
 */

// Collation Rule
#define NTFS_ATTRDEF_COLLATION_BINARY 0x00
#define NTFS_ATTRDEF_COLLATION_FILENAME 0x01
#define NTFS_ATTRDEF_COLLATION_STRING 0x02
#define NTFS_ATTRDEF_COLLATION_LONG 0x10
#define NTFS_ATTRDEF_COLLATION_SID 0x11
#define NTFS_ATTRDEF_COLLATION_HASH 0x12
#define NTFS_ATTRDEF_COLLATION_MULTIPLE_LONG 0x13

// Flags
#define NTFS_ATTRDEF_FLAGS_INDEXED     0x02
#define NTFS_ATTRDEF_FLAGS_RESIDENT    0x40
#define NTFS_ATTRDEF_FLAGS_NONRESIDENT 0x80

typedef struct {
  /*0x00*/ uint16_t name[64];
  /*0x80*/ uint32_t type;
  /*0x84*/ uint32_t displayRule;
  /*0x88*/ uint32_t collationRule;
  /*0x8C*/ uint32_t flags;
  struct {
    /*0x90*/ uint64_t minumum;
    /*0x98*/ uint64_t maximum;
  } size;
} ntfsAttrDef_t;

diskReturn_t ntfsNextAttrDef(const diskBuffer_t* buffer, ntfsAttrDef_t** attr);

#endif /* NTFSATTRDEFFILE_H_ */
