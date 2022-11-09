#ifndef ASS1_ROBDD_H
#define ASS1_ROBDD_H
#include "Func.h"
#include "Triple.h"
#include "FuncTriple.h"
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

    // bool-Operators
    const Func& AND(const Func& f, const Func& g);
    const Func& NAND(const Func& f, const Func& g);
    const Func& OR(const Func& f, const Func&g);
    const Func& XOR(const Func& f, const Func&g);
    const Func& NOR(const Func& f, const Func&g);
    const Func& NOT(const Func& f);
    const Func& ite(const Func& i, const Func& t, const Func& e);

private:
    // Members
    const Func* m_cTrue = new Func(true);
    const Func* m_cFalse = new Func(false);
    std::map<Triple, Func*> m_Unique;
    std::map<FuncTriple, Func*> m_Computed;

};
#endif //ASS1_ROBDD_H
