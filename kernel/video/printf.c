/**
 * @author (c) Eyal Rozenberg <eyalroz1@gmx.com>
 *             2021-2022, Haifa, Palestine/Israel
 * @author (c) Marco Paland (info@paland.com)
 *             2014-2019, PALANDesign Hannover, Germany
 *
 * @note Others have made smaller contributions to this file: see the
 * contributors page at https://github.com/eyalroz/printf/graphs/contributors
 * or ask one of the authors. The original code for exponential specifiers was
 * contributed by Martijn Jasperse <m.jasperse@gmail.com>.
 *
 * @brief Small stand-alone implementation of the printf family of functions
 * (`(v)printf`, `(v)s(n)printf` etc., geared towards use on embedded systems with
 * a very limited resources.
 *
 * @note the implementations are thread-safe; re-entrant; use no functions from
 * the standard library; and do not dynamically allocate any memory.
 *
 * @license The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
/*
	NOTE: This code has been modified from the original.
	For the original file, please see https://github.com/eyalroz/printf/blob/master/src/printf/printf.c
*/
#include <stdint.h>
#include <limits.h>
#include <stdbool.h>

#include "printf.h"
// This is unnecessary in C99, since compound initializers can be used,
// but:
// 1. Some compilers are finicky about this;
// 2. Some people may want to convert this to C89;
// 3. If you try to use it as C++, only C++20 supports compound literals
// static inline double_with_bit_access getBitAccess(double x) {
	// double_with_bit_access dwba;
	// dwba.F = x;
	// return dwba;
// }

// static inline int getSignBit(double x) {
	// The sign is stored in the highest bit
	// return (int) (getBitAccess(x).U >> (DOUBLE_SIZE_IN_BITS - 1));
// }

// static inline int getExp2(double_with_bit_access x) {
	// The exponent in an IEEE-754 floating-point number occupies a contiguous
	// sequence of bits (e.g. 52..62 for 64-bit doubles), but with a non-trivial representation: An
	// unsigned offset from some negative value (with the extremal offset values reserved for
	// special use).
	// return (int)((x.U >> DOUBLE_STORED_MANTISSA_BITS ) & DOUBLE_EXPONENT_MASK) - DOUBLE_BASE_EXPONENT;
// }
// Note: This function currently assumes it is not passed a '\0' c,
// or alternatively, that '\0' can be passed to the function in the output
// gadget. The former assumption holds within the printf library. It also
// assumes that the output gadget has been properly initialized.
static inline void putcharViaGadget(outputGadget_t* gadget, char c) {
	printfSize_t write_pos = gadget->pos++;
		// We're _always_ increasing pos, so as to count how may characters
		// _would_ have been written if not for the max_chars limitation
	if (write_pos >= gadget->max_chars) {
		return;
	}
	if (gadget->function != NULL) {
		// No check for c == '\0' .
		gadget->function(c, gadget->extra_function_arg);
	}
	else {
		// it must be the case that gadget->buffer != NULL , due to the constraint
		// on outputGadget_t ; and note we're relying on write_pos being non-negative.
		gadget->buffer[write_pos] = c;
	}
}

// Possibly-write the string-terminating '\0' character
static inline void appendTermWithGadget(outputGadget_t* gadget) {
	if (gadget->function != NULL || gadget->max_chars == 0) {
		return;
	}
	if (gadget->buffer == NULL) {
		return;
	}
	printfSize_t null_char_pos = gadget->pos < gadget->max_chars ? gadget->pos : gadget->max_chars - 1;
	gadget->buffer[null_char_pos] = '\0';
}

// We can't use putchar as is, since our output gadget
// only takes pointers to functions with an extra argument
static inline void putcharWrapper(char c, void* unused) {
	(void) unused;
	putchar(c);
}

static inline outputGadget_t discardingGadget(void) {
	outputGadget_t gadget;
	gadget.function = NULL;
	gadget.extra_function_arg = NULL;
	gadget.buffer = NULL;
	gadget.pos = 0;
	gadget.max_chars = 0;
	return gadget;
}

static inline outputGadget_t bufferGadget(char* buffer, size_t size) {
	printfSize_t usable_buffer_size = (size > PRINTF_MAX_POSSIBLE_BUFFER_SIZE) ?
		PRINTF_MAX_POSSIBLE_BUFFER_SIZE : (printfSize_t) size;
	outputGadget_t result = discardingGadget();
	if (buffer != NULL) {
		result.buffer = buffer;
		result.max_chars = usable_buffer_size;
	}
	return result;
}

static inline outputGadget_t functionGadget(void (*function)(char, void*), void* extraArg) {
	outputGadget_t result = discardingGadget();
	result.function = function;
	result.extra_function_arg = extraArg;
	result.max_chars = PRINTF_MAX_POSSIBLE_BUFFER_SIZE;
	return result;
}

static inline outputGadget_t externPutcharGadget(void) {
	return functionGadget(putcharWrapper, NULL);
}


// internal ASCII string to printfSize_t conversion
static printfSize_t atou_(const char** str) {
	printfSize_t i = 0U;
	while (isdigit(**str)) {
		i = i * 10U + (printfSize_t)(*((*str)++) - '0');
	}
	return i;
}


