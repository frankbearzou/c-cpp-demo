#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <string>
#include <exception>
#include <iostream>

using namespace std;

using boost::property_tree::ptree;

class IniReader
{
public:
	IniReader(const string& filename)
	{
		read_ini(filename, pt);
	}

	string getValue(const string& key) const
	{
		return pt.get<string>(key);
	}

private:
	ptree pt;
};

int main()
{
	try
	{
		IniReader reader("C:/Windows/win.ini");
		cout << reader.getValue("Mail.MAPI") << endl;
		cout << reader.getValue("MCI Extensions.mp4") << endl;
	}
	catch (std::exception &e)
	{
		std::cout << "Error: " << e.what() << "\n";
	}

	system("pause");
	return 0;
}
