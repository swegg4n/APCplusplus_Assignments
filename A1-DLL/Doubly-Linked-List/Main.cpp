#ifdef _DEBUG
#ifndef DBG_NEW
#include <stdlib.h>
#include <crtdbg.h>
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

#include <iostream>
using std::cout;

#include "TestLevel.h"

#include "List.hpp"


//template class List<int>;


void TestBasic();
void TestList();
void TestListIter();

int main() {
#ifdef DBG_NEW
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    std::locale::global(std::locale("swedish"));

    TestBasic();
    TestList();
    TestListIter();

#if 1
    new int(7);
    std::cout << "There should be one memory leak!";
#endif

    std::cin.get();
}

//template class List<int>;