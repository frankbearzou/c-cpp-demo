#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <string>
#include <map>
#include <set>
#include <exception>
#include <iostream>

using namespace std;

struct Book
{
	string author;
	string title;
	string genre;
	double price;
	string publish_date;
	string description;
};

ostream& operator<<(ostream& os, const Book& book)
{
	os << book.author << endl;
	os << book.title << endl;
	os << book.genre << endl;
	os << book.price << endl;
	os << book.publish_date << endl;
	os << book.description << endl;

	return os;
}

using boost::property_tree::ptree;

class Books
{
public:
	Books(const std::string &filename)
	{
		read_xml(filename, pt);
	}

	void load()
	{
		for (auto& child : pt.get_child("catalog"))
		{
			string id;
			Book book;

			if (child.first == "book")
				id = child.second.get<std::string>("<xmlattr>.id", "No id");
			book.author = child.second.get<string>("author");
			book.title = child.second.get<string>("title");
			book.genre = child.second.get<string>("genre");
			book.price = child.second.get<double>("price");
			book.publish_date = child.second.get<string>("publish_date");
			book.description = child.second.get<string>("description");
			boost::erase_all(book.description, "\n");

			_books[id] = book;
		}
	}

	void show() const
	{
		for (const auto& book : _books)
		{
			cout << book.first << endl;
			cout << book.second << endl;
		}
	}
private:
	map<string, Book> _books;
	ptree pt;
};

int main()
{
	try
	{
		Books books("books.xml");
		books.load();
		books.show();
	}
	catch (std::exception &e)
	{
		std::cout << "Error: " << e.what() << "\n";
	}

	system("pause");
	return 0;
}
