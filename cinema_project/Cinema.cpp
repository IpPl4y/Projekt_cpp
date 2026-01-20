#include "Cinema.h"

#include <iostream>
#include <cstdlib> // atoi
#include <fstream>
#include <sstream>
#include <filesystem>

Cinema::Cinema()
{
    dayActive = false;
    currentTimeMin = 0;
    movieIdCounter = 0;
    screeningIdCounter = 0;
    ticketIdCounter = 0;
}

void Cinema::addHall(const std::string &hallId, int rows, int cols)
{
    if (findHallIndexById(hallId) != -1) {error_info("Sala o takim ID już istnieje."); return;}

    Hall h(hallId, rows, cols);
    halls.push_back(h);
}

int Cinema::addMovie(const std::string &title, int durationMin, int ageRating)
{
    int id = getNextMovieId();
    Movie m(id, title, durationMin, ageRating);
    movies.push_back(m);
    return id;
}

int Cinema::addScreening(const std::string &title, const std::string &hallId, int startTimeMin, double price)
{
    if (halls.empty()) return error_info("Brak sal.");

    // Znajdowanie filmu po tytule
    int movieId = -1;
    
    for (size_t i = 0; i < movies.size(); i++) 
    {
        if (movies[i].getTitle() == title) {
            movieId = movies[i].getId();
            break;
        }
    }
    
    if (movieId == -1) return error_info("Nie ma filmu o tytule: " + title);

    int movieIndex = findMovieIndexById(movieId);

    if (movieIndex == -1) return error_info("Nie ma filmu o takim ID.");

    int hallIndex = findHallIndexById(hallId);
    
    if (hallIndex == -1) return error_info("Nie ma sali o takim ID.");

    // Jeden seans na salę / dzień - sprawdzenie
    for (size_t i = 0; i < screenings.size(); i++) 
    {
        if (screenings[i].getHallId() == hallId) return error_info("W tej sali jest juz seans na dziś.");
    }

    int id = getNextScreeningId();
    Screening s(id, movieId, hallId, startTimeMin, price);
    screenings.push_back(s);
    return id;
}


void Cinema::startDay()
{
    if (dayActive) {error_info("Dzień juz jest aktywny."); return;}
    
    if (screenings.empty()) {error_info("Brak seansów. Najpierw ustaw repertuar."); return;}
    
    dayActive = true;
    currentTimeMin = 0;
    money.resetDaily();

    std::cout << "\nRozpoczęto dzień - powodzenia!\n\n";
}

void Cinema::endDay()
{
    if (!dayActive) {error_info("Dzień nie jest aktywny."); return;}
    
    dayActive = false;

    for (size_t i = 0; i < screenings.size(); i++) 
    {
        screenings[i].clearSeats();
    }
    
    screenings.clear();

    std::cout << "\nZakończono dzień.\n\n";
}

bool Cinema::isDayActive() const 
{
    return dayActive;
}

void Cinema::advanceTime(int minutes)
{
    if (!dayActive) {error_info("Nie można zmieniać czasu, gdy dzień nie jest aktywny."); return;}

    if (minutes < 0) minutes = 0;

    currentTimeMin += minutes;

    if (currentTimeMin >= 24 * 60) 
    {
        dayActive = false;

        for (size_t i = 0; i < screenings.size(); i++) 
        {
        screenings[i].clearSeats();
        }
        screenings.clear();

        money.resetDaily(); 
        currentTimeMin = 0;     

        std::cout << "\nInfo: Dzien zakonczyl sie automatycznie (>=24h).\n\n";
    }
}

int Cinema::getCurrentTime() const
{
    return currentTimeMin;
}

const Money& Cinema::getMoney() const
{
    return money;
}

// ====== wypisywanie ======

void Cinema::printHalls() const
{
    std::cout << "\nSale:\n";
    
    for (size_t i = 0; i < halls.size(); i++) 
    {
        const Hall &h = halls[i];
        std::cout << "- " << h.getId()
                  << " (" << h.getRows() << "x" << h.getCols()
                  << ", miejsc: " << h.getCapacity() << ")\n";
    }

    std::cout << "\n";
}

