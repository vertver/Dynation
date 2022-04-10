#include "base.h"

#define PARAMETERS_COUNT 34

enum class DistortionType : int16_t
{
	NoneType,
	TubeDistortion,
	Sincrusher,
	HardClip,
	FBack,
	SoftClip,

	// For TypeConverter
	MaxEnum
};

enum class CompressorStatus : int16_t
{
	Disabled,
	PreComp,
	PostComp,

	// For TypeConverter
	MaxEnum
};
	
enum class CompressorMode : int16_t
{
	BasicCompressor,
	EnvelopeCompressor,
	RMSCompressor,
	AnalogCompressor,

	// For TypeConverter
	MaxEnum
};

struct CompressorState
{
	CompressorStatus CompStatus = CompressorStatus::Disabled;
	CompressorMode CompressorNode = CompressorMode::BasicCompressor;
	int16_t reserved2 = 0;
	int16_t reserved3 = 0;

	// Time block
	float Attack = 0.f;				// Attack 
	float Release = 0.f;			// Release
	float Threshold = 0.f;			// Linear threshold 
	float Ratio = 1.f;				// 1:x ration value

	// Mixing block
	float Reserved = 0.f;
	float ParallelMix = 1.f;		// Parallel mixing
	float PumpGain = 1.f;			// Linear amplifier value
	float AnalogSubmix = 0.f;		// Mix between two parallel compressor with unique sound
};

struct DynationState
{
	// States and modes block
	DistortionType DistortType = DistortionType::NoneType;
	int16_t reserved = 0;

	// Base block
	float DryWet = 1.f;				// Global dry/wet level
	float InputVolume = 1.f;		// Linear input volume
	float OutputVolume = 1.f;		// Linear output volume
	float TiltEQ = 0.5f;			// Tilt EQ level

	// Distortion block
	float Drive = 0.f;				// Distortion drive! (dry/wet of distortion + power of distortion)
	float Hardness = 0.f;			// Distortion hardness.

	// Bitcrusher
	float Downshifter = 0.f;		// Downshifter level
	float Bitshifter = 0.f;			// Bitshifter level

	// Compressor
	CompressorState FirstCompressor;
	CompressorState SecondCompressor;
};

#include "dynation_parameters.h"

class CStateStorage
{
private:
	StateType* CurrentState = nullptr;
	std::mutex DataLock;

public:
	CStateStorage(StateType* pInGainState) : CurrentState(pInGainState) {}
	~CStateStorage() { delete CurrentState; }

	void Lock() { DataLock.lock(); }
	void Unlock() { DataLock.unlock(); }
	StateType* State() { return this->CurrentState; }
};

class DynationView : BaseView
{
private:
	BasePlugin* Parent = nullptr;

public:
	DynationView(BasePlugin* InParent) : Parent(InParent) {}
	~DynationView() {}

public:
	virtual bool Initialize() override;
	virtual void Destroy() override;

public:
	virtual bool WindowAttached(void* ParentWindow) override;
	virtual bool WindowRemoved() override;

public:
	virtual bool IsPluginResizable() override;
	virtual void GetWindowSize(SRect& rect) override;
	virtual bool CheckSizeConstraint(SRect& rect) override;

public:
	virtual bool InformResize(SRect& rect) override;
};

class DynationPlugin : BasePlugin
{
private:
	CStateStorage* State = nullptr;
	PluginNotifier* Notifier = nullptr;
	PluginInfo Info = {};

private:
	DynationState TempState = {};

private:
	int32_t Samplerate = 0;
	int32_t BlockSize = 0;
	int32_t InputChannels = 0;
	int32_t OutputChannels = 0;

	std::unique_ptr<DynationContainer> Parameters;

private:
	void Reset();

public:
	DynationPlugin(CStateStorage* InGainState);
	~DynationPlugin();

public:
	virtual void GetPluginInfo(PluginInfo*& Info) override;
	virtual bool Initialize(PluginNotifier* InNotifier) override;
	virtual void Destroy() override;

public:
	virtual void Suspend() override;
	virtual void Resume() override;

public:
	virtual void SetChannelsCount(int32_t InputChannels, int32_t OutputChannels) override;
	virtual void SetSamplerate(int32_t Samplerate) override;
	virtual void SetBlockSize(int32_t BlockSize) override;

public:
	virtual std::string GetParameterNameString(int32_t ParameterIndex) override;
	virtual std::string GetParameterLabelString(int32_t ParameterIndex) override;
	virtual std::string GetParameterValueString(int32_t ParameterIndex) override;

public:
	virtual int32_t GetParametersCount() override;
	virtual float GetParameter(int32_t ParameterIdx) override;
	virtual void SetParameter(int32_t ParameterIdx, float ParameterValue) override;

public:
	virtual void Process(AudioProcessingBlock<float>& ProcessingBlock) override;
	virtual void ProcessDouble(AudioProcessingBlock<double>& ProcessingBlock) override;
};