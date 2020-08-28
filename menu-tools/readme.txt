===============================================================
  Luminous Arc 3 Menu Tools for character-pair font (v0.47)
===============================================================

Process steps:

--------------------
arm9dumper.exe - menu text extractor
--------------------
- place here japanese "arm9.bin" file
- run Arm9dumper.exe
- text will be extracted to new created "menuList.txt" 

------------------
arm9encoder.bat - character-pair font encoder
------------------
- place here translated "menuList.txt" (Shift-JIS, Unix newline separator (0x0A)) 
- run encoder.exe
- "menuListEncoded.txt" will be created. (not for edit, read only) 

------------------
arm9inserter.exe - menu text inserter
------------------
- place "menuListEncoded.txt"
- run Arm9inserter.exe
- "arm9out.bin" will be created, replace in Tinke and check results.

------------------------------------------------------------------------------

-------
Tips
-------
- make sure "menuList.txt" translated and dumped have the same number of lines
- Recommended tool for edit - notepad++

--------
Notes
--------
- arm9.bin has 4 tables of inventory, location, etc. text and most of shop menu text
  
  1st table - 1212 strings
  2nd table - 154 
  3rd table - 340 
  4th table - 187

  tables are divided by two newlines sequence therefore output/input files
  can have only four empty lines (at the end of each table).

  arm9.bin file has special script formatting codes:
    $n - new line
    %s - c-type string formatting for variable input

  first line of extracted file *.txt is a textheader (16 bytes)
  at the begining of every table is ignored for insertion text-caption (17 bytes)


Available default charset for character-pairs (80 characters) :

 !"#%&'()*+,-./:=?0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz

