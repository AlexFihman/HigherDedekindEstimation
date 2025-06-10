#pragma once
#include "stdint.h"

	typedef uint64_t mbf;

	extern int d0, d1, d2, d3, d4, d5, d6;
	extern mbf* f0;
	extern mbf* f1;
	extern mbf* f2;
	extern mbf* f3;
	extern mbf* f4;
	extern mbf* f5;
	extern mbf* f6;
	extern uint8_t level_of_item[128];

	struct mbf7
	{
		mbf f1;
		mbf f2;
		mbf7 operator&(const mbf7 x) const;
		mbf7 operator|(const mbf7 x) const;
		mbf7 bitneg() const;
		void SetBit(uint8_t bit);
		int Compare(mbf7 another);
		unsigned char GetBit(const int bit) const;
		unsigned char GetBitOld(const int bit) const;
		bool SpecialBit(const int bit) const;
	};

	struct mbf8
	{
		mbf7 f1;
		mbf7 f2;
	};

	extern uint8_t count_bits(int i);

	extern bool cmp6(const mbf f1, const mbf f2);

	extern bool cmp7(const mbf7 f1, const mbf7 f2);

	extern bool cmp8(const mbf8 f1, const mbf8 f2);

	extern int fcount(mbf const * const f, const int d);

	extern mbf* makespace(const int size);

	extern void pushlevel(mbf * const fnew, mbf const * const f, const int d, const int lvl);

	extern void makelevel(mbf*& fnext, mbf const * const f, int& dnext, const int d, const int lvl);

	extern int find_f6(mbf f);

	extern void init();
