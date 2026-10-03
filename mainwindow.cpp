#include "mainwindow.h"
#include "dicesimulator.h"

#include <QApplication>
#include <QMainWindow>
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUI();
}

void MainWindow::setupUI() {
    auto *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QVBoxLayout(centralWidget);

    QFont headerText;
    headerText.setPointSize(headerText.pointSize() + 1);

    auto *diceLabel = new QLabel("How many dice would you like to roll?");
    diceLabel->setFont(headerText);
    mainLayout->addWidget(diceLabel);

    setupDiceInputs();

    auto *checkboxLabel = new QLabel("Tell me when the following occurs:");
    checkboxLabel->setFont(headerText);
    mainLayout->addWidget(checkboxLabel);

    setupCheckboxes();

    mainLayout->addSpacing(25);

    setupButton();

    resultLabel = new QLabel;
    resultLabel->setAlignment(Qt::AlignCenter);
    resultLabel->setFont(headerText);
    mainLayout->addWidget(resultLabel);

    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(15);
    mainLayout->addStretch();
}

void MainWindow::setupDiceInputs() {
    auto *formLayout = new QFormLayout;

    for (int i = 0; i < diceInputs.size(); i++) {
        diceInputs[i] = new QLineEdit;
        diceInputs[i]->setPlaceholderText("0");

        diceInputs[i]->setMaximumWidth(65);
        diceInputs[i]->setValidator(new QIntValidator(0, 1000000, diceInputs[i]));

        formLayout->addRow((simulator.dieLabels[i] + ":").data(), diceInputs[i]);
    }

    formLayout->setSpacing(10);
    mainLayout->addLayout(formLayout);

    // TODO: Show approximate calculation time after trialsLabel input changes based on number of dice and number of trials
    auto *trialsLabel = new QLabel("How many trials would you like to run?");

    QFont font = trialsLabel->font();
    font.setPointSize(font.pointSize() + 1);
    trialsLabel->setFont(font);

    mainLayout->addWidget(trialsLabel);

    trialsInput = new QLineEdit;
    trialsInput->setPlaceholderText("trials");
    trialsInput->setMaximumWidth(85);
    trialsInput->setText("10000");
    trialsInput->setValidator(new QIntValidator(1, 1000000000, trialsInput));

    mainLayout->addWidget(trialsInput);
}

// TODO: Clean up variable names
void MainWindow::setupCheckboxes() {
    // =======================================================
    // Same number checkbox
    // =======================================================

    auto *firstVBoxLayout = new QVBoxLayout;

    auto *checkbox1Main = new QHBoxLayout;
    checkbox1Main->setSpacing(5);

    sameRollCheckbox = new QCheckBox;
    checkbox1Main->addWidget(sameRollCheckbox);

    auto *preNumBoxLabel1 = new QLabel("I roll");
    checkbox1Main->addWidget(preNumBoxLabel1);

    sameRollNumBox = new QLineEdit("3");
    sameRollNumBox->setValidator(new QIntValidator(2, 1000000, sameRollNumBox));
    sameRollNumBox->setFixedWidth(30);
    checkbox1Main->addWidget(sameRollNumBox);

    auto *postNumBoxLabel2 = new QLabel("of the same number");
    checkbox1Main->addWidget(postNumBoxLabel2);
    checkbox1Main->addStretch();

    auto *checkbox1Sub = new QHBoxLayout;
    checkbox1Sub->setContentsMargins(30, 0, 0, 0);

    auto *preNumBoxLabel2 = new QLabel("Number must be greater than or equal to:");
    checkbox1Sub->addWidget(preNumBoxLabel2);

    sameRollNumBox2 = new QLineEdit();
    sameRollNumBox2->setPlaceholderText("0");
    sameRollNumBox2->setFixedWidth(30);
    checkbox1Sub->addWidget(sameRollNumBox2);

    checkbox1Sub->addStretch();

    firstVBoxLayout->addLayout(checkbox1Main);
    firstVBoxLayout->addLayout(checkbox1Sub);
    firstVBoxLayout->setSpacing(2);

    mainLayout->addLayout(firstVBoxLayout);

    // Setting all widgets to disabled and connecting to toggle via checkbox
    QList<QWidget *> sameRollWidgets =
    {
        preNumBoxLabel1,
        sameRollNumBox,
        postNumBoxLabel2,
        preNumBoxLabel2,
        sameRollNumBox2
    };

    for (QWidget *w: sameRollWidgets)
        w->setEnabled(false);

    connect(
        sameRollCheckbox,
        &QCheckBox::toggled,
        this,
        [sameRollWidgets](bool checked) {
            for (QWidget *w: sameRollWidgets)
                w->setEnabled(checked);
        }
    );

    // =======================================================
    // Maximum values checkbox
    // =======================================================

    auto *secondVBoxLayout = new QVBoxLayout;

    auto *checkbox2Main = new QHBoxLayout;
    checkbox2Main->setSpacing(5);

    maxRollCheckbox = new QCheckBox;
    checkbox2Main->addWidget(maxRollCheckbox);

    auto *preLabel = new QLabel("I roll");
    checkbox2Main->addWidget(preLabel);

    maxRollNumBox = new QLineEdit("2");
    maxRollNumBox->setValidator(new QIntValidator(1, 1000000, maxRollNumBox));
    maxRollNumBox->setFixedWidth(30);

    checkbox2Main->addWidget(maxRollNumBox);

    auto *postLabel =
            new QLabel("maximum die values");

    checkbox2Main->addWidget(postLabel);

    checkbox2Main->addStretch();

    auto *checkbox2Sub = new QHBoxLayout;

    checkbox2Sub->setContentsMargins(30, 0, 0, 0);

    auto *label =
            new QLabel("Max die values based on:");

    checkbox2Sub->addWidget(label);

    maxRollDropdown = new QComboBox;

    maxRollDropdown->addItems(
        {
            "Highest die size",
            "Most numerous die size",
            "d4",
            "d6",
            "d8",
            "d10",
            "d12",
            "d20",
            "All (combined)"
        });

    maxRollDropdown->setFixedWidth(165);

    checkbox2Sub->addWidget(maxRollDropdown);
    checkbox2Sub->addStretch();

    secondVBoxLayout->addLayout(checkbox2Main);
    secondVBoxLayout->addLayout(checkbox2Sub);
    secondVBoxLayout->setSpacing(2);

    mainLayout->addLayout(secondVBoxLayout);

    // Setting all widgets to disabled and connecting to toggle via checkbox
    QList<QWidget *> maxWidgets =
    {
        preLabel,
        maxRollNumBox,
        postLabel,
        label,
        maxRollDropdown
    };

    for (QWidget *w: maxWidgets)
        w->setEnabled(false);

    connect(
        maxRollCheckbox,
        &QCheckBox::toggled,
        this,
        [maxWidgets](bool checked) {
            for (QWidget *w: maxWidgets)
                w->setEnabled(checked);
        }
    );
}

