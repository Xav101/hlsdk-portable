/* unaligned.h - macros for handling unaligned memory access */

#include <stdint.h>
#include <byteswap.h>

static inline uint16_t _unaligned16le( void *x )
{
	uint8_t *a = (uint8_t*)x;
	return (uint16_t)( a[0] | a[1] << 8 );
}

static inline uint16_t _unaligned16be( void *x )
{
	uint8_t *a = (uint8_t*)x;
	return (uint16_t)( a[0] << 8 | a[1] );
}

static inline uint32_t _unaligned32le( void *x )
{
	uint8_t *a = (uint8_t*)x;
	return (uint32_t)( a[0] | a[1] << 8 | a[2] << 16 | a[3] << 24 );
}

static inline uint32_t _unaligned32be( void *x )
{
	uint8_t *a = (uint8_t*)x;
	return (uint32_t)( a[0] << 24 | a[1] << 16 | a[2] << 8 | a[3] );
}

static void _swap_unaligned16le( uint16_t *x )
{
	uint16_t a = _unaligned16le( x );
	memcpy( x, (uint8_t*)&a, sizeof(uint16_t) );
}

static void _swap_unaligned32le( uint32_t *x )
{
	uint32_t a = _unaligned32le( x );
	memcpy( x, (uint8_t*)&a, sizeof(uint32_t) );
}

static float _unaligned_float_le( float *x )
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

#if XASH_LITTLE_ENDIAN
#define _unaligned16( x ) ( _unaligned16le( x ) )
#define _unaligned32( x ) ( _unaligned32le( x ) )
#elif XASH_BIG_ENDIAN
#define	_unaligned16( x ) ( _unaligned16be( x ) )
#define _unaligned32( x ) ( _unaligned32be( x ) )
#endif

/* List for platforms/architectures that need aligned data */
#if ( XASH_IRIX )

/* unaligned(x) - read unaligned value */
#define unaligned( x ) ( \
	  sizeof( x ) == sizeof( uint16_t ) ? _unaligned16( &(x) ) \
	: sizeof( x ) == sizeof( uint32_t ) ? _unaligned32( &(x) ) \
	: x )

#if ( XASH_BIG_ENDIAN )
	/* In-place swap macros for unaligned byteswap macros */
	#define ULittleShort( x )   ( _unaligned16le( &(x) ) )
	#define ULittleShortSW( x ) ( _swap_unaligned16le( &(x) ) )
	#define ULittleLong( x )    ( _unaligned32le( &(x) ) )
	#define ULittleLongSW( x )  ( _swap_unaligned32le( &(x) ) )

	#define ULittleFloat( x )   ( _unaligned_float_le( &(x) ) )
#else
	#define ULittleShort( x )   ( x )
	#define ULittleShortSW( x ) ( x )
	#define ULittleLong( x )    ( x )
	#define ULittleLongSW( x )  ( x )

	#define ULittleFloat( x )   ( x )
#endif

#else
#define unaligned( x ) ( x )
#endif
