#include <iostream>
#include <iterator>

using namespace std;

struct rand_tag
{

};

struct bi_tag
{

};

struct bi_iter : public iterator<bi_tag, float>
{

};

struct rand_iter : public iterator<rand_tag, int>
{

};

template<class _Ty, class Dis>
void adv(_Ty iter, Dis n, rand_tag)
{
	cout << "rand adv" << endl;
}

template<class _Ty, class Dis>
void adv(_Ty iter, Dis n, bi_tag)
{
	cout << "bi adv" << endl;
}

template<class _Ty>
typename iterator_traits<_Ty>::iterator_category
iter_cast(const _Ty&)
{
	typename iterator_traits<_Ty>::iterator_category cat;
	return (cat);
}

template<class _Ty, class Dis>
void adv(_Ty& iter, Dis n)
{
	// adv(iter, n, typename iterator_traits<_Ty>::iterator_category());
	adv(iter, n, iter_cast(iter));
}

void main()
{
	rand_iter i;
	adv(i, 3);

	bi_iter f;
	adv(f, 4);


	system("pause");
}
