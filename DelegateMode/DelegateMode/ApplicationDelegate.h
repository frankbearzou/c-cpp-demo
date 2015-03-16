#pragma once
#include "Application.h"
class ApplicationDelegate :
	public Application
{
public:
	ApplicationDelegate();
	~ApplicationDelegate();
	virtual bool initSystem();
	virtual bool do_business();
};

