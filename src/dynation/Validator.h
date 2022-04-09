/*******************************************************************************
* Copyright (C) Anton Kovalev (vertver), 2018 - 2022. All rights reserved.
* Copyright (C) Vladimir Shatrov (frowrik), 2018 - 2022. All rights reserved.
* Dynation plugin
* MIT License
***************************************************************************/
#pragma once
#include "VST2_Header.h"

class KeyValidation
{
private:
public:	
	bool SetKey(string128 KeyStr)
	{
		return true;
	}

	bool ValidateKey()
	{
		return true;
	}
};
