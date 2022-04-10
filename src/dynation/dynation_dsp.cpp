#include "dynation.h"

void
DynationPlugin::Process(AudioProcessingBlock<float>& ProcessingBlock)
{
	for (size_t i = 0; i < OutputChannels; i++) {
		memcpy(ProcessingBlock.Outputs[i], ProcessingBlock.Inputs[i], ProcessingBlock.FramesToProcess * sizeof(float));
	}
}

void
DynationPlugin::ProcessDouble(AudioProcessingBlock<double>& ProcessingBlock)
{

}
