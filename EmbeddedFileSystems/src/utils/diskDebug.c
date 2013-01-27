/*
 * The content of this file is licensed. You may obtain a copy of the license
 * at http://sourceforge.net/projects/ntfs/ or request it via email from the author.
 *
 * Do not remove or change this comment.
 *
 * The initial author of the code is:
 *   Thomas Schmid <schmid-thomas@gmx.net>
 */
#include "diskDebug.h"

#ifdef _DEBUG

  void ntfsHexDump(const uint8_t* bytes, uint8_t length)
  {
    uint8_t i;
   
    for (i=0; i<length; i++)
      ntfsDebug(L" 0x%02x",bytes[i]);
      
    ntfsDebug(L"\n");
  }

  void ntfsStrDump(const uint16_t* bytes, uint8_t length)
  {
    uint8_t i;

    for (i=0; i<length; i++)
    {
      if (bytes[i] > 255)
      {
        ntfsDebug(L"\nWarning: non printable character: 0x%04x \n",bytes[i]);
        continue;
      }

      putwc(bytes[i], stdout);
    }
  }

#else

  void ntfsHexDump(const uint8_t* bytes, uint8_t length)
  {
    return;
  }

  void ntfsStrDump(const uint16_t* bytes, uint8_t length)
  {
    return;
  }
#endif


