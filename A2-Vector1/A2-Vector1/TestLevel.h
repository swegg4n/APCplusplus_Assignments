#pragma once

#define LEVEL 99
#define G
#define VG

//Tanken �r att man h�jer Level successivt till 12
//Sen kan man s�tta level till 99
//    och definiera G s� k�r den riktiga testen ig�ng.
//Observera att LEVEL testerna �r mycket enkla
//det �r f�rst G som blir en ordentlig test.

/* S� h�r �r niv�erna man kan s�tta p� LEVEL
#if false
level 1
Vector();
size()
Invariant();

level 2
Vector(char*);
operator<<
operator==

level 3
operator<, >= ,,, allihop

level 4
    Vector(const Vector&);
    operator=

Level 5
operator[]
at

level 6
push_back

level 7
Move Constructor
Move Assignment

Level 8:
reserve
shrink_to_fit
resize
swap

level 10
VectorIter(const Link* node = nullptr)
begin()
end()
operator==
operator!=

Level 11
VectorIter(const VectorIter&)
VectorIter& operator=(const VectorIter&)

operator*()
operator->()

level 12
operator++
operator++(int)

operator--
operator--(int)

#endif

Observera att reverse_iterator inte testas i Basic Test!

*/