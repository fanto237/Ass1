//
// Created by Study on 08/11/2022.
//

#ifndef ASS1_ROBDD_H
#define ASS1_ROBDD_H
#include "Func.h"
#include "Triple.h"
#include <map>

class ROBDD{

public:

    // Constructors
    explicit ROBDD();
    ROBDD(const ROBDD& crArg) = delete;
    ROBDD& operator=(const ROBDD& crArg) = delete;

    // Methods
    const Func& genTrue() const;
    const Func& genFalse() const;
    Func& genVar(unsigned i);
    const Func& ite(const Func& i, const Func& t, const Func& e);

private:
    // Members
    const Func* m_cTrue = new Func(true);
    const Func* m_cFalse = new Func(false);
    std::map<Triple, Func> m_Unique;
};
#endif //ASS1_ROBDD_H
