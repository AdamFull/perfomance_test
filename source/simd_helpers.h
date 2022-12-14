#pragma once

#if IS_ARM_BUILD
#include <arm_neon.h>
#include <sys/auxv.h>
#elif IS_x86_6432_BUILD
#include "InstructionSet.h"
#endif

namespace simd
{
#if IS_ARM_BUILD
	// NEON
	int32x4_t neon_loadi32(int32_t* data);
	uint32x4_t neon_loadu32(uint32_t* data);
	void neon_shift_xor_merge32(uint32_t* data, uint32x4_t r0, uint32x4_t r1, uint32x4_t mask, uint32_t shift);
#elif IS_x86_6432_BUILD
	// SSE
	__m128i sse2_loadi32(__m128i* data);
	void sse2_shift_xor_merge32(__m128i* data, __m128i r0, __m128i r1, __m128i mask, uint32_t shift);
	void sse41_shift_blend32(__m128i* data, __m128i r0, __m128i r1, __m128i mask, uint32_t shift);

	// AVX
	__m256i avx_loadi32(__m256i* data);
	void avx2_shift_xor_merge32(__m256i* data, __m256i r0, __m256i r1, __m256i mask, uint32_t shift);
	void avx2_shift_blend32(__m256i* data, __m256i r0, __m256i r1, __m256i mask, uint32_t shift);
#endif
}