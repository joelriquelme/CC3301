#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "concat.h"

uint concat_bits(uint x, int n, uint y, int m) {
     // x= 11011
     // n= 3
     // y= 10101
     // m= 2
     // resultado = 01101
     uint i = -1;
     int mask1 = ~(i << n); // 0000000000111 (n 1's)
     int mask2 = ~(i << m); // 0000000000011 (m 1's)
     x &= mask1; // x= 0000000011
     x <<= m; //    x= 0000001100
     y &= mask2; // y= 0000000001
     x += y; //     x= 0000001101
     return x;
}