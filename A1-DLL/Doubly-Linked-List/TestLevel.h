#pragma once

#define LEVEL 99
#define VG
//#define Additional_Splice_Tests


//Tanken �r att man h�jer Level successivt till 12
//Sen kan man s�tta level till 99
//    och definiera G s� k�r den riktiga testen ig�ng.
//Observera att LEVEL testerna �r mycket enkla
//det �r f�rst G som blir en ordentlig test.

/* S� h�r �r niv�erna man kan s�tta p� LEVEL
#if false
level 1
List();
Count()
Invariant();

level 2
List(char*);
Print()
operator==

level 3
operator<, >= ,,, allihop

level 4
    List(const List&);

Level 5
empty()
front(och const)
back(och const)

level 6
push_back
pop_back

level 7
push_front
pop_front

level 10
ListIter(const Link* node = nullptr)
begin()
end()
operator==
operator!=

Level 11
ListIter(const ListIter&)
ListIter& operator=(const ListIter&)

operator*()
operator->()

level 12
operator++
operator++(int)

operator--
operator--(int)

#endif
*/