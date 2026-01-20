#ifndef SCREENING_H
#define SCREENING_H

#include <string>
#include <vector>

/*
Reprezentuje pojedynczy seans filmu w konkretnej sali o konkretnej godzinie

Zajęte miejsca trzymane są jako wektor wartości (0/1) przypisanych do indeksu miejsca
zgodnego z Hall::seatLabelToIndex / Hall::indexToSeatLabel
 */

class Screening {
    public:
        Screening();
        Screening(int id_, int movieId_, const std::string &hallId_, int startTimeMin_, double price_);

        int getId() const;
        int getMovieId() const;
        std::string getHallId() const;

        // Godzina startu w minutach od 00:00 (np. 18:30 -> 1110)
        int getStartTimeMin() const;
        
        double getPrice() const;
        
        // Sprawdza, czy miejsce o danym indeksie jest zajęte (poza zakresem -> false)
        bool isSeatTaken(int seatIndex) const;

        // Oznacza miejsce jako zajęte
        void takeSeat(int seatIndex);
        
        // Zwalnia miejsce
        void freeSeat(int seatIndex);
        
        // Zwraca liczbę miejsc oznaczonych jako zajęte (liczone po indeksach)
        int getTakenSeatsCount() const;
        
        // Czyści informację o zajętych miejscach (np. po zakończeniu dnia)
        void clearSeats();

    private:
        int id;
        int movieId;
        std::string hallId;
        int startTimeMin;
        double price;

        // 0 = wolne, 1 = zajęte; indeks odpowiada "seatIndex" w sali
        std::vector<int> takenSeatsFlags;
};

#endif
