#pragma once
class Application
{
public:
	Application();
	virtual ~Application();
	static Application* getInstance();
	virtual bool initSystem() = 0;
	virtual bool do_business() = 0;
	int get_value(int a, int b);
	void run();
protected:
	static Application *m_app;
};