void Cinema::printMovies() const
{
    std::cout << "\nFilmy:\n";
    
    for (size_t i = 0; i < movies.size(); i++) 
    {
        const Movie &m = movies[i];
        std::cout << "- ID " << m.getId()
                  << ": " << m.getTitle()
                  << " (" << m.getDuration() << " min, "
                  << m.getAgeRating() << "+), "
                  << "średnia ocena = " << m.getAverageRating()
                  << " (z " << m.getRatingsCount() << " opinii)\n";
    }

    std::cout << "\n";
}

void Cinema::printSeatsByTitle(const std::string &title, const std::string &hallId, int startTimeMin) const
{
    int movieId = -1;
    
    for (size_t i = 0; i < movies.size(); i++) 
    {
        if (movies[i].getTitle() == title) 
        {
            movieId = movies[i].getId();
            break;
        }
    }
    
    if (movieId == -1) {error_info("Sala o takim ID już istnieje."); return;}

    int sIndex = -1;
    
    for (size_t i = 0; i < screenings.size(); i++) 
    {
        const Screening &s = screenings[i];
        
        if (s.getMovieId() == movieId && s.getHallId() == hallId && s.getStartTimeMin() == startTimeMin) 
        {
            sIndex = (int)i;
            break;
        }
    }
    
    if (sIndex == -1) {error_info("Brak seansu dla tego filmu / sali / godziny."); return;}
    
    const Screening &s = screenings[sIndex];

    int hIndex = findHallIndexById(hallId);
    
    if (hIndex == -1) {error_info("Brak sali."); return;}
    
    const Hall &h = halls[hIndex];

    int rows = h.getRows();
    int cols = h.getCols();

    std::cout << "\nMapa miejsc dla \"" << title << "\" sala " << hallId << "\n";
    std::cout << "  ";
    
    for (int c = 0; c < cols; c++) 
    {
        char letter = (char)('A' + c);
        std::cout << " " << letter;
    }
    
    std::cout << "\n";

    for (int r = 0; r < rows; r++) 
    {
        int rowNumber = r + 1;
        if (rowNumber < 10) std::cout << " ";
        std::cout << rowNumber << " ";

        for (int c = 0; c < cols; c++) 
        {
            int idx = r * cols + c;
            bool taken = s.isSeatTaken(idx);

            // System znaków - . wolne, X zajęte
            char ch = taken ? 'X' : '.';
            std::cout << ch << " ";
        }
        
        std::cout << "\n";
    }

    std::cout << "Legenda: . wolne, X zajęte.\n\n";
}

void Cinema::printRepertoire() const
{
    std::cout << "\nRepertuar (seanse na dziś):\n";
    
    for (size_t i = 0; i < screenings.size(); i++) 
    {
        const Screening &s = screenings[i];

        int hallIndex = findHallIndexById(s.getHallId());
        int freeSeats = 0;
        
        if (hallIndex != -1) 
        {
            const Hall &h = halls[hallIndex];
            freeSeats = h.getCapacity() - s.getTakenSeatsCount();
        }

        int movieIndex = findMovieIndexById(s.getMovieId());
        std::string title = (movieIndex != -1) ? movies[movieIndex].getTitle() : "???";

        int hh = s.getStartTimeMin() / 60;
        int mm = s.getStartTimeMin() % 60;

        std::cout << "- Seans ID " << s.getId()
                  << ": \"" << title << "\""
                  << ", sala " << s.getHallId()
                  << ", start " << (hh < 10 ? "0" : "") << hh << ":"
                  << (mm < 10 ? "0" : "") << mm
                  << ", cena " << s.getPrice()
                  << ", wolne: " << freeSeats << "\n";
    }
    std::cout << "\n";
}

void Cinema::printRefundQueue() const
{
    std::cout << "\nZwroty oczekujące:\n";
    
    for (size_t i = 0; i < tickets.size(); i++) 
    {
        const Ticket &t = tickets[i];
        if (t.status == TicketStatus::RefundRequested) 
        {
            std::cout << "- ID biletu: " << t.id
                      << ", wnioskujący - " << t.ownerNickname
                      << ", seans - " << t.screeningId
                      << ", miejsce - " << t.seatLabel
                      << ", cena - " << t.price << "\n\n";
        }
    }
}

