#ifndef TRITSET1_TRIT_H
#define TRITSET1_TRIT_H

namespace TernaryLogic {
    enum Trit {
        Unknown = 0,
        True = 1,
        False = 2
    };

    TernaryLogic::Trit operator~(TernaryLogic::Trit tritA);
    TernaryLogic::Trit operator|(TernaryLogic::Trit tritA, TernaryLogic::Trit tritB);
    TernaryLogic::Trit operator&(TernaryLogic::Trit tritA, TernaryLogic::Trit tritB);
}

#endif //TRITSET1_TRIT_H
