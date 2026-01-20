#include "ChainOfResponsibility.h"

int main() {
    // main 只负责调用职责链模式的两个演示
    RunLogChainDemo();
    RunLeaveApprovalDemo();
    return 0;
}
