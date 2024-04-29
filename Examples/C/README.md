# C system programming - Examples

## Files primals

### 01_Working_with_files.c

An example that reads data from `infile` and prints the result into `outfile`. Note well controls on file descriptor and other features.

```console
$ gcc 01_Working_with_files.c -o test01
$ ./test01 infile outfile
```

### 02_Ridirection.c

An example that sets `infile` as `stdin` and `outfile` as `stdout`. The program write the content from `outfile` on `stdout`using `/dev/tty` for debug messages.

```console
$ gcc 02_Redirection.c -o test02
$ ./test02 infile outfile
```

### 03_Append.c

An example that appends lines from `stdin` on the `outfile` using the non-sequential primal `lseek()`.

```console
$ gcc 03_Append.c -o test03
$ ./test03 outfile
```