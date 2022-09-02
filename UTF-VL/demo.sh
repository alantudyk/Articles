#!/bin/sh

gcc -O3 -flto lib*.c d*.c -o demo && ./demo
