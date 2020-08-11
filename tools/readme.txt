===============================================================
  Luminous Arc 3 Script Tools for character-pair font (v0.45)
===============================================================

Usage:
	"tool_name".exe <inputFile> <outputFile>

Processing steps:

--------------------
dumper.exe - text extractor
--------------------
- place here file with text to extract and rename it to "str.bin"
- run dumper.exe
- text should be extracted to new created "outOrg.txt" 


========= optional add-ons tools ============

  --------------------
  ext2edit.exe - script files converter
  --------------------
  - place "outOrg.txt" in this folder
  - run ext2edit.exe
  - new file "forEdit.txt" is here - easy for editing (without $n and $p)

  --------------------
  formatter.exe - converts to "str" format lines
  --------------------
  - run "formatter.exe" ( it'll look for "forEdit.txt" )
  - auto-formatting for multiple text lines in one entry is now provided 
  - "inT.txt" created
  
  --------------------
  formDebug.exe - converts to "str" format lines (with debug mode)
  --------------------
  - make the same steps like in formatter.exe
  - "inT.txt" created (with filenames and structure/pointer's number
    in all of the the dialog box captions - for identy reasons)
	
=============================================

------------------
encoder.exe - character-pair font encoder
------------------
- place here translated "inT.txt" (Shift-JIS, Unix newline separator (0x0A)) 
- run encoder.exe
- "inTencoded.txt" will be created. (don't edit this file from this point, read only) 

------------------
strTabGen.exe - string table generator
------------------
- place "inTencoded.txt"
- run strTabGen.exe
- "strout.bin" will be created, replace in Tinke and check results.

------------------------------------------------------------------------------

-------
Tips
-------
- make sure "inTencoded.txt" and "outOrg.txt" have the same number of lines
- Recommended tool for edit - notepad++

--------
Notes
--------
- str*.bin is a string table binary structure of two strings (*char[2][])
  
  Textbox window allow to place:    

    in the first string of the structure element:
	- name of the character / box caption (no more than 12 halfsized letters)
    in the second string:
       - two lines of 20 Shift-JIS characters or 40 halfsized english characters 
       - box with the icon - you can place 16 Shift-JIS or 32 single chara-pairs

  str*.bin file has special script formatting codes:
    $n - new line
    $p - pause - awaits for button press( after that dialog box is cleaned )
    $e3 - option select (answer screen)
  
  You can place many textlines in one entry just place "$p" every next new line "$n"
  If 2nd string doesn't end with "$p" effect will be the same. (press button/pause)

  strsel.bin, strsana.bin (selection files) differ a bit:
  - first string is empty
  - second string sometimes contain symbol "-" 
  - you can place only 34 characters in the second string of the strsel.bin structure

  first line of extracted file *.txt is a textheader (16 bytes)

  few words about new optional tools for script edit:

  - lines in "forEdit.txt" shows size-limits of the first and second string
  - to type many lines in one entry and clear a box before the first newline ($n) and
    to continue typing ("text $p text text $p" sequence) put "$p" at the end of line.	
  - "~-" indicate end of the single entry (double string structure).

Available default font for character-pairs (80 characters) :

 !"#%&'()*+,-./:=?0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz

-------------
changelog
-------------
v0.45
- ext2edit and formDebug improved - more accurate filenames recognizing
- formDebug improved - strana, strsel file supported 

v0.44
- formatter and formDebug tool bugfix, now it should recognize more correct
  last controlcode in second string('$n'), extended strsel line to 36 characters

v0.43
- another small improvement for the ext2edit tool (34-characters line showing 
  size limit for the single entry/option of the selection file - strsel)

v0.42
- updated ext2edit tool - added structure/pointer numbering as a comment and shape 
  of the caption box line for easier reference and to work with debug mode 

v0.41
- improved ext2edit tool - short lines (32 characters) for the line size limit 
  of the dialog boxes with small portrait icon (all appearances)

v0.40
- added new tool - formatter with debug mode - shows filename and number 
  of the structure/pointer in the dialog box caption (instead of chara's name) 

v0.39
- added new optional tools for easy editing and auto-formatting ($n, $p)
- new directories and *.bat files for editors

v0.37
- removed whitespaces at the end of file, txt is now stream-friendly
- added that info to the textheader, code cleanup

v0.35
- added textheader (stores more or less important info from input binaries) 
- added batch-conversion directories pack (to work with big amount of files)
- added command line support for every script tools (it's backward compatible):
  Usage:
          "tool_name".exe <inputFile> <outputFile>

v0.31
- improved and simplified method of recognizing pointers in string table generator

v0.3
- str0.bin, strsel.bin, strana.bin files supported
- fixed method of recognizing pointers