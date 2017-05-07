To build and run, go to xv6-public folder and use command: make qemu-nox

We made 5 test cases, they are named "testprog1", "testprog2", "testprog3", "testprog4" and "testprog5"
To run those test cases, boot xv6 first then simply type: testprog#(e.g., testprog4)

For more test cases, you don't have to create a *.c file, we made it easier for users to do so:
To create a small directory: use "mkSmallFilesdir pathName", where "pathName" is arbitrary
for example, if you want to create a small directory named "smallDir", simply type in xv6 command window:
mkSmallFilesdir smallDir

To create a small file: use "mksmf path buffer". Note, small files can only be created in small
directories, so, make sure you've created a small directory before you create a small file
Example: mkSmallFilesdir smallDir, then create a small file named "small.txt" under smallDir with
content "hello" -> mksmf smallDir/small.txt hello. If you create small files without having created
a small directory first, an error message will be displayed
