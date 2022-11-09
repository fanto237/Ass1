#include "ROBDD.h"
#include <algorithm>
#include <map>


ROBDD::ROBDD() : m_cTrue(new Func(true)), m_cFalse(new Func(false)) {}

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
        FuncTriple obj(i, t, e);
        auto resComputed = m_Computed.find(obj);
        if(resComputed != m_Computed.end()){
            return *resComputed->second;
        }
        unsigned ciVar = std::min( std::min(i.getVar(), t.getVar()), e.getVar() );
        const Func& T = ite(i.getThen(ciVar), t.getThen(ciVar), e.getThen(ciVar));
        const Func& E = ite(i.getElse(ciVar), t.getElse(ciVar), e.getElse(ciVar));
        if(&T == &E){
            return T;
        }
        Triple entry = Triple(ciVar, T, E);
        auto resUnique = m_Unique.find(entry);
        if(resUnique != m_Unique.end() ){
            resUnique = m_Unique.insert({entry, new Func(ciVar, T, E)}).first;
        }
        Func* pTmp = resUnique->second;
        m_Computed.insert({obj, pTmp}); // TODO send file to Herr Kelb for further analyse
        return *resUnique->second;
    }
}

Func& ROBDD::genVar(unsigned i){
    Triple entry = Triple(i, genTrue(), genFalse());
    auto res = m_Unique.find(entry);
    if(!res){
        res = new Func(i, genTrue(), genFalse());
        m_Unique.insert(entry, res);
    }
    return *res;
}

//const Func &ROBDD::AND(const Func &f, const Func &g) {
//    return <#initializer#>;
//}
//
//const Func &ROBDD::NAND(const Func &f, const Func &g) {
//    return <#initializer#>;
//}
//
//const Func &ROBDD::OR(const Func &f, const Func &g) {
//    return <#initializer#>;
//}
//
//const Func &ROBDD::XOR(const Func &f, const Func &g) {
//    return <#initializer#>;
//}
//
//const Func &ROBDD::NOR(const Func &f, const Func &g) {
//    return <#initializer#>;
//}
//
//const Func &ROBDD::NOT(const Func &f) {
//    return <#initializer#>;
//}
