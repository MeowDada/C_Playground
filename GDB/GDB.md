# How to use GDB?
* Compile your program and output it as an excutable file. For example:
```console
$ gcc -o bug -ggdb bug.c
```
* Use <code>gdb \<filename\></code> to debug the specify file with given <code>\<filename\></code>. For example:
  * Note: 'bug' here represents a file instead of keyword.
```console
$ gdb bug
```
* And you might see something like this:
```text
GNU gdb (Ubuntu 8.1-0ubuntu3) 8.1.0.20180409-git
Copyright (C) 2018 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
<http://www.gnu.org/software/gdb/documentation/>.
For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from bug...done.
(gdb) 
```
* Using <code>l</code> command in gdb will show the corresponding source code
```text
(gdb) l
```
Then the source code will show up!
```c
(gdb) l
warning: Source file is more recent than executable.
1	#include <stdio.h>
2	#include <stdlib.h>
3	
4	int main(int argc, char **argv)
5	{
6	    char *string;
7	
8	    printf("Please input a string: ");
9	    gets(string);
10	    printf("\nYour string is: %s\n", string);
```
* Using <code>r</code> command to run the code
```text
(gdb) r
```
The result might look like this:
```text
Starting program: /home/jack/Desktop/Repos/C_Playground/GDB/bug 
Please input a string: hello

Program received signal SIGSEGV, Segmentation fault.
_IO_gets (buf=0x0) at iogets.c:53
53	iogets.c: No such file or directory.
(gdb) 
```
* Using <code>where</code> command to find out where the error happened.
```text
(gdb) where
```
The result will show as below:
```text
(gdb) where
#0  _IO_gets (buf=0x0) at iogets.c:53
#1  0x00005555555546bb in main (argc=1, argv=0x7fffffffdef8) at bug.c:9
(gdb) 
```
* By looking into this error, we can find that something went wrong at line #9 in bug.c, which is
```c
gets(string)
```
The only possibility that might cause error in function <code>gets()</code> should be the variable <code>string</code>
* Let's further check the value of <code>string</code> by giving this command:
```text
(gdb) p string
```
The value of <code>string</code> will be shown:
```text
$1 = string
```
* Exit gdb by command <code>quit</code>
```text
(gdb) quit
```