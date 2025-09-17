#include "cyclone/rational.h"

#include <assert.h>

#define MP_OK(action) \
	assert((action) == MP_OKAY)

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
