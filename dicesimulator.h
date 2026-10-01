#ifndef LARGEDICESIMULATOR_DICESIM_H
#define LARGEDICESIMULATOR_DICESIM_H

#include <random>
#include <array>
#include <vector>

struct SimulationSettings {
    bool checkSameNumber = false;
    int sameNumberCount = 3;
    int sameNumberMinimum = 0;

    bool checkMaximum = false;
    int maximumCount = 2;

    enum class MaximumMode {
        HighestDie,
        MostNumerousDie,
        D4,
        D6,
        D8,
        D10,
        D12,
        D20,
        Combined
    };

    MaximumMode maximumMode = MaximumMode::HighestDie;
};

// TODO: Add and implement a new struct for dice rolls that includes the value rolled along with the type of die rolled

class DiceSimulator {
public:
    struct SimulationResult {
        bool calculatedSameNumber = false;
        double sameNumberPercentage{};

        bool calculatedMaximum = false;
        double maximumPercentage{};
    };

    SimulationResult runMultipleTrials(
        int numd4,
        int numd6,
        int numd8,
        int numd10,
        int numd12,
        int numd20,
        int numTrials,
        const SimulationSettings &settings
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

    static int getMostNumerousDieSize(
        int numd4,
        int numd6,
        int numd8,
        int numd10,
        int numd12,
        int numd20
    );

    static int determineMaximumValue(
        int numd4,
        int numd6,
        int numd8,
        int numd10,
        int numd12,
        int numd20,
        SimulationSettings::MaximumMode mode
    );

    static bool checkSameNumberRolled(
        const std::vector<int> &nums,
        int numAmountNeeded,
        int minNum
    );

    static bool hasDoubleMax(
        const std::vector<int> &nums,
        int goalMax,
        int amountNeeded
    );
};

#endif //LARGEDICESIMULATOR_DICESIM_H
