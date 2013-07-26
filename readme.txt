









                      ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
                      ³ D O N ' T   P A N I C   ! ³
                      ³                           ³
                      ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ









Readme for sCT simple crypto tool, version 0.1beta
==================================================

sCT orginated from a small "learning C" course at the University of
Applied Science FH-Wedel, Germany. 
We should do a library for simple crypted file IO - and because of I 
ever missed a smart tool for simple and fast file crypting, I build 
sCT on top of these library. 

What it does
============

It encodes a file (text or binary) with a password, so that it is not readable 
without the password. The algorithm is simple, but effective.
For sure, it's not as safe as PGP or RSA, but it's enough for someone
who just knows a mouse is a computer device... :-)

Text can come from a file or from stdin, and can be saved in another file
or to stdout.

Usage is simple
===============

to encode a file:
  sct e -fin <file to encode> -fout <save file> -p <password>
to decode a file
  sct d -fin <encoded file> -fout <save file> -p <password>

* -fin and -fout are optional.
* if -fin is missing, stdin is assumed.
* if -fout is missing, stdout is assumed.
* if -p is missing, you will be asked.
  (not possible if input comes from stdin!)

For example, if you want to crypt the output of the "dir" command:

  dir | sct e -fout crypted.dir -p somepassword

and if you want to decode and print it on the screen

  sct d -fin crypted.dir -p somepassword

Compiling
=========

The sct.exe in this archive is an OS/2 program.
But sct is written in ANSI C, and you should be able to compile it under
every platform having an ANSI C compiler :-) 
It is developed under OS/2 4.0 with famous GNU C.

The makefile in this archive is for the OS/2 version of gnu make.

Feel free to play around with the source of sct. 
As I mentioned in the source files, I'm NOT a skilled C developer. So if you
find bugs or have hints for better code, please email me.

Contact
=======

Julian Buss
Tinsdaler Weg 127
22880 Wedel
Germany

julian@wh.fh-wedel.de

What's this "don't panic" thing?!?
==================================

huh... you DON'T know "Hitchikers Guide To The Galaxy" from Douglas
Adams?!? 
Read it! 
