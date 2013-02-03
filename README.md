EmbeddedFileSystems
===================

A light weight C filesystem library designed for embedded devices little RAM (e.g. ARM3/4...)
It's focus is on reading [NTFS](en.wikipedia.org/wiki/NTFS).

## Compatibility and Limitations

There is no offical NTFS Documentation. Some parts of NTFS are documented by Microsoft, some are reverse engineered 
by Computer Forensics Research Laboratories and some as encryption [EFS](http://en.wikipedia.org/wiki/Encrypting_File_System),
are not documented at all.

This puts hard limits on any NTFS implementation, especially if it's targeted for embedded microcontrollers wit little RAM.
So this library just supports read access. With Journaling Filesystems reading is easy while writing is expensive 
and need lot's of overhead and memory. 

In order to read from NTFS you need to be able to load at least two clusters at once into memory. This means typically 
at least 8KB. Including the overhead for this library you endup with roughtly 10KB for reading files from NTFS.

NTFS heavily relies upon 64Bit multiplication, additions and divisions. Compilers for most Microcontroller especially 
8-Bit abd 16-Bit ones do not support 64Bit arithemtics. So you need to implement these helpers by you own! If your compiler
supports C99, you're lucky. C99 requires a full support for 64Bit values.

## License

This libarary is free and open source software, it is made available to you 
under the terms of the [GNU Affero General Public License (AGPLv3)](http://www.fsf.org/licensing/licenses/agpl-3.0.html).

Refer to [Licensing information](LICENSE.md) for detail
