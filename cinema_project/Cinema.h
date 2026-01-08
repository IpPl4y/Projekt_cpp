#ifndef CINEMA_H
#define CINEMA_H

#include <vector>
#include <string>

#include "Movie.h"
#include "Hall.h"
#include "Screening.h"
#include "Ticket.h"
#include "Client.h"
#include "Money.h"

/*
Klasa przechowująca cały stan symulacji kina.

Odpowiada za:
- konfigurację (sale, filmy, seanse na dzień),
- przebieg dnia (czas symulacji, start / koniec dnia),
- operacje klienta (kupno biletów, zwroty, oceny),
- operacje admina (decyzje o zwrotach),
- trwały zapis / odczyt do plików tekstowych.
 */

class Cinema {
    public:
        Cinema();

        /*
        Metody odpowiedzialne za dodanie odpowiednio:
        - sali o konkretnej nazwie (ID) i wymiarach
        - filmu o tytule, długości trwania i ocenie wiekowej
        - seansu konkretnego filmu w konkretnej sali o czasie rozpoczęcia i cenie biletu
        */
        void addHall(const std::string &hallId, int rows, int cols);
        int addMovie(const std::string &title, int durationMin, int ageRating);
        int addScreening(const std::string &title, const std::string &hallId, int startTimeMin, double price);

        // Uruchamia dzień: od tego momentu możliwa jest sprzedaż biletów i zwroty
        void startDay();

        // Kończy dzień: blokuje operacje zależne od dnia oraz czyści seanse na dzień
        void endDay();

        bool isDayActive() const;

        // Przesuwa symulowany czas dnia o podaną liczbę minut (używane do demonstracji resetu dnia i warunku zwrotu biletu)
        void advanceTime(int minutes);
        
        // Aktualny czas symulacji w minutach od 00:00
        int getCurrentTime() const;

        /*
        Seria komend wyświetlających odpowiednio:
        - listę dodanych sal
        - listę dodanych filmow
        - mapę miejsc dla konkretnego seansu (film, sala, godzina)
        - repertuar (seanse na dzień, wolne miejsca, cena)
        - kolejka biletów oczekujących na decyzję admina o zwrocie
        - bilety konkretnego klienta
        */
        void printHalls() const;
        void printMovies() const;
        void printSeatsByTitle(const std::string &title, const std::string &hallId, int startTimeMin) const;
        void printRepertoire() const;
        void printRefundQueue() const;
        void printClientTickets(const std::string &nickname) const;

        // Pomocniczo: zamienia status biletu z liczby (0,1,2,3) na status tekstowy do UI
        std::string ticketStatusToString(TicketStatus status) const;

        /*
        Kupuje bilety na wskazany seans (po tytule, sali i godzinie)
        Jeśli lista miejsc jest pusta lub krótsza niż <count>, pozostałe miejsca są dobierane automatycznie
        */
        std::vector<int> buyTickets(const std::string &nickname,
                                    const std::string &title,
                                    const std::string &hallId,
                                    int startTimeMin,
                                    int count,
                                    const std::vector<std::string> &seats);

        /* 
        Zgłasza prośbę o zwrot biletu (zmienia status na "oczekuje na zwrot")
        Wymaga aktywnego dnia i czasu do seansu <= 60 min 
        */                           
        void requestRefund(const std::string &nickname, int ticketId);

        // Dodaje / aktualizuje ocenę filmu 1–5
        void rateMovie(const std::string &nickname, const std::string &title, int stars);

        /*
        Decyzja admina o zwrocie biletu:
        approve = 1: zwraca pieniądze i zwalnia miejsce w seansie
        approve = 0: odrzuca prośbę (bilet pozostaje w historii klienta)
        */
        void decideRefund(int ticketId, bool approve);

        // Zapisuje / wczytuje stan do katalogu data jako zestaw plików .txt (metadane, sale, filmy, seanse, bilety, oceny)
        bool saveToFiles(const std::string &dir) const;
        bool loadFromFiles(const std::string &dir);

        // Dostęp tylko do odczytu do stanu finansów (np. dla komendy money)
        const Money& getMoney() const;

    private:
        // Wyszukiwania pomocnicze (zwracają indeks w wektorze lub -1)
        int findHallIndexById(const std::string &hallId) const;
        int findMovieIndexById(int movieId) const;
        int findScreeningIndexById(int screeningId) const;
        int findClientIndexByNickname(const std::string &nickname) const;
        int findTicketIndexById(int ticketId) const;

        // Generatory ID (funkcje pomocniczne, aby ID były unikalne i pozwalały zapisywać stan między uruchomieniami)
        int getNextMovieId();
        int getNextScreeningId();
        int getNextTicketId();

        // Dobór pierwszego wolnego miejsca w seansie
        int findFirstFreeSeatIndex(const Screening &s, const Hall &h) const;

        bool dayActive;
        int currentTimeMin; // minuty od 0:00

        // Główny stan wszystkich zmiennych w aplikacji (przechowywany w pamięci + zapisywany do plików)
        std::vector<Hall> halls;
        std::vector<Movie> movies;
        std::vector<Screening> screenings;
        std::vector<Ticket> tickets;
        std::vector<Client> clients;

        Money money;

        // Liczniki ID (zapis w meta.txt dla zapisu stanu między uruchomieniami
        int movieIdCounter;
        int screeningIdCounter;
        int ticketIdCounter;

        // Minimalny czas (w minutach) przed seansem, aby zwrot był dozwolony
        static const int REFUND_LIMIT_MIN = 60;
};

#endif