// output the specified string in reverse, taking care of any zero-padding
static void outRev_(outputGadget_t* output, const char* buf, printfSize_t len, printfSize_t width, printfFlags_t flags) {
	const printfSize_t start_pos = output->pos;

	// pad spaces up to given width
	if (!(flags & FLAGS_LEFT) && !(flags & FLAGS_ZEROPAD)) {
		for (printfSize_t i = len; i < width; i++) {
			putcharViaGadget(output, ' ');
		}
	}

	// reverse string
	while (len) {
		putcharViaGadget(output, buf[--len]);
	}

	// append pad spaces up to given width
	if (flags & FLAGS_LEFT) {
		while (output->pos - start_pos < width) {
			putcharViaGadget(output, ' ');
		}
	}
}


// Invoked by printInt after the actual number has been printed, performing necessary
// work on the number's prefix (as the number is initially printed in reverse order)
static void printIntegerFinalization(outputGadget_t* output, char* buf, printfSize_t len, bool negative, numericBase_t base, printfSize_t precision, printfSize_t width, printfFlags_t flags) {
	printfSize_t unpadded_len = len;

	// pad with leading zeros
	if (!(flags & FLAGS_LEFT)) {
		if (width && (flags & FLAGS_ZEROPAD) && (negative || (flags & (FLAGS_PLUS | FLAGS_SPACE)))) {
			width--;
		}
		while ((flags & FLAGS_ZEROPAD) && (len < width) && (len < PRINTF_INTEGER_BUFFER_SIZE)) {
			buf[len++] = '0';
		}
	}

	while ((len < precision) && (len < PRINTF_INTEGER_BUFFER_SIZE)) {
		buf[len++] = '0';
	}

	if (base == BASE_OCTAL && (len > unpadded_len)) {
		// Since we've written some zeros, we've satisfied the alternative format leading space requirement
		flags &= ~FLAGS_HASH;
	}

	// handle hash
	if (flags & (FLAGS_HASH | FLAGS_POINTER)) {
		if (!(flags & FLAGS_PRECISION) && len && ((len == precision) || (len == width))) {
			// Let's take back some padding digits to fit in what will eventually
			// be the format-specific prefix
			if (unpadded_len < len) {
				len--; // This should suffice for BASE_OCTAL
			}
			if (len && (base == BASE_HEX || base == BASE_BINARY) && (unpadded_len < len)) {
				len--; // ... and an extra one for 0x or 0b
			}
		}
		if ((base == BASE_HEX) && !(flags & FLAGS_UPPERCASE) && (len < PRINTF_INTEGER_BUFFER_SIZE)) {
			buf[len++] = 'x';
		}
		else if ((base == BASE_HEX) && (flags & FLAGS_UPPERCASE) && (len < PRINTF_INTEGER_BUFFER_SIZE)) {
			buf[len++] = 'X';
		}
		else if ((base == BASE_BINARY) && (len < PRINTF_INTEGER_BUFFER_SIZE)) {
			buf[len++] = 'b';
		}
		if (len < PRINTF_INTEGER_BUFFER_SIZE) {
			buf[len++] = '0';
		}
	}

	if (len < PRINTF_INTEGER_BUFFER_SIZE) {
		if (negative) {
			buf[len++] = '-';
		}
		else if (flags & FLAGS_PLUS) {
			buf[len++] = '+';  // ignore the space if the '+' exists
		}
		else if (flags & FLAGS_SPACE) {
			buf[len++] = ' ';
		}
	}

	outRev_(output, buf, len, width, flags);
}

// An internal itoa-like function
static void printInt(outputGadget_t* output, printfUnsignedValue_t value, bool negative, numericBase_t base, printfSize_t precision, printfSize_t width, printfFlags_t flags) {
	char buf[PRINTF_INTEGER_BUFFER_SIZE];
	printfSize_t len = 0U;

	if (!value) {
		if ( !(flags & FLAGS_PRECISION) ) {
			buf[len++] = '0';
			flags &= ~FLAGS_HASH;
			// We drop this flag this since either the alternative and regular modes of the specifier
			// don't differ on 0 values, or (in the case of octal) we've already provided the special
			// handling for this mode.
		}
		else if (base == BASE_HEX) {
			flags &= ~FLAGS_HASH;
			// We drop this flag this since either the alternative and regular modes of the specifier
			// don't differ on 0 values
		}
	}
	else {
		while (value && (len < PRINTF_INTEGER_BUFFER_SIZE)) {
			const char digit = (char)(value % base);
			buf[len++] = (char)(digit < 10 ? '0' + digit : 'A' + digit - 10);
			value /= base;
		}
	}

	printIntegerFinalization(output, buf, len, negative, base, precision, width, flags);
}

#if (PRINTF_SUPPORT_DECIMAL_SPECIFIERS || PRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS)

// Stores a fixed-precision representation of a double relative
// to a fixed precision (which cannot be determined by examining this structure)
typedef struct {
	int_fast64_t integral;
	int_fast64_t fractional;
	// ... truncation of the actual fractional part of the double value, scaled
	// by the precision value
	bool is_negative;
} doubleComponents_t;

#define NUM_DECIMAL_DIGITS_IN_INT64_T 18
#define PRINTF_MAX_PRECOMPUTED_POWER_OF_10  NUM_DECIMAL_DIGITS_IN_INT64_T
static const double powOf10[NUM_DECIMAL_DIGITS_IN_INT64_T] = {
	1e00, 1e01, 1e02, 1e03, 1e04, 1e05, 1e06, 1e07, 1e08,
	1e09, 1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17
};

#define PRINTF_MAX_SUPPORTED_PRECISION NUM_DECIMAL_DIGITS_IN_INT64_T - 1


