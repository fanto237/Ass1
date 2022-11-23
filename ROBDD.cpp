#include "ROBDD.h"
#include <algorithm>
#include <map>
#include <cassert>


ROBDD::ROBDD() : m_cTrue(new Func(true)), m_cFalse(new Func(false)), m_Unique(), m_Computed(), m_UniqueKeys(),
                 m_Labels(),
                 m_Funcs(), m_FuncsSet(), m_GraphFile(), m_counter(0) {}

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
        std::cout << "trying to get the functions... \n";
        readFunctions(iscas);
    } else {
        std::cout << "iscas file not valid" << std::endl;
    }
}

void ROBDD::readFunctions(ISCAS iscas) {

    for (auto &input: iscas.inputs()) {
        std::cout << "trying to get the inputs... \n";
        std::cout << input << std::endl;
        std::cout << "input printed" << std::endl;

        std::cout << *m_Funcs.find(input)->second << std::endl;
        std::cout << m_Funcs.find(input)->first << std::endl;
        auto &func = genVar(input);
        std::cout << func << "func has been printed" << std::endl;

        m_Funcs.insert({input, &func});
        m_Labels.insert({func.getVar(), input});
        std::cout << "inputs loaded... \n";
    }

    for (auto &assignment: iscas.assignments()) {
        auto params = std::get<2>(assignment);
        auto op = std::get<1>(assignment);
        const Func *func = m_Funcs.find(*params.begin())->second;

        std::cout << "assignment found" << std::endl;
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
                    assert(false); // todo double check
                    break;
            }
        }
        if (op == ISCAS::NAND || op == ISCAS::NOR || op == ISCAS::NOT) {
            func = &NOT(*func);
        }
        m_Funcs.insert({std::get<0>(assignment), func});
        std::cout << "Function : " << std::get<0>(assignment) << " inserted: " << std::endl;
    }

    std::cout << "All functions inserted" << std::endl;
    m_GraphFile.open("graph.dot"); // todo muss durch stdout augegben werden
    m_GraphFile << "digraph {" << std::endl;
    m_GraphFile << "subgraph cluster_0{" << std::endl;
    m_GraphFile << "style=invis" << std::endl;

    for (auto &out: iscas.outputs()) {
        m_GraphFile << out << " [shape=plaintext]" << std::endl;
    }
    m_GraphFile << "}" << std::endl;
    draw(iscas);
}

void ROBDD::draw(ISCAS iscas) {
    std::vector<const Func *> funcs;
    for (auto &outpout: iscas.outputs()) {
        auto iter = m_Funcs.find(outpout);
        auto key = iter->second->getVar();

        m_GraphFile << "\"" <<iter->first <<"\"" << "->" << "\""<<iter->second <<"\""<< std::endl;
        m_GraphFile << "\""<<iter->second<<"\""<<" [label=" << m_Labels.find(key)->second << "]" << std::endl;
        m_GraphFile << *iter->second;

        funcs.push_back(iter->second->getThen(key));
        std::cout << "next then " << funcs.back() << std::endl;
        funcs.push_back(iter->second->getElse(key));
        std::cout << "next else " << funcs.back() << std::endl;

        while (!funcs.empty()) {
            auto func = funcs.back();
            auto res = m_FuncsSet.find(func);
            if ( res == m_FuncsSet.end()) {
                std::cout << func << " not visited " << std::endl;
                m_FuncsSet.insert(func);
                std::cout << *res << " inserted " << std::endl;

                if (func->isConstant()) {
                    std::cout << "constant " << std::endl;
                    if (func->isTrue())
                        // 1
                        m_GraphFile << "\"" << func << "\"" << " [shape=box, label= 1]" << std::endl;
                    else if (func->isFalse())

                        // 0
                        m_GraphFile << "\"" << func << "\"" << " [shape=box, label= 0]" << std::endl;
                    funcs.pop_back();
                    std::cout << "next func" << funcs.back() << std::endl;
                } else {
                    unsigned ivar = func->getVar();
                    std::cout << "Var = " << ivar << std::endl;
                    auto resLabel = m_Labels.find(ivar);
                    assert(resLabel != m_Labels.end());
                    m_GraphFile << "\"" << func << "\"" << " [label=" << resLabel->second << "]" << std::endl;
                    m_GraphFile << *func << std::endl;
                    if(func->getThen(ivar) && func->getElse(ivar)){
                        funcs.push_back(func->getThen(ivar));
                        funcs.push_back(func->getElse(ivar));
                    }
                    continue;
                }
            }
            funcs.pop_back();
        }
    }
    m_GraphFile << "}" << std::endl;
    m_GraphFile.close();
}
