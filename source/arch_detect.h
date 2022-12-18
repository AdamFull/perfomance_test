#pragma once

#if defined(__x86_64__) || defined(_M_X64) || defined(_M_X86) || defined(_M_IX86) || defined(_M_AMD64)
#define IS_x86_64 1
#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
#define IS_x86_32 1
#elif defined(__ARM_ARCH_2__)
#define IS_ARM2 1
#elif defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__)
#define IS_ARM3 1
#elif defined(__ARM_ARCH_4T__) || defined(__TARGET_ARM_4T)
#define IS_ARM4T 1
#elif defined(__ARM_ARCH_5_) || defined(__ARM_ARCH_5E_)
#define IS_ARM5 1
#elif defined(__ARM_ARCH_6T2_) || defined(__ARM_ARCH_6T2_)
#define IS_ARM6T2 1
#elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__)
#define IS_ARM6 1
#elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
#define IS_ARM7 1
#elif defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
#define IS_ARM7A 1
#elif defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
#define IS_ARM7R 1
#elif defined(__ARM_ARCH_7M__)
#define IS_ARM7M 1
#elif defined(__ARM_ARCH_7S__)
#define IS_ARM7S 1
#elif defined(__aarch64__) || defined(_M_ARM64)
#define IS_ARM64 1
#elif defined(mips) || defined(__mips__) || defined(__mips)
#define IS_MIPS 1
#elif defined(__sh__)
#define IS_SUPERH 1
#elif defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) || defined(__POWERPC__) || defined(__ppc__) || defined(__PPC__) || defined(_ARCH_PPC)
#define IS_POWERPC 1
#elif defined(__PPC64__) || defined(__ppc64__) || defined(_ARCH_PPC64)
#define IS_POWERPC64 1
#elif defined(__sparc__) || defined(__sparc);
#define IS_SPARC 1
#elif defined(__m68k__)
#define IS_M68K 1
#endif

#define IS_x86_6432_BUILD (IS_x86_64 || IS_x86_32)
#define IS_ARM_BUILD (IS_ARM2 || IS_ARM3 || IS_ARM4T || IS_ARM5 || IS_ARM6T2 || IS_ARM6 || IS_ARM7 || IS_ARM7A || IS_ARM7R || IS_ARM7M || IS_ARM7S || IS_ARM64)
#define IS_ARM64_BUILD (IS_ARM64)