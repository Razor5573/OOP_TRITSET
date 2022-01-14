#ifndef TRITSET1_TRITSET_H
#define TRITSET1_TRITSET_H
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include "Trit.h"

namespace TernaryLogic {
    static const unsigned int TRIT_BIT_SIZE = 2;
    static const unsigned int BITS_IN_ONE_BYTE_NUMBER = 8;
    static const unsigned int TRITS_IN_ONE_UINT_NUMBER = 16;

    static const unsigned int NO_TRITS_AMOUNT = 0;

    static const unsigned int CRASH_PROGRAM_CODE = 0;

    static const std::map<unsigned int, TernaryLogic::Trit> CODE_TRIT_VALUE_MAP {
            {static_cast<size_t> (Trit::Unknown), Trit::Unknown},
            {static_cast<size_t> (Trit::True), Trit::True},
            {static_cast<size_t> (Trit::False), Trit::False}
    };


    class TritSet {
    private:
        std::vector<unsigned int> set_;

        std::size_t initialSize_;
        std::size_t currentSize_;

        std::unordered_map<TernaryLogic::Trit, size_t, std::hash<int>> tritValuesCountsMap_{
                {Trit::False,   NO_TRITS_AMOUNT},
                {Trit::True,    NO_TRITS_AMOUNT},
                {Trit::Unknown, NO_TRITS_AMOUNT}
        };

        [[nodiscard]] TernaryLogic::Trit getTrit(unsigned int tritPosition) const;

        void setTrit(TernaryLogic::Trit tritValue, unsigned int tritPosition);

        static std::pair<TritSet, TritSet> createOperands(const TritSet &setA, const TritSet &setB);

        void resetTritValuesCountsMapData();

    public:
        class Reference {
        private:
            TritSet &setReference_;
            unsigned int indexReference_;
        public:
            friend class TritSet;

            Reference(TritSet &setReference, unsigned int indexReference) :
                    setReference_(setReference), indexReference_(indexReference) {};

            Reference &operator=(TernaryLogic::Trit tritValue);

            Reference &operator=(const Reference &reference);

            explicit operator TernaryLogic::Trit();
        };

        TritSet();

        explicit TritSet(size_t tritsNumber);

        ~TritSet() = default;

        size_t getSize() const;

        size_t capacity() const;

        void shrink();

        [[nodiscard]] size_t cardinality(TernaryLogic::Trit tritValue) const;

        [[nodiscard]] std::unordered_map<TernaryLogic::Trit, size_t, std::hash<int>> cardinality() const;

        void trim(size_t lastTritIndex);

        size_t length() const;

        Reference operator[](unsigned int tritIndex);

        TritSet operator~();

        TritSet operator|(const TritSet &anotherSet) const;

        TritSet operator&(const TritSet &anotherSet) const;

        std::ostream &operator<<(std::ostream &outputStream) const;
    };


}

#endif //TRITSET1_TRITSET_H
