#ifndef LARGEDIESIMULATOR_MAINWINDOW_H
#define LARGEDIESIMULATOR_MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QLineEdit>
#include <array>

#include "dicesimulator.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    void setupUI();
    void setupDiceInputs();
    void setupCheckboxes();
    void setupButton() const;

    QVBoxLayout *mainLayout{};

    std::array<QLineEdit*, 6> diceInputs{};

    DiceSimulator simulator{};
};

#endif //LARGEDIESIMULATOR_MAINWINDOW_H
