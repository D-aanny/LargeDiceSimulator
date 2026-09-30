#include "dicesimulator.h"
#include <random>
#include <vector>
#include <unordered_map>

// TODO: Replace IOStream logic with GUI compatibility
// TODO: Fully implement customizability for die rolling parameters

// Returns a random number with range [1, dieSize]
int DiceSimulator::rollRandNum(const int dieSize)
{
    std::uniform_real_distribution<double> dist(1.0, 20.0);
    return (static_cast<int>(dist(mt)) % dieSize) + 1;
}

std::vector<int> DiceSimulator::runSingleTrial(const int numd4, const int numd6, const int numd8, const int numd10, const int numd12, const int numd20)
{
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

bool DiceSimulator::hasTripleOrMore(const std::vector<int>& nums, int minNum) {
    std::unordered_map<int, int> countMap;

    for (int num : nums) {
        //if (num >= minNum)                                                        // Used to set current crit system where rolls must be 4 or higher to count for crit
            countMap[num]++;
        if (countMap[num] >= 3) {
            //std::cout << num << " has appeared 3 or more times." << std::endl;    // Found a number repeated 3 or more times
            return true;
        }
    }

    //std::cout << "No number found with 3 or more repetitions." << std::endl;
    return false;
}

bool DiceSimulator::hasDoubleMax(const std::vector<int>& nums, const int goalMax)
{
    int count = 0;

    for (int num : nums) {
        if (num == goalMax) {
            count++;
            if (count >= 2) {
                //std::cout << goalMax << " has appeared 2 or more times." << std::endl << std::endl;
                return true;
            }
        }
    }

    //std::cout << goalMax << " has not appeared 2 or more times." << std::endl << std::endl;
    return false;
}

void DiceSimulator::runMultipleTrials(const int numd4, const int numd6, const int numd8, const int numd10, const int numd12, const int numd20, const int numTrials)
{
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

    for (int i = 0; i < numTrials; i++)
    {
        std::vector<int> trial = runSingleTrial(numd4, numd6, numd8, numd10, numd12, numd20);

        //std::cout << "Trial " << i + 1 << ": ";

        for (const int j : trial)
            //std::cout << j << " ";

        //std::cout << std::endl;

        if(hasTripleOrMore(trial, 0))
            numTrialsWithTripleCrit++;

        if (hasDoubleMax(trial, maxNumGoal))
            numTrialsWithDblMaxCrit++;
    }

    //std::cout << "Summary: " << numTrials << " trials performed with " << numd4 << " d4, " << numd6 << " d6, " << numd8 << " d8, " << numd10 << " d10, " << numd12 << " d12, and " << numd20 << " d20." << std::endl;

    double percentageWithCrit = (static_cast<double>(numTrialsWithTripleCrit) / numTrials) * 100;

    //std::cout << "Percentage of trials with critical hit via rolling triples = " << percentageWithCrit << "%" << std::endl;

    percentageWithCrit = (static_cast<double>(numTrialsWithDblMaxCrit) / numTrials) * 100;

    //std::cout << "Percentage of trials with critical hit via rolling double max = " << percentageWithCrit << "%" << std::endl;
}