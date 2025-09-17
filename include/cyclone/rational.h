
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

#endif
