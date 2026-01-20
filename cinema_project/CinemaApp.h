#ifndef CINEMA_APP_H
#define CINEMA_APP_H

#include <string>
#include <vector>
#include "Cinema.h"

/*
Klasa tworząca podstawy UI dla symulatora kina

Odpowiada za:
- pętlę programu: wczytaj linię -> wykonaj -> wypisz wynik,
- logowanie i przełączanie ról ("guest", "admin", "client"),
- sprawdzenie poprawności liczby argumentów komend,
- utrzymanie stanu sesji klienta (wybrany seans do zakupu biletów).

Operacje związane z zarządzaniem obiektami (np. sprzedaż, zwroty, repertuar) znajdują się w klasie Cinema
 */

class CinemaApp {
    public:
        CinemaApp();

        /*
        Uruchamia główną pętlę programu
        Czyta polecenia ze standardowego wejścia aż do komendy "exit"
        */
        void run();

    private:
        // Pomoc dostępna odpowiednio w trybie "guest", "admin" i "client" (np. logowanie czy dodawanie seansu)
        void printGlobalHelp() const;
        void printAdminHelp() const;
        void printClientHelp() const;

        // ---- Obsługa komend w zależności od roli ----
        // Każda metoda przyjmuje:
        // - cmd  : pierwszy argument (np. "login", "buy", "repertoire"),
        // - args : wszystkie kolejne argumenty (args[0] == cmd).

        // Obsługa komend gościa, administratora i klienta (np. repertuar, zwroty, czas, lista biletów) 
        void handleGuestCommand(const std::string &cmd, const std::vector<std::string> &args);
        void handleAdminCommand(const std::string &cmd, const std::vector<std::string> &args);
        void handleClientCommand(const std::string &cmd, const std::vector<std::string> &args);

        std::vector<std::string> splitLine(const std::string &line) const;

        // Przeczywywanie stanu kina (w tym odczyt / zapis do pliku)
        Cinema cinema;

        // Sterowanie działaniem aplikacji (pętla run())
        bool running;

        std::string role;            // "guest", "admin", "client"
        std::string clientNickname;

        std::string selectedTitle;
        std::string selectedHallId;
        int selectedStartTimeMin;
        bool hasSelectedShow;
};

#endif


