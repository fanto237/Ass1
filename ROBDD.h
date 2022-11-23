#ifndef ASS1_ROBDD_H
#define ASS1_ROBDD_H

#include "Func.h"
#include "Triple.h"
#include "FuncTriple.h"
#include "ISCAS.h"
#include <map>
#include <set>
#include <fstream>

class ROBDD {

public:

    // Constructors
    explicit ROBDD();

    ROBDD(const ROBDD &crArg) = delete;

    ROBDD &operator=(const ROBDD &crArg) = delete;

    ~ROBDD();

    // Methods
    const Func &genTrue() const;

    const Func &genFalse() const;

    const Func &genVar(const std::string &name);

    void drawGraph(ISCAS iscas);

    // bool-Operators
    const Func &AND(const Func &f, const Func &g);

    const Func &NAND(const Func &f, const Func &g);

    const Func &OR(const Func &f, const Func &g);

    const Func &XOR(const Func &f, const Func &g);

    const Func &NOR(const Func &f, const Func &g);

    const Func &NOT(const Func &f);

    const Func &ite(const Func &i, const Func &t, const Func &e);

private:

    // Methods
    void readFunctions(ISCAS iscas);

    void draw(ISCAS iscas);

    // Members
    const Func *m_cTrue;
    const Func *m_cFalse;
    std::map<Triple, const Func *> m_Unique;
    std::map<FuncTriple, const Func *> m_Computed;
    std::map<std::string, unsigned> m_UniqueKeys;
    std::map<unsigned, std::string> m_Labels;
    std::map<std::string, const Func *> m_Funcs;
    std::set<const Func *> m_FuncsSet;
    std::fstream m_GraphFile;
    unsigned m_counter;
};

#endif //ASS1_ROBDD_H
