#pragma once

#include "utility.h"

namespace algorithm
{
	static bool algorithm_template_func(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer) { return false; }
	using _sign = std::remove_reference_t<decltype(algorithm_template_func)>;

    bool old_renewed(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer);

    // x86_64
#if IS_x86_6432_BUILD
    bool simd_sse2(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer);
    bool simd_sse41(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer);
    bool simd_avx(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer);
    bool simd_avx2(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer);
    bool simd_avx512(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer);
#elif IS_ARM_BUILD
    // arm
    bool simd_sve(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer);
    bool simd_sve2(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer);
    bool simd_neon(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer);
    bool simd_helium(int32_t idx, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer);
#endif
}

namespace executor
{
    static void parallel(algorithm::_sign&& algorithm, const std::vector<int32_t>& vIndices, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
    {
        util::parallel_for(vIndices.cbegin(), vIndices.cend(), [&](auto idx) {algorithm(idx, pRGBABuffer, pAlphaBuffer); });
    }

    // Running function in cycle
    static void cycle(algorithm::_sign&& algorithm, int32_t nSize, int32_t nStep, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
    {
        for (int32_t idx = 0; idx < nSize; idx += nStep) algorithm(idx, pRGBABuffer, pAlphaBuffer);
    }
}