#include "Money.h"

Money::Money()
{
    balance = 0.0;
    dailyIncome = 0.0;
}

double Money::getBalance() const
{
    return balance;
}

double Money::getDailyIncome() const
{
    return dailyIncome;
}

void Money::resetDaily()
{
    dailyIncome = 0.0;
}

void Money::addIncome(double amount)
{
    if (amount < 0.0) return;
    balance += amount;
    dailyIncome += amount;
}

void Money::addExpense(double amount)
{
    if (amount < 0.0) return;
    balance -= amount;
}

void Money::setBalance(double b)
{
    balance = b;
}

void Money::setDailyIncome(double d)
{
    dailyIncome = d;
}
