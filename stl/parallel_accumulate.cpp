#include <iostream>
#include <string>
#include <algorithm>
#include <thread>
#include <vector>
#include <numeric>

using namespace std;

template<typename Iter, typename T>
void accumulate_pack(Iter first, Iter last, T &val)
{
	val = std::accumulate(first, last, val);
}

template<typename Iter, typename T>
T parallel_accumulate(Iter first, Iter last, T val)
{
	auto hardware_threads = thread::hardware_concurrency();
	hardware_threads = hardware_threads == 0 ? 2 : hardware_threads;

	vector<thread> threads(hardware_threads);
	vector<T> res(hardware_threads);
	auto count = distance(first, last);
	auto block_size = count / (hardware_threads - 1);

	
	Iter block_first = first;
	for (unsigned int i = 0; i < hardware_threads - 1; ++i)
	{
		Iter block_last = block_first;
		advance(block_last, block_size);
		threads[i] = thread(fn<Iter, T>, block_first, block_last, ref(res[i]));
		block_first = block_last;
	}
	threads[hardware_threads - 1] = thread(accumulate_pack<Iter, T>, block_first, last, ref(res[hardware_threads - 1]));

	for_each(threads.begin(), threads.end(), mem_fn(&thread::join));

	return accumulate(res.begin(), res.end(), val);
}

int main()
{
	vector<int> vec(100);

	iota(vec.begin(), vec.end(), 1);
	for (auto a : vec)
	{
		cout << a << " ";
	}
	cout << endl;
	int n = 0;
	
	cout << parallel_accumulate(vec.begin(), vec.end(), 0) << endl;

	
	
	cout << "done" << endl;
	
	cin.get();
	return 0;
}
