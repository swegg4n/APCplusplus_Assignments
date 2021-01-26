#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG
#include <crtdbg.h>

#include "TestLevel.h"

#if (defined(LEVEL)  && (LEVEL>20) ||  !defined(LEVEL))   && (defined(VG) || defined(G))

#include "List.hpp"

using Cont = List<char>;
using  Iter = Cont::iterator;
using  CIter = Cont::const_iterator;

#define FOX List<char> Fox("Fox");
#define FOX0 List<char> Fox;
#define BAR List<char> Bar("Bar");
#define BAR0 List<char> Bar;

#include <cassert>
#include <string>

using std::cout;

template <class T>
struct IsConstOrConstRef {
	static const bool value = std::is_const<std::remove_reference<T>::type>::value;
};

template <class T>
bool IsConstOrConstRefFun(T& x) {
	return std::is_const<std::remove_reference<T>::type>::value;
};

void TestIterRel(); //L�ngst ner

void TestSpliceAndSwap(); // L�ngst ner

template <class T>
void SpliceTest(List<T>, List<T>);

void TestList() {
	{
		List<char> Foo("Foo");
		const List<char> FooC("Foo");
		List<char> Bar("Bar");
		const List<char> BarC("Bar");
	}

	//Typedef, m�ste fungera f�r att testprogrammet ska fungera!
	{
		std::string strHej("hej");
		List<std::string> a{}; a.push_back(strHej);
		List<std::string>::iterator i;
		i = a.begin();
		assert(*i == strHej);
	}
	//Tom testning av att alla constructorer och tilldelningsoperaoterer finns!
	{
		Cont a{};
		Cont b{ a };
		Cont c{ "hej" };
		Cont d{ std::move(c) };
		assert(c.Invariant());
		assert(d.Invariant());
		assert(a.Invariant());
		assert(b.Invariant());
	}
	//-	~List<char>() Kom ih�g destruktorn!
	{
		auto x = new List<char>("hej");
		noexcept(x -> ~List()); //Kolla om det kompilerar!
		delete x;
	}

	//-	copy constructor
	{
		Cont a("Hej"), c(a);
		assert(c == "Hej" && a == c);
		a.front() = 'a';
		assert(a == "aej" && c == "Hej");
		Cont x, y(x);
		assert(x.Invariant() && y.Invariant());
	}

	//-	List<char>(char *)
	{
		List<char> v1("foo"); assert(v1 == "foo");
		List<char> vec(v1); assert(vec == "foo");
		List<char> v3("bar");  assert(v3 == "bar");
	}
	////	-	operator =(Str�ng str�ng)
	//{
	//    List<char> Foo("Foo");
	//    const List<char> FooC("Foo");
	//    List<char> Bar("Bar");
	//    const List<char> BarC("Bar");
	//    Cont a("hej");
	//    Bar = a;
	//    assert(Bar.Invariant());
	//    assert(Bar == "hej");
	//    assert((Bar = a) == a);
	//    assert((Bar = Bar) == a);	//self assignment
	//}

	////move!
	//{
	//    List<char> Foo("Foo");
	//    List<char> Bar("Bar");

	//    Bar = std::move(Foo);
	//    assert(Bar.Invariant()); assert(Foo.Invariant());
	//    assert(Bar == "Foo" && Foo == "");


	//    //move empty
	//    Bar = std::move(Foo);
	//    assert(Bar.Invariant()); assert(Foo.Invariant());
	//    assert(Bar == "" && Foo == "");

	//}

	////move const
	//{
	//    List<char> Foo("Foo");
	//    const List<char> FooC("Foo");
	//    List<char> Bar("Bar");
	//    const List<char> BarC("Bar");
	//    Bar = BarC;
	//    Bar = std::move(FooC);
	//    assert(Bar.Invariant()); assert(FooC.Invariant());
	//    assert(Bar == "Foo" && FooC == "Foo");
	//}

	// front, back;
	{
		Cont a("Hej");
		assert(a.front() == 'H' && a.back() == 'j');
		a.front() = 'x'; assert(a == "xej");
	}

	// insert erase
	{
		Cont a("abcde");
		auto it = ++++a.begin(); //c
		it = a.insert(it, 'X');
		assert(a.Invariant());
		assert(*it == 'X' && a == "abXcde");
		assert(a.Invariant());
		it = ----a.end(); //d
		it = a.erase(it);
		assert(a.Invariant());
		assert(*it == 'e' && a == "abXce");

	}
	//-	push och pop
	{
		Cont a{};
		char c = 'b';
		a.push_front(c);    //To use & version
		assert(a == "b");
		a.push_back('c');   //WIll use && version
		assert(a == "bc");
		a.push_front('a');
		assert(a == "abc");
		a.pop_front();
		assert(a == "bc");
		a.pop_back();
		assert(a == "b");
		a.push_back('x');
		assert(a == "bx");
	}

	//Test av Insert&& Struken!
	{
		//List<C> a;
		//a.push_front(C());
		//assert(C::usedConstr == "DCCC");
		//C c;
		//assert(C::usedConstr == "DC");
		//a.push_front(c);
		//assert(C::usedConstr == "CC");
		//assert(a.size() == 4);
		//a.push_front(std::move(c));
		//assert(C::usedConstr == "MC");
		//assert(a.size() == 5);
	}

	//size och empty
	{
		List<char> Bar("Bar");
		const List<char> BarC("Bar");
		Cont a{};
		assert(a.Invariant());

		//-	size();
		assert(3 == BarC.size() && 0 == a.size());
		//-	empty();
		assert(!BarC.empty() && a.empty());

	}

	//Check const of some functions!
	{
		List<char> Foo("Foo");
		const List<char> FooC("Foo");
		List<char> Bar("Bar");
		const List<char> BarC("Bar");
		BarC.front();
		assert(!IsConstOrConstRefFun(Bar.front()));
		assert(IsConstOrConstRefFun(BarC.front()));
		assert(!IsConstOrConstRefFun(Bar.back()));
		assert(IsConstOrConstRefFun(BarC.back()));
	}

	TestSpliceAndSwap();
	TestIterRel();
}

