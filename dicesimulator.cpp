#include "dicesimulator.h"
#include <random>
#include <vector>
#include <unordered_map>

// TODO: Fully implement customizability for die rolling parameters

int DiceSimulator::rollRandNum(const int dieSize) {
    std::uniform_int_distribution<int> dist(1, dieSize);
    return dist(mt);
}

std::vector<int> DiceSimulator::runSingleTrial(
    const int numd4,
    const int numd6,
    const int numd8,
    const int numd10,
    const int numd12,
    const int numd20) {
    std::vector<int> rolledNums;

    for (int i = 0; i < numd4; i++)
        rolledNums.push_back(rollRandNum(4));

    for (int i = 0; i < numd6; i++)
        rolledNums.push_back(rollRandNum(6));

    for (int i = 0; i < numd8; i++)
        rolledNums.push_back(rollRandNum(8));

    for (int i = 0; i < numd10; i++)
        rolledNums.push_back(rollRandNum(10));

    for (int i = 0; i < numd12; i++)
        rolledNums.push_back(rollRandNum(12));

    for (int i = 0; i < numd20; i++)
        rolledNums.push_back(rollRandNum(20));

    return rolledNums;
}

bool DiceSimulator::checkSameNumberRolled(
    const std::vector<int> &nums,
    int numAmountNeeded,
    int minNum) {
    std::unordered_map<int, int> countMap;

    for (int num: nums) {
        if (num >= minNum) // Used to set current crit system where rolls must be 4 or higher to count for crit
            countMap[num]++;

        if (countMap[num] >= numAmountNeeded) {
            return true;
        }
    }

    return false;
}

bool DiceSimulator::hasDoubleMax(
    const std::vector<int> &nums,
    const int goalMax,
    const int amountNeeded) {
    int count = 0;

    for (int num: nums) {
        if (num == goalMax) {
            count++;

            if (count >= amountNeeded)
                return true;
        }
    }
    return false;
}

int DiceSimulator::getMostNumerousDieSize(int numd4, int numd6, int numd8, int numd10, int numd12, int numd20) {
    int mostNumerousCount = 0;
    int mostNumerousDie = 4;

    std::array<int, 6> counts = {numd4, numd6, numd8, numd10, numd12, numd20};

    std::array<int, 6> sizes = {4, 6, 8, 10, 12, 20};

    for (int i = 0; i < counts.size(); i++) {
        if (counts[i] > mostNumerousCount) {
            mostNumerousCount = counts[i];
            mostNumerousDie = sizes[i];
        }
    }

    return mostNumerousDie;
}

int DiceSimulator::determineMaximumValue(int numd4, int numd6, int numd8, int numd10, int numd12, int numd20,
                                         SimulationSettings::MaximumMode mode) {
    switch (mode) {
        case SimulationSettings::MaximumMode::D4:
            return 4;

        case SimulationSettings::MaximumMode::D6:
            return 6;

        case SimulationSettings::MaximumMode::D8:
            return 8;

        case SimulationSettings::MaximumMode::D10:
            return 10;

        case SimulationSettings::MaximumMode::D12:
            return 12;

        case SimulationSettings::MaximumMode::D20:
            return 20;

        case SimulationSettings::MaximumMode::HighestDie: {
            if (numd20 > 0) return 20;
            if (numd12 > 0) return 12;
            if (numd10 > 0) return 10;
            if (numd8 > 0) return 8;
            if (numd6 > 0) return 6;
            return 4;
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

    if (settings.maximumMode == SimulationSettings::MaximumMode::MostNumerousDie) {
        maxNumGoal = getMostNumerousDieSize(numd4, numd6, numd8, numd10, numd12, numd20);
    } else {
        maxNumGoal = determineMaximumValue(numd4, numd6, numd8, numd10, numd12, numd20, settings.maximumMode);
    }

    for (int i = 0; i < numTrials; i++) {
        const std::vector<int> trial =
                runSingleTrial(
                    numd4,
                    numd6,
                    numd8,
                    numd10,
                    numd12,
                    numd20
                );

        if (settings.checkSameNumber) {
            if (checkSameNumberRolled(trial, settings.sameNumberCount, settings.sameNumberMinimum)) {
                sameNumberCount++;
            }
        }

        if (settings.checkMaximum) {
            if (hasDoubleMax(trial, maxNumGoal, settings.maximumCount)) {
                maximumCount++;
            }
        }
    }

    SimulationResult result;

    if (settings.checkSameNumber) {
        result.calculatedSameNumber = true;
        result.sameNumberPercentage = static_cast<double>(sameNumberCount) / numTrials * 100.0;
    }

    if (settings.checkMaximum) {
        result.calculatedMaximum = true;
        result.maximumPercentage = static_cast<double>(maximumCount) / numTrials * 100.0;
    }

    return result;
}
