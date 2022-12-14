#include "image.h"

#include <atomic>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace testing;

CImage::CImage(const fs::path& srPath)
{
	load(srPath);
}

CImage::~CImage()
{
	close();
}

void CImage::load(const fs::path& srPath)
{
	// Loading image data end store it in smart pointer
	pRawData = stbi_load(srPath.string().c_str(), &nWidth, &nHeight, &nChannels, nChannelsReq);

	pBuffer = image_ptr(pRawData, stbi_image_free);

	nBytesPerRow = nWidth * nChannelsReq;
	nSize = nBytesPerRow * nHeight;

	vIndices.resize(nSize);
	std::iota(vIndices.begin(), vIndices.end(), 0);
}

void CImage::close()
{
	pBuffer = nullptr;
}

void CImage::save(const fs::path& srPath)
{
	auto ext = srPath.extension().string();
	save(srPath, util::hash(ext.c_str()));
}

void CImage::save(const fs::path& srPath, size_t format)
{
	int result{0};
	switch (format)
	{
	case format::png: {
		result = stbi_write_png(srPath.string().c_str(), nWidth, nHeight, nChannelsReq, pRawData, nBytesPerRow);
	} break;
	case format::jpg: {
		auto pRawDataTemp = new uint8_t[nSize];
		std::memcpy(pRawDataTemp, pRawData, nSize);
		auto pJpgRaw = stbi__convert_format(pRawDataTemp, nChannelsReq, 3, nWidth, nHeight);
		result = stbi_write_jpg(srPath.string().c_str(), nWidth, nHeight, 3, pJpgRaw, 15);
	} break;
	case format::bmp: {
		result = stbi_write_bmp(srPath.string().c_str(), nWidth, nHeight, nChannelsReq, pRawData);
	} break;
	case format::tga: {
		result = stbi_write_tga(srPath.string().c_str(), nWidth, nHeight, nChannelsReq, pRawData);
	} break;
	case format::hdr: {
		auto pHdrData = new float[nSize];
		stbi__hdr_convert(pHdrData, pRawData, nChannelsReq);
		result = stbi_write_hdr(srPath.string().c_str(), nWidth, nHeight, nChannelsReq, pHdrData);
		delete[] pHdrData;
	} break;
	default: {
		std::runtime_error("Save format is not supported");
	} break;
	}
}

std::unique_ptr<CImage> CImage::clone()
{
	auto this_clone = std::make_unique<CImage>();
	this_clone->nWidth = nWidth;
	this_clone->nHeight = nHeight;
	this_clone->nChannels = nChannels;
	this_clone->nChannelsReq = nChannelsReq;
	this_clone->nSize = nSize;
	this_clone->nBytesPerRow = nBytesPerRow;

	this_clone->vIndices = vIndices;

	this_clone->pRawData = new uint8_t[nSize];
	std::memcpy(this_clone->pRawData, pRawData, nSize);
	this_clone->pBuffer = image_ptr(this_clone->pRawData, stbi_image_free);

	return this_clone;
}

bool CImage::compare(const std::unique_ptr<CImage>& pOther, double& fSameless)
{
	std::atomic<size_t> iGoods{ 0 };
	bool bResult{ true };
	util::parallel_for(vIndices.cbegin(), vIndices.cend(),
		[&](auto idx)
		{
			bool comp = pRawData[idx] & *pOther->raw(idx);
			if (comp)
				iGoods.fetch_add(1, std::memory_order_relaxed);

			bResult = bResult && comp;
		});

	fSameless = static_cast<double>(iGoods.load()) / static_cast<double>(nSize);

	return bResult;
}

uint8_t* CImage::raw(size_t idx)
{
	return pRawData + idx;
}