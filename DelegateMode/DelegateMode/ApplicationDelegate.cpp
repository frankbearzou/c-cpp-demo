#include "ApplicationDelegate.h"
#include <iostream>

using namespace std;

ApplicationDelegate::ApplicationDelegate()
{
}


ApplicationDelegate::~ApplicationDelegate()
{
}

bool ApplicationDelegate::initSystem()
{
	cout << "in derived class" << endl;
	return true;
}

bool ApplicationDelegate::do_business()
{
	cout << "this is business logic, I can do something here..." << endl;
	return true;
}