void Cinema::printClientTickets(const std::string &nickname) const
{
    int clientIndex = findClientIndexByNickname(nickname);
    
    if (clientIndex == -1) {error_info("Brak klienta: " + nickname); return;}

    const Client &c = clients[clientIndex];
    std::cout << "\nBilety klienta " << c.getNickname() << ":\n";

    const std::vector<int> &ids = c.getTicketIds();
    
    for (size_t i = 0; i < ids.size(); i++) 
    {
        int ticketIndex = findTicketIndexById(ids[i]);
        if (ticketIndex == -1) continue;

        const Ticket &t = tickets[ticketIndex];

        std::cout << "- ID biletu " << t.id
                  << ", seans = " << t.screeningId
                  << ", miejsce = " << t.seatLabel
                  << ", cena = " << t.price
                  << ", status = " << ticketStatusToString(t.status)
                  << "\n";
    }

    std::cout << "\n";
}

int Cinema::error_info(const std::string& msg, int code) const
{
    std::cerr << "\nBłąd: " << msg << "\n\n";
    return code;
}

// ====== operacje klienta ======

std::string Cinema::ticketStatusToString(TicketStatus status) const
{
    switch (status) 
    {
        case TicketStatus::Purchased:
            return "kupiony";
        case TicketStatus::RefundRequested:
            return "oczekuje na zwrot";
        case TicketStatus::Refunded:
            return "zwrócony";
        case TicketStatus::RefundRejected:
            return "odrzucono zwrot";
        default:
            return "nieznany";
    }
}

std::vector<int> Cinema::buyTickets(const std::string &nickname,
                                    const std::string &title,
                                    const std::string &hallId,
                                    int startTimeMin,
                                    int count,
                                    const std::vector<std::string> &seats)
{
    std::vector<int> created;

    if (!dayActive) {error_info("Dzień nie jest aktywny."); return created;}

    if (count <= 0) {error_info("Liczba biletów musi być większa niż 0."); return created;}

    // 1) Znajdowanie filmu po tytule
    int movieId = -1;
    
    for (size_t i = 0; i < movies.size(); i++) 
    {
        if (movies[i].getTitle() == title) 
        {
            movieId = movies[i].getId();
            break;
        }
    }
    
    if (movieId == -1) {error_info("Nie ma filmu o tytule: " + title); return created;}

    // 2) Znajdowanie seansu dla tego filmu / sali / godziny
    int sIndex = -1;
    
    for (size_t i = 0; i < screenings.size(); i++) 
    {
        const Screening &s = screenings[i];
        if (s.getMovieId() == movieId && s.getHallId() == hallId && s.getStartTimeMin() == startTimeMin) 
        {
            sIndex = (int)i;
            break;
        }
    }

    if (sIndex == -1) {error_info("Brak seansu dla tego filmu / sali / godziny."); return created;}

    Screening &s = screenings[sIndex];

    // 3) Znajdowanie sali
    int hIndex = findHallIndexById(s.getHallId());
    
    if (hIndex == -1) 
    {
        std::cout << "\nBłąd: Brak sali.\n\n";
        return created;
    }

    Hall &h = halls[hIndex];

    // 4) Znajdowanie / tworzenie klienta
    int cIndex = findClientIndexByNickname(nickname);
    
    if (cIndex == -1) 
    {
        Client c(nickname);
        clients.push_back(c);
        cIndex = (int)clients.size() - 1;
    }
    
    Client &client = clients[cIndex];

    // 5) Rezerwacja miejsc i tworzenie biletów
    int seatsProvided = (int)seats.size();
    
    for (int i = 0; i < count; i++) 
    {
        std::string seatLabel;
        
        if (i < seatsProvided) 
        {
            seatLabel = seats[i];
        } 
        else 
        {
            seatLabel = "AUTO";
        }

        int seatIndex = -1;

        if (seatLabel == "AUTO" || seatLabel == "auto") 
        {
            seatIndex = findFirstFreeSeatIndex(s, h);
            
            if (seatIndex == -1) {error_info("Brak wolnych miejsc." + title); break;}
            
            seatLabel = h.indexToSeatLabel(seatIndex);
        } 
        else 
        {
            if (!h.isSeatLabelValid(seatLabel)) {error_info("Niepoprawne miejsce: " + seatLabel); break;}
            
            seatIndex = h.seatLabelToIndex(seatLabel);
            
            if (s.isSeatTaken(seatIndex)) {error_info("Miejsce zajęte: " + seatLabel); break;}
        }

        s.takeSeat(seatIndex);

        Ticket t;
        t.id = getNextTicketId();
        t.screeningId = s.getId();      // ID seansu
        t.seatLabel = seatLabel;
        t.price = s.getPrice();
        t.ownerNickname = nickname;
        t.purchaseTimeMin = currentTimeMin;
        t.status = TicketStatus::Purchased;

        tickets.push_back(t);
        client.addTicketId(t.id);
        money.addIncome(t.price);

        created.push_back(t.id);
    }

    return created;
}



