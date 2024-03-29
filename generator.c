// Include standard headers.
#include <stdint.h>
#include <stdio.h>
// Include the header for this file.
#include "generator.h"
// Creates a linear generator.
generator generator_Linear(int a, int b)
{
	return (generator){.type = GENERATOR_TYPE_LINEAR,
		.data.linear = {.a = a, .b = b}};
}
// Creates a generator of power 2.
generator generator_Power2(void)
{
	return (generator){.type = GENERATOR_TYPE_POWER2};
}
// Creates a symmetric generator.
generator generator_Symmetric(int bits)
{
	return (generator) {.type = GENERATOR_TYPE_SYMMETRIC,
		.data.symmetric = {.bits = bits}};
}
// Creates an upper triangle generator.
generator generator_UpperTriangle(int width)
{
	return (generator){.type = GENERATOR_TYPE_UPPER_TRIANGLE,
		.data.matrix = {.width = width}};
}
// Gets the next number in sequence from a linear generator.
int generator_linear_Next
(generator_linear g, uint64_t previous, uint64_t *res)
{
	*res = ((previous - g.b) / g.a + 1) * g.a + g.b;
	if (*res < previous) return 0;
	// If the number is larger, we did not go beyond the end.
	return 1;
}
// Returns the next power 2 number.
int generator_power2_Next
(generator_power2 g, uint64_t previous, uint64_t *res)
{
	// Special case when the previous number is 0.
	if (previous == 0ULL) {
		*res = 1ULL;
		return 1;
	}
	// General case.
	*res = ((previous >> 1) + 1) << 1;
	if (*res < previous) return 0;
	// If the number is larger, we did not go beyond the end.
	return 1;
}
// A macro for reversing the bits in a byte.
#define REVERSE(b) \
((((b) * 0x80200802ULL) & 0x0884422110ULL) * 0x0101010101ULL >> 32)
// Returns the next number for a symmetric generator.
int generator_symmetric_Next
(generator_symmetric g, uint64_t previous, uint64_t *res)
{
	// Need a mask to filter out the part.
	int odd = g.bits & 1;
	// Odd.
	int half = (g.bits >> 1) + odd;
	uint64_t halfmask = ~0ULL >> (64-half);
	// Get the number that creates the mirror image.
	uint64_t p = previous >> (half-odd);
	p++;
	if (p > halfmask) return 0;
	// Support reversing of multiple bytes.
	if (half > 8) {
		*res = 0ULL;
		int i;
		for (i = 0; i < half; i++) {
			*res |= ((p >> i) & 1) << (half-i-1);
		}
		*res |= (p << (half-odd));
		return 1;
	} else {
		// If not beyond the scope, create the mirror image.
		*res = (p << (half-odd)) | ((REVERSE(p) >> (8-half)) & halfmask);
		return 1;
	}
	return 0;
}
// Returns the next number for an upper triangle generator.
int generator_upper_triangle_Next
(generator_matrix g, uint64_t previous, uint64_t *res)
{
	uint64_t y = previous / g.width;
	uint64_t x = previous - y * g.width;
	x = x <= y ? y+1 : x+1;
	if (x >= g.width) {
		y++;
		x = y+1;
	}
	if (x >= g.width || y >= g.width) return 0;
	
	*res = x + y * g.width;
	return 1;
}
// Returns the next generated number for all kinds of generators.
int generator_Next(generator g, uint64_t previous, uint64_t *res)
{
	switch (g.type) {
		case GENERATOR_TYPE_LINEAR:
			return generator_linear_Next(g.data.linear, previous, res);
		case GENERATOR_TYPE_POWER2:
			return generator_power2_Next(g.data.power2, previous, res);
		case GENERATOR_TYPE_SYMMETRIC:
			return generator_symmetric_Next(g.data.symmetric, previous, res);
		case GENERATOR_TYPE_UPPER_TRIANGLE:
			return generator_upper_triangle_Next(g.data.matrix, previous, res);
	}
	// Unknown type of generator.
	return 0;
}
// Returns the least value generated from two generators.
int generator_Or(generator a, generator b, uint64_t previous, uint64_t *res)
{
	uint64_t aVal, bVal;
	int ok = generator_Next(a, previous, &aVal);
	if (ok) *res = aVal;
	// Check the other generator.
	ok |= generator_Next(b, previous, &bVal);
	if (ok == 0) return 0;
	// Both got a value so we return the smallest one.
	if (bVal < aVal) *res = bVal;
	return 1;
}
// Returns the numbers generated in common by two generators.
int generator_And(generator a, generator b, uint64_t previous, uint64_t *res)
{
	while (1) {
		uint64_t aVal, bVal;
		int ok = generator_Next(a, previous, &aVal);
		if (!ok) return 0;
		// Check the other generator.
		ok = generator_Next(b, previous, &bVal);
		if (!ok) return 0;
		// Both got a value so we return the smallest one.
		if (bVal == aVal) {*res = bVal; return 1;}
		// None equal, go to next number.
		previous = aVal < bVal ? aVal : bVal;
	}
	return 0;
}
// Returns the numbers generated by _a_ that is not generated by _b_.
int generator_Except(generator a, generator b, uint64_t previous, uint64_t *res)
{
	while (1) {
		uint64_t aVal, bVal;
		int ok = generator_Next(a, previous, &aVal);
		if (!ok) return 0;
		// Check the other generator.
		ok = generator_Next(b, previous, &bVal);
		if (!ok || bVal != aVal) {return *res = aVal; return 1;}
		// Go to next number.
		previous = aVal < bVal ? aVal : bVal;
	}
	return 0;
}

