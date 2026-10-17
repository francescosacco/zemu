#ifndef Z80_UTILS_H
#define Z80_UTILS_H

#include <stdint.h>
#include <stdbool.h>

#define GETBIT(x,y)                              ( x & ( 0x01 << y ) )
#define GETMSB(x)                                ( ( uint8_t ) ( x >> 8 ) )
#define GETLSB(x)                                ( ( uint8_t )   x        )

#endif // Z80_UTILS_H
