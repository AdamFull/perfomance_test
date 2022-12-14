#include "merging_test.h"

extern fs::path writablePath;

using namespace testing;

CImageMergingTest::CImageMergingTest()
{
    loadConfig();
}

CImageMergingTest::~CImageMergingTest()
{
    mImages.clear();
    mTestCases.clear();
    mTestResults.clear();
}

const FTestChunk& CImageMergingTest::getFastest(const FTestResult& testResult) const
{
    auto result = std::min_element(testResult.vChunks.begin(), testResult.vChunks.end(),
        [](const FTestChunk& l, const FTestChunk& r) -> bool
        {
            return l.fMean < r.fMean;
        });

    return *result;
}

const FTestChunk& CImageMergingTest::getSlowest(const FTestResult& testResult) const
{
    auto result = std::max_element(testResult.vChunks.begin(), testResult.vChunks.end(),
        [](const FTestChunk& l, const FTestChunk& r) -> bool
        {
            return l.fMean < r.fMean;
        });

    return *result;
}

// Show results for any case and algorithm and save them to json
std::string CImageMergingTest::finalize()
{
    for (auto& [srCaseName, testResult] : mTestResults)
    {
        std::cout << fmt::format("\nCase \"{}\" scores\n", srCaseName);

        std::sort(testResult.vChunks.begin(), testResult.vChunks.end(), [](const FTestChunk& l, const FTestChunk& r)
            { return l.fMean < r.fMean; });

        // Show score and set place
        uint32_t place{ 1 };
        for (auto& result : testResult.vChunks)
        {
            result.iPlace = place++;
            std::cout << fmt::format("\"{}\": {:.5f}\n", result.srName, result.fMean);
        }

        auto fastest = getFastest(testResult);
        auto slowest = getSlowest(testResult);
        
        testResult.srBest = fastest.srName;
        testResult.srWorse = slowest.srName;
        testResult.fDelta = slowest.fMean - fastest.fMean;

        std::cout << fmt::format("Best: \"{}\"\nWorse:\"{}\"\nDelta: {}ms\n", testResult.srBest, testResult.srWorse, testResult.fDelta);
    }

    auto srScoresPath = writablePath / "scores.json";
    file::write_json(srScoresPath, mTestResults, 4);

    return srScoresPath.string();
}

const std::string CImageMergingTest::getAvaliableTestCasesAndAlgorithms() const
{
    auto json = nlohmann::json();
    std::vector<std::string> vTestCases;
    std::vector<std::string> vAlgorithms;

    for (auto& [srName, ci] : mTestCases)
        vTestCases.emplace_back(srName);

    for (auto& [srName, function] : mAlgorithms)
        vAlgorithms.emplace_back(srName);

    json["test_cases"] = vTestCases;
    json["algorithms"] = vAlgorithms;

    return json.dump();
}

void CImageMergingTest::selectTestCase(const std::string& srCase)
{
    mTestResults.clear();
    mImages.clear();

    srTestCase = srCase;

    // Load selected test case
    if (!srTestCase.empty())
    {
        auto found = mTestCases.find(srTestCase);
        if (found != mTestCases.end())
        {
            mImages.emplace(found->first, std::make_unique<FImageGroup>(found->second));
            mTestResults[found->first] = FTestResult{};
        }
    }
    // Load all test cases
    else
    {
        for (auto& [srCase, imgCI] : mTestCases)
        {
            mImages.emplace(srCase, std::make_unique<FImageGroup>(imgCI));
            mTestResults[srCase] = FTestResult{};
        }
    }
}

void CImageMergingTest::loadConfig()
{
    if (fs::exists(writablePath / tests_output_folder))
        fs::remove_all(writablePath / tests_output_folder);
    fs::create_directories(writablePath / tests_output_folder);

    auto result = file::read_json(writablePath / "test_cases.json", mTestCases);
    assert(result && "Cannot open config file");
}

FTestChunk CImageMergingTest::run_image_merging_test(testing_func_sign& function, const std::string& srTestName, const std::string& srCaseName, const std::unique_ptr<FImageGroup>& group, size_t executions)
{
    FTestChunk result;
    std::vector<double> vMeanResults;

    result.srName = srTestName;

    auto srCasePath = writablePath / tests_output_folder / srCaseName;

    if (!fs::exists(srCasePath))
        fs::create_directories(srCasePath);

    auto srOutput = fmt::format("{}/{}.{}", srCasePath.string(), srTestName, output_image_format);

    // Allocating temporary image buffers
    auto pSrc = group->pSrc->clone();
    auto pMask = group->pMask->clone();

    for (size_t exec = 0; exec < executions; ++exec)
    {
        std::vector<double> vExecutionResults;

        // Executing tests
        for (size_t nTest = 0; nTest < test_times; ++nTest)
        {
            vExecutionResults.emplace_back(function(pSrc->getWidth(), pSrc->getHeight(), pSrc->getBytesPerRow(), pSrc->raw(), pMask->raw()));

            // Save only first image for validation
            if (nTest == 0 && exec == 0 && save_output_results)
                pSrc->save(srOutput);
        }

        if (executions == 1)
            math::calculate_statistics(vExecutionResults, result.fMean, result.fVariance, result.fDeviation);
        else
            vMeanResults.emplace_back(math::mean(vExecutionResults.begin(), vExecutionResults.end(), static_cast<double>(test_times)));
    }

    // Calculating statistics and total score (then deviation is lower, then better)
    if (!vMeanResults.empty())
        math::calculate_statistics(vMeanResults, result.fMean, result.fVariance, result.fDeviation);

    result.iScore = 1.0 / result.fMean / result.fDeviation;
    result.bValid = group->pRef->compare(pSrc, result.fSameless);

    return result;
}