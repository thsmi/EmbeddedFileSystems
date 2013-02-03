  Datarun (z.B. 32 E7 12 FE ED 02)
  :length cluster (4bit) 0x3
  :length length starting from cluster (4Bit) 0x2

  : start from cluster (hier 2 Bytes)
    E7 12

  :cluster (3Bytes)
    FE ED 02

  -> bedeutet am Cluser FE ED 02 (191998) exakt E7 12 (5231) Bytes lesen

  alle nachfolgenden runs sind relativ zu dem start cluster im ersten run.
  Sprünge können positivt als auch negativ sein. Little endian

  /*  ID (z.B. 10 00 00 00) 
  length (z.B. 00 00 00 60) 96 Decimal! (4bytes)

  $Data
  :ID 80 00 00 00 (4Bytes)
  :length (4Bytes)
  :resident (1Bytes) 0 oder 1(resident)

  */
  