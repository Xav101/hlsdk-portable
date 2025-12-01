/* unaligned.h - macros for handling unaligned memory access */

#ifndef UNALIGNED_H
#define UNALIGNED_H

#include <stdint.h>
#include <byteswap.h>

#define Swap32( x ) (((int)(((x)&255)<<24)) + ((int)((((x)>>8)&255)<<16)) + ((int)(((x)>>16)&255)<<8) + (((x) >> 24)&255))
#define Swap16( x ) ((short)( (((short)(x) >> 8) & 255) + (((short)(x) & 255) << 8)))

static inline short _le_unaligned16( short *x )
{
	short y;
	memcpy( &y, x, sizeof(short) );
	return Swap16( y );
}

static inline int _le_unaligned32( int *x )
{
	int y;
	memcpy( &y, x, sizeof(int) );
	return Swap32( y );
}

static inline void _le_unaligned16_store( short *x )
{
	short y = _le_unaligned16( x );
	memcpy( x, &y, sizeof(short) );
}

static inline void _le_unaligned32_store( int *x )
{
	int y = _le_unaligned32( x );
	memcpy( x, &y, sizeof(int) );
}

static float _le_unaligned_float( float *x )
{
	union
	{
		float *f;
		unsigned char *b;
	} dat1;

	union
	{
		float f;
		unsigned char b[4];
	} dat2;

	dat1.f = x;
	dat2.b[0] = dat1.b[3];
	dat2.b[1] = dat1.b[2];
	dat2.b[2] = dat1.b[1];
	dat2.b[3] = dat1.b[0];

	return dat2.f;
}

/* List for platforms/architectures that need swap and aligned data */
#if ( XASH_IRIX )
	/* In-place swap macros for unaligned byteswap macros */
	#define ULittleShort( x )   ( _le_unaligned16( &(x) ) )
	#define ULittleLong( x )    ( _le_unaligned32( &(x) ) )
	#define ULittleShortSW( x ) ( _le_unaligned16_store( &(x) ) )
	#define ULittleLongSW( x )  ( _le_unaligned32_store( &(x) ) )
	#define ULittleFloat( x )   ( _le_unaligned_float( &(x) ) )
#else
	#define ULittleShort( x )   ( x )
	#define ULittleShortSW( x ) ( x )
	#define ULittleLong( x )    ( x )
	#define ULittleLongSW( x )  ( x )
	#define ULittleFloat( x )   ( x )
#endif


#endif // UNALIGNED_H
