#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QRegExpValidator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    cardType = "";

    // Numbers only
    ui->numberInput->setValidator(new QRegExpValidator(QRegExp("[0-9]*")));

    connect(ui->numberInput, &QLineEdit::textChanged, this, &MainWindow::checkType);
    connect(ui->isValidButton, &QPushButton::pressed, this, &MainWindow::isValid);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::checkType(QString number)
{
    if (number.isEmpty())
    {
        cardType = "";
        ui->cardType->setText(cardType);
        return;
    }

    if (number.at(0) == "2")
    {
        cardType = "MIR";
    }

    else if (number.at(0) == "3")
    {
        if (number.at(1) == "0" || number.at(1) == "6"
                || number.at(1) == "8")
        {
            cardType = "Diners Club";
        }
        else if (number.at(1) == "1" || number.at(1) == "5")
        {
            cardType = "JCB International";
        }
        else if (number.at(1) == "4" || number.at(1) == "7")
        {
            cardType = "American Express";
        }
    }

    else if (number.at(0) == "4")
    {
        cardType = "VISA";
    }

    else if (number.at(0) == "5")
    {
        if (number.at(1) == "0" || number.at(1) == "6"
                || number.at(1) == "7" || number.at(1) == "8")
        {
            cardType = "Maestro";
        }
        else if (number.at(1) == "1" || number.at(1) == "2"
                 || number.at(1) == "3" || number.at(1) == "4"
                 || number.at(1) == "5")
        {
            cardType = "MasterCard";
        }
    }

    else if (number.at(0) == "6")
    {
        if (number.at(1) == "0")
        {
            cardType = "Discover";
        }
        else if (number.at(1) == "2")
        {
            cardType = "China UnionPay";
        }
        else if (number.at(1) == "3" || number.at(1) == "7")
        {
            cardType = "Maestro";
        }
    }

    else if (number.at(0) == "7")
    {
        cardType = "UEC";
    }

    ui->cardType->setText(cardType);
}

void MainWindow::isValid()
{
    bool ok;
    long long number = ui->numberInput->text().toLongLong(&ok);
    std::vector<long long> numbers;

    if (ok)
    {
        while (number > 0)
        {
            numbers.push_back(number % 10);
            number /= 10;
        }
        std::reverse(numbers.begin(), numbers.end());

        // Luhn Algorithm Check
        int sum = 0;
        int parity = numbers.size() % 2;
        for (int i = 0; i < numbers.size(); i++)
        {
            if (i % 2 != parity)
            {
                sum += numbers.at(i);
            }
            else if (numbers.at(i) > 4)
            {
                sum += 2 * numbers.at(i) - 9;
            }
            else
            {
                sum += 2 * numbers.at(i);
            }
        }

        if (sum % 10 == 0)
        {
            ui->isValid->setText("Valid!");
        }
        else
        {
            ui->isValid->setText("Not Valid!");
        }
    }
    else
    {
        QMessageBox::warning(this, tr("Warning"), tr("Unexpected error while reading card number"));
    }
}

