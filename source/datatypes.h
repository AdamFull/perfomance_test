#pragma once

#include "image.h"

namespace testing
{
    // Structure for loading test variants from json
    struct FImagePairCI
    {
        std::string srSrcPath;
        std::string srMaskPath;
        std::string srRefPath;
    };

    void to_json(nlohmann::json& json, const FImagePairCI& type);
    void from_json(const nlohmann::json& json, FImagePairCI& type);

    // Image data group
    struct FImageGroup
    {
        std::unique_ptr<CImage> pSrc;
        std::unique_ptr<CImage> pMask;
        std::unique_ptr<CImage> pRef;

        FImageGroup() = default;
        FImageGroup(const FImagePairCI& ci);

        void open(const FImagePairCI& ci);
    };

    struct FTestChunk
    {
        size_t iPlace{ 0 };
        size_t iScore{ 0 };
        std::string srName{};
        double fMean{ 0.0 };
        double fVariance{ 0.0 };
        double fDeviation{ 0.0 };
        bool bValid{ false };
        double fSameless{ 0.0 };
    };

    void to_json(nlohmann::json& json, const FTestChunk& type);
    void from_json(const nlohmann::json& json, FTestChunk& type);

    struct FTestResult
    {
        std::vector<FTestChunk> vChunks;
        std::string srBest;
        std::string srWorse;
        double fDelta{ 0.0 };
    };

    void to_json(nlohmann::json& json, const FTestResult& type);
    void from_json(const nlohmann::json& json, FTestResult& type);
}
