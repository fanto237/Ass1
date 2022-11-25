//
// Created by Study on 08/11/2022.
//

#include "Triple.h"

Triple::Triple(unsigned int iVar, const Func &fThen, const Func &fElse) : m_ciVar(iVar), m_cThen(fThen),
                                                                          m_cElse(fElse) {}

bool operator<(const Triple &crArg1, const Triple &crArg2) {
    return crArg1.m_ciVar < crArg2.m_ciVar ||
           (crArg1.m_ciVar == crArg2.m_ciVar && &crArg1.m_cThen < &crArg2.m_cThen) ||
           (crArg1.m_ciVar == crArg2.m_ciVar && &crArg1.m_cThen == &crArg2.m_cThen &&
            &crArg1.m_cElse < &crArg2.m_cElse);
}
