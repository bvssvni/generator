#ifndef GENERATOR_GUARD
#define GENERATOR_GUARD
// This is the types of generators.
enum {
	GENERATOR_TYPE_LINEAR = 1,
	GENERATOR_TYPE_POWER2 = 2,
	GENERATOR_TYPE_SYMMETRIC = 3,
};
typedef struct generator generator;
// Describes data for a linear generator.
// The formula is of type ax + b.
typedef struct
{
	int a;
	int b;
} generator_linear;
// Power2 has no data.
typedef struct
{
} generator_power2;
// Symmetric generator has limited number of bits.
typedef struct
{
	int bits;
} generator_symmetric;
// Unites types of generators into one structure.
struct generator
{
	int type;
	union {
		generator_linear linear;
		generator_power2 power2;
		generator_symmetric symmetric;
	} data;
};
// Creates a generator of the form ax + b.
generator generator_Linear(int a, int b);
// Creates a generator of the form 2^x.
generator generator_Power2(void);
// Creates a symmetric generator.
// This is the generator of numbers where the upper half is a mirror image
// of the lower.
generator generator_Symmetric(int bits);
// Returns the next number from a generator.
int generator_Next(generator g, uint64_t previous, uint64_t *res);
// Combines numbers created from two generators with an OR operation.
int generator_Or(generator a, generator b, uint64_t previous, uint64_t *res);
// Combines numbers created from two generators with an AND operation.
int generator_And(generator a, generator b, uint64_t previous, uint64_t *res);
// Combins numbers created from two generators with an EXCEPT operation.
int generator_Except(generator a, generator b, uint64_t previous, uint64_t *res);
// End guard macro.
#endif
// Used to emulate namespace aliasing.
#ifndef FUNC_GUARD
#define FUNC_GUARD
#define func(ns, n) static __typeof__(ns##_##n) * const n = ns##_##n
#endif
