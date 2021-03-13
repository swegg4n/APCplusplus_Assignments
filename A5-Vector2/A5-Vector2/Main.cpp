//#ifdef _DEBUG
//#ifndef DBG_NEW
//#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
//#define new DBG_NEW
//#endif
//#endif  // _DEBUG
//#include <crtdbg.h>


#include "TestLevel.h"

#include "Vector.hpp"

#include <iostream>
using std::cout;

void TestPushBack();

void TestBasic();
void TestVector();
void TestIter();
//void TestIterInAlg();
//void TestRevIterInAlg();


int main() {

#ifdef DBG_NEW
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    std::locale::global(std::locale("swedish"));

#ifdef WRITEDESC
    WriteDesc<int>();
#endif

    /*std::cout <<
        "�ndra i TestLevel.h f�r att kontrollera vad som testas\n"
        "G� stegvis upp med LEVEL fr�n 1 till ungef�r 25, �ndra den sen till 99\n"
        "Och kontrollera vilket betyg som testas genom att definiera VG_BETYG eller G_BETYG\n"
        "Change the includes of Vector.hpp and VectIter.hpp to your files\n\n"
        ;*/
    std::cout << "Selected is " <<
#ifdef VG_BETYG
        "VG"
#else
#ifdef G_BETYG
        "G"
#else
        "????"
#endif
#endif
        << "\n\n";

    TestBasic();
    TestVector();
    TestIter();

    //TestIterInAlg();
    //TestRevIterInAlg();

//    Dalloc<double>().allocate(1); cout << "\n\ndet finns en minnesl�cka i main, avsiktligt!\n s� ni kan se att er minnesl�ckstest fungerar\n";
    //std::cin.get();
}
