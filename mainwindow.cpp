#include "mainwindow.h"
#include "dicesimulator.h"
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

    simulator.runMultipleTrials(2, 2, 2, 0, 0, 0, 200);
}

// TODO: Link internal logic to existing UI elements
void MainWindow::setupUI()
{
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

    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(15);
    mainLayout->addStretch();
}

void MainWindow::setupDiceInputs() {

    auto *formLayout = new QFormLayout;

    for (int i = 0; i < diceInputs.size(); i++) {
        diceInputs[i] = new QLineEdit;
        diceInputs[i]->setPlaceholderText(simulator.dieLabels[i].data());
    }
    // for (QLineEdit *diceInput : diceInputs) {
    //     diceInput = new QLineEdit;
    //     diceInput->setPlaceholderText(simulator.dieLabels[i].data());
    // }

    QLineEdit *inputs[] =
    {
        diceInputs[0],
        diceInputs[1],
        diceInputs[2],
        diceInputs[3],
        diceInputs[4],
        diceInputs[5]
    };

    for (QLineEdit *input : inputs)
    {
        input->setMaximumWidth(50);
        input->setValidator(new QIntValidator(input));
    }

    formLayout->setSpacing(10);

    for (int i = 0; i < diceInputs.size(); i++) {
        formLayout->addRow((simulator.dieLabels[i] + ":").data(), diceInputs[i]);
    }

    mainLayout->addLayout(formLayout);

    // TODO: Show approximate calculation time after trialsLabel input changes based on number of dice and number of trials
    auto *trialsLabel = new QLabel("How many trials would you like to run?");

    QFont font = trialsLabel->font();
    font.setPointSize(font.pointSize() + 1);
    trialsLabel->setFont(font);

    mainLayout->addWidget(trialsLabel);

    auto *trialsInput = new QLineEdit;

    trialsInput->setPlaceholderText("trials");
    trialsInput->setMaximumWidth(85);
    trialsInput->setText("10000");
    trialsInput->setValidator(new QIntValidator(trialsInput));

    mainLayout->addWidget(trialsInput);
}

// TODO: Clean up variable names
void MainWindow::setupCheckboxes()
{
    // =======================================================
    // Same number checkbox
    // =======================================================

    auto *firstVBoxLayout = new QVBoxLayout;

    auto *checkbox1Main = new QHBoxLayout;
    checkbox1Main->setSpacing(5);

    auto *sameRollCheckbox = new QCheckBox;
    checkbox1Main->addWidget(sameRollCheckbox);

    auto *preNumBoxLabel1 = new QLabel("I roll");
    checkbox1Main->addWidget(preNumBoxLabel1);

    auto *sameRollNumBox = new QLineEdit("3");
    sameRollNumBox->setFixedWidth(30);
    checkbox1Main->addWidget(sameRollNumBox);

    auto *postNumBoxLabel2 = new QLabel("of the same number");
    checkbox1Main->addWidget(postNumBoxLabel2);
    checkbox1Main->addStretch();

    auto *checkbox1Sub = new QHBoxLayout;
    checkbox1Sub->setContentsMargins(30,0,0,0);

    auto *preNumBoxLabel2 = new QLabel("Number must be greater than or equal to:");
    checkbox1Sub->addWidget(preNumBoxLabel2);

    auto *sameRollNumBox2 = new QLineEdit("0");
    sameRollNumBox2->setFixedWidth(30);
    checkbox1Sub->addWidget(sameRollNumBox2);

    checkbox1Sub->addStretch();

    firstVBoxLayout->addLayout(checkbox1Main);
    firstVBoxLayout->addLayout(checkbox1Sub);
    firstVBoxLayout->setSpacing(2);

    mainLayout->addLayout(firstVBoxLayout);

    // Setting all widgets to disabled and connecting to toggle via checkbox
    QList<QWidget*> sameRollWidgets =
    {
        preNumBoxLabel1,
        sameRollNumBox,
        postNumBoxLabel2,
        preNumBoxLabel2,
        sameRollNumBox2
    };

    for(QWidget *w : sameRollWidgets)
        w->setEnabled(false);

    connect(
        sameRollCheckbox,
        &QCheckBox::toggled,
        this,
        [sameRollWidgets](bool checked)
        {
            for(QWidget *w : sameRollWidgets)
                w->setEnabled(checked);
        }
    );

    // =======================================================
    // Maximum values checkbox
    // =======================================================

    auto *secondVBoxLayout = new QVBoxLayout;

    auto *checkbox2Main = new QHBoxLayout;
    checkbox2Main->setSpacing(5);

    auto *maxRollCheckbox = new QCheckBox;
    checkbox2Main->addWidget(maxRollCheckbox);

    auto *preLabel = new QLabel("I roll");
    checkbox2Main->addWidget(preLabel);


    auto *maxRollNumBox = new QLineEdit("2");

    maxRollNumBox->setFixedWidth(30);

    checkbox2Main->addWidget(maxRollNumBox);

    auto *postLabel =
        new QLabel("maximum die values");

    checkbox2Main->addWidget(postLabel);

    checkbox2Main->addStretch();

    auto *checkbox2Sub = new QHBoxLayout;

    checkbox2Sub->setContentsMargins(30,0,0,0);

    auto *label =
        new QLabel("Max die values based on:");

    checkbox2Sub->addWidget(label);

    auto *dropdown = new QComboBox;

    dropdown->addItems(
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

    dropdown->setFixedWidth(165);

    checkbox2Sub->addWidget(dropdown);
    checkbox2Sub->addStretch();

    secondVBoxLayout->addLayout(checkbox2Main);
    secondVBoxLayout->addLayout(checkbox2Sub);
    secondVBoxLayout->setSpacing(2);

    mainLayout->addLayout(secondVBoxLayout);

    // Setting all widgets to disabled and connecting to toggle via checkbox
    QList<QWidget*> maxWidgets =
    {
        preLabel,
        maxRollNumBox,
        postLabel,
        label,
        dropdown
    };

    for(QWidget *w : maxWidgets)
        w->setEnabled(false);

    connect(
        maxRollCheckbox,
        &QCheckBox::toggled,
        this,
        [maxWidgets](bool checked)
        {
            for(QWidget *w : maxWidgets)
                w->setEnabled(checked);
        }
    );
}

// TODO: Add "Rolling..." text animation and have results appear after button press
void MainWindow::setupButton() const {
    auto *button = new QPushButton("Roll Dice");

    button->setFixedSize(200, 50);
    QFont buttonFont = button->font();
    buttonFont.setPointSize(buttonFont.pointSize() + 6);
    button->setFont(buttonFont);

    mainLayout->addWidget(button, 0, Qt::AlignCenter);
}
