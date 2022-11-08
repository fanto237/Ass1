#include "ROBDD.h"
#include <algorithm>
#include <map>


ROBDD::ROBDD() : m_cTrue(new Func(true)), m_cFalse(new Func(false)), m_Unique() {}

const Func &ROBDD::genTrue() const{
    return *m_cTrue;
}

const Func &ROBDD::genFalse() const {
    return *m_cFalse;
}

const Func &ROBDD::ite(const Func &i, const Func &t, const Func &e) {
    if(i.isTrue())
        return t;
    else if(i.isFalse())
        return e;
    else if(t.isTrue() && e.isFalse())
        return i;
    else{
        unsigned ciVar = std::min( std::min(i.getVar(), t.getVar()), e.getVar() );
        const Func& T = ite(*i.getThen(ciVar), *t.getThen(ciVar), *e.getThen(ciVar));
        const Func& E = ite(*i.getElse(ciVar), *t.getElse(ciVar), *e.getElse(ciVar));
        if(T == E){
            return T;
        }
        Triple entry = Triple(ciVar, T, E);
        auto res = m_Unique.find(entry);
        if(!res){
            res = new Func(ciVar, T, E);
            m_Unique.insert(entry, res);
        }
        return *res;

        Func* tmp =  new Func(ciVar, T, E);
        return *tmp;
    }
}

Func& ROBDD::genVar(unsigned i){
    Triple* entry = new Triple(i, genTrue(), genFalse());
    auto res = m_Unique.find(entry);
    if(!res){
        res = new Func(i, genTrue(), genFalse());
        m_Unique.insert(entry, res);
    }
    return *res;
}