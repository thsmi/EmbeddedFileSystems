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

#ifndef EXFAT_FILENAME_ENTRY_H_
  #define EXFAT_FILENAME_ENTRY_H_

  // Filename Extension
  // if more than one entry is neccessary the filename will continue in the next entry again starting
  // at offset 0x02
  // maximal 17 records = 255 characters -> 1 character = 2bytes

  #define EXFAT_TYPE_FILENAME_ENTRY         0xC1
  #define EXFAT_TYPE_FILENAME_ENTRY_UNUSED  0x41

  /* Flags:
   *   (1<<0) Alloc Possible, must be 0
   *   (1<<1) NoFatChain, must be 0
   */

  typedef struct {
    /*0x00*/ uint8_t type; // 0xC1 or 41
    /*0x01*/ uint8_t flags;
    /*0x02*/ uint16_t filename[15]; // length is defined in 0cC0 Record.
  } extfatFileNameEntry_t;

  /* Directory entries follow immediately the Stream extension directory.
    Maximal 17 Filename entries are possible with 15 characters each
    -> maximal 255 characters. Unused portions must be zero padded.

    Invalid characters:

  [ Code Point | Character | Description     ]
  [------------+-----------+-----------------]
  [ 0x0000...  |           | Control codes   ]
  [  ...0x001F |           |                 ]
  [   0x0022   |     "     | Quotation mark  ]
  [   0x002A   |     *     | Asterisk        ]
  [   0x002F   |     /     | Forward slash   ]
  [   0x003A   |     :     | Colon           ]
  [   0x003C   |     <     | Less than       ]
  [   0x003E   |     >     | Greater than    ]
  [   0x003F   |     ?     | Question mark   ]
  [   0x005C   |     \     | Back slash      ]
  [   0x007C   |     |     | Vertical bar    ]
   */

#endif /* EXFATFILENAMEEXTENSION_H_ */
