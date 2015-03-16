#include <iostream>

using namespace std;

// new feature in c++11. This is very useful.
enum class Color1 : int { RED = 1, GREEN = 2, BLUE = 3 };

void fn(Color1 r)
{
	if (r == Color1::RED)
	{
		cout << "red" << endl;
	}
	else if (r == Color1::GREEN)
	{
		cout << "green" << endl;
	}
	else if (r == Color1::BLUE)
	{
		cout << "blue" << endl;
	}
}

void main()
{
	fn(Color1::BLUE);

	system("pause");
}