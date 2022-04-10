#include "dynation.h"

bool 
DynationView::Initialize()
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