// Break up a double number - which is known to be a finite non-negative number -
// into its base-10 parts: integral - before the decimal point, and fractional - after it.
// Taken the precision into account, but does not change it even internally.
static doubleComponents_t getComponents(double number, printfSize_t precision) {
	doubleComponents_t number_;
	number_.is_negative = getSignBit(number);
	double absNumber = (number_.is_negative) ? -number : number;
	number_.integral = (int_fast64_t)absNumber;
	double remainder = (absNumber - (double) number_.integral) * powOf10[precision];
	number_.fractional = (int_fast64_t)remainder;

	remainder -= (double) number_.fractional;

	if (remainder > 0.5) {
		++number_.fractional;
		// handle rollover, e.g. case 0.99 with precision 1 is 1.0
		if ((double) number_.fractional >= powOf10[precision]) {
			number_.fractional = 0;
			++number_.integral;
		}
	}
	else if ((remainder == 0.5) && ((number_.fractional == 0U) || (number_.fractional & 1U))) {
		// if halfway, round up if odd OR if last digit is 0
		++number_.fractional;
	}

	if (precision == 0U) {
		remainder = absNumber - (double) number_.integral;
		if ((!(remainder < 0.5) || (remainder > 0.5)) && (number_.integral & 1)) {
			// exactly 0.5 and ODD, then round up
			// 1.5 -> 2, but 2.5 -> 2
			++number_.integral;
		}
	}
	return number_;
}

#if PRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS
typedef struct {
	double rawFactor;
	bool multiply; // if true, need to multiply by rawFactor; otherwise need to divide by it
} scalingFactor_t;

static double applyScaling(double num, scalingFactor_t normalization) {
	return normalization.multiply ? num * normalization.rawFactor : num / normalization.rawFactor;
}

static double unapplyScaling(double normalized, scalingFactor_t normalization) {
	return normalization.multiply ? normalized / normalization.rawFactor : normalized * normalization.rawFactor;
}

static scalingFactor_t updateNormalization(scalingFactor_t sf, double extraMultiFactor) {
	scalingFactor_t result;
	if (sf.multiply) {
		result.multiply = true;
		result.rawFactor = sf.rawFactor * extraMultiFactor;
	}
	else {
		int factorExp2 = getExp2(getBitAccess(sf.rawFactor));
		int extraFactorExp2 = getExp2(getBitAccess(extraMultiFactor));

		// Divide the larger-exponent raw rawFactor by the smaller
		if (PRINTF_ABS(factorExp2) > PRINTF_ABS(extraFactorExp2)) {
			result.multiply = false;
			result.rawFactor = sf.rawFactor / extraMultiFactor;
		}
		else {
			result.multiply = true;
			result.rawFactor = extraMultiFactor / sf.rawFactor;
		}
	}
	return result;
}

static doubleComponents_t getNormalizedComponents(bool negative, printfSize_t precision, double nonNormalized, scalingFactor_t normalization, int flooredExp10) {
	doubleComponents_t components;
	components.is_negative = negative;
	double scaled = applyScaling(nonNormalized, normalization);

	bool close_to_representation_extremum = ( (-flooredExp10 + (int) precision) >= DBL_MAX_10_EXP - 1 );
	if (close_to_representation_extremum) {
		// We can't have a normalization factor which also accounts for the precision, i.e. moves
		// some decimal digits into the mantissa, since it's unrepresentable, or nearly unrepresentable.
		// So, we'll give up early on getting extra precision...
		return getComponents(negative ? -scaled : scaled, precision);
	}
	components.integral = (int_fast64_t) scaled;
	double remainder = nonNormalized - unapplyScaling((double) components.integral, normalization);
	double precPowerOf10 = powOf10[precision];
	scalingFactor_t accountForPrecision = updateNormalization(normalization, precPowerOf10);
	double scaled_remainder = applyScaling(remainder, accountForPrecision);
	double rounding_threshold = 0.5;

	components.fractional = (int_fast64_t) scaled_remainder; // when precision == 0, the assigned value should be 0
	scaled_remainder -= (double) components.fractional; //when precision == 0, this will not change scaled_remainder

	components.fractional += (scaled_remainder >= rounding_threshold);
	if (scaled_remainder == rounding_threshold) {
		// banker's rounding: Round towards the even number (making the mean error 0)
		components.fractional &= ~((int_fast64_t) 0x1);
	}
	// handle rollover, e.g. the case of 0.99 with precision 1 becoming (0,100),
	// and must then be corrected into (1, 0).
	// Note: for precision = 0, this will "translate" the rounding effect from
	// the fractional part to the integral part where it should actually be
	// felt (as precPowerOf10 is 1)
	if ((double) components.fractional >= precPowerOf10) {
		components.fractional = 0;
		++components.integral;
	}
	return components;
}
#endif // PRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS

