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

bool DiceSimulator::hasTripleOrMore(
    const std::vector<int> &nums,
    int minNum) {
    std::unordered_map<int, int> countMap;

    for (int num: nums) {
        if (num >= minNum) // Used to set current crit system where rolls must be 4 or higher to count for crit
            countMap[num]++;

        if (countMap[num] >= 3) {
            return true;
        }
    }

    return false;
}

bool DiceSimulator::hasDoubleMax(
    const std::vector<int> &nums,
    const int goalMax) {
    int count = 0;

    for (int num: nums) {
        if (num == goalMax) {
            count++;

            if (count >= 2)
                return true;
        }
    }
    return false;
}

DiceSimulator::SimulationResult DiceSimulator::runMultipleTrials(
    const int numd4,
    const int numd6,
    const int numd8,
    const int numd10,
    const int numd12,
    const int numd20,
    const int numTrials) {
    if (numTrials <= 0)
        return {};

    int numTrialsWithTripleCrit = 0;
    int numTrialsWithDblMaxCrit = 0;

    int maxNumGoal = 4;

    if (numd20 != 0)
        maxNumGoal = 20;
    else if (numd12 != 0)
        maxNumGoal = 12;
    else if (numd10 != 0)
        maxNumGoal = 10;
    else if (numd8 != 0)
        maxNumGoal = 8;
    else if (numd6 != 0)
        maxNumGoal = 6;

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

        if (hasTripleOrMore(trial, 0))
            numTrialsWithTripleCrit++;

        if (hasDoubleMax(trial, maxNumGoal))
            numTrialsWithDblMaxCrit++;
    }

    SimulationResult result;

    result.triplePercentage = static_cast<double>(numTrialsWithTripleCrit) / numTrials * 100.0;
    result.doubleMaxPercentage = static_cast<double>(numTrialsWithDblMaxCrit) / numTrials * 100.0;

    return result;
}
