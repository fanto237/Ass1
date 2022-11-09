//
// Created by lucie on 09/11/2022.
//

#ifndef ASS1_FUNCTRIPLE_H
#define ASS1_FUNCTRIPLE_H


#include "Func.h"

class FuncTriple {
public:
    FuncTriple(const Func& f, const Func &g, const Func& h);
    FuncTriple(const FuncTriple& crArg) = delete;
    const FuncTriple& operator=(const FuncTriple& crArg) = delete;

    // operators
    friend bool operator<(const FuncTriple& f1, const FuncTriple& f2);

};


#endif //ASS1_FUNCTRIPLE_H
