StegoBmp
========

This is for learning purposes only. Use at your own risk and do no harm. 

Textin reads a 24-bit bitmap and writes a 24-bit bitmap with an encoded string. Textout reads a 24-bit bitmap and outputs the encoded message to stdout.

Build
=====

`cc -o textin textin.c`

`cc -o textout textout.c`

Usage
=====

`textin infile.bmp outfile.bmp "message" [cryptkey]`

infile.bmp is a 24-bit bitmap.
outfile.bmp is a file name to write out. Writes a 24-bit bitmap.
"message" is a string to be encoded.
cryptkey is optional, without brackets - used for simple XOR encryption.

`textout infile.bmp [cryptkey]`

infile.bmp is a 24-bit bitmap.
cryptkey is optional, used to decrypt XOR encryption.