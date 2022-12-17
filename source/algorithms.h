#pragma once

#include "utility.h"
#include "simd_helpers.h"

namespace algorithm
{
	static void algorithm_template_func(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer) {  }
	using _sign = std::remove_reference_t<decltype(algorithm_template_func)>;

    static void old_renewed(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
    {
        *(pRGBABuffer + 3 + 4 * idx) = *(pAlphaBuffer + 4 * idx);
    }

    // x86_64
#if IS_x86_6432_BUILD
    static void simd_sse2_16(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
    {
        static const __m128i xorMask = _mm_set1_epi32((int)0xff000000);

        auto* src_rgba = simd::get_intrin_ptr<__m128i>(pRGBABuffer, idx);
        auto* src_alpha = simd::get_intrin_ptr<__m128i>(pAlphaBuffer, idx);
        auto r0 = simd::sse2_loadi32(src_rgba);
        auto r1 = simd::sse2_loadi32(src_alpha);
        simd::sse2_shift_xor_merge32(src_rgba, r0, r1, xorMask, 0);
    }

    static void simd_sse2_32(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
    {
        static const __m128i xorMask = _mm_set1_epi32((int)0xff000000);

        auto* src_rgba = simd::get_intrin_ptr<__m128i>(pRGBABuffer, idx);
        auto* src_alpha = simd::get_intrin_ptr<__m128i>(pAlphaBuffer, idx);
        auto r0 = simd::sse2_loadi32(src_rgba);
        auto r1 = simd::sse2_loadi32(src_alpha);
        simd::sse2_shift_xor_merge32(src_rgba, r0, r1, xorMask, 0);

        src_rgba = simd::get_intrin_ptr<__m128i>(pRGBABuffer, idx, 16);
        src_alpha = simd::get_intrin_ptr<__m128i>(pAlphaBuffer, idx, 16);
        r0 = simd::sse2_loadi32(src_rgba);
        r1 = simd::sse2_loadi32(src_alpha);
        simd::sse2_shift_xor_merge32(src_rgba, r0, r1, xorMask, 0);
    }

    static void simd_sse2_64(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
    {
        static const __m128i xorMask = _mm_set1_epi32((int)0xff000000);
        
        auto* src_rgba = simd::get_intrin_ptr<__m128i>(pRGBABuffer, idx);
        auto* src_alpha = simd::get_intrin_ptr<__m128i>(pAlphaBuffer, idx);
        auto r0 = simd::sse2_loadi32(src_rgba);
        auto r1 = simd::sse2_loadi32(src_alpha);
        simd::sse2_shift_xor_merge32(src_rgba, r0, r1, xorMask, 0);

        src_rgba = simd::get_intrin_ptr<__m128i>(pRGBABuffer, idx, 16);
        src_alpha = simd::get_intrin_ptr<__m128i>(pAlphaBuffer, idx, 16);
        r0 = simd::sse2_loadi32(src_rgba);
        r1 = simd::sse2_loadi32(src_alpha);
        simd::sse2_shift_xor_merge32(src_rgba, r0, r1, xorMask, 0);

        src_rgba = simd::get_intrin_ptr<__m128i>(pRGBABuffer, idx, 32);
        src_alpha = simd::get_intrin_ptr<__m128i>(pAlphaBuffer, idx, 32);
        r0 = simd::sse2_loadi32(src_rgba);
        r1 = simd::sse2_loadi32(src_alpha);
        simd::sse2_shift_xor_merge32(src_rgba, r0, r1, xorMask, 0);

        src_rgba = simd::get_intrin_ptr<__m128i>(pRGBABuffer, idx, 48);
        src_alpha = simd::get_intrin_ptr<__m128i>(pAlphaBuffer, idx, 48);
        r0 = simd::sse2_loadi32(src_rgba);
        r1 = simd::sse2_loadi32(src_alpha);
        simd::sse2_shift_xor_merge32(src_rgba, r0, r1, xorMask, 0);
    }

    static void simd_sse41_16(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
    {
        static const __m128i blendMask4 = _mm_set_epi32(0x80000000, 0x80000000, 0x80000000, 0x80000000); // sse2

        auto* src_rgba = simd::get_intrin_ptr<__m128i>(pRGBABuffer, idx);
        auto* src_alpha = simd::get_intrin_ptr<__m128i>(pAlphaBuffer, idx);
        auto r0 = simd::sse2_loadi32(src_rgba);
        auto r1 = simd::sse2_loadi32(src_alpha);
        simd::sse41_shift_blend32(src_rgba, r0, r1, blendMask4, 0);
    }

    static void simd_sse41_32(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
    {
        static const __m128i blendMask4 = _mm_set_epi32(0x80000000, 0x80000000, 0x80000000, 0x80000000); // sse2

        auto* src_rgba = simd::get_intrin_ptr<__m128i>(pRGBABuffer, idx);
        auto* src_alpha = simd::get_intrin_ptr<__m128i>(pAlphaBuffer, idx);
        auto r0 = simd::sse2_loadi32(src_rgba);
        auto r1 = simd::sse2_loadi32(src_alpha);
        simd::sse41_shift_blend32(src_rgba, r0, r1, blendMask4, 0);

        src_rgba = simd::get_intrin_ptr<__m128i>(pRGBABuffer, idx, 16);
        src_alpha = simd::get_intrin_ptr<__m128i>(pAlphaBuffer, idx, 16);
        r0 = simd::sse2_loadi32(src_rgba);
        r1 = simd::sse2_loadi32(src_alpha);
        simd::sse41_shift_blend32(src_rgba, r0, r1, blendMask4, 0);
    }

    static void simd_sse41_64(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
    {
        static const __m128i blendMask4 = _mm_set_epi32(0x80000000, 0x80000000, 0x80000000, 0x80000000); // sse2

        auto* src_rgba = simd::get_intrin_ptr<__m128i>(pRGBABuffer, idx);
        auto* src_alpha = simd::get_intrin_ptr<__m128i>(pAlphaBuffer, idx);
        auto r0 = simd::sse2_loadi32(src_rgba);
        auto r1 = simd::sse2_loadi32(src_alpha);
        simd::sse41_shift_blend32(src_rgba, r0, r1, blendMask4, 0);

        src_rgba = simd::get_intrin_ptr<__m128i>(pRGBABuffer, idx, 16);
        src_alpha = simd::get_intrin_ptr<__m128i>(pAlphaBuffer, idx, 16);
        r0 = simd::sse2_loadi32(src_rgba);
        r1 = simd::sse2_loadi32(src_alpha);
        simd::sse41_shift_blend32(src_rgba, r0, r1, blendMask4, 0);

        src_rgba = simd::get_intrin_ptr<__m128i>(pRGBABuffer, idx, 32);
        src_alpha = simd::get_intrin_ptr<__m128i>(pAlphaBuffer, idx, 32);
        r0 = simd::sse2_loadi32(src_rgba);
        r1 = simd::sse2_loadi32(src_alpha);
        simd::sse41_shift_blend32(src_rgba, r0, r1, blendMask4, 0);

        src_rgba = simd::get_intrin_ptr<__m128i>(pRGBABuffer, idx, 48);
        src_alpha = simd::get_intrin_ptr<__m128i>(pAlphaBuffer, idx, 48);
        r0 = simd::sse2_loadi32(src_rgba);
        r1 = simd::sse2_loadi32(src_alpha);
        simd::sse41_shift_blend32(src_rgba, r0, r1, blendMask4, 0);
    }

    //static void simd_avx_32(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
    //{
    //    const __m256i xorMask256 = _mm256_set1_epi32((int)0xff000000);
    //    auto* src_rgba = (__m256i*)(pRGBABuffer + idx);
    //    auto* src_alpha = (__m256i*)(pAlphaBuffer + idx);
    //
    //    auto r0 = simd::avx_loadi32(src_rgba);
    //    auto r1 = simd::avx_loadi32(src_alpha);
    //    simd::avx2_shift_xor_merge32(src_rgba, r0, r1, xorMask256, 0);
    //}

    static void simd_avx2_32(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
    {
        static const __m256i blendMask8 = _mm256_set_epi32(
            0x80000000, 0x80000000, 0x80000000, 0x80000000,
            0x80000000, 0x80000000, 0x80000000, 0x80000000); // avx

        auto* src_rgba = simd::get_intrin_ptr<__m256i>(pRGBABuffer, idx);
        auto* src_alpha = simd::get_intrin_ptr<__m256i>(pAlphaBuffer, idx);

        auto r0 = simd::avx_loadi32(src_rgba);
        auto r1 = simd::avx_loadi32(src_alpha);
        simd::avx2_shift_blend32(src_rgba, r0, r1, blendMask8, 0);
    }

    static void simd_avx2_64(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
    {
        static const __m256i blendMask8 = _mm256_set_epi32(
            0x80000000, 0x80000000, 0x80000000, 0x80000000,
            0x80000000, 0x80000000, 0x80000000, 0x80000000); // avx

        auto* src_rgba = simd::get_intrin_ptr<__m256i>(pRGBABuffer, idx);
        auto* src_alpha = simd::get_intrin_ptr<__m256i>(pAlphaBuffer, idx);
        auto r0 = simd::avx_loadi32(src_rgba);
        auto r1 = simd::avx_loadi32(src_alpha);
        simd::avx2_shift_blend32(src_rgba, r0, r1, blendMask8, 0);

        src_rgba = simd::get_intrin_ptr<__m256i>(pRGBABuffer, idx, 32);
        src_alpha = simd::get_intrin_ptr<__m256i>(pAlphaBuffer, idx, 32);
        r0 = simd::avx_loadi32(src_rgba);
        r1 = simd::avx_loadi32(src_alpha);
        simd::avx2_shift_blend32(src_rgba, r0, r1, blendMask8, 0);
    }

    static void simd_avx512_64(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
    {
        auto* src_rgba = (__m512i*)(pRGBABuffer + idx);
        auto* src_alpha = (__m512i*)(pAlphaBuffer + idx);

        auto r0 = _mm512_load_si512(src_rgba);
        auto r1 = _mm512_load_si512(src_alpha);
        r1 = _mm512_slli_epi32(r1, 24);
        r1 = _mm512_xor_epi32(_mm512_set1_epi32((int)0xff000000), r1);

        auto r2 = _mm512_xor_si512(r0, r1);
        _mm512_storeu_si512(src_rgba, r2);
    }
#elif IS_ARM_BUILD
    // arm
    static void simd_neon_16(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
    {
        auto xorMask = vdupq_n_s32(0xff000000);
        auto* src_rgba = (uint32_t*)(pRGBABuffer + idx);
        auto* src_alpha = (uint32_t*)(pAlphaBuffer + idx);
    
        auto r0 = vld1q_u32(src_rgba);
        auto r1 = vld1q_u32(src_alpha);
    
        __builtin_prefetch(src_rgba + 4, 1, 1);
        __builtin_prefetch(src_alpha + 4, 0, 1);
    
        simd::neon_shift_xor_merge32(src_rgba, r0, r1, xorMask, 0);
    }

    static void simd_neon_32(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
    {
        auto xorMask = vdupq_n_s32(0xff000000);
        auto* src_rgba = (uint32_t*)(pRGBABuffer + idx);
        auto* src_alpha = (uint32_t*)(pAlphaBuffer + idx);
    
        auto r0 = vld1q_u32(src_rgba);
        auto r1 = vld1q_u32(src_alpha);
    
        auto r2 = vld1q_u32(src_rgba + 4);
        auto r3 = vld1q_u32(src_alpha + 4);
    
        __builtin_prefetch(src_rgba + 8, 1, 1);
        __builtin_prefetch(src_alpha + 8, 0, 1);
    
        simd::neon_shift_xor_merge32(src_rgba, r0, r1, xorMask, 0);
        simd::neon_shift_xor_merge32(src_rgba, r2, r3, xorMask, 4);
    }

    static void simd_neon_64(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
    {
        auto xorMask = vdupq_n_s32(0xff000000);
        auto* src_rgba = (uint32_t*)(pRGBABuffer + idx);
        auto* src_alpha = (uint32_t*)(pAlphaBuffer + idx);

        auto r0 = vld1q_u32(src_rgba);
        auto r1 = vld1q_u32(src_alpha);

        auto r2 = vld1q_u32(src_rgba + 4);
        auto r3 = vld1q_u32(src_alpha + 4);

        auto r4 = vld1q_u32(src_rgba + 8);
        auto r5 = vld1q_u32(src_alpha + 8);

        auto r6 = vld1q_u32(src_rgba + 12);
        auto r7 = vld1q_u32(src_alpha + 12);

        __builtin_prefetch(src_rgba + 16, 1, 1);
        __builtin_prefetch(src_alpha + 16, 0, 1);

        simd::neon_shift_xor_merge32(src_rgba, r0, r1, xorMask, 0);
        simd::neon_shift_xor_merge32(src_rgba, r2, r3, xorMask, 4);
        simd::neon_shift_xor_merge32(src_rgba, r4, r5, xorMask, 8);
        simd::neon_shift_xor_merge32(src_rgba, r6, r7, xorMask, 12);
    }

#endif
}

namespace executor
{
    template<int32_t Stride>
    static void parallel(algorithm::_sign&& algorithm, int32_t nSize, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
    {
        static std::vector<int32_t> vIndices;
        if (vIndices.empty())
        {
            for (int32_t idx = 0; idx < nSize; idx += Stride)
                vIndices.emplace_back(idx);
        }

        util::parallel_for(vIndices.cbegin(), vIndices.cend(), [&](auto idx) {algorithm(idx, pRGBABuffer, pAlphaBuffer); });
    }

    // Running function in cycle
    static void cycle(algorithm::_sign&& algorithm, int32_t nSize, int32_t nStep, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
    {
        for (int32_t idx = 0; idx < nSize; idx += nStep) algorithm(idx, pRGBABuffer, pAlphaBuffer);
    }
}