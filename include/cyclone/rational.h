
#ifndef _RATIONAL_H_
#define _RATIONAL_H_

#include "cyclone/bignum.h"

typedef struct {
	mp_int numerator;
	mp_int denominator;
} rational;

rational rational_zero(void);
rational rational_of(mp_int *numerator, mp_int *denominator);
rational rational_simplify(rational *fraction);
double   rational_to_double(rational *fraction);

// Comparisons
enum rational_cmp {
	RATIONAL_GT  = MP_GT,
	RATIONAL_EQ  = MP_EQ,
	RATIONAL_LT  = MP_LT,
	RATIONAL_GTE =  2,
	RATIONAL_LTE = -2
};
 
#define rational_is_fraction(a) (mp_get_i32(&((a)->denominator)) > 1)
int rational_compare(enum rational_cmp cmp, rational *a, rational *b);
int rational_compare_mp(enum rational_cmp cmp, rational *a, mp_int *b, int mask);
int rational_compare_double(enum rational_cmp cmp, rational *a, double b, int mask);
int rational_compare_integer(enum rational_cmp cmp, rational *a, int b, int mask);

// Purely Rational Arithmetic

#endif
