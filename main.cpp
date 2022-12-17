#include "utility.h"
#include "merging_test.h"
#include "algorithms.h"

#ifdef ANDROID
#include <jni.h>
#endif

using namespace testing;

fs::path writablePath;
std::unique_ptr<CImageMergingTest> pTestHandle;

struct FCpuFeature
{
    FCpuFeature(const std::string& name, int status) : name(name), status((bool)status) {}
    std::string name;
    bool status;
};

void to_json(nlohmann::json& json, const FCpuFeature& type)
{
    json = nlohmann::json
    {
        {"name", type.name},
        {"status", type.status}
    };
}

//void from_json(const nlohmann::json& json, FCpuFeature& type)
//{
//    
//}

// Helper function for running tests with time measurement
template<class _Func, class... _Args>
double run_test_func(_Func&& function, _Args&& ...args)
{
    auto start = std::chrono::high_resolution_clock::now();
    function(std::forward<_Args>(args)...);
    return std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - start).count();
}

void initialize_test(const std::string& srWritablePath)
{
    writablePath = srWritablePath;
    pTestHandle = std::make_unique<CImageMergingTest>();

    pTestHandle->add([](int32_t nWidth, int32_t nHeight, int32_t nBytesPerRow, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
        {
            return run_test_func(&executor::cycle, algorithm::old_renewed, nWidth * nHeight, 1, pRGBABuffer, pAlphaBuffer);
        }, "old_renewed");
    
    pTestHandle->add([](int32_t nWidth, int32_t nHeight, int32_t nBytesPerRow, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
        {
            return run_test_func(&executor::parallel<1>, algorithm::old_renewed, nWidth * nHeight, pRGBABuffer, pAlphaBuffer);
        }, "old_renewed_par");

#if IS_x86_6432_BUILD
    static const auto features = cpu_features::GetX86Info().features;
    
    if (features.avx512f && features.avx512cd && features.avx512er && features.avx512pf && features.avx512bw && features.avx512dq && features.avx512vl)
    {
        pTestHandle->add([](int32_t nWidth, int32_t nHeight, int32_t nBytesPerRow, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
            {
                return run_test_func(&executor::parallel<64>, algorithm::simd_avx512_64, nBytesPerRow * nHeight, pRGBABuffer, pAlphaBuffer);
            }, "simd_avx512_ep");
    }
    if(features.avx && features.avx2)
    {
        pTestHandle->add([](int32_t nWidth, int32_t nHeight, int32_t nBytesPerRow, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
            {
                return run_test_func(&executor::parallel<32>, algorithm::simd_avx2_32, nBytesPerRow * nHeight, pRGBABuffer, pAlphaBuffer);
            }, "simd_avx2_ep_32");
        pTestHandle->add([](int32_t nWidth, int32_t nHeight, int32_t nBytesPerRow, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
            {
                return run_test_func(&executor::parallel<64>, algorithm::simd_avx2_64, nBytesPerRow * nHeight, pRGBABuffer, pAlphaBuffer);
            }, "simd_avx2_ep_64");
    }
    if (features.sse2 && features.sse4_1)
    {
        pTestHandle->add([](int32_t nWidth, int32_t nHeight, int32_t nBytesPerRow, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
            {
                return run_test_func(&executor::parallel<16>, algorithm::simd_sse41_16, nBytesPerRow * nHeight, pRGBABuffer, pAlphaBuffer);
            }, "simd_sse41_ep_16");
        pTestHandle->add([](int32_t nWidth, int32_t nHeight, int32_t nBytesPerRow, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
            {
                return run_test_func(&executor::parallel<32>, algorithm::simd_sse41_32, nBytesPerRow * nHeight, pRGBABuffer, pAlphaBuffer);
            }, "simd_sse41_ep_32");
        pTestHandle->add([](int32_t nWidth, int32_t nHeight, int32_t nBytesPerRow, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
            {
                return run_test_func(&executor::parallel<64>, algorithm::simd_sse41_64, nBytesPerRow * nHeight, pRGBABuffer, pAlphaBuffer);
            }, "simd_sse41_ep_64");
    }
    if (features.sse2)
    {
        pTestHandle->add([](int32_t nWidth, int32_t nHeight, int32_t nBytesPerRow, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
            {
                return run_test_func(&executor::parallel<16>, algorithm::simd_sse2_16, nBytesPerRow * nHeight, pRGBABuffer, pAlphaBuffer);
            }, "simd_sse2_ep_16");
    
        pTestHandle->add([](int32_t nWidth, int32_t nHeight, int32_t nBytesPerRow, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
            {
                return run_test_func(&executor::parallel<32>, algorithm::simd_sse2_32, nBytesPerRow * nHeight, pRGBABuffer, pAlphaBuffer);
            }, "simd_sse2_ep_32");
        pTestHandle->add([](int32_t nWidth, int32_t nHeight, int32_t nBytesPerRow, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
            {
                return run_test_func(&executor::parallel<64>, algorithm::simd_sse2_64, nBytesPerRow * nHeight, pRGBABuffer, pAlphaBuffer);
            }, "simd_sse2_ep_64");
    }

    
#elif IS_ARM_BUILD
    static const auto features = cpu_features::GetArmInfo().features;

    if (features.neon)
    {
        pTestHandle->add([&](int32_t nWidth, int32_t nHeight, int32_t nBytesPerRow, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
            {
                return run_test_func(&executor::parallel<16>, algorithm::simd_neon_16, nBytesPerRow * nHeight, pRGBABuffer, pAlphaBuffer);
            }, "simd_neon_16");
        pTestHandle->add([&](int32_t nWidth, int32_t nHeight, int32_t nBytesPerRow, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
            {
                return run_test_func(&executor::parallel<32>, algorithm::simd_neon_32, nBytesPerRow * nHeight, pRGBABuffer, pAlphaBuffer);
            }, "simd_neon_32");
        pTestHandle->add([&](int32_t nWidth, int32_t nHeight, int32_t nBytesPerRow, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
            {
                return run_test_func(&executor::parallel<64>, algorithm::simd_neon_64, nBytesPerRow * nHeight, pRGBABuffer, pAlphaBuffer);
            }, "simd_neon_64");
    }
#endif
}

std::string get_cpu_features()
{
    std::vector<FCpuFeature> vFeatures;
#if IS_x86_6432_BUILD
    static const auto cpuinfo = cpu_features::GetX86Info();

    vFeatures.emplace_back(FCpuFeature{ "fpu", cpuinfo.features.fpu });
    vFeatures.emplace_back(FCpuFeature{ "tsc", cpuinfo.features.cx8 });
    vFeatures.emplace_back(FCpuFeature{ "cx8", cpuinfo.features.cx8 });
    vFeatures.emplace_back(FCpuFeature{ "clfsh", cpuinfo.features.clfsh });
    vFeatures.emplace_back(FCpuFeature{ "mmx", cpuinfo.features.mmx });
    vFeatures.emplace_back(FCpuFeature{ "aes", cpuinfo.features.aes });
    vFeatures.emplace_back(FCpuFeature{ "erms", cpuinfo.features.erms });
    vFeatures.emplace_back(FCpuFeature{ "f16c", cpuinfo.features.f16c });
    vFeatures.emplace_back(FCpuFeature{ "fma4", cpuinfo.features.fma4 });
    vFeatures.emplace_back(FCpuFeature{ "fma3", cpuinfo.features.fma3 });
    vFeatures.emplace_back(FCpuFeature{ "vaes", cpuinfo.features.vaes });
    vFeatures.emplace_back(FCpuFeature{ "vpclmulqdq", cpuinfo.features.vpclmulqdq });
    vFeatures.emplace_back(FCpuFeature{ "bmi1", cpuinfo.features.bmi1 });
    vFeatures.emplace_back(FCpuFeature{ "hle", cpuinfo.features.hle });
    vFeatures.emplace_back(FCpuFeature{ "bmi2", cpuinfo.features.bmi2 });
    vFeatures.emplace_back(FCpuFeature{ "bmrtmi2", cpuinfo.features.rtm });
    vFeatures.emplace_back(FCpuFeature{ "clflushopt", cpuinfo.features.clflushopt });
    vFeatures.emplace_back(FCpuFeature{ "clwb", cpuinfo.features.clwb });

    vFeatures.emplace_back(FCpuFeature{ "sse", cpuinfo.features.sse });
    vFeatures.emplace_back(FCpuFeature{ "sse2", cpuinfo.features.sse2 });
    vFeatures.emplace_back(FCpuFeature{ "sse3", cpuinfo.features.sse3 });
    vFeatures.emplace_back(FCpuFeature{ "ssse3", cpuinfo.features.ssse3 });
    vFeatures.emplace_back(FCpuFeature{ "sse4_1", cpuinfo.features.sse4_1 });
    vFeatures.emplace_back(FCpuFeature{ "sse4_2", cpuinfo.features.sse4_2 });
    vFeatures.emplace_back(FCpuFeature{ "sse4a", cpuinfo.features.sse4a });

    vFeatures.emplace_back(FCpuFeature{ "avx", cpuinfo.features.avx });
    vFeatures.emplace_back(FCpuFeature{ "avx_vnni", cpuinfo.features.avx_vnni });
    vFeatures.emplace_back(FCpuFeature{ "avx2", cpuinfo.features.avx2 });

    vFeatures.emplace_back(FCpuFeature{ "avx512f", cpuinfo.features.avx512f });
    vFeatures.emplace_back(FCpuFeature{ "avx512cd", cpuinfo.features.avx512cd });
    vFeatures.emplace_back(FCpuFeature{ "avx512er", cpuinfo.features.avx512er });
    vFeatures.emplace_back(FCpuFeature{ "avx512pf", cpuinfo.features.avx512pf });
    vFeatures.emplace_back(FCpuFeature{ "avx512bw", cpuinfo.features.avx512bw });
    vFeatures.emplace_back(FCpuFeature{ "avx512dq", cpuinfo.features.avx512dq });
    vFeatures.emplace_back(FCpuFeature{ "avx512vl", cpuinfo.features.avx512vl });
    vFeatures.emplace_back(FCpuFeature{ "avx512ifma", cpuinfo.features.avx512ifma });
    vFeatures.emplace_back(FCpuFeature{ "avx512vbmi", cpuinfo.features.avx512vbmi });
    vFeatures.emplace_back(FCpuFeature{ "avx512vbmi2", cpuinfo.features.avx512vbmi2 });
    vFeatures.emplace_back(FCpuFeature{ "avx512vnni", cpuinfo.features.avx512vnni });
    vFeatures.emplace_back(FCpuFeature{ "avx512bitalg", cpuinfo.features.avx512bitalg });
    vFeatures.emplace_back(FCpuFeature{ "avx512vpopcntdq", cpuinfo.features.avx512vpopcntdq });
    vFeatures.emplace_back(FCpuFeature{ "avx512_4vnniw", cpuinfo.features.avx512_4vnniw });
    vFeatures.emplace_back(FCpuFeature{ "avx512_4vbmi2", cpuinfo.features.avx512_4vbmi2 });
    vFeatures.emplace_back(FCpuFeature{ "avx512_second_fma", cpuinfo.features.avx512_second_fma });
    vFeatures.emplace_back(FCpuFeature{ "avx512_4fmaps", cpuinfo.features.avx512_4fmaps });
    vFeatures.emplace_back(FCpuFeature{ "avx512_bf16", cpuinfo.features.avx512_bf16 });
    vFeatures.emplace_back(FCpuFeature{ "avx512_vp2intersect", cpuinfo.features.avx512_vp2intersect });
    vFeatures.emplace_back(FCpuFeature{ "avx512_fp16", cpuinfo.features.avx512_fp16 });
    vFeatures.emplace_back(FCpuFeature{ "amx_bf16", cpuinfo.features.amx_bf16 });
    vFeatures.emplace_back(FCpuFeature{ "amx_tile", cpuinfo.features.amx_tile });
    vFeatures.emplace_back(FCpuFeature{ "amx_int8", cpuinfo.features.amx_int8 });

    vFeatures.emplace_back(FCpuFeature{ "pclmulqdq", cpuinfo.features.pclmulqdq });
    vFeatures.emplace_back(FCpuFeature{ "smx", cpuinfo.features.smx });
    vFeatures.emplace_back(FCpuFeature{ "sgx", cpuinfo.features.sgx });
    vFeatures.emplace_back(FCpuFeature{ "cx16", cpuinfo.features.cx16 });
    vFeatures.emplace_back(FCpuFeature{ "sha", cpuinfo.features.sha });
    vFeatures.emplace_back(FCpuFeature{ "popcnt", cpuinfo.features.popcnt });
    vFeatures.emplace_back(FCpuFeature{ "movbe", cpuinfo.features.movbe });
    vFeatures.emplace_back(FCpuFeature{ "rdrnd", cpuinfo.features.rdrnd });

    vFeatures.emplace_back(FCpuFeature{ "dca", cpuinfo.features.dca });
    vFeatures.emplace_back(FCpuFeature{ "ss", cpuinfo.features.ss });
    vFeatures.emplace_back(FCpuFeature{ "adx", cpuinfo.features.adx });
    vFeatures.emplace_back(FCpuFeature{ "lzcnt", cpuinfo.features.lzcnt });
    vFeatures.emplace_back(FCpuFeature{ "gfni", cpuinfo.features.gfni });
    vFeatures.emplace_back(FCpuFeature{ "movdiri", cpuinfo.features.movdiri });
    vFeatures.emplace_back(FCpuFeature{ "movdir64b", cpuinfo.features.movdir64b });
    vFeatures.emplace_back(FCpuFeature{ "fs_rep_mov", cpuinfo.features.fs_rep_mov });
    vFeatures.emplace_back(FCpuFeature{ "fz_rep_movsb", cpuinfo.features.fz_rep_movsb });
    vFeatures.emplace_back(FCpuFeature{ "fs_rep_stosb", cpuinfo.features.fs_rep_stosb });
    vFeatures.emplace_back(FCpuFeature{ "fs_rep_cmpsb_scasb", cpuinfo.features.fs_rep_cmpsb_scasb });
#elif IS_ARM_BUILD
    static const auto cpuinfo = cpu_features::GetArmInfo();

    //json["other"]["implementer"] = cpuinfo.implementer;
    //json["other"]["architecture"] = cpuinfo.architecture;
    //json["other"]["variant"] = cpuinfo.variant;
    //json["other"]["part"] = cpuinfo.part;
    //json["other"]["revision"] = cpuinfo.revision;

    vFeatures.emplace_back(FCpuFeature{ "swp", cpuinfo.features.swp });
    vFeatures.emplace_back(FCpuFeature{ "half", cpuinfo.features.half });
    vFeatures.emplace_back(FCpuFeature{ "thumb", cpuinfo.features.thumb });
    vFeatures.emplace_back(FCpuFeature{ "_26bit", cpuinfo.features._26bit });
    // program counter)
    vFeatures.emplace_back(FCpuFeature{ "fastmult", cpuinfo.features.fastmult });
    vFeatures.emplace_back(FCpuFeature{ "fpa", cpuinfo.features.fpa });
    vFeatures.emplace_back(FCpuFeature{ "vfp", cpuinfo.features.vfp });
    vFeatures.emplace_back(FCpuFeature{ "edsp", cpuinfo.features.edsp });
    // others above)
    vFeatures.emplace_back(FCpuFeature{ "java", cpuinfo.features.java });
    vFeatures.emplace_back(FCpuFeature{ "iwmmxt", cpuinfo.features.iwmmxt });
    vFeatures.emplace_back(FCpuFeature{ "crunch", cpuinfo.features.crunch });
    vFeatures.emplace_back(FCpuFeature{ "thumbee", cpuinfo.features.thumbee });
    vFeatures.emplace_back(FCpuFeature{ "neon", cpuinfo.features.neon });
    vFeatures.emplace_back(FCpuFeature{ "vfpv3", cpuinfo.features.vfpv3 });
    vFeatures.emplace_back(FCpuFeature{ "vfpv3d16", cpuinfo.features.vfpv3d16 });
    vFeatures.emplace_back(FCpuFeature{ "tls", cpuinfo.features.tls });
    vFeatures.emplace_back(FCpuFeature{ "vfpv4", cpuinfo.features.vfpv4 });
    vFeatures.emplace_back(FCpuFeature{ "idiva", cpuinfo.features.idiva });
    vFeatures.emplace_back(FCpuFeature{ "idivt", cpuinfo.features.idivt });
    vFeatures.emplace_back(FCpuFeature{ "vfpd32", cpuinfo.features.vfpd32 });
    vFeatures.emplace_back(FCpuFeature{ "lpae", cpuinfo.features.lpae });
    // 32-bit architecture)
    vFeatures.emplace_back(FCpuFeature{ "evtstrm", cpuinfo.features.evtstrm });
    vFeatures.emplace_back(FCpuFeature{ "aes", cpuinfo.features.aes });
    vFeatures.emplace_back(FCpuFeature{ "pmull", cpuinfo.features.pmull });
    vFeatures.emplace_back(FCpuFeature{ "sha1", cpuinfo.features.sha1 });
    vFeatures.emplace_back(FCpuFeature{ "sha2", cpuinfo.features.sha2 });
    vFeatures.emplace_back(FCpuFeature{ "crc32", cpuinfo.features.crc32 });
#endif

    auto json = nlohmann::json(vFeatures);
    return json.dump();
}

std::string start_tests(const std::string& srAlgorithm, size_t exec_times)
{
    pTestHandle->run(srAlgorithm, exec_times);
    return pTestHandle->finalize();
}

#ifdef ANDROID
extern "C"
JNIEXPORT void JNICALL
Java_com_example_perfomance_1test_MainActivity_initializeTestEnvironment(
        JNIEnv *env,
        jobject thiz,
        jobject asset_manager,
        jstring writable_path) {

    jboolean isCopy;
    const char *convertedValue = (env)->GetStringUTFChars(writable_path, &isCopy);
    std::string srWrPath = std::string(convertedValue);

    initialize_test(srWrPath);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_perfomance_1test_ui_main_testing_RunTestsFragment_setTestCase(
        JNIEnv *env,
        jobject thiz,
        jstring test_case) {

    jboolean isCopy;
    const char *convertedValue = (env)->GetStringUTFChars(test_case, &isCopy);
    std::string srCase = std::string(convertedValue);

    pTestHandle->selectTestCase(srCase);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_perfomance_1test_ui_main_testing_RunTestsFragment_runTest(
        JNIEnv *env,
        jobject thiz,
        jstring algorithm,
        jfloat num_iterations) {

    jboolean isCopy;
    const char *convertedValue = (env)->GetStringUTFChars(algorithm, &isCopy);
    std::string srAlgorithm = std::string(convertedValue);
    if (srAlgorithm == "all")
        srAlgorithm = "";

    auto result = run_test_func(start_tests, srAlgorithm, static_cast<size_t>(num_iterations));
    std::string scores_data;
    file::read(writablePath / "scores.json", scores_data);
    return env->NewStringUTF(scores_data.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_perfomance_1test_ui_main_testing_RunTestsFragment_getAvaliableTestCasesAndAlgorithms(
        JNIEnv *env,
        jobject thiz) {

    auto srJson = pTestHandle->getAvaliableTestCasesAndAlgorithms();
    return env->NewStringUTF(srJson.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_perfomance_1test_ui_main_cpuinfo_CpuInfoFragment_getCpuFeatures(
        JNIEnv *env,
        jobject thiz)
        {
    auto srJson = get_cpu_features();
    return env->NewStringUTF(srJson.c_str());
}
#endif

#ifdef _WIN32
int main()
{
    initialize_test(fs::current_path().string());
    pTestHandle->selectTestCase("aerith_hd");
    auto result = run_test_func(start_tests, "", 10);
    return 0;
}
#endif
