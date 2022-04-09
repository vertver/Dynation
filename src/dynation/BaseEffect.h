#pragma once
#include "Header.h"

class CEffect
{
public:
	virtual void Process(PROCESS_DATA* processData) = 0;
	virtual void ProcessDouble(PROCESS_DATA_DOUBLE* processData) = 0;
	virtual void SetEffectParameter(void* pParam) = 0;
};