static void printBrokenDec(
	doubleComponents_t number_, outputGadget_t* output, printfSize_t precision,
	printfSize_t width, printfFlags_t flags, char *buf, printfSize_t len) {
	if (precision != 0U) {
		// do fractional part, as an unsigned number

		printfSize_t count = precision;

		// %g/%G mandates we skip the trailing 0 digits...
		if ((flags & FLAGS_ADAPT_EXP) && !(flags & FLAGS_HASH) && (number_.fractional > 0)) {
			while(true) {
				int_fast64_t digit = number_.fractional % 10U;
				if (digit != 0) {
					break;
				}
				--count;
				number_.fractional /= 10U;

			}
			// ... and even the decimal point if there are no
			// non-zero fractional part digits (see below)
		}

		if (number_.fractional > 0 || !(flags & FLAGS_ADAPT_EXP) || (flags & FLAGS_HASH) ) {
			while (len < PRINTF_DECIMAL_BUFFER_SIZE) {
				--count;
				buf[len++] = (char)('0' + number_.fractional % 10U);
				if (!(number_.fractional /= 10U)) {
					break;
				}
			}
			// add extra 0s
			while ((len < PRINTF_DECIMAL_BUFFER_SIZE) && (count > 0U)) {
				buf[len++] = '0';
				--count;
			}
			if (len < PRINTF_DECIMAL_BUFFER_SIZE) {
				buf[len++] = '.';
			}
		}
	}
	else {
		if ((flags & FLAGS_HASH) && (len < PRINTF_DECIMAL_BUFFER_SIZE)) {
			buf[len++] = '.';
		}
	}

	// Write the integer part of the number (it comes after the fractional
	// since the character order is reversed)
	while (len < PRINTF_DECIMAL_BUFFER_SIZE) {
		buf[len++] = (char)('0' + (number_.integral % 10));
		if (!(number_.integral /= 10)) {
			break;
		}
	}

	// pad leading zeros
	if (!(flags & FLAGS_LEFT) && (flags & FLAGS_ZEROPAD)) {
		if (width && (number_.is_negative || (flags & (FLAGS_PLUS | FLAGS_SPACE)))) {
			width--;
		}
		while ((len < width) && (len < PRINTF_DECIMAL_BUFFER_SIZE)) {
			buf[len++] = '0';
		}
	}

	if (len < PRINTF_DECIMAL_BUFFER_SIZE) {
		if (number_.is_negative) {
			buf[len++] = '-';
		}
		else if (flags & FLAGS_PLUS) {
			buf[len++] = '+';  // ignore the space if the '+' exists
		}
		else if (flags & FLAGS_SPACE) {
			buf[len++] = ' ';
		}
	}

	outRev_(output, buf, len, width, flags);
}

// internal ftoa for fixed decimal floating point
static void printDecimal(outputGadget_t* output, double number, printfSize_t precision, printfSize_t width, printfFlags_t flags, char* buf, printfSize_t len) {
	doubleComponents_t value_ = getComponents(number, precision);
	printBrokenDec(value_, output, precision, width, flags, buf, len);
}

#if PRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS

// A floor function - but one which only works for numbers whose
// floor value is representable by an int.
static int bastardizedFloor(double x) {
	if (x >= 0) { return (int) x; }
	int n = (int) x;
	return ( ((double) n) == x ) ? n : n-1;
}

// Computes the base-10 logarithm of the input number - which must be an actual
// positive number (not infinity or NaN, nor a sub-normal)
static double log10OfPositive(double positiveNum) {
	// The implementation follows David Gay (https://www.ampl.com/netlib/fp/dtoa.c).
	//
	// Since log_10 ( M * 2^x ) = log_10(M) + x , we can separate the components of
	// our input number, and need only solve log_10(M) for M between 1 and 2 (as
	// the base-2 mantissa is always 1-point-something). In that limited range, a
	// Taylor series expansion of log10(x) should serve us well enough; and we'll
	// take the mid-point, 1.5, as the point of expansion.

	double_with_bit_access dwba = getBitAccess(positiveNum);
	// based on the algorithm by David Gay (https://www.ampl.com/netlib/fp/dtoa.c)
	int exp2 = getExp2(dwba);
	// drop the exponent, so dwba.F comes into the range [1,2)
	dwba.U = (dwba.U & (((double_uint_t) (1) << DOUBLE_STORED_MANTISSA_BITS) - 1U)) |
					 ((double_uint_t) DOUBLE_BASE_EXPONENT << DOUBLE_STORED_MANTISSA_BITS);
	double z = (dwba.F - 1.5);
	return (
		// Taylor expansion around 1.5:
		0.1760912590556812420           // Expansion term 0: ln(1.5)            / ln(10)
		+ z     * 0.2895296546021678851 // Expansion term 1: (M - 1.5)   * 2/3  / ln(10)
#if PRINTF_LOG10_TAYLOR_TERMS > 2
		- z*z   * 0.0965098848673892950 // Expansion term 2: (M - 1.5)^2 * 2/9  / ln(10)
#if PRINTF_LOG10_TAYLOR_TERMS > 3
		+ z*z*z * 0.0428932821632841311 // Expansion term 2: (M - 1.5)^3 * 8/81 / ln(10)
#endif
#endif
		// exact log_2 of the exponent x, with logarithm base change
		+ exp2 * 0.30102999566398119521 // = exp2 * log_10(2) = exp2 * ln(2)/ln(10)
	);
}


