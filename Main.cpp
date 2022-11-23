#include <iostream>
#include "ISCAS.h"
#include "ROBDD.h"


int main() {

    ISCAS iscas(stdin);
    ROBDD robdd;

    robdd.drawGraph(iscas);

    return 0;
}
