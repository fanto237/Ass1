//
// Created by lucie on 09/11/2022.
//

#ifndef ASS1_FUNCTRIPLE_H
#define ASS1_FUNCTRIPLE_H

#include "Func.h"

class FuncTriple {
public:
    // constructors
    FuncTriple(const Func &iVar, const Func &fThen, const Func &fElse);

    // operators
    friend bool operator<(const FuncTriple &crArg1, const FuncTriple &crArg2);

    // members
private:
    const Func &m_ciVar;
    const Func &m_cThen;
    const Func &m_cElse;
};


#endif //ASS1_FUNCTRIPLE_H
