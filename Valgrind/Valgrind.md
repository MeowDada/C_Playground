# What is Valgrind?
A useful tool for detecting memory faults, memory leaks and performance analysis.

# How to install?
On Ubuntu:
```console
$ sudo apt-get install valgrind
```

# Notice
In order to use valgrind to detect potential memory problems, we must add parameter<code>-g</code> when compiling a C program by <code>gcc</code>. When it comes to compling a C++ program, <code>-fno-inline</code> should be added. And the most important thing is that **DO NOT** enable any optimization option when compiling a program if you want to apply it with valgrind. 

# Memory dectection examples
## Uninitialized variables
In the file 'no_init_test.c', there is a variable <code>x</code> is declared without initialization. Our goal is to find it out by using valgrind. The step are shown below:

 1. Compile the program with parameter <code>-g</code>
```console
$ gcc -o no_init_test no_init_test.c -g
```
 2. Use command <code>valgrind</code> with <code>memcheck</code>
```console
$ valgrind --tool=memcheck --show-reachable=yes \
--read-var-info=yes -verbose --error-limit=yes --time-stamp=yes --leak-check=full \
--log-file=mycode1.log ./no_init_test
```
3. Checkout the result inside the file <code>mycode1.log</code>
```console
$ less mycode1.log
```
4. The result might look like this
```text
...
==00:00:00:04.365 27733== HEAP SUMMARY:
==00:00:00:04.365 27733==     in use at exit: 0 bytes in 0 blocks
==00:00:00:04.365 27733==   total heap usage: 1 allocs, 1 frees, 1,024 bytes allocated
==00:00:00:04.365 27733== 
==00:00:00:04.365 27733== All heap blocks were freed -- no leaks are possible
==00:00:00:04.365 27733== 
==00:00:00:04.365 27733== Use --track-origins=yes to see where uninitialised values come from
==00:00:00:04.365 27733== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
==00:00:00:04.365 27733== 
==00:00:00:04.365 27733== 1 errors in context 1 of 1:
==00:00:00:04.365 27733== Conditional jump or move depends on uninitialised value(s)
==00:00:00:04.365 27733==    at 0x10864D: main (no_init_test.c:6)
==00:00:00:04.365 27733== 
==00:00:00:04.365 27733== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```
## Out of bound Error
Let's try to detect OOB error, which is in the file 'oob_test.c'.
1. Compile the program with parameter <code>-g</code>
```console
$ gcc -o oob_test oob_test.c -g
```
 2. Use command <code>valgrind</code> with <code>memcheck</code>
```console
$ valgrind --tool=memcheck --show-reachable=yes \
--read-var-info=yes -verbose --error-limit=yes --time-stamp=yes --leak-check=full \
--log-file=mycode2.log ./oob_test
```
3. Checkout the result inside the file <code>mycode2.log</code>
```console
$ less mycode2.log
```
4. The result might look like this
```text
==00:00:00:04.340 31499== Invalid write of size 4
==00:00:00:04.340 31499==    at 0x1086DB: main (oob_test.c:13)
==00:00:00:04.340 31499==  Address 0x522d054 is 0 bytes after a block of size 20 alloc'd
==00:00:00:04.340 31499==    at 0x4C2FB0F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==00:00:00:04.340 31499==    by 0x1086B0: main (oob_test.c:8)
==00:00:00:04.340 31499== 
==00:00:00:04.340 31499== Invalid read of size 4
==00:00:00:04.340 31499==    at 0x1086E5: main (oob_test.c:15)
==00:00:00:04.340 31499==  Address 0x522d054 is 0 bytes after a block of size 20 alloc'd
==00:00:00:04.340 31499==    at 0x4C2FB0F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==00:00:00:04.340 31499==    by 0x1086B0: main (oob_test.c:8)
==00:00:00:04.340 31499== 
--00:00:00:04.341 31499-- REDIR: 0x4fca1d0 (libc.so.6:__strchrnul_avx2) redirected to 0x4c37020 (strchrnul)
--00:00:00:04.349 31499-- REDIR: 0x4fcaab0 (libc.so.6:__mempcpy_avx_unaligned_erms) redirected to 0x4c37130 (mempcpy)
--00:00:00:04.366 31499-- REDIR: 0x4ed3950 (libc.so.6:free) redirected to 0x4c30cd0 (free)
==00:00:00:04.372 31499== 
==00:00:00:04.372 31499== HEAP SUMMARY:
==00:00:00:04.372 31499==     in use at exit: 20 bytes in 1 blocks
==00:00:00:04.372 31499==   total heap usage: 2 allocs, 1 frees, 1,044 bytes allocated
==00:00:00:04.372 31499== 
==00:00:00:04.372 31499== Searching for pointers to 1 not-freed blocks
==00:00:00:04.373 31499== Checked 70,280 bytes
==00:00:00:04.373 31499== 
==00:00:00:04.373 31499== 20 bytes in 1 blocks are definitely lost in loss record 1 of 1
==00:00:00:04.373 31499==    at 0x4C2FB0F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==00:00:00:04.373 31499==    by 0x1086B0: main (oob_test.c:8)
==00:00:00:04.373 31499== 
==00:00:00:04.373 31499== LEAK SUMMARY:
==00:00:00:04.373 31499==    definitely lost: 20 bytes in 1 blocks
==00:00:00:04.373 31499==    indirectly lost: 0 bytes in 0 blocks
==00:00:00:04.373 31499==      possibly lost: 0 bytes in 0 blocks
==00:00:00:04.373 31499==    still reachable: 0 bytes in 0 blocks
==00:00:00:04.373 31499==         suppressed: 0 bytes in 0 blocks
==00:00:00:04.373 31499== 
==00:00:00:04.373 31499== ERROR SUMMARY: 3 errors from 3 contexts (suppressed: 0 from 0)
==00:00:00:04.373 31499== 
```