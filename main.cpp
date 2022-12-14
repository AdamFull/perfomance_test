#include "utility.h"
#include "merging_test.h"
#include "algorithms.h"

#ifdef ANDROID
#include <jni.h>
#endif

using namespace testing;

fs::path writablePath;
std::unique_ptr<CImageMergingTest> pTestHandle;
std::vector<int32_t> vIndices;
std::vector<int32_t> vIndices16;
std::vector<int32_t> vIndices32;
std::vector<int32_t> vIndices64;

// Helper function for running tests with time measurement
template<class _Func, class... _Args>
double run_test_func(_Func&& function, _Args&& ...args)
{
    auto start = std::chrono::high_resolution_clock::now();
    function(std::forward<_Args>(args)...);
    return std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - start).count();
}


void update_indices(std::vector<int32_t>& vIndices, int32_t nBytesPerRow, int32_t nHeight, int32_t shift)
{
    if (vIndices.size() != nBytesPerRow * nHeight / shift)
    {
        vIndices.clear();
        for (int32_t idx = 0; idx < nBytesPerRow * nHeight; idx += shift)
            vIndices.emplace_back(idx);
    }
}

void initialize_test(const std::string& srWritablePath)
{
    writablePath = srWritablePath;
    pTestHandle = std::make_unique<CImageMergingTest>();

    pTestHandle->add([&](int32_t nWidth, int32_t nHeight, int32_t nBytesPerRow, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
        {
            return run_test_func(&executor::cycle, algorithm::old_renewed, nWidth * nHeight, 1, pRGBABuffer, pAlphaBuffer);
        }, "old_renewed");
    
    pTestHandle->add([&](int32_t nWidth, int32_t nHeight, int32_t nBytesPerRow, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
        {
            update_indices(vIndices, nWidth, nHeight, 1);
            return run_test_func(&executor::parallel, algorithm::old_renewed, vIndices, pRGBABuffer, pAlphaBuffer);
        }, "old_renewed_par");

#if IS_x86_6432_BUILD
    pTestHandle->add([&](int32_t nWidth, int32_t nHeight, int32_t nBytesPerRow, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
        {
            update_indices(vIndices16, nBytesPerRow, nHeight, 16);
            return run_test_func(&executor::parallel, algorithm::simd_sse2, vIndices16, pRGBABuffer, pAlphaBuffer);
        }, "simd_sse2_ep");


    pTestHandle->add([&](int32_t nWidth, int32_t nHeight, int32_t nBytesPerRow, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
        {
            update_indices(vIndices16, nBytesPerRow, nHeight, 16);
            return run_test_func(&executor::parallel, algorithm::simd_sse41, vIndices16, pRGBABuffer, pAlphaBuffer);
        }, "simd_sse41_ep");

    pTestHandle->add([&](int32_t nWidth, int32_t nHeight, int32_t nBytesPerRow, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
        {
            update_indices(vIndices32, nBytesPerRow, nHeight, 32);
            return run_test_func(&executor::parallel, algorithm::simd_avx2, vIndices32, pRGBABuffer, pAlphaBuffer);
        }, "simd_avx2_ep");

    pTestHandle->add([&](int32_t nWidth, int32_t nHeight, int32_t nBytesPerRow, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
        {
            update_indices(vIndices32, nBytesPerRow, nHeight, 32);
            return run_test_func(&executor::parallel, algorithm::simd_avx, vIndices32, pRGBABuffer, pAlphaBuffer);
        }, "simd_avx2_mask_ep");

    pTestHandle->add([&](int32_t nWidth, int32_t nHeight, int32_t nBytesPerRow, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
        {
            update_indices(vIndices64, nBytesPerRow, nHeight, 64);
            return run_test_func(&executor::parallel, algorithm::simd_avx512, vIndices64, pRGBABuffer, pAlphaBuffer);
        }, "simd_avx512_ep");
#elif IS_ARM_BUILD
    pTestHandle->add([&](int32_t nWidth, int32_t nHeight, int32_t nBytesPerRow, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
        {
            return run_test_func(&executor::cycle, algorithm::simd_neon, nBytesPerRow * nHeight, 64, pRGBABuffer, pAlphaBuffer);
        }, "simd_neon");
    pTestHandle->add([&](int32_t nWidth, int32_t nHeight, int32_t nBytesPerRow, uint8_t* pRGBABuffer, uint8_t* pAlphaBuffer)
        {
            update_indices(vIndices64, nBytesPerRow, nHeight, 64);
            return run_test_func(&executor::parallel, algorithm::simd_neon, vIndices64, pRGBABuffer, pAlphaBuffer);
        }, "simd_neon_par");
#endif // ANDROID
}

void start_tests(const std::string& srAlgorithm, size_t exec_times)
{
    pTestHandle->run(srAlgorithm, exec_times);
    pTestHandle->finalize();
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
Java_com_example_perfomance_1test_MainActivity_setTestCase(
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
Java_com_example_perfomance_1test_MainActivity_runTest(
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
    auto scores_path = fmt::format("Execution time: {:.5f}s", result / 1000.f);
    return env->NewStringUTF(scores_path.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_perfomance_1test_MainActivity_getAvaliableTestCasesAndAlgorithms(
        JNIEnv *env,
        jobject thiz) {

    auto srJson = pTestHandle->getAvaliableTestCasesAndAlgorithms();
    return env->NewStringUTF(srJson.c_str());
}
#endif

#ifdef _WIN32
int main()
{
    initialize_test(fs::current_path().string());
    pTestHandle->selectTestCase("aerith_hd");
    auto result = run_test_func(start_tests, "", 100);
    return 0;
}
#endif