void Cinema::requestRefund(const std::string &nickname, int ticketId)
{
    if (!dayActive) {error_info("Zwroty możliwe tylko w trakcie dnia."); return;}

    int tIndex = findTicketIndexById(ticketId);
    
    if (tIndex == -1) {error_info("Brak biletu."); return;}
    Ticket &t = tickets[tIndex];
    
    if (t.ownerNickname != nickname) {error_info("To nie jest Twój bilet."); return;}
    
    if (t.status != TicketStatus::Purchased) {error_info("Bilet nie jest w stanie Purchased."); return;}

    int sIndex = findScreeningIndexById(t.screeningId);
    
    if (sIndex == -1) {error_info("Brak seansu."); return;}
    
    Screening &s = screenings[sIndex];

    int minutesToStart = s.getStartTimeMin() - currentTimeMin;
    
    if (minutesToStart <= REFUND_LIMIT_MIN) {error_info("Zbyt późno na zwrot biletu (<= 60 min do seansu)."); return;}

    t.status = TicketStatus::RefundRequested;
    std::cout << "\nPomyślnie zawnioskowano o zwrot biletu. Czeka on teraz na decyzję administratora.\n\n";
}

void Cinema::rateMovie(const std::string &nickname, const std::string &title, int stars)
{
    // 1) Znajdowanie filmu po tytule
    int mIndex = -1;
    
    for (size_t i = 0; i < movies.size(); i++) 
    {
        if (movies[i].getTitle() == title) 
        {
            mIndex = (int)i;
            break;
        }
    }
    
    if (mIndex == -1) {error_info("Brak filmu o tytule: " + title); return;}
    
    int movieId = movies[mIndex].getId();

    // 2) Znajdowanie klienta
    int cIndex = findClientIndexByNickname(nickname);
    
    if (cIndex == -1) {error_info("Nie znaleziono klienta."); return;}
    
    Client &c = clients[cIndex];

    // 3) Sprawdzenie, czy kiedykolwiek miał bilet na ten film
    bool hasTicket = false;
    const std::vector<int> &ids = c.getTicketIds();

    for (size_t i = 0; i < ids.size(); i++) 
    {
        int tIndex = findTicketIndexById(ids[i]);
        if (tIndex == -1) continue;
        const Ticket &t = tickets[tIndex];

        // Odczywanie filmu z ID seansu zapisanym na bilecie
        int sIndex = findScreeningIndexById(t.screeningId);
        if (sIndex == -1) continue;

        if (screenings[sIndex].getMovieId() == movieId) 
        {
            hasTicket = true;
            break;
        }
    }

    if (!hasTicket) {error_info("Nie masz żadnego biletu na film: " + title); return;}

    // 4) Zapisywanie oceny (1-5)
    movies[mIndex].addOrUpdateRating(nickname, stars);
    std::cout << "\nOcena zapisana. Dziękujemy!\n\n";
}

// ====== operacje admina ======

