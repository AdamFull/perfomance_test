#include "simd_helpers.h"

namespace simd
{
#if IS_ARM_BUILD
	int32x4_t neon_loadi32(int32_t* data)
	{
		return vld1q_s32(data);
	}

	uint32x4_t neon_loadu32(uint32_t* data)
	{
		return vld1q_u32(data);
	}

	void neon_shift_xor_merge32(uint32_t* data, uint32x4_t r0, uint32x4_t r1, uint32x4_t mask, uint32_t shift)
	{
		r1 = vshlq_n_u32(r1, 24);
		r1 = veorq_u32(mask, r1);
		r0 = veorq_u32(r0, r1);
		vst1q_u32(data + shift, r0);
	}
#elif IS_x86_6432_BUILD
	__m128i sse2_loadi32(__m128i* data)
	{
		return _mm_load_si128(data); // sse2
	}

	void sse2_shift_xor_merge32(__m128i* data, __m128i r0, __m128i r1, __m128i mask, uint32_t shift)
	{
		r1 = _mm_slli_epi32(r1, 24); // sse2
		r1 = _mm_xor_si128(mask, r1); // sse2
		r0 = _mm_xor_si128(r0, r1); // sse2
		_mm_storeu_si128(data + shift, r0); // sse2
	}

	void sse41_shift_blend32(__m128i* data, __m128i r0, __m128i r1, __m128i mask, uint32_t shift)
	{
		r1 = _mm_slli_epi32(r1, 8); // sse2
		r0 = _mm_blendv_epi8(r0, r1, mask); // sse4.1
		_mm_storeu_si128(data + shift, r0); // sse2
	}

	// AVX
	__m256i avx_loadi32(__m256i* data)
	{
		return _mm256_load_si256(data);
	}

	void avx2_shift_xor_merge32(__m256i* data, __m256i r0, __m256i r1, __m256i mask, uint32_t shift)
	{
		r1 = _mm256_slli_epi32(r1, 24); // AVX2
		r1 = _mm256_xor_si256(mask, r1); // AVX2
		r0 = _mm256_xor_si256(r0, r1); // AVX2
		_mm256_storeu_si256(data + shift, r0); // AVX
	}

	void avx2_shift_blend32(__m256i* data, __m256i r0, __m256i r1, __m256i mask, uint32_t shift)
	{
		r1 = _mm256_slli_epi32(r1, 8); // AVX2
		r0 = _mm256_blendv_epi8(r0, r1, mask); // AVX2
		_mm256_storeu_si256(data + shift, r0); // AVX
	}
#endif
}