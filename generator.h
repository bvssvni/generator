/*
 Generator - Boolean algebra on 64 bit pattern generators, written in C.
 BSD license.
 by Sven Nilsen, 2012
 http://www.cutoutpro.com
 
 Version: 0.000 in angular degrees version notation
 http://isprogrammingeasy.blogspot.no/2012/08/angular-degrees-versioning-notation.html
 */
/*
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 The views and conclusions contained in the software and documentation are those
 of the authors and should not be interpreted as representing official policies,
 either expressed or implied, of the FreeBSD Project.
 */
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
