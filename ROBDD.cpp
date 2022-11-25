#include "ROBDD.h"
#include <algorithm>
#include <map>
#include <cassert>


ROBDD::ROBDD() : m_cTrue(new Func(true)), m_cFalse(new Func(false)), m_Unique(), m_Computed(), m_UniqueKeys(),
                 m_Labels(),
                 m_Funcs(), m_FuncsSet(), m_GraphFile(), m_counter(0) {}


ROBDD::~ROBDD() {
    delete m_cTrue;
    delete m_cFalse;

    for (auto &iter: m_Unique) {
        delete iter.second;
    }
}

const Func &ROBDD::genTrue() const {
    return *m_cTrue;
}

const Func &ROBDD::genFalse() const {
    return *m_cFalse;
}

const Func &ROBDD::genVar(const std::string &name) {
    auto tmp = m_UniqueKeys.find(name);

    if (tmp == m_UniqueKeys.end()) {
        tmp = m_UniqueKeys.insert({name, ++m_counter}).first;
    }
    unsigned index = tmp->second;
    Triple entry = Triple(index, genTrue(), genFalse());
    auto resUnique = m_Unique.find(entry);
    if (resUnique == m_Unique.end()) {
        resUnique = m_Unique.insert({entry, new Func(index, genTrue(), genFalse())}).first;
        m_Unique.insert({entry, resUnique->second});
    }
    return *resUnique->second;
}

const Func &ROBDD::ite(const Func &i, const Func &t, const Func &e) {
    if (i.isTrue())
        return t;
    else if (i.isFalse())
        return e;
    else if (t.isTrue() && e.isFalse())
        return i;
    else {
        FuncTriple obj(i, t, e);
        auto resComputed = m_Computed.find(obj);
        if (resComputed != m_Computed.end()) {
            return *resComputed->second;
        }
        unsigned ciVar = std::min(std::min(i.getVar(), t.getVar()), e.getVar());
        const Func &T = ite(*i.getThen(ciVar), *t.getThen(ciVar), *e.getThen(ciVar));
        const Func &E = ite(*i.getElse(ciVar), *t.getElse(ciVar), *e.getElse(ciVar));
        if (&T == &E) {
            m_Computed.insert({obj, &T});
            return T;
        }
        Triple entry = Triple(ciVar, T, E);
        auto resUnique = m_Unique.find(entry);
        if (resUnique == m_Unique.end()) {
            resUnique = m_Unique.insert({entry, new Func(ciVar, T, E)}).first;
        }
        m_Computed.insert({obj, resUnique->second});
        return *resUnique->second;
    }
}


const Func &ROBDD::AND(const Func &f, const Func &g) {
    return ite(f, g, genFalse());
}

const Func &ROBDD::NAND(const Func &f, const Func &g) {
    return NOT(AND(f, g));
}

const Func &ROBDD::OR(const Func &f, const Func &g) {
    return ite(f, genTrue(), g);
}

const Func &ROBDD::XOR(const Func &f, const Func &g) {
    return ite(f, NOT(g), g);
}

const Func &ROBDD::NOR(const Func &f, const Func &g) {
    return NOT(OR(f, g));
}

const Func &ROBDD::NOT(const Func &f) {
    return ite(f, genFalse(), genTrue());
}

void ROBDD::drawGraph(ISCAS iscas) {
    if (iscas.isValid()) {
        readFunctions(iscas);
    } else {
        std::cout << "iscas file not valid" << std::endl;
    }
}

void ROBDD::readFunctions(ISCAS iscas) {

    for (auto &input: iscas.inputs()) {
        auto &func = genVar(input);
        m_Funcs.insert({input, &func});
        m_Labels.insert({func.getVar(), input});
    }

    for (auto &assignment: iscas.assignments()) {
        auto params = std::get<2>(assignment);
        auto op = std::get<1>(assignment);
        const Func *func = m_Funcs.find(*params.begin())->second;

        for (auto i = params.begin() + 1; i != params.end(); ++i) {
            switch (op) {
                case ISCAS::AND:
                case ISCAS::NAND:
                    func = &AND(*func, *m_Funcs.find(*i)->second);
                    break;

                case ISCAS::OR:
                case ISCAS::NOR:
                    func = &OR(*func, *m_Funcs.find(*i)->second);
                    break;

                case ISCAS::XOR:
                    func = &XOR(*func, *m_Funcs.find(*i)->second);
                    break;

                default:
                    assert(false);
                    break;
            }
        }
        if (op == ISCAS::NAND || op == ISCAS::NOR || op == ISCAS::NOT) {
            func = &NOT(*func);
        }
        m_Funcs.insert({std::get<0>(assignment), func});
    }

//    std::cout << "All functions inserted" << std::endl;
    std::cout << "digraph {" << std::endl;
    std::cout << "subgraph cluster_0{" << std::endl;
    std::cout << "style=invis" << std::endl;

    for (auto &out: iscas.outputs()) {
        std::cout << out << " [shape=plaintext]" << std::endl;
    }
    std::cout << "}" << std::endl;
    draw(iscas);
}

void ROBDD::draw(ISCAS iscas) {
    std::vector<const Func *> funcs;
    for (auto &outpout: iscas.outputs()) {
        auto iter = m_Funcs.find(outpout);

        std::cout << "\"" << iter->first << "\"" << "->" << "\"" << iter->second << "\"" << std::endl;
        funcs.push_back(iter->second);

        while (!funcs.empty()) {
            auto func = funcs.back();
            auto res = m_FuncsSet.find(func);
            if (res == m_FuncsSet.end()) {
                // func not visited yet
                m_FuncsSet.insert(func);
                if (func->isConstant()) {
                    if (func->isTrue()) {
                        //1
                        std::cout << "\"" << func << "\"" << " [shape=box, label= 1]" << std::endl;
                    } else if (func->isFalse()) {
                        // 0
                        std::cout << "\"" << func << "\"" << " [shape=box, label= 0]" << std::endl;
                    }
                    funcs.pop_back();
                    continue;
                } else {

                    unsigned ivar = func->getVar();
                    auto resLabel = m_Labels.find(ivar);
                    assert(resLabel != m_Labels.end());
                    std::cout << "\"" << func << "\"" << " [label=" << resLabel->second << "]" << std::endl;
                    std::cout << *func << std::endl;
                    funcs.push_back(func->getElse(ivar));
                    funcs.push_back(func->getThen(ivar));
                    continue;
                }
            }
            funcs.pop_back();
        }
    }
    std::cout << "}" << std::endl;
}

