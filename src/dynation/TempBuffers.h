#pragma once

struct PROCESS_DATA_VIRTUAL : public PROCESS_DATA {
	PROCESS_DATA_VIRTUAL() {
		InChannelsCount = 0;
		OutChannelsCount = 0;
		SamplesCount = 0;
		for (u32 i = 0; i < DATAMAXCHANNELS; i++) {
			In[i] = nullptr;
			Out[i] = nullptr;
		}
	}

	PROCESS_DATA_VIRTUAL(PROCESS_DATA_VIRTUAL& Base, u32 Offset, u32 Samples) {
		assert(Base.SamplesCount >= Samples + Offset); // чекаем границу
		InChannelsCount  = Base.InChannelsCount;
		OutChannelsCount = Base.OutChannelsCount;
		SamplesCount  = Samples; 
		for (u32 i = 0; i < DATAMAXCHANNELS; i++) {
			In[i]  = &Base.In[i][Offset];
			Out[i] = &Base.Out[i][Offset];
		}
	}
};

struct PROCESS_DATA_BUFFER: public PROCESS_DATA {
	std::vector<f32>	InData[DATAMAXCHANNELS];
	std::vector<f32>	OutData[DATAMAXCHANNELS];

	PROCESS_DATA_BUFFER() {
		InChannelsCount = 0;
		OutChannelsCount = 0;
		for (u32 i = 0; i < DATAMAXCHANNELS; i++) {
			In[i]  = nullptr;
			Out[i] = nullptr;
		}
	}

	~PROCESS_DATA_BUFFER() {
		InChannelsCount = 0;
		OutChannelsCount = 0;
		for (u32 i = 0; i < DATAMAXCHANNELS; i++) {
			In[i] = nullptr;
			Out[i] = nullptr;
		}
	}

	void  Resize(u32 InChannelsCountNew, u32 OutChannelsCountNew, u32 SamplesCountNew) {
		if (SamplesCountNew == 0) return;
		
		if (InChannelsCountNew != InChannelsCount) {
			for (u32 i = InChannelsCountNew; i < InChannelsCount; i++) {
				InData[i].clear();
				In[i] = nullptr;
			}
			InChannelsCount = InChannelsCountNew;
		}

		if (OutChannelsCountNew != OutChannelsCount) {
			for (u32 i = OutChannelsCountNew; i < OutChannelsCount; i++) {
				OutData[i].clear();
				Out[i] = nullptr;
			}
			OutChannelsCount = OutChannelsCountNew;
		}

		if (SamplesCountNew != SamplesCount) {
			for (u32 i = 0; i < InChannelsCount; i++) {
				InData[i].resize(SamplesCountNew);
			}
			for (u32 i = 0; i < OutChannelsCount; i++) {
				OutData[i].resize(SamplesCountNew);
			}
			SamplesCount = SamplesCountNew;
		}

		for (u32 i = 0; i < InChannelsCount; i++) {
			In[i]  = &InData[i][0];
		}
		for (u32 i = 0; i < OutChannelsCount; i++) {
			Out[i] = &OutData[i][0];
		}
	}
};
