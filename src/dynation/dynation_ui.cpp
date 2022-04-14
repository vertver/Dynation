/*******************************************************************************
* Copyright (C) Anton Kovalev (vertver), 2018 - 2022. All rights reserved.
* Copyright (C) Vladimir Shatrov (frowrik), 2018 - 2020. All rights reserved.
* Dynation plugin
* MIT License
***************************************************************************/
#include "dynation.h"

bool 
DynationView::Initialize(PluginNotifier* InNotifier)
{
	return false;
}

void
DynationView::Destroy()
{

}

bool 
DynationView::WindowAttached(void* ParentWindow)
{
	return false;
}

bool 
DynationView::WindowRemoved()
{
	return false;
}

bool 
DynationView::IsPluginResizable()
{
	return false;
}

void 
DynationView::GetWindowSize(SRect& rect)
{

}

bool
DynationView::CheckSizeConstraint(SRect& rect)
{
	return false;
}

bool
DynationView::InformResize(SRect& rect)
{
	return false;
}
