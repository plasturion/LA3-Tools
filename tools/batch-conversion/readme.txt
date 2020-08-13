===================================================
  Luminous Arc 3 Batch conversion pack (v0.40)
===================================================

This pack allows to work with big amount of files

------------------
Process diagram:
------------------

[1-input]
   |	
  (A)
   |
[2A-extracted]
   |      |
  (B1)   (B2)
   |      |
   |  [2B-forEdit] 
   |   |    |
   |  (B3) (B4)
   |   |    |
[3-translated]
   |
  (C)
   |
[4-encoded]
   |
  (D)
   |
[5-output]
   
------------------------------
Translation processing steps: 
------------------------------
- place str*.bin files in 1-input" folder
- run  A -> B2
- edit/translate files in "2B-forEdit"
- run B3 -> C -> D

  you may run validator-FC if files("forEdit.txt") are not changed,  
  few bytes differences are because few "$n" are at the end of original lines
  in fact there's no difference what is the last control code of the second string

------------------------------------------------
Translation processing steps (with debug mode): 
------------------------------------------------
- place str*.bin files in 1-input" folder
- run  A -> B2
- edit/translate files in "2B-forEdit"
- run B4 -> C -> D
  
  Now every character's name in caption of dialog box is replaced 
  by filename and structure/pointer's number.
  If you want to work with "forEdit" view optionally you can copy files
  from "3-translated" to "2A-extracted" and run (B2) ext2edit again. 
  
------------------------------
Validation processing steps:
------------------------------
- place str*.bin files in "1-input" folder
- run  A -> B1 -> C -> D (*.bat)
- run validator-FC and look for "fc_logs"


