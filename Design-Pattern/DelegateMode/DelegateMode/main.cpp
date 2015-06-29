#include "Application.h"
#include "ApplicationDelegate.h"
#include <iostream>

using namespace std;

void main()
{
	ApplicationDelegate app;
	app.getInstance()->run();
	system("pause");
}