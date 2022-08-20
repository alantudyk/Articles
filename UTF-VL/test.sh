#!/bin/sh

gcc -O3 -flto *.c -o utf-vl &&
([ -f wild.utf-8 ] || curl -o wild.utf-8 https://ja.wikipedia.org/wiki/DEATH_STRANDING) &&
./utf-vl -e wild.utf-8 wild.utf-vl &&
./utf-vl -d wild.utf-vl /tmp/wild.utf-8.dec &&
cmp wild.utf-8 /tmp/wild.utf-8.dec &&
echo &&
stat --printf "\tutf-8  =>  %s bytes\n" wild.utf-8  &&
stat --printf "\tutf-vl =>  %s bytes\n" wild.utf-vl &&
echo "\n\tOK.\n" || echo "\n\tNot OK.\n"
