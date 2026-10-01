#ifndef LARGEDICESIMULATOR_DICESIM_H
#define LARGEDICESIMULATOR_DICESIM_H

#include <random>
#include <array>
#include <vector>

class DiceSimulator {
public:
    struct SimulationResult {
        double triplePercentage{};
        double doubleMaxPercentage{};
    };

    SimulationResult runMultipleTrials(
        int numd4,
        int numd6,
        int numd8,
        int numd10,
        int numd12,
        int numd20,
        int numTrials
    );

    std::array<std::string, 6> dieLabels{
        "d4",
        "d6",
        "d8",
        "d10",
        "d12",
        "d20"
    };

private:
    std::mt19937 mt{std::random_device{}()};

    int rollRandNum(int dieSize);

    std::vector<int> runSingleTrial(
        int numd4,
        int numd6,
        int numd8,
        int numd10,
        int numd12,
        int numd20
    );

    bool hasTripleOrMore(
        const std::vector<int> &nums,
        int minNum
    );

    bool hasDoubleMax(
        const std::vector<int> &nums,
        int goalMax
    );
};

#endif //LARGEDICESIMULATOR_DICESIM_H