template<class C>
void TestRel(const C& lhs, const C& rhs,
	bool(*trueFun)(const C& lhs, const C& rhs),
	bool(*falseFun)(const C& lhs, const C& rhs)) {
	assert(trueFun(lhs, rhs));
	assert(falseFun(lhs, rhs));
}

void TestIterRel() {
#define RelTest(a, b, c, d)  a c b && !(a d b)

	List<char> vecAbcdef("Abcdef");
	List<char> vecBbcdef("Bbcdef");
	List<char> vecAbcdeF("AbcdeF");
	List<char> vecAbcdefg("Abcdefg");

	{ bool f = RelTest(vecAbcdef, vecAbcdef, == , != ); assert(f); }
	{ bool f = RelTest(vecAbcdef, vecAbcdeF, != , == ); assert(f); }
	{ bool f = RelTest(vecAbcdef, vecBbcdef, != , == ); assert(f); }
	{ bool f = RelTest(vecAbcdef, vecAbcdefg, != , == ); assert(f); }
	{ bool f = RelTest(vecAbcdefg, vecAbcdef, != , == ); assert(f); }

	{ bool f = RelTest(vecAbcdef, vecAbcdef, == , < ); assert(f); }
	bool f1 = vecAbcdeF < vecBbcdef;
	bool f2 = vecAbcdef < vecAbcdeF;
	bool f3 = vecAbcdef < vecAbcdef;
	{ bool f = RelTest(vecAbcdef, vecAbcdeF, > , < ); assert(f); }
	{ bool f = RelTest(vecAbcdef, vecBbcdef, <, >); assert(f); }
	{ bool f = RelTest(vecAbcdef, vecAbcdefg, <, >); assert(f); }

	{ bool f = RelTest(vecAbcdef, vecAbcdef, <= , != ); assert(f); }
	{ bool f = RelTest(vecAbcdef, vecAbcdeF, >= , <= ); assert(f); }
	{ bool f = RelTest(vecAbcdef, vecBbcdef, <= , >= ); assert(f); }
	{ bool f = RelTest(vecAbcdef, vecAbcdefg, <= , >= ); assert(f); }

	//swap(vecAbcdef, vecAbcdefg);
	//assert(vecAbcdef == "Abcdefg");
	//assert(vecAbcdefg == "Abcdef");
}

