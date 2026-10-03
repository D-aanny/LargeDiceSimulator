#include "dicesimulator.h"
#include <random>
#include <vector>
#include <unordered_map>

// TODO: Fully implement customizability for die rolling parameters

int DiceSimulator::rollRandNum(const int dieSize) {
    std::uniform_int_distribution<int> dist(1, dieSize);
    return dist(mt);
}

std::vector<RollResult> DiceSimulator::runSingleTrial(
    const int numd4,
    const int numd6,
    const int numd8,
    const int numd10,
    const int numd12,
    const int numd20) {
    std::vector<RollResult> rolledNums;

    for (int i = 0; i < numd4; i++)
        rolledNums.push_back({4, rollRandNum(4)});

    for (int i = 0; i < numd6; i++)
        rolledNums.push_back({6, rollRandNum(6)});

    for (int i = 0; i < numd8; i++)
        rolledNums.push_back({8, rollRandNum(8)});

    for (int i = 0; i < numd10; i++)
        rolledNums.push_back({10, rollRandNum(10)});

    for (int i = 0; i < numd12; i++)
        rolledNums.push_back({12, rollRandNum(12)});

    for (int i = 0; i < numd20; i++)
        rolledNums.push_back({20, rollRandNum(20)});

    return rolledNums;
}

bool DiceSimulator::checkDuplicateRolls(
    const std::vector<RollResult> &rolls,
    const int numAmountNeeded,
    const int minNum) {
    std::unordered_map<int, int> countMap;

    for (RollResult roll: rolls) {
        if (roll.rolledValue >= minNum) // Used to set current crit system where rolls must be 4 or higher to count for crit
            countMap[roll.rolledValue]++;

        if (countMap[roll.rolledValue] >= numAmountNeeded) {
            return true;
        }
    }

    return false;
}

bool DiceSimulator::checkMaximumRolls(
    const std::vector<RollResult> &rolls,
    const int goalDieSize,
    const int amountNeeded,
    const bool combined) {
    int count = 0;

    for (RollResult roll: rolls) {
        if (combined) {
            if (roll.rolledValue == roll.dieSize)
                count++;
        }
        else {
            if (roll.dieSize == goalDieSize && roll.rolledValue == goalDieSize)
                count++;
        }
        if (count >= amountNeeded)
            return true;
    }

    return false;
}

int DiceSimulator::getMostNumerousDieSize(int numd4, int numd6, int numd8, int numd10, int numd12, int numd20) {
    int mostNumerousCount = 0;
    int mostNumerousDie = 4;

    const std::array<int, 6> counts = {numd4, numd6, numd8, numd10, numd12, numd20};

    const std::array<int, 6> sizes = {4, 6, 8, 10, 12, 20};

    for (int i = 0; i < counts.size(); i++) {
        if (counts[i] > mostNumerousCount) {
            mostNumerousCount = counts[i];
            mostNumerousDie = sizes[i];
        }
    }

    return mostNumerousDie;
}

int DiceSimulator::determineMaximumValue(int numd4, int numd6, int numd8, int numd10, int numd12, int numd20,
                                         SimulationSettings::MaxValueDropdown mode) {
    switch (mode) {
        case SimulationSettings::MaxValueDropdown::D4:
            return 4;

        case SimulationSettings::MaxValueDropdown::D6:
            return 6;

        case SimulationSettings::MaxValueDropdown::D8:
            return 8;

        case SimulationSettings::MaxValueDropdown::D10:
            return 10;

        case SimulationSettings::MaxValueDropdown::D12:
            return 12;

        case SimulationSettings::MaxValueDropdown::D20:
            return 20;

        case SimulationSettings::MaxValueDropdown::HighestDie: {
            if (numd20 > 0) return 20;
            if (numd12 > 0) return 12;
            if (numd10 > 0) return 10;
            if (numd8 > 0) return 8;
            if (numd6 > 0) return 6;
            return 4;
        }

        case SimulationSettings::MaxValueDropdown::MostNumerousDie: {
            return getMostNumerousDieSize(numd4, numd6, numd8, numd10, numd12, numd20);
        }

        default:
            return 0;
    }
}

DiceSimulator::SimulationResult DiceSimulator::runMultipleTrials(
    const int numd4,
    const int numd6,
    const int numd8,
    const int numd10,
    const int numd12,
    const int numd20,
    const int numTrials,
    const SimulationSettings &settings) {
    if (numTrials <= 0)
        return {};

    int sameNumberCount = 0;
    int maximumCount = 0;
    int maxNumGoal = 4;

    if (settings.maxValueDropdown == SimulationSettings::MaxValueDropdown::MostNumerousDie) {
        maxNumGoal = getMostNumerousDieSize(numd4, numd6, numd8, numd10, numd12, numd20);
    } else {
        maxNumGoal = determineMaximumValue(numd4, numd6, numd8, numd10, numd12, numd20, settings.maxValueDropdown);
    }

    for (int i = 0; i < numTrials; i++) {
        const std::vector<RollResult> trial =
                runSingleTrial(
                    numd4,
                    numd6,
                    numd8,
                    numd10,
                    numd12,
                    numd20
                );

        if (settings.checkSameNumber) {
            if (checkDuplicateRolls(trial, settings.sameNumberCount, settings.sameNumberMinimum)) {
                sameNumberCount++;
            }
        }

        if (settings.checkMaximum) {
            const bool combined = (settings.maxValueDropdown == SimulationSettings::MaxValueDropdown::Combined);
            if (checkMaximumRolls(trial, maxNumGoal, settings.maximumCount, combined)) {
                maximumCount++;
            }
        }
    }

    SimulationResult result;

    if (settings.checkSameNumber) {
        result.calculatedDuplicate = true;
        result.duplicatePercentage = static_cast<double>(sameNumberCount) / numTrials * 100.0;
    }

    if (settings.checkMaximum) {
        result.calculatedMaximum = true;
        result.maximumPercentage = static_cast<double>(maximumCount) / numTrials * 100.0;
    }

    return result;
}
