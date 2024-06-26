# C system programming - Examples

## Files primals

### 01_Working_with_files.c

An example that reads data from `infile` and prints the result into `outfile`. Note well controls on file descriptor and other features.

```console
$ gcc 01_Working_with_files.c -o test01
$ ./test01 infile outfile
```

### 02_Redirection.c

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

## Processes management

### 04_Child_process.c

An example that creates a child process with `fork()` primal and waits the child end before executing the ramaining father's code.

```console
$ gcc 04_Child_process.c -o test04
$ ./test04
```

### 05_Child_return_value.c

An example that creates a child process with `fork()` and catches his child return value. Macro option are shown as comments or with marginal functions.

```console
$ gcc 05_Child_return_value.c -o test05
$ ./test05
```

### 06_IDinfo.c

An example that uses primals to get process information.

```console
$ gcc 06_IDinfo.c -o test06
$ ./test06
```

## Command execution

### 07_exec_family.c

An example that uses primals from `exec` family.

```console
$ gcc 07_exec_family.c -o test07
$ ./test07
```

### 08_zombie_process.c

An example that creates a child-zombie-process: the father doesn't wait his child.

```console
$ gcc 08_zombie_process.c -o test08
$ ./test08
```

### 09_pipe.c

An example that creates a pipe tunnel between father and child.

```console
$ gcc 09_pipe.c -o test09
$ ./test09
```

### 10_multiple-pipe.c

An example for maniging multiple pipe tunnels.

```console
$ gcc 10_multiple-pipe.c -o test10
$ ./test10
```

### 11_pipeline_onwards.c

An example about data pipeline that sends onwards a `num` value using a `pipe_t` array.

```console
$ gcc 11_pipeline_onwards.c -o test11
$ ./test11
```

### 12_pipeline_ring.c

An example about data pipeline that sends into a ring route a `num` value using a `pipe_t` array.

```console
$ gcc 12_pipeline_ring.c -o test12
$ ./test12
```