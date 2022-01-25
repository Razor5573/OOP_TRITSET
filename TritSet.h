#ifndef TRITSET1_TRITSET_H
#define TRITSET1_TRITSET_H
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include "Trit.h"
#include <memory>

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


    class ITritSet{
    private:
        virtual TernaryLogic::Trit getTrit(unsigned int tritPosition) const = 0;
        virtual void setTrit(TernaryLogic::Trit tritValue, unsigned int tritPosition) = 0;
        friend class TritSet;
    };

    class FakeTritSet : public ITritSet{
    public:
        explicit FakeTritSet(TernaryLogic::Trit trit);
    private:
        FakeTritSet() = default;
        TernaryLogic::Trit trit_;
        Trit getTrit(unsigned int tritPosition) const override;
        void setTrit(TernaryLogic::Trit tritValue, unsigned int tritPosition) override;
    };


    class TritSet : public ITritSet{
    private:
        std::vector<unsigned int> set_;


        std::size_t initialSize_;
        std::size_t currentSize_;

        std::unordered_map<TernaryLogic::Trit, size_t, std::hash<int>> tritValuesCountsMap_{
                {Trit::False,   NO_TRITS_AMOUNT},
                {Trit::True,    NO_TRITS_AMOUNT},
                {Trit::Unknown, NO_TRITS_AMOUNT}
        };

        [[nodiscard]] TernaryLogic::Trit getTrit(unsigned int tritPosition) const override;

        void setTrit(TernaryLogic::Trit tritValue, unsigned int tritPosition) override;
        void adjustSetSizeAndRecountTrits(const TernaryLogic::Trit tritValue, const unsigned int tritPosition);


        static std::pair<TritSet, TritSet> createOperands(const TritSet &setA, const TritSet &setB);

        void resetTritValuesCountsMapData();

    public:
        class Reference {
        private:
            ITritSet *setReference_;
            unsigned int indexReference_;
            bool isFakeRef;
        public:
            friend class TritSet;

            Reference(ITritSet *setReference, unsigned int indexReference) :
                    setReference_(setReference), indexReference_(indexReference) {
                isFakeRef = false;
            };
            Reference(const Reference& anotherRef);

            Reference &operator=(TernaryLogic::Trit tritValue);

            Reference &operator=(const Reference &reference);

            explicit operator TernaryLogic::Trit();

            ~Reference();
        };

        class Iterator{
        public:
            Iterator(const Iterator& it);

            Reference operator++(int);
            Reference operator++();
            Reference operator--(int);
            Reference operator--();

            Iterator& operator=(const Iterator& it);
            bool operator!= (const Iterator& it) const;
            bool operator== (const Iterator& it) const;
            Reference& operator* ();

        private:
            friend TritSet;
            Iterator(TritSet* set, size_t index);
            TritSet* set_{};
            size_t index_{};
        };
    private:
        std::vector<std::shared_ptr<Reference>> memory_;
    public:


        TritSet();

        TritSet(size_t tritsNumber);

        TritSet(const TritSet& anotherSet);

        ~TritSet() = default;

        size_t getSize() const;

        size_t capacity() const;

        void shrink();

        [[nodiscard]] size_t cardinality(TernaryLogic::Trit tritValue) const;

        [[nodiscard]] std::unordered_map<TernaryLogic::Trit, size_t, std::hash<int>> cardinality() const;

        void trim(size_t lastTritIndex);

        size_t length() const;

        Reference& operator[](unsigned int tritIndex);
        Trit operator[](unsigned int tritIndex) const;

        TritSet operator~();

        TritSet operator|(const TritSet &anotherSet) const;

        TritSet operator&(const TritSet &anotherSet) const;

        std::ostream &operator<<(std::ostream &outputStream) const;

        Iterator begin();

        Iterator end();
    };


}

#endif //TRITSET1_TRITSET_H
