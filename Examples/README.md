# SHELL - Examples

## For cycle use
* File arguments iteration
* Directory elements iteration 

Execute using:
```console
$ sh FOR.sh Arg_001 Arg_002 Arg_003
```

## Case construct use
The CASE consturct is often used for checking arguments format (absolute or relative arguments path format) but also to check the argument type (number, string, etc.)

Execute using:
```console
$ sh CASE.sh /nomeDir/nomeFile ./nomeFile nomeFile 123 . provaminuscole
```

## Arguments list control
* Applying controls on the first argument and on the others
* Applying controls on N arguments and on the last argument

Execute using:
```console
$ sh PARAM_1.sh 120 /nomeDir/nomeFile ./nomeFile nomeFile
$ sh PARAM_2.sh /nomeDir/nomeFile ./nomeFile nomeFile 120
```

## Counting files into a directory hierarcy
* _METHOD A_: Counting files using a temporary file to save file data (file path)
* _METHOD B_: Counting files using an environment variable and the script return value
* _METHOD C_: Counting files using a local shell variable and the script return value
* _METHOD D_: Counting files using using a temporary file to save the counter value

## Counting levels of a hierarcy
Counting levels of a hierarcy using a temporary file and the recursive script return value.
