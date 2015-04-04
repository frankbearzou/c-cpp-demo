#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

class MyButton
{
public:
	virtual void create() = 0;
	virtual void click() = 0;
};

class MyWinButton : public MyButton
{
public:
	void create() override
	{
		MessageBox(0, "Windows Button created", "created", 0);
	}

	void click() override
	{
		MessageBox(0, "click", "click", 0);
	}
};

class LinButton : public MyButton
{
public:
	void create() override
	{
		cout << "Linux Button created" << endl;
	}

	void click() override
	{
		cout << "Linux Button clicked" << endl;
	}
};

class MyText
{
public:
	MyText()
	{}

	MyText(string s) : _s(s)
	{}

	void setText(string s)
	{
		_s = s;
	}

	virtual void show() = 0;
protected:
	string _s;
};

class WinText : public MyText
{
public:
	WinText()
	{}

	void show() override
	{
		MessageBox(0, _s.data(), _s.data(), 0);
	}
};

class LinText : public MyText
{
public:
	void show() override
	{
		cout << _s << endl;
	}
};

class IFactory
{
public:
	virtual MyButton* createButton() = 0;
	virtual MyText* createText() = 0;
};

class WinFactory : public IFactory
{
public:
	MyButton* createButton() override
	{
		return new MyWinButton;
	}

	MyText* createText() override
	{
		return new WinText;
	}
};

class LinFacroty : public IFactory
{
public:
	MyButton* createButton() override
	{
		return new LinButton;
	}

	MyText* createText() override
	{
		return new LinText;
	}
};

class FactoryManager
{
public:
	static IFactory* getFactory()
	{
#ifdef WIN32
		return new WinFactory;
#else
		return new LinFacroty;
#endif
	}
};

void main()
{
	IFactory *factory = FactoryManager::getFactory();
	auto button = factory->createButton();
	button->create();
	button->click();

	auto text = factory->createText();
	text->setText(string("hello"));
	text->show();

	system("pause");
}
