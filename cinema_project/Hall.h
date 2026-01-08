#ifndef HALL_H
#define HALL_H

#include <string>

/*
Reprezentuje salę kinową jako siatkę miejsc rows x cols.

Miejsca mają dwa formaty:
- indeks używany wewnętrznie (np. w Screening),
- etykieta tekstowa (np. "1A") używana w interfejsie użytkownika.

Konwencja mapowania:
index = row * cols + col, gdzie row i col są liczone od 0.
Etykieta: numer rzędu od 1 + litera kolumny od 'A'.
*/

class Hall {
    public:
        Hall();
        Hall(const std::string &id_, int rows_, int cols_);

        // ID sali, używane do wyboru seansu i zapisu do plików
        std::string getId() const;

        int getRows() const;
        int getCols() const;

        // Liczba wszystkich miejsc w sali (rows * cols)
        int getCapacity() const;

        // Sprawdza poprawność etykiety miejsca (np. "1A", "10C")
        bool isSeatLabelValid(const std::string &label) const;

        //Zamienia etykietę miejsca (np. "1A") na indeks (0..capacity-1).
        int seatLabelToIndex(const std::string &label) const;

        // Zamienia indeks miejsca na etykietę (np. 0 -> "1A") (wykorzystywane w automatycznym przydziale miejsc)
        std::string indexToSeatLabel(int index) const;

    private:
        std::string id;
        int rows;
        int cols;
};

#endif
