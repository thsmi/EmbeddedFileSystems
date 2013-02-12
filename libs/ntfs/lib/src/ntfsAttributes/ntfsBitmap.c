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

/*
 This attribute is currently used in two places: indexes (e.g. directories), $MFT. N.B. The index entries and the FILE records also have flags in them to show if they are in use or not.

In an index, the bit field shows which index entries are in use. Each bit represents one VCN of the index allocation.

In the $MFT, the bit field shows which FILE records are in use.
*/