static double pow10Ofint(int flooredExp10) {
	// A crude hack for avoiding undesired behavior with barely-normal or slightly-subnormal values.
	if (flooredExp10 == DOUBLE_MAX_SUBNORMAL_EXPONENT_OF_10) {
		return DOUBLE_MAX_SUBNORMAL_POWER_OF_10;
	}
	// Compute 10^(flooredExp10) but (try to) make sure that doesn't overflow
	double_with_bit_access dwba;
	int exp2 = bastardizedFloor(flooredExp10 * 3.321928094887362 + 0.5);
	const double z  = flooredExp10 * 2.302585092994046 - exp2 * 0.6931471805599453;
	const double z2 = z * z;
	dwba.U = ((double_uint_t)(exp2) + DOUBLE_BASE_EXPONENT) << DOUBLE_STORED_MANTISSA_BITS;
	// compute exp(z) using continued fractions,
	// see https://en.wikipedia.org/wiki/Exponential_function#Continued_fractions_for_ex
	dwba.F *= 1 + 2 * z / (2 - z + (z2 / (6 + (z2 / (10 + z2 / 14)))));
	return dwba.F;
}

static void printExpontNum(outputGadget_t* output, double number, printfSize_t precision, printfSize_t width, printfFlags_t flags, char* buf, printfSize_t len) {
	const bool negative = getSignBit(number);
	// This number will decrease gradually (by factors of 10) as we "extract" the exponent out of it
	double absNumber =  negative ? -number : number;

	int flooredExp10;
	bool absExp10CoveredByPowTbl;
	scalingFactor_t normalization;


	// Determine the decimal exponent
	if (absNumber == 0.0) {
		// TODO: This is a special-case for 0.0 (and -0.0); but proper handling is required for denormals more generally.
		flooredExp10 = 0; // ... and no need to set a normalization factor or check the powers table
	}
	else  {
		double exp10 = log10OfPositive(absNumber);
		flooredExp10 = bastardizedFloor(exp10);
		double p10 = pow10Ofint(flooredExp10);
		// correct for rounding errors
		if (absNumber < p10) {
			flooredExp10--;
			p10 /= 10;
		}
		absExp10CoveredByPowTbl = PRINTF_ABS(flooredExp10) < PRINTF_MAX_PRECOMPUTED_POWER_OF_10;
		normalization.rawFactor = absExp10CoveredByPowTbl ? powOf10[PRINTF_ABS(flooredExp10)] : p10;
	}

	// We now begin accounting for the widths of the two parts of our printed field:
	// the decimal part after decimal exponent extraction, and the base-10 exponent part.
	// For both of these, the value of 0 has a special meaning, but not the same one:
	// a 0 exponent-part width means "don't print the exponent"; a 0 decimal-part width
	// means "use as many characters as necessary".

	bool fallbackToDecOnly = false;
	if (flags & FLAGS_ADAPT_EXP) {
		int required_significant_digits = (precision == 0) ? 1 : (int) precision;
		// Should we want to fall-back to "%f" mode, and only print the decimal part?
		fallbackToDecOnly = (flooredExp10 >= -4 && flooredExp10 < required_significant_digits);
		// Now, let's adjust the precision
		// This also decided how we adjust the precision value - as in "%g" mode,
		// "precision" is the number of _significant digits_, and this is when we "translate"
		// the precision value to an actual number of decimal digits.
		int precision_ = fallbackToDecOnly ?
										 (int) precision - 1 - flooredExp10 :
				(int) precision - 1; // the presence of the exponent ensures only one significant digit comes before the decimal point
		precision = (precision_ > 0 ? (unsigned) precision_ : 0U);
		flags |= FLAGS_PRECISION;   // make sure printBrokenDec respects our choice above
	}

	normalization.multiply = (flooredExp10 < 0 && absExp10CoveredByPowTbl);
	bool should_skip_normalization = (fallbackToDecOnly || flooredExp10 == 0);
	doubleComponents_t decimalPartComponents =
		should_skip_normalization ?
		getComponents(negative ? -absNumber : absNumber, precision) :
		getNormalizedComponents(negative, precision, absNumber, normalization, flooredExp10);

	// Account for roll-over, e.g. rounding from 9.99 to 100.0 - which effects
	// the exponent and may require additional tweaking of the parts
	if (fallbackToDecOnly) {
		if ((flags & FLAGS_ADAPT_EXP) && flooredExp10 >= -1 && decimalPartComponents.integral == powOf10[flooredExp10 + 1]) {
			flooredExp10++; // Not strictly necessary, since floored_exp10 is no longer really used
			precision--;
			// ... and it should already be the case that decimalPartComponents.fractional == 0
		}
		// TODO: What about rollover strictly within the fractional part?
	}
	else {
		if (decimalPartComponents.integral >= 10) {
			flooredExp10++;
			decimalPartComponents.integral = 1;
			decimalPartComponents.fractional = 0;
		}
	}

	// the floored_exp10 format is "E%+03d" and largest possible floored_exp10 value for a 64-bit double
	// is "307" (for 2^1023), so we set aside 4-5 characters overall
	printfSize_t exp10PartWidth = fallbackToDecOnly ? 0U : (PRINTF_ABS(flooredExp10) < 100) ? 4U : 5U;

	printfSize_t decimalPartWidth =
		((flags & FLAGS_LEFT) && exp10PartWidth) ?
			// We're padding on the right, so the width constraint is the exponent part's
			// problem, not the decimal part's, so we'll use as many characters as we need:
			0U :
			// We're padding on the left; so the width constraint is the decimal part's
			// problem. Well, can both the decimal part and the exponent part fit within our overall width?
			((width > exp10PartWidth) ?
				// Yes, so we limit our decimal part's width.
				// (Note this is trivially valid even if we've fallen back to "%f" mode)
				width - exp10PartWidth :
				// No; we just give up on any restriction on the decimal part and use as many
				// characters as we need
				0U);

	const printfSize_t printentExpStartPos = output->pos;
	printBrokenDec(decimalPartComponents, output, precision, decimalPartWidth, flags, buf, len);

	if (! fallbackToDecOnly) {
		putcharViaGadget(output, (flags & FLAGS_UPPERCASE) ? 'E' : 'e');
		printInt(output,
									ABS_FOR_PRINTING(flooredExp10),
									flooredExp10 < 0, 10, 0, exp10PartWidth - 1,
								FLAGS_ZEROPAD | FLAGS_PLUS);
		if (flags & FLAGS_LEFT) {
			// We need to right-pad with spaces to meet the width requirement
			while (output->pos - printentExpStartPos < width) {
				putcharViaGadget(output, ' ');
			}
		}
	}
}
#endif  // PRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS

