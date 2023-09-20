// NOTE: Follows same license as printf.c!

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

// If you want to include this implementation file directly rather than
// link against, this will let you control the functions' visibility,
// e.g. make them static so as not to clash with other objects also
// using them.
#define PRINTF_VISIBILITY


// 'ntoa' conversion buffer size, this must be big enough to hold one converted
// numeric number including padded zeros (dynamically created on stack)
#ifndef PRINTF_INTEGER_BUFFER_SIZE
#define PRINTF_INTEGER_BUFFER_SIZE    32
#endif

// size of the fixed (on-stack) buffer for printing individual decimal numbers.
// this must be big enough to hold one converted floating-point value including
// padded zeros.
#ifndef PRINTF_DECIMAL_BUFFER_SIZE
#define PRINTF_DECIMAL_BUFFER_SIZE    32
#endif

// Support for the decimal notation floating point conversion specifiers (%f, %F)
#ifndef PRINTF_SUPPORT_DECIMAL_SPECIFIERS
#define PRINTF_SUPPORT_DECIMAL_SPECIFIERS 0
#endif

// Support for the exponential notation floating point conversion specifiers (%e, %g, %E, %G)
#ifndef PRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS
#define PRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS 0
#endif

// Support for the length write-back specifier (%n)
#ifndef PRINTF_SUPPORT_WRITEBACK_SPECIFIER
#define PRINTF_SUPPORT_WRITEBACK_SPECIFIER 1
#endif

// Default precision for the floating point conversion specifiers (the C standard sets this at 6)
#ifndef PRINTF_DEFAULT_FLOAT_PRECISION
#define PRINTF_DEFAULT_FLOAT_PRECISION  6
#endif

// According to the C languages standard, printf() and related functions must be able to print any
// integral number in floating-point notation, regardless of length, when using the %f specifier -
// possibly hundreds of characters, potentially overflowing your buffers. In this implementation,
// all values beyond this threshold are switched to exponential notation.
#ifndef PRINTF_MAX_INTEGRAL_DIGITS_FOR_DECIMAL
#define PRINTF_MAX_INTEGRAL_DIGITS_FOR_DECIMAL 9
#endif

// Support for the long long integral types (with the ll, z and t length modifiers for specifiers
// %d,%i,%o,%x,%X,%u, and with the %p specifier). Note: 'L' (long double) is not supported.
#ifndef PRINTF_SUPPORT_LONG_LONG
#define PRINTF_SUPPORT_LONG_LONG 1
#endif

// The number of terms in a Taylor series expansion of log_10(x) to
// use for approximation - including the power-zero term (i.e. the
// value at the point of expansion).
#ifndef PRINTF_LOG10_TAYLOR_TERMS
#define PRINTF_LOG10_TAYLOR_TERMS 4
#endif

#if PRINTF_LOG10_TAYLOR_TERMS <= 1
#error "At least one non-constant Taylor expansion is necessary for the log10() calculation"
#endif

// Be extra-safe, and don't assume format specifiers are completed correctly
// before the format string end.
#ifndef PRINTF_CHECK_FOR_NUL_IN_FORMAT_SPECIFIER
#define PRINTF_CHECK_FOR_NUL_IN_FORMAT_SPECIFIER 1
#endif

#define PRINTF_PREFER_DECIMAL     true
#define PRINTF_PREFER_EXPONENTIAL false

///////////////////////////////////////////////////////////////////////////////

// The following will convert the number-of-digits into an exponential-notation literal
#define PRINTF_CONCATENATE(s1, s2) s1##s2
#define PRINTF_EXPAND_THEN_CONCATENATE(s1, s2) PRINTF_CONCATENATE(s1, s2)
#define PRINTF_FLOAT_NOTATION_THRESHOLD PRINTF_EXPAND_THEN_CONCATENATE(1e,PRINTF_MAX_INTEGRAL_DIGITS_FOR_DECIMAL)

// internal flag definitions
#define FLAGS_ZEROPAD   (1U <<  0U)
#define FLAGS_LEFT      (1U <<  1U)
#define FLAGS_PLUS      (1U <<  2U)
#define FLAGS_SPACE     (1U <<  3U)
#define FLAGS_HASH      (1U <<  4U)
#define FLAGS_UPPERCASE (1U <<  5U)
#define FLAGS_CHAR      (1U <<  6U)
#define FLAGS_SHORT     (1U <<  7U)
#define FLAGS_INT       (1U <<  8U)
	// Only used with PRINTF_SUPPORT_MSVC_STYLE_INTEGER_SPECIFIERS
#define FLAGS_LONG      (1U <<  9U)
#define FLAGS_LONG_LONG (1U << 10U)
#define FLAGS_PRECISION (1U << 11U)
#define FLAGS_ADAPT_EXP (1U << 12U)
#define FLAGS_POINTER   (1U << 13U)
	// Note: Similar, but not identical, effect as FLAGS_HASH
#define FLAGS_SIGNED    (1U << 14U)
	// Only used with PRINTF_SUPPORT_MSVC_STYLE_INTEGER_SPECIFIERS

#ifdef PRINTF_SUPPORT_MSVC_STYLE_INTEGER_SPECIFIERS

