#ifndef ASS1_FUNC_H
#define ASS1_FUNC_H

#include <iostream>


class Func {

public:
    // constructors
    explicit Func(bool b);

    Func(unsigned iVagr, const Func &t, const Func &e);

    Func(const Func &crArg) = delete;

    Func &operator=(const Func &crArg) = delete;

    // methods
    const Func *getThen(unsigned iVar) const;

    const Func *getElse(unsigned iVar) const;

    unsigned getVar() const;

    bool isTrue() const;

    bool isFalse() const;

    bool isConstant() const;

    // operators
    friend std::ostream &operator<<(std::ostream &os, const Func &crArg);

    friend bool operator<(const Func &crArg1, const Func &crArg2);

private:
    // members
    static const unsigned TRUE = ~0; //0xffffffff
    static const unsigned FALSE = TRUE - 1;
    unsigned m_ciVar;
    const Func *m_cThen = nullptr;
    const Func *m_cElse = nullptr;
};

#endif //ASS1_FUNC_H
