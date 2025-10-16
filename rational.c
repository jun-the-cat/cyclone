#include "cyclone/rational.h"

#include <limits.h>
#include <assert.h>

#define MP_OK(action) assert((action) == MP_OKAY)

rational
rational_zero(void)
{
	mp_int n, d;
    MP_OK(mp_init_i32(&n, 0));
	MP_OK(mp_init_i32(&d, 1));
	return (rational) { n, d };
}

rational
rational_of(mp_int *numerator, mp_int *denominator)
{
	rational fraction = { *numerator, *denominator };
	return rational_simplify(&fraction);
}

rational
rational_simplify(rational *fraction)
{
	mp_int gcd, rem;
	rational simplified = rational_zero();
	MP_OK(mp_init_multi(&gcd, &rem, NULL));
	MP_OK(mp_gcd(&(fraction->numerator), &(fraction->denominator), &gcd));
	MP_OK(mp_div(&(fraction->numerator), &gcd, &(simplified.numerator), &rem));
	MP_OK(mp_div(&(fraction->denominator), &gcd, &(simplified.denominator), &rem));
	return simplified;
}

double
rational_to_double(rational *fraction)
{
	return mp_get_double(&(fraction->numerator)) / mp_get_double(&(fraction->denominator));
}

// Rationals

int
rational_compare(enum rational_cmp cmp, rational *a, rational *b)
{
	if (RATIONAL_GTE == cmp) {
		return rational_compare(RATIONAL_EQ, a, b)
			|| rational_compare(RATIONAL_GT, a, b);
	} else if (RATIONAL_LTE == cmp) {
		return rational_compare(RATIONAL_EQ, a, b)
			|| rational_compare(RATIONAL_LT, a, b);
	}
	
	int denom_eq = mp_cmp(&(a->denominator), &(b->denominator)) == MP_EQ;
    
	if (denom_eq) {
	    return mp_cmp(&(a->numerator), &(b->numerator)) == cmp;
	}
	else {
		double d_a = rational_to_double(a),
			   d_b = rational_to_double(b);

		if (RATIONAL_LT == cmp) {
			return d_a < d_b;
		}
		else if (RATIONAL_GT == cmp) {
			return d_a > d_b;
		}
	}

	return 0;
}

int
rational_compare_mp(enum rational_cmp cmp, rational *a, mp_int *b, int mask)
{
	if (RATIONAL_GTE == cmp) {
		return rational_compare_mp(RATIONAL_EQ, a, b, mask)
			|| rational_compare_mp(RATIONAL_GT, a, b, mask);
	} else if (RATIONAL_LTE == cmp) {
		return rational_compare_mp(RATIONAL_EQ, a, b, mask)
			|| rational_compare_mp(RATIONAL_LT, a, b, mask);
	}
	
	if (rational_is_fraction(a)) {
		if (RATIONAL_EQ == cmp) {
			return 0;
		}

		double d_a = rational_to_double(a),
			   d_b = mp_get_double(b);
		if (RATIONAL_LT == cmp) {
			return (d_a < d_b) ^ mask;
		}
		else {
			return (d_a > d_b) ^ mask;
		}
	}
	else {
		return (mp_cmp(&(a->numerator), b) == cmp) ^ mask;
	}
}

int
rational_compare_integer(enum rational_cmp cmp, rational *a, int b, int mask)
{
	if (RATIONAL_GTE == cmp) {
		return rational_compare_integer(RATIONAL_EQ, a, b, mask)
			|| rational_compare_integer(RATIONAL_GT, a, b, mask);
	} else if (RATIONAL_LTE == cmp) {
		return rational_compare_integer(RATIONAL_EQ, a, b, mask)
			|| rational_compare_integer(RATIONAL_LT, a, b, mask);
	}

	if (rational_is_fraction(a)) {
		if (RATIONAL_EQ == cmp) {
			return 0;
		}

		double d_a = rational_to_double(a);
		if (RATIONAL_LT == cmp) {
			return (d_a < b) ^ mask;
		}
		else {
			return (d_a > b) ^ mask;
		}
	}
	else {
		int numerator = mp_get_i32(&(a->numerator));
		if (RATIONAL_EQ == cmp) {
			return numerator == b;
		}
		else if (RATIONAL_LT == cmp) {
			return (numerator < b) ^ mask;
		}
		else {
			return (numerator > b) ^ mask;
		}
	}
}

int
rational_compare_double(enum rational_cmp cmp, rational *a, double b, int mask)
{
	if (RATIONAL_GTE == cmp) {
		return rational_compare_double(RATIONAL_EQ, a, b, mask)
			|| rational_compare_double(RATIONAL_GT, a, b, mask);
	} else if (RATIONAL_LTE == cmp) {
		return rational_compare_double(RATIONAL_EQ, a, b, mask)
			|| rational_compare_double(RATIONAL_LT, a, b, mask);
	}
	
	double d_a = rational_to_double(a);
	if (RATIONAL_EQ == cmp) {
		// TODO: Comparisons with inexact decimals in this way are flaky at best.
		// Make sure to look at using DBL_EPSILON to get a more robust equality
		// comparison written.
		return d_a == b;
	}
	else if (RATIONAL_LT == cmp) {
		return (d_a < b) ^ mask;
	}
	else {
		return (d_a > b) ^ mask;
	}
}
