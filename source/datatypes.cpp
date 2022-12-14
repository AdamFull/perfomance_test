#include "datatypes.h"
#include "utility.h"

extern fs::path writablePath;

namespace testing
{
	void to_json(nlohmann::json& json, const FImagePairCI& type)
	{
		json = nlohmann::json
		{
			{"src", type.srSrcPath},
			{"mask", type.srMaskPath},
			{"ref", type.srRefPath}
		};
	}

	void from_json(const nlohmann::json& json, FImagePairCI& type)
	{
		nlohmann::parse_to("src", json, type.srSrcPath);
		nlohmann::parse_to("mask", json, type.srMaskPath);
		nlohmann::parse_to("ref", json, type.srRefPath);
	}

	void to_json(nlohmann::json& json, const FTestChunk& type)
	{
		json = nlohmann::json
		{
			{"name", type.srName},
			{"place", type.iPlace},
			{"score", type.iScore},
			{"mean", type.fMean},
			{"variance", type.fVariance},
			{"deviation", type.fDeviation},
			{"valid", type.bValid},
			{"sameless", type.fSameless},
		};
	}

	void from_json(const nlohmann::json& json, FTestChunk& type)
	{
		nlohmann::parse_to("name", json, type.srName);
		nlohmann::parse_to("place", json, type.iPlace);
		nlohmann::parse_to("score", json, type.iScore);
		nlohmann::parse_to("mean", json, type.fMean);
		nlohmann::parse_to("variance", json, type.fVariance);
		nlohmann::parse_to("deviation", json, type.fDeviation);
		nlohmann::parse_to("valid", json, type.bValid);
		nlohmann::parse_to("sameless", json, type.fSameless);
	}

	void to_json(nlohmann::json& json, const FTestResult& type)
	{
		json = nlohmann::json
		{
			{"chunks", type.vChunks},
			{"best", type.srBest},
			{"worse", type.srWorse},
			{"delta", type.fDelta}
		};
	}

	void from_json(const nlohmann::json& json, FTestResult& type)
	{
		nlohmann::parse_to("chunks", json, type.vChunks);
		nlohmann::parse_to("best", json, type.srBest);
		nlohmann::parse_to("worse", json, type.srWorse);
		nlohmann::parse_to("delta", json, type.fDelta);
	}
}

using namespace testing;

FImageGroup::FImageGroup(const FImagePairCI& ci)
{
	open(ci);
}

void FImageGroup::open(const FImagePairCI& ci)
{
	pSrc = std::make_unique<CImage>(writablePath / ci.srSrcPath);
	pMask = std::make_unique<CImage>(writablePath / ci.srMaskPath);
	pRef = std::make_unique<CImage>(writablePath / ci.srRefPath);
}