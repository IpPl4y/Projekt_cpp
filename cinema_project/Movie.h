#ifndef MOVIE_H
#define MOVIE_H

#include <string>
#include <vector>

/*
Reprezentuje film w systemie kina.

Przechowuje dane opisowe (tytuł, czas trwania, ograniczenie wiekowe) oraz listę ocen
Ocena jest przypisana do nicku klienta: jeśli ten sam nick oceni ponownie, ocena jest nadpisywana.
*/

class Movie {
    public:
        Movie();
        Movie(int id, const std::string &title, int durationMin, int ageRating);

        int getId() const;
        std::string getTitle() const;
        int getDuration() const;
        int getAgeRating() const;

        /*
        Dodaje lub aktualizuje ocenę klienta.
        Oceny są ograniczane do zakresu 1-5 (wartości spoza zakresu są zaokrąglane w górę lub w dół)
        */
        void addOrUpdateRating(const std::string &nickname, int stars); // 1-5

        // Średnia ocena filmu - gdy brak ocen zwraca 0
        double getAverageRating() const;

        // Liczba zapisanych unikalnych ocen
        int getRatingsCount() const;

        // Pojedyncza ocena: nick + liczba gwiazdek
        struct Rating 
        {
            std::string nickname;
            int stars;
        };

        // Zwraca wszystkie oceny (np. do zapisu do pliku ratings.txt)
        const std::vector<Rating>& getRatings() const;

    private:
        int id;
        std::string title;
        int durationMin;
        int ageRating;

        // Lista ocen (max 1 wpis na nick)
        std::vector<Rating> ratings;
};

#endif
