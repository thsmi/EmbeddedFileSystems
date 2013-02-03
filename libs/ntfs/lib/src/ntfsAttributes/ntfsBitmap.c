/*
 * ntfsBitmap.c
 *
 *  Created on: 21.01.2013
 *      Author: admin
 */

/*
 This attribute is currently used in two places: indexes (e.g. directories), $MFT. N.B. The index entries and the FILE records also have flags in them to show if they are in use or not.

In an index, the bit field shows which index entries are in use. Each bit represents one VCN of the index allocation.

In the $MFT, the bit field shows which FILE records are in use.
*/
