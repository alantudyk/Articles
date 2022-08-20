#!/bin/sh

gcc -O3 -flto *.c -o utf-vl &&
sudo mv utf-vl /bin/utf-vl