#ifdef VG
void TestSpliceAndSwap()
{
	{
		FOX BAR;
		swap(Fox, Bar);
		assert(Fox.Invariant());
		assert(Bar.Invariant());
		assert(Fox == "Bar" && Bar == "Fox");
		swap(Fox, Bar);
		assert(Fox == "Fox" && Bar == "Bar");
	}
	{
		FOX BAR0;
		swap(Fox, Bar);
		assert(Fox.Invariant());
		assert(Bar.Invariant());
		assert(Fox == "" && Bar == "Fox");
		swap(Fox, Bar);
		assert(Fox == "Fox" && Bar == "");
	}
	{
		FOX0 BAR;
		swap(Fox, Bar);
		assert(Fox.Invariant());
		assert(Bar.Invariant());
		assert(Fox == "Bar" && Bar == "");
		swap(Fox, Bar);
		assert(Fox == "" && Bar == "Bar");
	}
	{
		FOX0 BAR0;
		swap(Fox, Bar);
		assert(Fox.Invariant());
		assert(Bar.Invariant());
		assert(Fox == "" && Bar == "");
		swap(Fox, Bar);
		assert(Fox == "" && Bar == "");
	}

#ifdef  Additional_Splice_Tests
	{
		List<char> Foo("abc");
		List<char> Bar("123");
		SpliceTest(Foo, Bar);
	}
	{
		List<char> Foo("abc");
		List<char> Bar("");
		SpliceTest(Foo, Bar);
	}
	{
		List<char> Foo("");
		List<char> Bar("123");
		SpliceTest(Foo, Bar);
	}
	{
		List<char> Foo("");
		List<char> Bar("");
		SpliceTest(Foo, Bar);
}
#endif //Additional_Splice_Tests


	//Test Splice:
	//this (List)
	//can be empty or nonempty
	//Pos can be first, middle or end
	//Gives 4 cases
	//
	//other can be
	//empty or non empty
	//range to insert
	//from an empty, nonempty list
	//can be empty, one, more
	//can start at first or middle
	//can end at middle, end
	//gives 1+1+2*2=6
	//Testing 24 cases are to much!
	//We test:
	//with List empty:
	//       other empty range or nonempty range
	//with nonmepty List (3 Nodes)
	//     empty other
	//     one other, insert this
	//     3 in other list
	//         insert [1,4)
	//         insert [1,3)
	//         insert [1,1)
	//         insert [2,3)
	//         insert [2,4)
	//OBS jag har inte orkat skriva in alla testcasen som jag f�resl�r ovan!
	{
		FOX0 BAR0;
		Fox.splice(Fox.begin(), Bar, Bar.begin(), Bar.end());
		assert(Fox == "" && Bar == "");
	}
	{
		FOX0 BAR;
		Fox.splice(Fox.begin(), Bar, Bar.begin(), Bar.begin());
		Fox.splice(Fox.begin(), Bar, ++Bar.begin(), ++Bar.begin());
		assert(Fox == "" && Bar == "Bar");
	}
	{
		FOX0 BAR;
		Fox.splice(Fox.begin(), Bar, Bar.begin(), ++Bar.begin());
		assert(Fox == "B" && Bar == "ar");
	}
	{
		FOX0 BAR;
		Fox.splice(Fox.begin(), Bar, --Bar.end(), Bar.end());
		assert(Fox == "r" && Bar == "Ba");
	}
	{
		FOX BAR;
		Fox.splice(Fox.begin(), Bar, Bar.end(), Bar.end());
		assert(Fox == "Fox" && Bar == "Bar");
	}
	{
		FOX BAR;
		Fox.splice(++Fox.begin(), Bar, --Bar.end(), Bar.end());

		std::cout << "Fox: " << Fox << std::endl;
		std::cout << "Bar: " << Bar << std::endl;

		assert(Fox == "Frox" && Bar == "Ba");
	}
	{
		FOX BAR;
		Fox.splice(Fox.end(), Bar, Bar.begin(), ++Bar.begin());
		assert(Fox == "FoxB" && Bar == "ar");
	}
}


#endif VG

void XXX() {
	List<char> a("xyz");
	List<char>::iterator jjj;
	List<char>::iterator kkk = a.begin();
	for (auto it = a.begin(); it != a.end(); ++it) {}
	List<char>::const_iterator iii = a.cbegin();
	const List<char> c("xyz");
	List<char>::const_iterator ccc = c.cbegin();
	for (auto it = c.begin(); it != c.end(); ++it) {}
}

#else
void TestList() {}
#endif


template<class T>
void SpliceTest(List<T> Foo, List<T> Bar)
{
	std::cout << "\n__Before Splice__" << std::endl;
	std::cout << "Foo: " << Foo << std::endl;
	std::cout << "Bar: " << Bar << std::endl;
	Foo.splice(Foo.begin(), Bar, Bar.begin(), ++Bar.begin());
	std::cout << "__After Splice (Foo.begin(), Bar, Bar.begin(), ++Bar.begin())__" << std::endl;
	std::cout << "Foo: " << Foo << std::endl;
	std::cout << "Bar: " << Bar << std::endl;
	std::cout << std::endl;
}