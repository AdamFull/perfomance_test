#pragma once

#include "datatypes.h"
#include "utility.h"
#include <ufunction.hpp>

namespace testing
{
    using testing_func_sign = utl::function<double(int32_t, int32_t, int32_t, uint8_t*, uint8_t*)>;
    // Class for testing image merging algorithms
    class CImageMergingTest
    {
    public:
        CImageMergingTest();
        ~CImageMergingTest();

        // Run test with name "srName" for algorithm "function"

        void run(const std::string& srName = "", size_t times = 1);

        template<class _Func>
        void add(_Func&& function, const std::string& srName)
        {
            mAlgorithms.emplace(srName, std::forward<_Func>(function));
        }

        // Get fastest algorithm result
        const FTestChunk& getFastest(const FTestResult& testResult) const;

        // Get slowest algorithm result
        const FTestChunk& getSlowest(const FTestResult& testResult) const;

        const std::string getAvaliableTestCasesAndAlgorithms() const;

        // Show test results
        std::string finalize();
        // Load test cases
        void selectTestCase(const std::string& srCase);
    private:
        void loadConfig();
        // Run test execution
        FTestChunk run_image_merging_test(testing_func_sign& function, const std::string& srTestName, const std::string& srCaseName, const std::unique_ptr<FImageGroup>& group, size_t executions = 1);
    private:
        std::string srTestCase;
        std::map<std::string, testing_func_sign> mAlgorithms;
        std::map<std::string, FImagePairCI> mTestCases;
        std::map<std::string, FTestResult> mTestResults;
        std::map<std::string, std::unique_ptr<FImageGroup>> mImages;
    };
}