=================================
   Luminous Arc 3 Tools v 0.47
=================================

-------------
 Changelog
-------------
v0.47
- added new tools (arm9.bin dumper/inserter) for menu text
- added exception for c-type string formatting code "%s" (encoder)

v0.46
- improved format for easy editing (forEdit) in the text field,
  line that begins with semi-colon symbol ";" is ignored now (comment line)
- formDebug minor bugfix (if the "str" substring in the file name not present)

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
- added new tool - formatter with debug mode - it shows filename and number 
  of the structure/pointer in the dialog box caption (instead of chara's name) 

v0.39
- added new optional tools for easy editing and auto-formatting 
- new directories and *.bat files for editors / translators

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
- improved and simplified recognizing amount of pointers for strTabGen.exe

v0.3
- str0.bin, strsel.bin, strana.bin files supported
- fixed recognizing pointers

v0.2
 - fixed font size in fontgenerator (gap removed)


Credits:
- plasturion

Special Thanks for:
- LuminousCat for idea, testing and motivation
- Minecrafter for some useful tips
- Lon0z for the prediction skills (first bug)
- and all of the rest involved in the project ;)

Cheers!