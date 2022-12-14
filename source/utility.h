#pragma once

#include <algorithm>
#include <future>
#include <uthreadpool.hpp>

namespace util
{
    size_t constexpr hash(char const* input)
    {
        return *input ? static_cast<size_t>(*input) + 33ull * hash(input + 1) : 5381ull;
    }

    template<class _FwdIt, class _Func>
    static void parallel_for_each(_FwdIt _First, _FwdIt _Last, _Func&& function, int numSegments = 0)
    {
        auto len = std::distance(_First, _Last);
        numSegments = numSegments > 0 ? numSegments : std::thread::hardware_concurrency() / 4;
        int segmentSize = len / numSegments;

        std::vector<std::future<void>> futures(numSegments - 1);
        uint32_t segment{ 0 };
        for (segment = 0; segment < numSegments - 1; ++segment)
        {
            auto segmentStart = _First + segment * segmentSize;
            auto segmentEnd = _First + (segment + 1) * segmentSize;

            futures[segment] = std::async(std::launch::async, [&function](_FwdIt segmentStart, _FwdIt segmentEnd) 
                {
                    for (auto i = segmentStart; i != segmentEnd; ++i) function(*i);
                }, segmentStart, segmentEnd);
        }

        auto segmentStart = _First + segment * segmentSize;
        auto segmentEnd = _First + (segment + 1) * segmentSize;
        for (auto i = segmentStart; i != segmentEnd; ++i) 
            function(*i);

        for (auto& future : futures) future.wait();
    }

    template<class _RanIt, class _Func>
    void parallel_for(_RanIt _First, _RanIt _Last, _Func&& function)
    {
#if defined(ANDROID)
    std::for_each(_First, _Last, std::forward<_Func>(function));
#else
    std::for_each(std::execution::par_unseq, _First, _Last, std::forward<_Func>(function));
#endif
    }
}

namespace file
{
	static bool read(const fs::path& path, std::string& data)
	{
		if (fs::exists(path))
		{
			std::ifstream file(path, std::ios_base::in | std::ios_base::binary);
			file.rdbuf()->pubsetbuf(0, 0);

			data = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
			return true;
		}

		return false;
	}

	static bool write(const fs::path& path, const std::string& data)
	{
		std::ofstream file(path, std::ios::out | std::ios::binary);
		file << data;
		file.close();

		return true;
	}

	static bool write(const fs::path& path, std::vector<uint8_t>& binary)
	{
		std::ofstream file(path, std::ios::out | std::ios::binary);
		file.write((char*)&binary[0], binary.size() * sizeof(binary[0]));
		file.close();

		return true;
	}

    template<class _Ty>
    static bool read_json(const fs::path& path, _Ty& type)
    {
        std::string data{};
        if (read(path, data))
        {
            if (!data.empty())
            {
                auto json = nlohmann::json::parse(data);
                if (!json.empty())
                {
                    json.get_to(type);
                    return true;
                }
            }
        }

        return false;
    }

    template<class _Ty>
    static bool write_json(const fs::path& path, const _Ty& type, int32_t indent = -1)
    {
        auto json = nlohmann::json(type);
        if (!json.empty())
        {
            auto data = json.dump(indent);
            if (!data.empty())
                return write(path, data);
        }

        return false;
    }

    template<class _Ty>
    static bool read_bson(const fs::path& path, _Ty& type)
    {
        std::string data{};
        if (read(path, data))
        {
            auto json = nlohmann::json::from_bson(data);
            if (!json.empty())
            {
                json.get_to(type);
                return true;
            }
        }

        return false;
    }

    template<class _Ty>
    static bool write_bson(const fs::path& path, const _Ty& type)
    {
        auto json = nlohmann::json(type);
        if (!json.empty())
        {
            auto data = nlohmann::json::to_bson(json);
            if (!data.empty())
                return write(path, data);
        }

        return false;
    }
}

namespace nlohmann
{
	template<class _Ty>
	inline void parse_to(const std::string& name, const nlohmann::json& json, _Ty& type)
	{
		if (auto obj = json.find(name); obj != json.end())
			obj->get_to(type);
	}
}

namespace math
{
	template<class _RanIt>
	static double mean(const _RanIt _First, const _RanIt _Last, double _Whole)
	{
		return std::accumulate(_First, _Last, 0.0, [_Whole](double a, double b) { return a + b / _Whole; });
	}

	template<class _RanIt>
    static double variance(const _RanIt _First, const _RanIt _Last, double _Mean, double _Whole)
	{
		return std::accumulate(_First, _Last, 0.0, [_Mean, _Whole](double a, double b) { return a + std::pow(b - _Mean, 2.0) / _Whole; });
	}

    static double deviation(double _Variance)
	{
		return _Variance == 0.0 ? 1.0 : std::sqrt(_Variance);
	}

    template<class _Ty>
    static void calculate_statistics(const std::vector<_Ty>& _vals, _Ty& _mean, _Ty& _variance, _Ty& _deviation)
    {
        auto fIter = static_cast<_Ty>(_vals.size());
        _mean = mean(_vals.begin(), _vals.end(), fIter);
        _variance = variance(_vals.begin(), _vals.end(), _mean, fIter);
        _deviation = deviation(_variance);
    }
}