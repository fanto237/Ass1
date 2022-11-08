//
// Created by Study on 08/11/2022.
//

#include "Func.h"

Func::Func(bool b) : m_ciVar(b ? TRUE : FALSE), m_cThen(nullptr), m_cElse(nullptr) {}

Func::Func(unsigned iVar, const Func &t, const Func &e) : m_ciVar(iVar), m_cThen(&t), m_cElse(&e) {}


const Func *Func::getThen(unsigned iVar) const {
    return iVar == m_ciVar ? m_cThen : this;
}

const Func *Func::getElse(unsigned iVar) const {
    return iVar == m_ciVar ? m_cElse : this;
}

unsigned Func::getVar() const {
    return m_ciVar;
}

bool Func::isTrue() const {
    return m_ciVar == TRUE;
}

bool Func::isFalse() const {
    return m_ciVar == FALSE;
}

bool Func::isConstant() const {
    return isTrue() || isFalse();
}

bool operator==(const Func &crArg1, const Func &crArg2) {
    return crArg1.m_ciVar == crArg2.m_ciVar && crArg1.m_cThen == crArg2.m_cThen && crArg1.m_cElse == crArg2.m_cElse;
}