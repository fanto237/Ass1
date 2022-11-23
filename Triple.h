//
// Created by Study on 08/11/2022.
//

#ifndef ASS1_TRIPLE_H
#define ASS1_TRIPLE_H


#include "Func.h"

class Triple {

public:

    // constructors
    Triple(unsigned iVar, const Func &fThen, const Func &fElse);

    // operators
    friend bool operator<(const Triple &crArg1, const Triple &crArg2);

private:
    // members
    const unsigned m_ciVar;
    const Func &m_cThen;
    const Func &m_cElse;
};

#endif //ASS1_TRIPLE_H