void Cinema::decideRefund(int ticketId, bool approve)
{
    if (!dayActive) {error_info("Zwroty tylko w trakcie aktywnego dnia."); return;}

    int tIndex = findTicketIndexById(ticketId);
    
    if (tIndex == -1) {error_info("Brak biletu."); return;}
    
    Ticket &t = tickets[tIndex];

    if (t.status != TicketStatus::RefundRequested) {error_info("Ten bilet nie oczekuje na zwrot."); return;}

    int sIndex = findScreeningIndexById(t.screeningId);
    
    if (sIndex == -1) {error_info("Brak seansu."); return;}
    
    Screening &s = screenings[sIndex];

    int hIndex = findHallIndexById(s.getHallId());
    
    if (hIndex == -1) {error_info("Brak sali."); return;}
    
    Hall &h = halls[hIndex];

    if (approve) 
    {
        int seatIndex = h.seatLabelToIndex(t.seatLabel);
        s.freeSeat(seatIndex);
        t.status = TicketStatus::Refunded;
        money.addExpense(t.price);
        std::cout << "\nPomyślnie zaakceptowano zwrot biletu.\n\n";
    } 
    else 
    {
        t.status = TicketStatus::RefundRejected;
        std::cout << "\nPomyślnie odrzucono prośbę o zwrot biletu.\n\n";
    }
}

// ====== zapisywanie / odczyt z pliku =======

static std::vector<std::string> splitByChar(const std::string &s, char delim)
{
    std::vector<std::string> out;
    std::string part;
    std::stringstream ss(s);
    
    while (std::getline(ss, part, delim)) 
    {
        out.push_back(part);
    }
    
    return out;
}

static int toIntSafe(const std::string &s)
{
    return std::atoi(s.c_str());
}

static double toDoubleSafe(std::string s)
{
    
    for (size_t i = 0; i < s.size(); i++) 
    {
        if (s[i] == ',') s[i] = '.';
    }
    
    return std::atof(s.c_str());
}

bool Cinema::saveToFiles(const std::string &dir) const
{
    std::filesystem::create_directories(dir);

    // 1) meta.txt: dayActive | currentTimeMin | movieIdCounter | screeningIdCounter | ticketIdCounter | balance | dailyIncome
    {
        std::ofstream f(dir + "/meta.txt");
        if (!f.is_open()) return false;

        f << (dayActive ? 1 : 0) << "|"
          << currentTimeMin << "|"
          << movieIdCounter << "|"
          << screeningIdCounter << "|"
          << ticketIdCounter << "|"
          << money.getBalance() << "|"
          << money.getDailyIncome()
          << "\n";
    }

    // 2) halls.txt: hallId | rows | cols
    {
        std::ofstream f(dir + "/halls.txt");
        if (!f.is_open()) return false;

        for (size_t i = 0; i < halls.size(); i++) 
        {
            f << halls[i].getId() << "|"
              << halls[i].getRows() << "|"
              << halls[i].getCols() << "\n";
        }
    }

    // 3) movies.txt: movieId | title | duration | age_rating
    {
        std::ofstream f(dir + "/movies.txt");
        if (!f.is_open()) return false;

        for (size_t i = 0; i < movies.size(); i++) 
        {
            f << movies[i].getId() << "|"
              << movies[i].getTitle() << "|"
              << movies[i].getDuration() << "|"
              << movies[i].getAgeRating() << "\n";
        }
    }

    // 4) ratings.txt: movieId | nickname | stars
    {
        std::ofstream f(dir + "/ratings.txt");
        if (!f.is_open()) return false;

        for (size_t i = 0; i < movies.size(); i++) 
        {
            const Movie &m = movies[i];
            const std::vector<Movie::Rating> &rs = m.getRatings();

            for (size_t j = 0; j < rs.size(); j++) 
            {
                f << m.getId() << "|"
                  << rs[j].nickname << "|"
                  << rs[j].stars << "\n";
            }
        }
    }

    // 5) screenings.txt: screeningId | movieId | hallId | startTimeMin | price | takenSeats
    {
        std::ofstream f(dir + "/screenings.txt");
        if (!f.is_open()) return false;

        for (size_t i = 0; i < screenings.size(); i++) 
        {
            const Screening &s = screenings[i];

            int hIndex = findHallIndexById(s.getHallId());
            int cap = 0;
            if (hIndex != -1) cap = halls[hIndex].getCapacity();

            f << s.getId() << "|"
              << s.getMovieId() << "|"
              << s.getHallId() << "|"
              << s.getStartTimeMin() << "|"
              << s.getPrice() << "|";

            bool first = true;
            for (int seat = 0; seat < cap; seat++) 
            {
                if (s.isSeatTaken(seat)) 
                {
                    if (!first) f << ",";
                    f << seat;
                    first = false;
                }
            }
            f << "\n";
        }
    }

    // 6) tickets.txt: ticketId | screeningId | seatLabel | price | ownerNickname | purchaseTimeMin | status
    {
        std::ofstream f(dir + "/tickets.txt");
        if (!f.is_open()) return false;

        for (size_t i = 0; i < tickets.size(); i++) 
        {
            const Ticket &t = tickets[i];
            f << t.id << "|"
              << t.screeningId << "|"
              << t.seatLabel << "|"
              << t.price << "|"
              << t.ownerNickname << "|"
              << t.purchaseTimeMin << "|"
              << (int)t.status << "\n";
        }
    }

    return true;
}

