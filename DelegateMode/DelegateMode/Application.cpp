#include "Application.h"
#include <iostream>

using namespace std;

Application* Application::m_app = nullptr;

Application::Application()
{
	if (m_app == nullptr)
	{
		m_app = this;
	}
}


Application::~Application()
{
}

Application* Application::getInstance()
{
	return m_app;
}

int Application::get_value(int a, int b)
{
	return a + b;
}

void Application::run()
{
	if (!initSystem())
	{
		cout << "init system fatal." << endl;
	}

	if (!do_business())
	{
		cout << "do something..." << endl;
	}

	int res = get_value(3, 5);
	cout << "result:" << res << endl;
}