// TODO: Add "Rolling..." text animation using a background thread for processing
void MainWindow::setupButton() {
    auto *button = new QPushButton("Roll Dice");

    button->setFixedSize(200, 50);
    QFont buttonFont = button->font();
    buttonFont.setPointSize(buttonFont.pointSize() + 6);
    button->setFont(buttonFont);

    mainLayout->addWidget(button, 0, Qt::AlignCenter);

    connect(button, &QPushButton::clicked, this, &MainWindow::rollDice);
}

void MainWindow::rollDice() {
    std::array<int, 6> diceCounts{};
    SimulationSettings settings;
    QString output;

    settings.checkSameNumber = sameRollCheckbox->isChecked();
    settings.checkMaximum = maxRollCheckbox->isChecked();

    // If neither checkbox is selected, return without running any trials
    if (settings.checkSameNumber == false && settings.checkMaximum == false) {
        output = "No conditions selected.";
        resultLabel->setText(output);
        return;
    }

    if (settings.checkSameNumber) {
        settings.sameNumberCount = sameRollNumBox->text().toInt();
        settings.sameNumberMinimum = sameRollNumBox2->text().toInt();
    }

    if (settings.checkMaximum) {
        settings.maximumCount = maxRollNumBox->text().toInt();
    }

    if (settings.checkMaximum) {
        QString choice = maxRollDropdown->currentText();

        if (choice == "Highest die size")
            settings.maxValueDropdown = SimulationSettings::MaxValueDropdown::HighestDie;

        else if (choice == "Most numerous die size")
            settings.maxValueDropdown = SimulationSettings::MaxValueDropdown::MostNumerousDie;

        else if (choice == "d4")
            settings.maxValueDropdown = SimulationSettings::MaxValueDropdown::D4;

        else if (choice == "d6")
            settings.maxValueDropdown = SimulationSettings::MaxValueDropdown::D6;

        else if (choice == "d8")
            settings.maxValueDropdown = SimulationSettings::MaxValueDropdown::D8;

        else if (choice == "d10")
            settings.maxValueDropdown = SimulationSettings::MaxValueDropdown::D10;

        else if (choice == "d12")
            settings.maxValueDropdown = SimulationSettings::MaxValueDropdown::D12;

        else if (choice == "d20")
            settings.maxValueDropdown = SimulationSettings::MaxValueDropdown::D20;

        else if (choice == "All (combined)")
            settings.maxValueDropdown = SimulationSettings::MaxValueDropdown::Combined;
    }

    for (int i = 0; i < diceInputs.size(); i++) {
        diceCounts[i] = diceInputs[i]->text().toInt();
    }

    const int numTrials = trialsInput->text().toInt();

    if (numTrials <= 0) {
        resultLabel->setText("Please enter a valid number of trials");
        return;
    }

    // Show status before starting calculation
    resultLabel->setText("Rolling...");

    // Force Qt to repaint immediately
    QApplication::processEvents();

    const auto result = simulator.runMultipleTrials(
        diceCounts[0], //d4
        diceCounts[1], //d6
        diceCounts[2], //d8
        diceCounts[3], //d10
        diceCounts[4], //d12
        diceCounts[5], //d20
        numTrials,
        settings);

    if (result.calculatedDuplicate) {
        output += QString("Duplicate rolls: %1%\n")
                .arg(result.duplicatePercentage, 0, 'f', 2);
    }

    if (result.calculatedMaximum) {
        output += QString("Maximum rolls: %1%\n")
                .arg(result.maximumPercentage, 0, 'f', 2);
    }

    resultLabel->setText(output);
}
