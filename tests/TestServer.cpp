#include <criterion/criterion.h>
#include "Utils.hpp"

Test(atof, string_to_float)
{
	float	result;
	float	expected;

	result = 3.5;
	expected = 3.5;
	cr_assert_float_eq(result, expected, EPSILON);
}
