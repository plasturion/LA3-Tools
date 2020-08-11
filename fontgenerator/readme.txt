============================================================
  Luminous Arc 3 Character-pair font generator (v0.2)
============================================================

------------
How to use:
------------
- place here "font1010_16.bin" file and "fonty.txt"
- run "fontpairs.bat"
- output files will be created: "font1010_16out.bin" and "fpairs.txt"  
 
-------
Tips
-------
- you can edit "fonty.txt" for your needs, first value is number of 
  single characters (max 80), it may be less so binary output file
  will be smaller. Remember that every space has a meaning so keep the
  file structure in the same way how is present now. Using different charset
  than default require modify "encoder.cpp" file (const N and signs[] array).

--------
Notes
--------
- expected result for default font file: 878kb - CRC32( 78C05159 )
- first 623 font characters are the same, character-pairs ale placed right after
- Available default charset in "fonty.txt" (80 characters) :

 !"#%&'()*+,-./:=?0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz


-----------
Changelog
-----------
- bug fixed - too big output file (removed unnecessary gap)