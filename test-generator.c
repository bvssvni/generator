#if 0
gcc -o test-generator *.c -Wall -Wfatal-errors -O3
if [ "$?" = "0" ]; then
	time ./test-generator
fi
exit
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "generator.h"

func(generator, Linear);
func(generator, Power2);
func(generator, Symmetric);
func(generator, Next);
func(generator, Or);
func(generator, And);
func(generator, Except);

void test_linear_1(void)
{
	generator g = Linear(1, 0);
	uint64_t buf;
	Next(g, 0, &buf);
	assert(buf == 1ULL);
	Next(g, 1, &buf);
	assert(buf == 2ULL);
	Next(g, 2, &buf);
	assert(buf == 3ULL);
}

void test_power2_1(void)
{
	generator g = Power2();
	uint64_t buf;
	Next(g, 0, &buf);
	assert(buf == 1ULL);
	Next(g, 1, &buf);
	assert(buf == 2ULL);
	Next(g, 2, &buf);
	assert(buf == 4ULL);
}

void test_symmetric_1(void)
{
	generator g = Symmetric(16);
	uint64_t buf;
	Next(g, 0, &buf);
	assert(buf == 0x180ULL);
	Next(g, 0x180ULL, &buf);
	assert(buf == 0x240ULL);
	Next(g, 0x240ULL, &buf);
	assert(buf == 0x3C0ULL);
}

void test_symmetric_2(void)
{
	generator g = Symmetric(8);
	uint64_t buf;
	Next(g, 0, &buf);
	assert(buf == 0x18ULL);
	Next(g, 0x18ULL, &buf);
	assert(buf == 0x24ULL);
	Next(g, 0x24ULL, &buf);
	assert(buf == 0x3CULL);
}

void test_symmetric_3(void)
{
	generator g = Symmetric(64);
	uint64_t buf;
	Next(g, 0, &buf);
	assert(buf == 0x180000000ULL);
	Next(g, 0x180000000ULL, &buf);
	assert(buf == 0x240000000ULL);
	Next(g, 0x240000000ULL, &buf);
	assert(buf == 0x3C0000000ULL);
}

void test_symmetric_4(void)
{
	generator g = Symmetric(15);
	uint64_t buf;
	Next(g, 0, &buf);
	assert(buf == 0x80ULL);
	Next(g, 0x80ULL, &buf);
	assert(buf == 0x140ULL);
	Next(g, 0x140ULL, &buf);
	assert(buf == 0x1C0ULL);
}


void test_symmetric_5(void)
{
	generator g = Symmetric(7);
	uint64_t buf;
	Next(g, 0, &buf);
	assert(buf == 0x8ULL);
	Next(g, 0x8ULL, &buf);
	assert(buf == 0x14ULL);
	Next(g, 0x14ULL, &buf);
	assert(buf == 0x1CULL);
}

void test_symmetric_6(void)
{
	generator g = Symmetric(63);
	uint64_t buf;
	Next(g, 0, &buf);
	assert(buf == 0x80000000ULL);
	Next(g, 0x80000000ULL, &buf);
	assert(buf == 0x140000000ULL);
	Next(g, 0x140000000ULL, &buf);
	assert(buf == 0x1C0000000ULL);
}

void test_or_1(void)
{
	generator a = Linear(2,0);
	generator b = Linear(3,0);
	uint64_t buf;
	Or(a, b, 0, &buf);
	assert(buf == 2ULL);
	Or(a, b, 2ULL, &buf);
	assert(buf == 3ULL);
	Or(a, b, 3ULL, &buf);
	assert(buf == 4ULL);
	Or(a, b, 4ULL, &buf);
	assert(buf == 6ULL);
}

void test_and_1(void)
{
	generator a = Linear(2,0);
	generator b = Linear(3,0);
	uint64_t buf;
	And(a, b, 0ULL, &buf);
	assert(buf == 6ULL);
	And(a, b, 6ULL, &buf);
	assert(buf == 12ULL);
	And(a, b, 12ULL, &buf);
	assert(buf == 18ULL);
}

void test_except_1(void)
{
	generator a = Linear(2,0);
	generator b = Linear(3,0);
	uint64_t buf;
	Except(a, b, 0ULL, &buf);
	assert(buf == 2ULL);
	Except(a, b, 2ULL, &buf);
	assert(buf == 4ULL);
	Except(a, b, 4ULL, &buf);
	assert(buf == 8ULL);
}

int main(int argc, char *argv[])
{
	test_linear_1();
	test_power2_1();
	test_symmetric_1();
	test_symmetric_2();
	test_symmetric_3();
	test_symmetric_4();
	test_symmetric_5();
	test_symmetric_6();
	test_or_1();
	test_and_1();
	test_except_1();
	return 0;
}
