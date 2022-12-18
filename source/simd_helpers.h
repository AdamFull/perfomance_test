#pragma once

#include "arch_detect.h"

#if IS_ARM_BUILD
#include <arm_neon.h>
#if IS_ARM64_BUILD
#include <cpuinfo_aarch64.h>
#else
#include <cpuinfo_arm.h>
#endif // IS_ARM64_BUILD
#elif IS_x86_6432_BUILD
#include <cpuinfo_x86.h>
#endif

namespace simd
{
#if IS_ARM_BUILD
	// NEON
	static int32x4_t neon_loadi32(int32_t* data)
	{
		return vld1q_s32(data);
	}

	static uint32x4_t neon_loadu32(uint32_t* data)
	{
		return vld1q_u32(data);
	}

	static void neon_shift_xor_merge32(uint32_t* data, uint32x4_t r0, uint32x4_t r1, uint32x4_t mask, uint32_t shift)
	{
		r1 = vshlq_n_u32(r1, 24);
		r1 = veorq_u32(mask, r1);
		r0 = veorq_u32(r0, r1);
		vst1q_u32(data + shift, r0);
}
#elif IS_x86_6432_BUILD

	template<class _IntrinType>
	static _IntrinType* get_intrin_ptr(uint8_t* data, int32_t idx, int32_t stride = 0)
	{
		return (_IntrinType*)(data + idx + stride);
	}

	// SSE
	static __m128i sse2_loadi32(__m128i* data)
	{
		return _mm_load_si128(data); // sse2
	}

	static void sse2_shift_xor_merge32(__m128i* data, __m128i r0, __m128i r1, __m128i mask, uint32_t shift)
	{
		r1 = _mm_slli_epi32(r1, 24); // sse2
		r1 = _mm_xor_si128(mask, r1); // sse2
		r0 = _mm_xor_si128(r0, r1); // sse2
		_mm_storeu_si128(data + shift, r0); // sse2
}

	static void sse41_shift_blend32(__m128i* data, __m128i r0, __m128i r1, __m128i mask, uint32_t shift)
	{
		r1 = _mm_slli_epi32(r1, 8); // sse2
		r0 = _mm_blendv_epi8(r0, r1, mask); // sse4.1
		_mm_storeu_si128(data + shift, r0); // sse2
	}

	// AVX
	static __m256i avx_loadi32(__m256i* data)
	{
		return _mm256_load_si256(data);
	}

	static void avx2_shift_xor_merge32(__m256i* data, __m256i r0, __m256i r1, __m256i mask, uint32_t shift)
	{
		r1 = _mm256_slli_epi32(r1, 24); // AVX2
		r1 = _mm256_xor_si256(mask, r1); // AVX2
		r0 = _mm256_xor_si256(r0, r1); // AVX2
		_mm256_storeu_si256(data + shift, r0); // AVX
	}

	static void avx2_shift_blend32(__m256i* data, __m256i r0, __m256i r1, __m256i mask, uint32_t shift)
	{
		r1 = _mm256_slli_epi32(r1, 8); // AVX2
		r0 = _mm256_blendv_epi8(r0, r1, mask); // AVX2
		_mm256_storeu_si256(data + shift, r0); // AVX
	}
#endif
}