#!/bin/sh

#02
> fileSorg
ln fileSorg $HOME/Desktop/collegamento
ls -l $HOME/Desktop/collegamento

#03
rm fileSorg
ls -l $HOME/Desktop/collegamento

#04
rm $HOME/Desktop/collegamento

#05
ln fileSorg $HOME/collegamento

#06
mkdir linkSw
ln -s provaDirBis linkSw/miolinkSW
