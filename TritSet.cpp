#include "TritSet.h"
#include <cmath>
#include <iostream>

TernaryLogic::TritSet::TritSet() {
    initialSize_ = 0;
    currentSize_ = 0;
    set_.resize(initialSize_, static_cast<unsigned int>(TernaryLogic::Trit::Unknown));
}

TernaryLogic::TritSet::TritSet(std::size_t tritsNumber) {
    auto setElementSize = ceil((double) tritsNumber * TRIT_BIT_SIZE / BITS_IN_ONE_BYTE_NUMBER / sizeof(unsigned int));
    initialSize_ = tritsNumber;
    tritValuesCountsMap_[Unknown] = tritsNumber;
    currentSize_ = initialSize_;
    set_.resize(setElementSize, static_cast<unsigned int>(TernaryLogic::Trit::Unknown));
}

TernaryLogic::Trit TernaryLogic::TritSet::getTrit(const unsigned int tritPosition) const {
    size_t elementSetIndex = tritPosition * TRIT_BIT_SIZE / (sizeof(unsigned int) * BITS_IN_ONE_BYTE_NUMBER);
    size_t initialElementTritBit = tritPosition * TRIT_BIT_SIZE % (sizeof(unsigned int) * BITS_IN_ONE_BYTE_NUMBER);
    unsigned int offset = (TRITS_IN_ONE_UINT_NUMBER - 1) * TRIT_BIT_SIZE - initialElementTritBit;

    unsigned int tritValue = (set_[elementSetIndex] >> offset) & (unsigned int) 3;

    try {
        return CODE_TRIT_VALUE_MAP.at(tritValue);
    } catch (std::exception &exception) {
        std::cerr << "There is an attempt to get an non-existent trit code.";
        exit(CRASH_PROGRAM_CODE);
    }
}

void TernaryLogic::TritSet::setTrit(const TernaryLogic::Trit tritValue, const unsigned int tritPosition) {
    --tritValuesCountsMap_[getTrit(tritPosition)];

    size_t elementIndex = tritPosition * TRIT_BIT_SIZE / (sizeof(unsigned int) * BITS_IN_ONE_BYTE_NUMBER);
    size_t initialElementTritBit = tritPosition * TRIT_BIT_SIZE % (sizeof(unsigned int) * BITS_IN_ONE_BYTE_NUMBER);
    unsigned int offset = (TRITS_IN_ONE_UINT_NUMBER - 1) * TRIT_BIT_SIZE - initialElementTritBit;

    unsigned int bitMask = ~((unsigned int) 3 << offset);
    set_[elementIndex] &= bitMask;
    set_[elementIndex] |= ((unsigned int) tritValue << offset);

    ++tritValuesCountsMap_[getTrit(tritPosition)];
}

std::pair<TernaryLogic::TritSet, TernaryLogic::TritSet>
TernaryLogic::TritSet::createOperands(const TernaryLogic::TritSet &setA, const TernaryLogic::TritSet &setB) {
    size_t maxSetSize = setA.getSize() > setB.getSize() ? setA.getSize() : setB.getSize();
    TritSet extendedMinSet(maxSetSize);

    if (maxSetSize == setA.getSize()) {
        for (int i = 0; i < setB.getSize(); i++) {
            extendedMinSet[i] = setB.getTrit(i);
        }
        return std::make_pair(setA, extendedMinSet);
    } else {
        for (int i = 0; i < setA.getSize(); i++) {
            extendedMinSet[i] = setA.getTrit(i);
        }
        return std::make_pair(setB, extendedMinSet);
    }
}

void TernaryLogic::TritSet::resetTritValuesCountsMapData() {
    tritValuesCountsMap_[True] = NO_TRITS_AMOUNT;
    tritValuesCountsMap_[False] = NO_TRITS_AMOUNT;
    tritValuesCountsMap_[Unknown] = initialSize_;
}

size_t TernaryLogic::TritSet::getSize() const {
    return currentSize_;
}

size_t TernaryLogic::TritSet::capacity() const {
    return (set_.capacity() * TRITS_IN_ONE_UINT_NUMBER);
}

void TernaryLogic::TritSet::shrink() {
    int lastSetTritIndex = length() - 1;

    if (lastSetTritIndex >= 0) {
        size_t increaseUnknownTritsCount = this->getSize() - (lastSetTritIndex + 1);
        tritValuesCountsMap_[Unknown] -= increaseUnknownTritsCount;
        auto newSetElementSize = ceil((double) lastSetTritIndex * TRIT_BIT_SIZE
                                      / (sizeof(unsigned int) * BITS_IN_ONE_BYTE_NUMBER));
        set_.resize(newSetElementSize);
        currentSize_ = lastSetTritIndex + 1;
    } else {
        size_t initialSetElementSize = ceil((double) initialSize_ * TRIT_BIT_SIZE
                                            / BITS_IN_ONE_BYTE_NUMBER / sizeof(unsigned int));
        set_.resize(initialSetElementSize);
        currentSize_ = initialSize_;
        resetTritValuesCountsMapData();
    }

    set_.shrink_to_fit();
}