#define FLAGS_INT8 FLAGS_CHAR


#if   (SHRT_MAX   == 32767LL)
#define FLAGS_INT16       FLAGS_SHORT
#elif (INT_MAX    == 32767LL)
#define FLAGS_INT16       FLAGS_INT
#elif (LONG_MAX   == 32767LL)
#define FLAGS_INT16       FLAGS_LONG
#elif (LLONG_MAX  == 32767LL)
#define FLAGS_INT16       FLAGS_LONG_LONG
#else
#error "No basic integer type has a size of 16 bits exactly"
#endif

#if   (SHRT_MAX   == 2147483647LL)
#define FLAGS_INT32       FLAGS_SHORT
#elif (INT_MAX    == 2147483647LL)
#define FLAGS_INT32       FLAGS_INT
#elif (LONG_MAX   == 2147483647LL)
#define FLAGS_INT32       FLAGS_LONG
#elif (LLONG_MAX  == 2147483647LL)
#define FLAGS_INT32       FLAGS_LONG_LONG
#else
#error "No basic integer type has a size of 32 bits exactly"
#endif

#if   (SHRT_MAX   == 9223372036854775807LL)
#define FLAGS_INT64       FLAGS_SHORT
#elif (INT_MAX    == 9223372036854775807LL)
#define FLAGS_INT64       FLAGS_INT
#elif (LONG_MAX   == 9223372036854775807LL)
#define FLAGS_INT64       FLAGS_LONG
#elif (LLONG_MAX  == 9223372036854775807LL)
#define FLAGS_INT64       FLAGS_LONG_LONG
#else
#error "No basic integer type has a size of 64 bits exactly"
#endif

#endif // PRINTF_SUPPORT_MSVC_STYLE_INTEGER_SPECIFIERS


typedef unsigned int printfFlags_t;

#define BASE_BINARY    2
#define BASE_OCTAL     8
#define BASE_DECIMAL  10
#define BASE_HEX      16

typedef uint8_t numericBase_t;

#if PRINTF_SUPPORT_LONG_LONG
typedef unsigned long long printfUnsignedValue_t;
typedef long long          printfSignedValue_t;
#else
typedef unsigned long printfUnsignedValue_t;
typedef long          printfSignedValue_t;
#endif

// The printf()-family functions return an `int`; it is therefore
// unnecessary/inappropriate to use size_t - often larger than int
// in practice - for non-negative related values, such as widths,
// precisions, offsets into buffers used for printing and the sizes
// of these buffers. instead, we use:
typedef unsigned int printfSize_t;
#define PRINTF_MAX_POSSIBLE_BUFFER_SIZE INT_MAX
	// If we were to nitpick, this would actually be INT_MAX + 1,
	// since INT_MAX is the maximum return value, which excludes the
	// trailing '\0'.

#if (PRINTF_SUPPORT_DECIMAL_SPECIFIERS || PRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS)
#include <float.h>
#if FLT_RADIX != 2
#error "Non-binary-radix floating-point types are unsupported."
#endif

#if DBL_MANT_DIG == 24

#define DOUBLE_SIZE_IN_BITS 32
typedef uint32_t double_uint_t;
#define DOUBLE_EXPONENT_MASK 0xFFU
#define DOUBLE_BASE_EXPONENT 127
#define DOUBLE_MAX_SUBNORMAL_EXPONENT_OF_10 -38
#define DOUBLE_MAX_SUBNORMAL_POWER_OF_10 1e-38

#elif DBL_MANT_DIG == 53

#define DOUBLE_SIZE_IN_BITS 64
typedef uint64_t double_uint_t;
#define DOUBLE_EXPONENT_MASK 0x7FFU
#define DOUBLE_BASE_EXPONENT 1023
#define DOUBLE_MAX_SUBNORMAL_EXPONENT_OF_10 -308
#define DOUBLE_MAX_SUBNORMAL_POWER_OF_10 1e-308

#else
#error "Unsupported double type configuration"
#endif
#define DOUBLE_STORED_MANTISSA_BITS (DBL_MANT_DIG - 1)

typedef union {
	double_uint_t U;
	double        F;
} double_with_bit_access;

#define PRINTF_ABS(_x) ( (_x) > 0 ? (_x) : -(_x) )

#endif // (PRINTF_SUPPORT_DECIMAL_SPECIFIERS || PRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS)

// Note in particular the behavior here on LONG_MIN or LLONG_MIN; it is valid
// and well-defined, but if you're not careful you can easily trigger undefined
// behavior with -LONG_MIN or -LLONG_MIN
#define ABS_FOR_PRINTING(_x) ((printfUnsignedValue_t) ( (_x) > 0 ? (_x) : -((printfSignedValue_t)_x) ))

// wrapper (used as buffer) for output function type
//
// One of the following must hold:
// 1. max_chars is 0
// 2. buffer is non-null
// 3. function is non-null
//
// ... otherwise bad things will happen.
typedef struct {
	void (*function)(char c, void* extra_arg);
	void* extra_function_arg;
	char* buffer;
	printfSize_t pos;
	printfSize_t max_chars;
} outputGadget_t;