## Overview
I made this encoding intentionally incompatible with C strings because I realized
that C strings are not an example of "elegant" design, they are a big performance issue
because you have to scan a string byte by byte every time you need to determine its length.
## O(1) random access to code point by index
See `vl_rchar_at()`.
