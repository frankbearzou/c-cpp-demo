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
			sz.push_back(1);
		}
	}

	bool connected(int p, int q)
	{
		return root(p) == root(q);
	}

	void union_op(int p, int q)
	{
		int i = root(p);
		int j = root(q);
		if (i == j)
			return;
		if (sz[i] < sz[j])
		{
			id[i] = j;
			sz[j] += sz[i];
		}
		else
		{
			id[j] = i;
			sz[i] += sz[j];
		}
	}

	void show()
	{
		copy(id.cbegin(), id.cend(), ostream_iterator<int>(cout, " "));
		cout << endl;
	}

private:
	int root(int i)
	{
		while (i != id[i])
		{
			i = id[i];
		}
		
		return i;
	}

private:
	vector<int> id;
	vector<int> sz;
};

int main()
{
	QuickFindUF uf(10);
	uf.show();

	cin.get();
	return 0;
}