bool Cinema::loadFromFiles(const std::string &dir)
{
    halls.clear();
    movies.clear();
    screenings.clear();
    tickets.clear();
    clients.clear();

    dayActive = false;
    currentTimeMin = 0;
    movieIdCounter = 0;
    screeningIdCounter = 0;
    ticketIdCounter = 0;
    money.setBalance(0.0);
    money.setDailyIncome(0.0);

    // 1) meta.txt
    {
        std::ifstream f(dir + "/meta.txt");
        
        if (f.is_open()) 
        {
            std::string line;
            
            if (std::getline(f, line)) 
            {
                std::vector<std::string> p = splitByChar(line, '|');
                
                if (p.size() >= 7) 
                {
                    dayActive = (toIntSafe(p[0]) != 0);
                    currentTimeMin = toIntSafe(p[1]);
                    movieIdCounter = toIntSafe(p[2]);
                    screeningIdCounter = toIntSafe(p[3]);
                    ticketIdCounter = toIntSafe(p[4]);
                    money.setBalance(toDoubleSafe(p[5]));
                    money.setDailyIncome(toDoubleSafe(p[6]));
                }
            }
        }
    }

    // 2) halls.txt
    {
        std::ifstream f(dir + "/halls.txt");
        
        if (f.is_open()) 
        {
            std::string line;
            
            while (std::getline(f, line)) 
            {
                if (line.size() == 0) continue;
                std::vector<std::string> p = splitByChar(line, '|');
                if (p.size() < 3) continue;

                std::string hallId = p[0];
                int rows = toIntSafe(p[1]);
                int cols = toIntSafe(p[2]);

                halls.push_back(Hall(hallId, rows, cols));
            }
        }
    }

    // 3) movies.txt
    {
        std::ifstream f(dir + "/movies.txt");
        
        if (f.is_open()) 
        {
            std::string line;
            
            while (std::getline(f, line)) 
            {
                if (line.size() == 0) continue;
                std::vector<std::string> p = splitByChar(line, '|');
                if (p.size() < 4) continue;

                int id = toIntSafe(p[0]);
                std::string title = p[1];
                int dur = toIntSafe(p[2]);
                int age = toIntSafe(p[3]);

                movies.push_back(Movie(id, title, dur, age));
            }
        }
    }

    // 4) ratings.txt
    {
        std::ifstream f(dir + "/ratings.txt");
        
        if (f.is_open()) 
        {
            std::string line;
            
            while (std::getline(f, line)) 
            {
                if (line.size() == 0) continue;
                std::vector<std::string> p = splitByChar(line, '|');
                if (p.size() < 3) continue;

                int movieId = toIntSafe(p[0]);
                std::string nick = p[1];
                int stars = toIntSafe(p[2]);

                int mIndex = findMovieIndexById(movieId);
                
                if (mIndex != -1) 
                {
                    movies[mIndex].addOrUpdateRating(nick, stars);
                }
            }
        }
    }

    // 5) screenings.txt
    {
        std::ifstream f(dir + "/screenings.txt");
        
        if (f.is_open()) 
        {
            std::string line;
            
            while (std::getline(f, line)) 
            {
                if (line.size() == 0) continue;
                std::vector<std::string> p = splitByChar(line, '|');
                if (p.size() < 6) continue;

                int id = toIntSafe(p[0]);
                int movieId = toIntSafe(p[1]);
                std::string hallId = p[2];
                int startMin = toIntSafe(p[3]);
                double price = toDoubleSafe(p[4]);
                std::string taken = p[5];

                Screening s(id, movieId, hallId, startMin, price);
                screenings.push_back(s);

                // wczytaj zajęte miejsca
                if (taken.size() > 0) 
                {
                    std::vector<std::string> seats = splitByChar(taken, ',');
                    
                    for (size_t i = 0; i < seats.size(); i++) 
                    {
                        int seatIndex = toIntSafe(seats[i]);
                        screenings.back().takeSeat(seatIndex);
                    }
                }
            }
        }
    }

    // 6) tickets.txt
    {
        std::ifstream f(dir + "/tickets.txt");
        
        if (f.is_open()) 
        {
            std::string line;
            
            while (std::getline(f, line)) 
            {
                if (line.size() == 0) continue;
                std::vector<std::string> p = splitByChar(line, '|');
                if (p.size() < 7) continue;

                Ticket t;
                t.id = toIntSafe(p[0]);
                t.screeningId = toIntSafe(p[1]);
                t.seatLabel = p[2];
                t.price = toDoubleSafe(p[3]);
                t.ownerNickname = p[4];
                t.purchaseTimeMin = toIntSafe(p[5]);
                t.status = (TicketStatus)toIntSafe(p[6]);

                tickets.push_back(t);
            }
        }
    }

    // 7) Odbudowanie listy klientów z biletów
    {
        clients.clear();

        for (size_t i = 0; i < tickets.size(); i++) 
        {
            const Ticket &t = tickets[i];

            int cIndex = findClientIndexByNickname(t.ownerNickname);
            
            if (cIndex == -1) 
            {
                clients.push_back(Client(t.ownerNickname));
                cIndex = (int)clients.size() - 1;
            }
            
            clients[cIndex].addTicketId(t.id);
        }
    }

    return true;
}

