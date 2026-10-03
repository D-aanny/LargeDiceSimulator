#ifndef LARGEDIESIMULATOR_MAINWINDOW_H
#define LARGEDIESIMULATOR_MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>

#include <array>

#include "dicesimulator.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void setupUI();

    void setupDiceInputs();

    void setupCheckboxes();

    void setupButton();

    void rollDice();

    QVBoxLayout *mainLayout{};

    std::array<QLineEdit *, 6> diceInputs{};

    QLineEdit *trialsInput{};

    QCheckBox *sameRollCheckbox{};
    QLineEdit *sameRollNumBox{};
    QLineEdit *sameRollNumBox2{};

    QCheckBox *maxRollCheckbox{};
    QLineEdit *maxRollNumBox{};
    QComboBox *maxRollDropdown{};

    QLabel *resultLabel{};

    DiceSimulator simulator{};
};

#endif //LARGEDIESIMULATOR_MAINWINDOW_H