static void printFloat(outputGadget_t* output, double value, printfSize_t precision, printfSize_t width, printfFlags_t flags, bool preferExponential) {
	char buf[PRINTF_DECIMAL_BUFFER_SIZE];
	printfSize_t len = 0U;

	// test for special values
	if (value != value) {
		outRev_(output, "nan", 3, width, flags);
		return;
	}
	if (value < -DBL_MAX) {
		outRev_(output, "fni-", 4, width, flags);
		return;
	}
	if (value > DBL_MAX) {
		outRev_(output, (flags & FLAGS_PLUS) ? "fni+" : "fni", (flags & FLAGS_PLUS) ? 4U : 3U, width, flags);
		return;
	}

	if (!preferExponential &&
			((value > PRINTF_FLOAT_NOTATION_THRESHOLD) || (value < -PRINTF_FLOAT_NOTATION_THRESHOLD))) {
		// The required behavior of standard printf is to print _every_ integral-part digit -- which could mean
		// printing hundreds of characters, overflowing any fixed internal buffer and necessitating a more complicated
		// implementation.
#if PRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS
		printExpontNum(output, value, precision, width, flags, buf, len);
#endif
		return;
	}

	// set default precision, if not set explicitly
	if (!(flags & FLAGS_PRECISION)) {
		precision = PRINTF_DEFAULT_FLOAT_PRECISION;
	}

	// limit precision so that our integer holding the fractional part does not overflow
	while ((len < PRINTF_DECIMAL_BUFFER_SIZE) && (precision > PRINTF_MAX_SUPPORTED_PRECISION)) {
		buf[len++] = '0'; // This respects the precision in terms of result length only
		precision--;
	}

#if PRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS
	if (preferExponential)
		printExpontNum(output, value, precision, width, flags, buf, len);
	else
#endif
		printDecimal(output, value, precision, width, flags, buf, len);
}

#endif  // (PRINTF_SUPPORT_DECIMAL_SPECIFIERS || PRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS)

// Advances the format pointer past the flags, and returns the parsed flags
// due to the characters passed
static printfFlags_t parseFlags(const char** format) {
	printfFlags_t flags = 0U;
	do {
		switch (**format) {
			case '0': flags |= FLAGS_ZEROPAD; (*format)++; break;
			case '-': flags |= FLAGS_LEFT;    (*format)++; break;
			case '+': flags |= FLAGS_PLUS;    (*format)++; break;
			case ' ': flags |= FLAGS_SPACE;   (*format)++; break;
			case '#': flags |= FLAGS_HASH;    (*format)++; break;
			default : return flags;
		}
	} while (true);
}

