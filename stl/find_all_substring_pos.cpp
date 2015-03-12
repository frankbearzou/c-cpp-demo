#include <iostream>
#include <vector>
#include <string>
 
using namespace std;
 
vector<string::size_type> find_all_substring_pos(string str, string search_str)
{
	vector<string::size_type> result;
	string::size_type index = 0;
 
	while ((index = str.find(search_str, index)) != str.npos)
	{
		//cout << index << endl;
		result.push_back(index);
		++index;
	}
 
	return result;
}
 
void main()
{
	string str("abc cab cba abc nabcc!");
	string search_str("abc");
	
	auto vec = find_all_substring_pos(str, search_str);
	for (const auto &p : vec)
	{
		cout << p << endl;
	}
 
	system("pause");
}
