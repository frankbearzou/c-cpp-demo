#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

class QuickFindUF
{
public:
	QuickFindUF(int N)
	{
		for (int i = 0; i < N; ++i)
		{
			id.push_back(i);
		}
	}

	bool connected(int p, int q)
	{
		return id[p] == id[q];
	}

	void union_op(int p, int q)
	{
		int pid = id[p];
		int qid = id[q];
		for_each(id.begin(), id.end(), [=](int &i){if (i == pid) i = qid; });
	}

	void show()
	{
		copy(id.cbegin(), id.cend(), ostream_iterator<int>(cout, " "));
		cout << endl;
	}
private:
	vector<int> id;
};

int main()
{
	QuickFindUF uf(10);
	uf.show();

	cin.get();
}
