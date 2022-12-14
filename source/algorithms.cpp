#include "algorithms.h"
#include "simd_helpers.h"

#if IS_x86_6432_BUILD
const InstructionSet::InstructionSet_Internal InstructionSet::CPU_Rep;
#endif

namespace algorithm
{
    bool old_renewed(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
    {
        *(pRGBABuffer + 3 + 4 * idx) = *(pAlphaBuffer + 4 * idx);
        return true;
    }

#if IS_x86_6432_BUILD
    const __m128i blendMask4 = _mm_set_epi32(0x80000000, 0x80000000, 0x80000000, 0x80000000); // sse2
    const __m256i blendMask8 = _mm256_set_epi32(
        0x80000000, 0x80000000, 0x80000000, 0x80000000,
        0x80000000, 0x80000000, 0x80000000, 0x80000000); // avx

    bool simd_sse2(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
    {
        if (InstructionSet::SSE2())
        {
            const __m128i xorMask = _mm_set1_epi32((int)0xff000000);
            auto* src_rgba = (__m128i*)(pRGBABuffer + idx);
            auto* src_alpha = (__m128i*)(pAlphaBuffer + idx);

            auto r0 = simd::sse2_loadi32(src_rgba);
            auto r1 = simd::sse2_loadi32(src_alpha);
            simd::sse2_shift_xor_merge32(src_rgba, r0, r1, xorMask, 0);

            return true;
        }
        return false;
    }

    bool simd_sse41(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
    {
        if (InstructionSet::SSE41())
        {
            auto* src_rgba = (__m128i*)(pRGBABuffer + idx);
            auto* src_alpha = (__m128i*)(pAlphaBuffer + idx);

            auto r0 = simd::sse2_loadi32(src_rgba);
            auto r1 = simd::sse2_loadi32(src_alpha);
            simd::sse41_shift_blend32(src_rgba, r0, r1, blendMask4, 0);
            return true;
        }
        return false;
    }

    bool simd_avx(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
    {
        if (InstructionSet::AVX())
        {
            const __m256i xorMask256 = _mm256_set1_epi32((int)0xff000000);
            auto* src_rgba = (__m256i*)(pRGBABuffer + idx);
            auto* src_alpha = (__m256i*)(pAlphaBuffer + idx);

            auto r0 = simd::avx_loadi32(src_rgba);
            auto r1 = simd::avx_loadi32(src_alpha);
            simd::avx2_shift_xor_merge32(src_rgba, r0, r1, xorMask256, 0);

            return true;
        }
        return false;
    }

    bool simd_avx2(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
    {
        if (InstructionSet::AVX() && InstructionSet::AVX2())
        {
            auto* src_rgba = (__m256i*)(pRGBABuffer + idx);
            auto* src_alpha = (__m256i*)(pAlphaBuffer + idx);

            auto r0 = simd::avx_loadi32(src_rgba);
            auto r1 = simd::avx_loadi32(src_alpha);
            simd::avx2_shift_blend32(src_rgba, r0, r1, blendMask8, 0);

            return true;
        }
        return false;
    }

    bool simd_avx512(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
    {
        if (InstructionSet::AVX512CD() && InstructionSet::AVX512ER())
        {
            auto* src_rgba = (__m512i*)(pRGBABuffer + idx);
            auto* src_alpha = (__m512i*)(pAlphaBuffer + idx);

            auto r0 = _mm512_load_si512(src_rgba);
            auto r1 = _mm512_load_si512(src_alpha);
            r1 = _mm512_slli_epi32(r1, 24);
            r1 = _mm512_xor_epi32(_mm512_set1_epi32((int)0xff000000), r1);

            auto r2 = _mm512_xor_si512(r0, r1);
            _mm512_storeu_si512(src_rgba, r2);

            return true;
        }
        return false;
    }
#elif IS_ARM_BUILD

    bool simd_sve(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
    {
        return false;
    }

    bool simd_sve2(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
    {
        return false;
    }

    bool simd_neon(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
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

        __builtin_prefetch (src_rgba + 12, 1, 1);
        __builtin_prefetch (src_alpha + 12, 0, 1);

        simd::neon_shift_xor_merge32(src_rgba, r0, r1, xorMask, 0);
        simd::neon_shift_xor_merge32(src_rgba, r2, r3, xorMask, 4);
        simd::neon_shift_xor_merge32(src_rgba, r4, r5, xorMask, 8);
        simd::neon_shift_xor_merge32(src_rgba, r6, r7, xorMask, 12);

        return true;
    }

    bool simd_helium(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
    {
        return false;
    }
#endif
}