size_t TernaryLogic::TritSet::cardinality(const TernaryLogic::Trit tritValue) const {
    switch (tritValue) {
        case True:
            return tritValuesCountsMap_.at(True);
        case False:
            return tritValuesCountsMap_.at(False);
        default:
            return tritValuesCountsMap_.at(Unknown);
    }
}

std::unordered_map<TernaryLogic::Trit, size_t, std::hash<int>> TernaryLogic::TritSet::cardinality() const {
    return tritValuesCountsMap_;
}


void TernaryLogic::TritSet::trim(const size_t lastTritIndex) {
    if (lastTritIndex >= currentSize_) {
        return;
    }

    size_t newCurrentSize = lastTritIndex + 1;
    size_t newSetElementSize = ceil(
            (double) newCurrentSize * TRIT_BIT_SIZE / (sizeof(unsigned int) * BITS_IN_ONE_BYTE_NUMBER));
    size_t newSetTritCapacity = newSetElementSize * TRITS_IN_ONE_UINT_NUMBER;
    for (size_t currentSetTritIndex = newCurrentSize; currentSetTritIndex < newSetTritCapacity; currentSetTritIndex++) {
        this->operator[](currentSetTritIndex) = Unknown;
    }
    currentSize_ = newCurrentSize;
    set_.resize(newSetElementSize);
    set_.shrink_to_fit();
}

size_t TernaryLogic::TritSet::length() const {
    int lastSetTritIndex = -1;
    for (size_t currentSetTritIndex = 0; currentSetTritIndex < currentSize_; currentSetTritIndex++) {
        auto currentTritValue = getTrit(currentSetTritIndex);
        if (currentTritValue != Unknown) {
            lastSetTritIndex = currentSetTritIndex;
        }
    }
    return (lastSetTritIndex + 1);
}

TernaryLogic::TritSet::Reference TernaryLogic::TritSet::operator[](const unsigned int tritIndex) {
    return Reference(*this, tritIndex);
}

TernaryLogic::TritSet TernaryLogic::TritSet::operator~()  {
    TritSet resultSet(getSize());

    for (size_t curTritIndex = 0; curTritIndex < resultSet.getSize(); curTritIndex++) {
        resultSet[curTritIndex] = ~getTrit(curTritIndex);
    }

    return resultSet;
}

TernaryLogic::TritSet TernaryLogic::TritSet::operator|(const TernaryLogic::TritSet &anotherSet) const {
    size_t resultSetSize = this->getSize() > anotherSet.getSize() ? this->getSize() : anotherSet.getSize();
    TritSet resultSet(resultSetSize);
    auto operandsPair = createOperands(*this, anotherSet);

    for (size_t curTritIndex = 0; curTritIndex < resultSet.getSize(); curTritIndex++) {
        resultSet[curTritIndex] =
                operandsPair.first.getTrit(curTritIndex) | operandsPair.second.getTrit(curTritIndex);
    }

    return resultSet;
}

TernaryLogic::TritSet TernaryLogic::TritSet::operator&(const TernaryLogic::TritSet &anotherSet) const {
    size_t resultSetSize = this->getSize() > anotherSet.getSize() ? this->getSize() : anotherSet.getSize();
    TritSet resultSet(resultSetSize);
    auto operandsPair = createOperands(*this, anotherSet);

    for (size_t curTritIndex = 0; curTritIndex < resultSet.getSize(); curTritIndex++) {
        resultSet[curTritIndex] =
                operandsPair.first.getTrit(curTritIndex) & operandsPair.second.getTrit(curTritIndex);
    }

    return resultSet;
}

std::ostream &TernaryLogic::TritSet::operator<<(std::ostream &outputStream) const {
    outputStream << "Trit_index : Trit_value" << std::endl;
    for (size_t curTritIndex = 0; curTritIndex < this->getSize(); curTritIndex++) {
        outputStream << "         " << curTritIndex << " : " << this->getTrit(curTritIndex) << std::endl;
    }

    return outputStream;
}

TernaryLogic::TritSet::Reference &TernaryLogic::TritSet::Reference::operator=(const TernaryLogic::Trit tritValue) {
    if (indexReference_ < setReference_.getSize()) {
        setReference_.setTrit(tritValue, indexReference_);
    } else if (tritValue != Unknown) {
        size_t increaseUnknownTritsCount = (indexReference_ + 1) - setReference_.getSize();
        setReference_.tritValuesCountsMap_[Unknown] += increaseUnknownTritsCount;
        setReference_.currentSize_ = indexReference_ + 1;
        auto newSetElementSize = ceil(((double) setReference_.getSize()) * TRIT_BIT_SIZE
                                      / BITS_IN_ONE_BYTE_NUMBER / sizeof(unsigned int));
        setReference_.set_.resize(newSetElementSize);
        setReference_.setTrit(tritValue, indexReference_);
    }

    return *this;
}

TernaryLogic::TritSet::Reference &
TernaryLogic::TritSet::Reference::operator=(const TernaryLogic::TritSet::Reference &reference) {
    auto newTritValue = reference.setReference_.getTrit(reference.indexReference_);
    setReference_.setTrit(newTritValue, indexReference_);
    return *this;
}

TernaryLogic::TritSet::Reference::operator TernaryLogic::Trit() {
    return setReference_.getTrit(indexReference_);
}

