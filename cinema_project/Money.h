#ifndef MONEY_H
#define MONEY_H

/*
Prosty licznik finansów kina.

balance     - aktualny stan konta (wpływy minus zwroty)
dailyIncome - dzienny przychód liczony jako suma wpływów w trakcie dnia
              (zwroty go nie zmniejszają; są odzwierciedlane w balance)

Info: metody addIncome/addExpense ignorują wartości ujemne
*/

class Money {
    public:
        Money();

        double getBalance() const;
        double getDailyIncome() const;

        // Settery używane głównie przy odczycie z plików (meta.txt)
        void setBalance(double b);
        void setDailyIncome(double d);

        // Zeruje licznik dziennego przychodu (np. po zakończeniu dnia)
        void resetDaily();

        // Dodaje wpływ: zwiększa balance i dailyIncome
        void addIncome(double amount);

        // Dodaje wydatek: zmniejsza balance (dailyIncome bez zmian, używane do zwrotów biletów)
        void addExpense(double amount);

    private:
        double balance;
        double dailyIncome;
};

#endif
