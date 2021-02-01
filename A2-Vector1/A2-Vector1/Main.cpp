#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

#include <type_traits>

#include "Vector.hpp"
#include "VectIterTest.h"


#include <iostream>
#include <cassert>

using std::cout;


void TestVector();

int main() {
#ifdef DBG_NEW
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    std::locale::global(std::locale("swedish"));
    TestVector();
    TestIterPart1();
    TestIterInAlg();

#if 0
    new int;
    cout << "det finns en minnesl�cka i main, avsiktligt!\n s� ni kan se att er minnesl�ckstest fungerar\n";
#endif
    std::cin.get();

}
