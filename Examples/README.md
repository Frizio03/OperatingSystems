# SHELL - Examples

## For cycle use
* File arguments iteration
* Directory elements iteration 

Exeute using:
```console
$ sh FOR.sh Arg_001 Arg_002 Arg_003
```

## Case construct use
The CASE consturct is often used for checking arguments format (absolute or relative arguments path format) but also to check the argument type (number, string, etc.)

Exeute using:
```console
$ sh CASE.sh /nomeDir/nomeFile ./nomeFile nomeFile 123 provaminuscole
```

## Counting files into a directory hierarcy
* _METHOD A_: Counting files using a temporary file to save file data (file path)
* _METHOD B_: Counting files using an environment variable and the script return value
* _METHOD C_: Counting files using a local shell variable and the script return value
* _METHOD D_: Counting files using using a temporary file to save the counter value