static inline void formatStringLoop(outputGadget_t* output, const char* format, va_list args) {
#if PRINTF_CHECK_FOR_NUL_IN_FORMAT_SPECIFIER
#define ADVANCE_IN_FORMAT_STRING(cptr_) do { (cptr_)++; if (!*(cptr_)) return; } while(0)
#else
#define ADVANCE_IN_FORMAT_STRING(cptr_) (cptr_)++
#endif


	while (*format) {
		if (*format != '%') {
			// A regular content character
			putcharViaGadget(output, *format);
			format++;
			continue;
		}
		// We're parsing a format specifier: %[flags][width][.precision][length]
		ADVANCE_IN_FORMAT_STRING(format);

		printfFlags_t flags = parseFlags(&format);

		// evaluate width field
		printfSize_t width = 0U;
		if (isdigit(*format)) {
			width = (printfSize_t) atou_(&format);
		}
		else if (*format == '*') {
			const int w = va_arg(args, int);
			if (w < 0) {
				flags |= FLAGS_LEFT;    // reverse padding
				width = (printfSize_t)-w;
			}
			else {
				width = (printfSize_t)w;
			}
			ADVANCE_IN_FORMAT_STRING(format);
		}

		// evaluate precision field
		printfSize_t precision = 0U;
		if (*format == '.') {
			flags |= FLAGS_PRECISION;
			ADVANCE_IN_FORMAT_STRING(format);
			if (isdigit(*format)) {
				precision = (printfSize_t) atou_(&format);
			}
			else if (*format == '*') {
				const int precision_ = va_arg(args, int);
				precision = precision_ > 0 ? (printfSize_t) precision_ : 0U;
				ADVANCE_IN_FORMAT_STRING(format);
			}
		}

		// evaluate length field
		switch (*format) {
#ifdef PRINTF_SUPPORT_MSVC_STYLE_INTEGER_SPECIFIERS
			case 'I' : {
				ADVANCE_IN_FORMAT_STRING(format);
				// Greedily parse for size in bits: 8, 16, 32 or 64
				switch(*format) {
					case '8':               flags |= FLAGS_INT8;
						ADVANCE_IN_FORMAT_STRING(format);
						break;
					case '1':
						ADVANCE_IN_FORMAT_STRING(format);
					if (*format == '6') { format++; flags |= FLAGS_INT16; }
						break;
					case '3':
						ADVANCE_IN_FORMAT_STRING(format);
						if (*format == '2') { ADVANCE_IN_FORMAT_STRING(format); flags |= FLAGS_INT32; }
						break;
					case '6':
						ADVANCE_IN_FORMAT_STRING(format);
						if (*format == '4') { ADVANCE_IN_FORMAT_STRING(format); flags |= FLAGS_INT64; }
						break;
					default: break;
				}
				break;
			}
#endif
			case 'l' :
				flags |= FLAGS_LONG;
				ADVANCE_IN_FORMAT_STRING(format);
				if (*format == 'l') {
					flags |= FLAGS_LONG_LONG;
					ADVANCE_IN_FORMAT_STRING(format);
				}
				break;
			case 'h' :
				flags |= FLAGS_SHORT;
				ADVANCE_IN_FORMAT_STRING(format);
				if (*format == 'h') {
					flags |= FLAGS_CHAR;
					ADVANCE_IN_FORMAT_STRING(format);
				}
				break;
			case 't' :
				flags |= (sizeof(ptrdiff_t) == sizeof(long) ? FLAGS_LONG : FLAGS_LONG_LONG);
				ADVANCE_IN_FORMAT_STRING(format);
				break;
			case 'j' :
				flags |= (sizeof(intmax_t) == sizeof(long) ? FLAGS_LONG : FLAGS_LONG_LONG);
				ADVANCE_IN_FORMAT_STRING(format);
				break;
			case 'z' :
				flags |= (sizeof(size_t) == sizeof(long) ? FLAGS_LONG : FLAGS_LONG_LONG);
				ADVANCE_IN_FORMAT_STRING(format);
				break;
			default:
				break;
		}

		// evaluate specifier
		switch (*format) {
			case 'd' :
			case 'i' :
			case 'u' :
			case 'x' :
			case 'X' :
			case 'o' :
			case 'b' : {

				if (*format == 'd' || *format == 'i') {
					flags |= FLAGS_SIGNED;
				}

				numericBase_t base;
				if (*format == 'x' || *format == 'X') {
					base = BASE_HEX;
				}
				else if (*format == 'o') {
					base =  BASE_OCTAL;
				}
				else if (*format == 'b') {
					base =  BASE_BINARY;
				}
				else {
					base = BASE_DECIMAL;
					flags &= ~FLAGS_HASH; // decimal integers have no alternative presentation
				}

				if (*format == 'X') {
					flags |= FLAGS_UPPERCASE;
				}

				format++;
				// ignore '0' flag when precision is given
				if (flags & FLAGS_PRECISION) {
					flags &= ~FLAGS_ZEROPAD;
				}

				if (flags & FLAGS_SIGNED) {
					// A signed specifier: d, i or possibly I + bit size if enabled

					if (flags & FLAGS_LONG_LONG) {
#if PRINTF_SUPPORT_LONG_LONG
						const long long value = va_arg(args, long long);
						printInt(output, ABS_FOR_PRINTING(value), value < 0, base, precision, width, flags);
#endif
					}
					else if (flags & FLAGS_LONG) {
						const long value = va_arg(args, long);
						printInt(output, ABS_FOR_PRINTING(value), value < 0, base, precision, width, flags);
					}
					else {
						// We never try to interpret the argument as something potentially-smaller than int,
						// due to integer promotion rules: Even if the user passed a short int, short unsigned
						// etc. - these will come in after promotion, as int's (or unsigned for the case of
						// short unsigned when it has the same size as int)
						const int value =
							(flags & FLAGS_CHAR) ? (signed char) va_arg(args, int) :
							(flags & FLAGS_SHORT) ? (short int) va_arg(args, int) :
							va_arg(args, int);
						printInt(output, ABS_FOR_PRINTING(value), value < 0, base, precision, width, flags);
					}
				}
				else {
					// An unsigned specifier: u, x, X, o, b

					flags &= ~(FLAGS_PLUS | FLAGS_SPACE);

					if (flags & FLAGS_LONG_LONG) {
#if PRINTF_SUPPORT_LONG_LONG
						printInt(output, (printfUnsignedValue_t) va_arg(args, unsigned long long), false, base, precision, width, flags);
#endif
					}
					else if (flags & FLAGS_LONG) {
						printInt(output, (printfUnsignedValue_t) va_arg(args, unsigned long), false, base, precision, width, flags);
					}
					else {
						const unsigned int value =
							(flags & FLAGS_CHAR) ? (unsigned char)va_arg(args, unsigned int) :
							(flags & FLAGS_SHORT) ? (unsigned short int)va_arg(args, unsigned int) :
							va_arg(args, unsigned int);
						printInt(output, (printfUnsignedValue_t) value, false, base, precision, width, flags);
					}
				}
				break;
			}
#if PRINTF_SUPPORT_DECIMAL_SPECIFIERS
			case 'f' :
			case 'F' :
				if (*format == 'F') flags |= FLAGS_UPPERCASE;
				printFloat(output, va_arg(args, double), precision, width, flags, PRINTF_PREFER_DECIMAL);
				format++;
				break;
#endif
#if PRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS
			case 'e':
			case 'E':
			case 'g':
			case 'G':
				if ((*format == 'g')||(*format == 'G')) flags |= FLAGS_ADAPT_EXP;
				if ((*format == 'E')||(*format == 'G')) flags |= FLAGS_UPPERCASE;
				printFloat(output, va_arg(args, double), precision, width, flags, PRINTF_PREFER_EXPONENTIAL);
				format++;
				break;
#endif  // PRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS
			case 'c' : {
				printfSize_t l = 1U;
				// pre padding
				if (!(flags & FLAGS_LEFT)) {
					while (l++ < width) {
						putcharViaGadget(output, ' ');
					}
				}
				// char output
				putcharViaGadget(output, (char) va_arg(args, int) );
				// post padding
				if (flags & FLAGS_LEFT) {
					while (l++ < width) {
						putcharViaGadget(output, ' ');
					}
				}
				format++;
				break;
			}

			case 's' : {
				const char* p = va_arg(args, char*);
				if (p == NULL) {
					outRev_(output, ")llun(", 6, width, flags);
				}
				else {
					printfSize_t l = (printfSize_t)strnlen_s(p, precision ? precision : PRINTF_MAX_POSSIBLE_BUFFER_SIZE);
					// pre padding
					if (flags & FLAGS_PRECISION) {
						l = (l < precision ? l : precision);
					}
					if (!(flags & FLAGS_LEFT)) {
						while (l++ < width) {
							putcharViaGadget(output, ' ');
						}
					}
					// string output
					while ((*p != 0) && (!(flags & FLAGS_PRECISION) || precision)) {
						putcharViaGadget(output, *(p++));
						--precision;
					}
					// post padding
					if (flags & FLAGS_LEFT) {
						while (l++ < width) {
							putcharViaGadget(output, ' ');
						}
					}
				}
				format++;
				break;
			}

			case 'p' : {
				width = sizeof(void*) * 2U + 2; // 2 hex chars per byte + the "0x" prefix
				flags |= FLAGS_ZEROPAD | FLAGS_POINTER;
				uintptr_t value = (uintptr_t)va_arg(args, void*);
				printInt(output, (printfUnsignedValue_t) value, false, BASE_HEX, precision, width, flags);
				format++;
				break;
			}

			case '%' :
				putcharViaGadget(output, '%');
				format++;
				break;

			// Many people prefer to disable support for %n, as it lets the caller
			// engineer a write to an arbitrary location, of a value the caller
			// effectively controls - which could be a security concern in some cases.
#if PRINTF_SUPPORT_WRITEBACK_SPECIFIER
			case 'n' : {
				if       (flags & FLAGS_CHAR)      *(va_arg(args, char*))      = (char) output->pos;
				else if  (flags & FLAGS_SHORT)     *(va_arg(args, short*))     = (short) output->pos;
				else if  (flags & FLAGS_LONG)      *(va_arg(args, long*))      = (long) output->pos;
#if PRINTF_SUPPORT_LONG_LONG
				else if  (flags & FLAGS_LONG_LONG) *(va_arg(args, long long*)) = (long long int) output->pos;
#endif // PRINTF_SUPPORT_LONG_LONG
				else                               *(va_arg(args, int*))       = (int) output->pos;
				format++;
				break;
			}
#endif // PRINTF_SUPPORT_WRITEBACK_SPECIFIER

			default :
				putcharViaGadget(output, *format);
				format++;
				break;
		}
	}
}

