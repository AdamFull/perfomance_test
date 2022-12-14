#pragma once

#include "utility.h"

namespace testing
{
	namespace format
	{
		constexpr size_t png = util::hash(".png");
		constexpr size_t jpg = util::hash(".jpg");
		constexpr size_t hdr = util::hash(".hdr");
		constexpr size_t bmp = util::hash(".bmp");
		constexpr size_t tga = util::hash(".tga");
	}

	using image_ptr = std::unique_ptr<uint8_t, void(*)(void*)>;

	class CImage
	{
	public:
		CImage() = default;
		CImage(const fs::path& srPath);
		~CImage();

		// Load image from file
		void load(const fs::path& srPath);
		// Deallocate image
		void close();
		// Save image to file
		void save(const fs::path& srPath);
		// Save image to file with specific format
		void save(const fs::path& srPath, size_t format);
		// Make clone of this image
		std::unique_ptr<CImage> clone();
		// Compare image bytes
		bool compare(const std::unique_ptr<CImage>& pOther, double& fSameless);
		// Get raw image pointer
		uint8_t* raw(size_t idx = 0);

		// Getters
		inline int32_t getWidth() const { return nWidth; }
		inline int32_t getHeight() const { return nHeight; }
		inline int32_t getChannels() const { return nChannels; }
		inline int32_t getChannelsReq() const { return nChannelsReq; }
		inline size_t getSize() const { return nSize; }
		inline int32_t getBytesPerRow() const { return nBytesPerRow; }
	private:
		int32_t nWidth{ 0 };
		int32_t nHeight{ 0 };
		int32_t nChannels{ 0 };
		int32_t nChannelsReq{ 4 };
		size_t nSize{ 0 };
		int32_t nBytesPerRow{ 0 };
		std::vector<int32_t> vIndices;
		image_ptr pBuffer{ nullptr, nullptr };
		uint8_t* pRawData{ nullptr };
	};
}