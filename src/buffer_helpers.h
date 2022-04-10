/*******************************************************************************
* Copyright (C) Anton Kovalev (vertver), 2018 - 2022. All rights reserved.
* Copyright (C) Vladimir Shatrov (frowrik), 2018 - 2020. All rights reserved.
* Dynation plugin
* MIT License
***************************************************************************/
#pragma once

template<typename T>
class Buffer
{
private:
	size_t SamplesCount = 0;
	std::vector<T> ArrayBuffer;

public:
	void Clear()
	{
		memset(ArrayBuffer.data(), 0, ArrayBuffer.size() * sizeof(T));
	}

	void Flush()
	{
		ArrayBuffer.clear();
	}

	bool IsResetRequired(size_t SamplesCount)
	{
		if (this->SamplesCount >= SamplesCount) {
			return false;
		}

		return true;
	}

	void Reset(size_t SamplesCount)
	{
		if (!IsResetRequired(SamplesCount)) {
			Clear();
			return;
		}

		Flush();
		ArrayBuffer.resize(SamplesCount);
		this->SamplesCount = SamplesCount;
	}

	T* GetBuffer()
	{
		return ArrayBuffer.data();
	}

	size_t GetSamplesCount()
	{
		return SamplesCount;
	}

	T& operator[](std::size_t idx)
	{
		return ArrayBuffer[idx];
	}
};

template<typename T>
class PlanarBuffer
{
private:
	size_t FramesCount = 0;
	size_t ChannelsCount = 0;
	T* pArrayData[MAX_CHANNELS] = {};
	std::vector<T> PlanarArrayBuffer[MAX_CHANNELS];

public:
	void Clear()
	{
		for (auto& elem : PlanarArrayBuffer) {
			memset(elem.data(), 0, elem.size() * sizeof(T));
		}
	}

	void Flush()
	{
		for (auto& elem : PlanarArrayBuffer) {
			elem.clear();
		}
	}

	bool IsResetRequired(size_t ChannelsCount, size_t FramesCount)
	{
		if (ChannelsCount > MAX_CHANNELS) {
			return false;
		}

		if (this->FramesCount >= FramesCount) {
			return false;
		}

		if (this->ChannelsCount >= ChannelsCount) {
			return false;
		}

		return true;
	}

	void Reset(size_t ChannelsCount, size_t FramesCount)
	{
		if (!IsResetRequired(ChannelsCount, FramesCount)) {
			Clear();
			return;
		}

		Flush();
		for (size_t i = 0; i < ChannelsCount; i++) {
			PlanarArrayBuffer[i].resize(FramesCount);
		}

		for (size_t i = 0; i < ChannelsCount; i++) {
			pArrayData[i] = PlanarArrayBuffer[i].data();
		}

		this->FramesCount = FramesCount;
		this->ChannelsCount = ChannelsCount;
	}

	T** GetPlanar()
	{
		return pArrayData;
	}

	T* GetBuffer(size_t Idx)
	{
		return pArrayData[Idx];
	}

	size_t GetChannelsCount()
	{
		return ChannelsCount;
	}

	size_t GetFramesCount()
	{
		return FramesCount;
	}

	T* operator[](std::size_t idx)
	{
		return GetBuffer(idx);
	}
};