// internal vsnprintf - used for implementing _all library functions
static int vsnprintf_impl(outputGadget_t* output, const char* format, va_list args) {
	// Note: The library only calls vsnprintf_impl() with output->pos being 0. However, it is
	// possible to call this function with a non-zero pos value for some "remedial printing".
	formatStringLoop(output, format, args);

	// termination
	appendTermWithGadget(output);

	// return written chars without terminating \0
	return (int)output->pos;
}

///////////////////////////////////////////////////////////////////////////////

int vprintf(const char* format, va_list arg) {
	outputGadget_t gadget = externPutcharGadget();
	return vsnprintf_impl(&gadget, format, arg);
}

int vsnprintf(char* s, size_t n, const char* format, va_list arg) {
	outputGadget_t gadget = bufferGadget(s, n);
	return vsnprintf_impl(&gadget, format, arg);
}

int vsprintf(char* s, const char* format, va_list arg) {
	return vsnprintf(s, PRINTF_MAX_POSSIBLE_BUFFER_SIZE, format, arg);
}

int vfctprintf(void (*out)(char c, void* extraArg), void* extraArg, const char* format, va_list arg) {
	outputGadget_t gadget = functionGadget(out, extraArg);
	return vsnprintf_impl(&gadget, format, arg);
}

int printf(const char* format, ...) {
	va_list args;
	va_start(args, format);
	const int ret = vprintf(format, args);
	va_end(args);
	return ret;
}

int sprintf(char* s, const char* format, ...) {
	va_list args;
	va_start(args, format);
	const int ret = vsprintf(s, format, args);
	va_end(args);
	return ret;
}

int snprintf(char* s, size_t n, const char* format, ...) {
	va_list args;
	va_start(args, format);
	const int ret = vsnprintf(s, n, format, args);
	va_end(args);
	return ret;
}

int fctprintf(void (*out)(char c, void* extraArg), void* extraArg, const char* format, ...) {
	va_list args;
	va_start(args, format);
	const int ret = vfctprintf(out, extraArg, format, args);
	va_end(args);
	return ret;
}