// ====== pomocnicze (gettery i indeksowanie) ======

int Cinema::findHallIndexById(const std::string &hallId) const
{
    for (size_t i = 0; i < halls.size(); i++) 
    {
        if (halls[i].getId() == hallId) return (int)i;
    }
    
    return -1;
}

int Cinema::findMovieIndexById(int movieId) const
{
    for (size_t i = 0; i < movies.size(); i++) 
    {
        if (movies[i].getId() == movieId) return (int)i;
    }
    
    return -1;
}

int Cinema::findScreeningIndexById(int screeningId) const
{
    for (size_t i = 0; i < screenings.size(); i++) 
    {
        if (screenings[i].getId() == screeningId) return (int)i;
    }
    
    return -1;
}

int Cinema::findClientIndexByNickname(const std::string &nickname) const
{
    for (size_t i = 0; i < clients.size(); i++) 
    {
        if (clients[i].getNickname() == nickname) return (int)i;
    }
    
    return -1;
}

int Cinema::findTicketIndexById(int ticketId) const
{
    for (size_t i = 0; i < tickets.size(); i++) 
    {
        if (tickets[i].id == ticketId) return (int)i;
    }
    
    return -1;
}

int Cinema::getNextMovieId()
{
    movieIdCounter++;
    return movieIdCounter;
}

int Cinema::getNextScreeningId()
{
    screeningIdCounter++;
    return screeningIdCounter;
}

int Cinema::getNextTicketId()
{
    ticketIdCounter++;
    return ticketIdCounter;
}

int Cinema::findFirstFreeSeatIndex(const Screening &s, const Hall &h) const
{
    int cap = h.getCapacity();
    
    for (int i = 0; i < cap; i++) 
    {
        if (!s.isSeatTaken(i)) return i;
    }
    
    return -1;
}
