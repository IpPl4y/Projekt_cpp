#include "CinemaApp.h"

#include <iostream>
#include <sstream>
#include <cstdlib>   // atoi, atof

CinemaApp::CinemaApp() 
{
    running = true;
    role = "guest";
    clientNickname = "";
    selectedTitle = "";
    selectedHallId = "";
    selectedStartTimeMin = 0;
    hasSelectedShow = false;

}

void CinemaApp::printGlobalHelp() const 
{
    std::cout << "\nSymulator kina - pomoc ogólna\n";
    std::cout << "Obecnie jesteś w trybie gościa (guest). Możliwe działania:\n\n";
    std::cout << "  login admin <hasło>      (zaloguj się do panelu administratora)\n";
    std::cout << "  login client <nick>      (zaloguj się jako wybrany klient)\n";
    std::cout << "  exit                     (zamknięcie programu)\n\n";

    std::cout << "Po zalogowaniu jako admin / klient użyj -help, aby zobaczyć dostępne komendy.\n\n";
}

void CinemaApp::printAdminHelp() const 
{
    std::cout << "\nAdministrator - dostępne komendy:\n";
    std::cout << "  help / -help                                                  (Wypisanie listy dostępnych komend administratora.)\n";
    std::cout << "  add_hall <id> <rows> <cols>                                   (dodawanie sali w formacie: ID sali - rzędy - kolumny, np. Alfa 5 6 - sala Alfa z 5x6 = 30 miejsc.)\n";                                  
    std::cout << "  add_movie <title> <durationMin> <ageRating>                   (dodawanie filmu w formacie: tytuł - czas trwania - ograniczenie wiekowe (tytuł - 1 słowo, np. Finding_Nemo).)\n";
    std::cout << "  add_screening <title> <hallId> <HH:MM> <price>                (Seans na dany dzień: tytuł - sala - godz. rozpoczęcia - cena (maks. 1 film na salę).)\n";
    std::cout << "  start_day                                                     (Uruchamia dzień - można kupować i zwracać bilety, zaczyna płynąć czas.)\n";
    std::cout << "  end_day                                                       (Kończy dzień.)\n";
    std::cout << "  halls                                                         (Wypisanie listy sal.)\n";
    std::cout << "  movies                                                        (Wypisanie listy filmów z średnią oceną i liczbą ocen.)\n";
    std::cout << "  repertoire                                                    (Wypisanie seansów na dziś: ID seansu - tytuł - sala - godzina - cena - liczba wolnych miejsc.)\n";
    std::cout << "  refunds                                                       (Lista biletów, które klienci zgłosili do zwrotu i czekają na decyzję.)\n";
    std::cout << "  decide_refund <ticketId> <0/1>                                (Podjdęcie decyzji o zwrocie: 1 = zaakceptuj, 0 = odrzuć.)\n";
    std::cout << "  time <minutes>                                                (Przesunięcie symulowanego czasu dnia o podaną liczbę minut.)\n";
    std::cout << "  money                                                         (Pokazuje stan konta i dzienny zarobek (w formacie: Balans konta, Dzisiejszy przychód).)\n";
    std::cout << "  logout                                                        (Wylogowanie z panelu administratora - powrót do konta [guest].)\n\n";
}

void CinemaApp::printClientHelp() const
{
    std::cout << "\nKlient -  dostępne komendy:\n";
    std::cout << "  help / -help                                                  (Wypisanie listy dostępnych komend klienta.)\n";
    std::cout << "  repertoire                                                    (Wypisanie seansów na dziś: ID seansu - tytuł - sala - godzina - cena - liczba wolnych miejsc.)\n";
    std::cout << "  select_show <title> <hallId> <HH:MM>                          (Wybór filmu do zakupu biletu w formacie: film - ID sali - godzina rozpoczęcia.)\n";                      
    std::cout << "  buy <count> [lista_miejsc...]  (dla wybranego seansu)         (Kupno biletu na film (po wybraniu filmu komenda select_show).)\n";     
    std::cout << "                                                                 np: select_show Shrek Alfa 18:30 - wybór filmu Shrek o 18:30 w sali Alfa.\n";
    std::cout << "                                                                 np. buy 3 1A 1B 1C - zakup trzech biletów na wybrany film (przy braku podania miejsc, przydzielane automatycznie).)\n";
    std::cout << "  mytickets                                                     (Wypisanie wszystkich biletów danego klienta (ID, seans, miejsce, cena, status)\n";
    std::cout << "  refund <ticketId>                                             (Zgłoszenie prośby o zwrot biletu. Warunki: Dzień musi być aktywny, do seansu pozostało >= 60 min.)\n";
    std::cout << "  rate <title> <1-5>                                            (Ocena filmu w skali od 1 do 5, np. rate Shrek 5 (warunek: na film musiał zostać kupiony bilet).)\n";
    std::cout << "  time <minutes>                                                (Przesunięcie symulowanego czasu dnia o podaną liczbę minut.)\n";                 
    std::cout << "  logout                                                        (Wylogowanie z panelu klienta - powrót do konta [guest].)\n\n";
}

std::vector<std::string> CinemaApp::splitLine(const std::string &line) const
{
    std::vector<std::string> out;
    std::stringstream ss(line);
    std::string token;
    
    while (ss >> token) 
    {
        out.push_back(token);
    }
    
    return out;
}

void CinemaApp::handleGuestCommand(const std::string &cmd, const std::vector<std::string> &args)
{
    if (cmd == "help" || cmd == "-help") 
    {
        printGlobalHelp();
        return;
    }

    if (cmd == "login") 
    {
        if (args.size() < 3) 
        {
            std::cout << "\nUżycie: login admin <haslo> / login client <nick>\n\n";
            return;
        }

        std::string who = args[1];

        if (who == "admin") 
        {
            std::string pass = args[2];
            if (pass == "admin") 
            {
                role = "admin";
                clientNickname = "";
                std::cout << "\nWitamy! Zalogowano jako administrator.\n";
                std::cout << "W celu uzyskania pomocy z dostępnymi komendami napisz -help.\n\n";
            } 
            else 
            {
                std::cout << "\nNiepoprawne hasło. Spróbuj ponownie.\n\n";
            }
        } 
        else if (who == "client") 
        {
            role = "client";
            clientNickname = args[2];
            std::cout << "\nWitamy! Zalogowano jako klient: " << clientNickname << "\n";
            std::cout << "W celu uzyskania pomocy z dostępnymi komendami napisz -help.\n\n";
        } 
        else 
        {
            std::cout << "\nNieznany typ logowania. Spróbuj zalogować się jako administrator lub klient.\n\n";
        }
        return;
    }

    std::cout << "\nNieznana komenda. Zaloguj się jako administrator / klient lub zakończ program.\n\n";
}

void CinemaApp::handleAdminCommand(const std::string &cmd, const std::vector<std::string> &args)
{
    if (cmd == "help" || cmd == "-help") 
    {
        printAdminHelp();
        return;
    }

    if (cmd == "logout") 
    {
        role = "guest";
        clientNickname = "";
        std::cout << "\nWylogowano.\n\n";
        return;
    }

    if (cmd == "add_hall") 
    {
        if (args.size() != 4) 
        {
            std::cout << "\nUżycie: add_hall <id> <rows> <cols>\n\n";
        } 
        else 
        {
            std::string id = args[1];
            int rows = std::atoi(args[2].c_str());
            int cols = std::atoi(args[3].c_str());
            cinema.addHall(id, rows, cols);
            std::cout << "\nDodano halę " << id << " o wymiarach " << rows << "x" << cols << "\n\n";
        }
        return;
    }

    if (cmd == "add_movie") 
    {
        if (args.size() != 4) 
        {
            std::cout << "\nUżycie: add_movie <title_bez_spacji> <durationMin> <ageRating>\n";
            std::cout << "(Tytuł podaj bez spacji, a z podkreśleniem, np. Finding_Nemo.)\n\n";
        } 
        else 
        {
            std::string title = args[1];
            int dur = std::atoi(args[2].c_str());
            int age = std::atoi(args[3].c_str());
            int id = cinema.addMovie(title, dur, age);
            std::cout << "\nDodano film " << title << " pod numer ID " << id << ".\n\n";
        }
        return;
    }

    if (cmd == "add_screening") 
    {
        if (args.size() != 5) 
        {
            std::cout << "\nUżycie: add_screening <title> <hallId> <HH:MM> <price>\n\n";
        } 
        else 
        {
            std::string title = args[1];
            std::string hallId = args[2];

            std::string hhmm = args[3];
            int pos = (int)hhmm.find(':');
            int hh = 0, mm = 0;
            
            if (pos != -1) 
            {
                hh = std::atoi(hhmm.substr(0, pos).c_str());
                mm = std::atoi(hhmm.substr(pos + 1).c_str());
            }
            
            int startMin = hh * 60 + mm;

            std::string priceStr = args[4];
            
            for (size_t i = 0; i < priceStr.size(); i++) 
            {
                if (priceStr[i] == ',') priceStr[i] = '.';
            }
            
            double price = std::atof(priceStr.c_str());


            int sid = cinema.addScreening(title, hallId, startMin, price);
            
            if (sid != -1) 
            {
                std::cout << "\nDodano seans pod numer ID " << sid << ".\n\n";
            }
        }
        return;
    }

    if (cmd == "start_day") 
    {
        cinema.startDay();
        return;
    }

    if (cmd == "end_day") 
    {
        cinema.endDay();
        return;
    }

    if (cmd == "halls") 
    {
        cinema.printHalls();
        return;
    }

    if (cmd == "movies") 
    {
        cinema.printMovies();
        return;
    }

    if (cmd == "repertoire") 
    {
        cinema.printRepertoire();
        return;
    }

    if (cmd == "refunds") 
    {
        cinema.printRefundQueue();
        return;
    }

    if (cmd == "decide_refund") 
    {
        if (args.size() != 3) 
        {
            std::cout << "\nUżycie: decide_refund <ticketId> <0/1>\n\n";
        } 
        else 
        {
            int ticketId = std::atoi(args[1].c_str());
            int flag = std::atoi(args[2].c_str());
            bool approve = (flag != 0);
            cinema.decideRefund(ticketId, approve);
        }
        return;
    }

    if (cmd == "time") 
    {
        if (args.size() != 2) 
        {
            std::cout << "\nUżycie: time <minutes>\n\n";
        } 
        else 
        {
            int m = std::atoi(args[1].c_str());
            cinema.advanceTime(m);
            std::cout << "Czas (min): " << cinema.getCurrentTime() << "\n";
        }
        return;
    }

    if (cmd == "money") 
    {
        const Money &mon = cinema.getMoney();
        std::cout << "\nPodsumowanie przychodów kina:\n";
        std::cout << "Balans konta = " << mon.getBalance() << ", Dzisiejszy przychód = " << mon.getDailyIncome() << "\n\n";
        return;
    }

    std::cout << "\nNieznana komenda administratora. Użyj komendy -help.\n\n";
}

void CinemaApp::handleClientCommand(const std::string &cmd, const std::vector<std::string> &args)
{
    if (cmd == "help" || cmd == "-help") 
    {
        printClientHelp();
        return;
    }

    if (cmd == "logout") 
    {
        role = "guest";
        clientNickname = "";

        // Wyczyszczenie wyboru seansu
        hasSelectedShow = false;
        selectedTitle = "";
        selectedHallId = "";
        selectedStartTimeMin = 0;

        std::cout << "\nWylogowano.\n\n";
        return;
    }


    if (cmd == "repertoire") 
    {
        cinema.printRepertoire();
        return;
    }

    if (cmd == "select_show")  
    {
        if (args.size() != 4) 
        {
            std::cout << "\nUżycie: select_show <title> <hallId> <HH:MM>\n\n";
        } 
        else 
        {
            std::string title = args[1];
            std::string hallId = args[2];

            std::string hhmm = args[3];
            int pos = (int)hhmm.find(':');
            int hh = 0, mm = 0;
            
            if (pos != -1) 
            {
                hh = std::atoi(hhmm.substr(0, pos).c_str());
                mm = std::atoi(hhmm.substr(pos + 1).c_str());
            }
            
            int startMin = hh * 60 + mm;

            // Zapisanie wyboru seansu
            selectedTitle = title;
            selectedHallId = hallId;
            selectedStartTimeMin = startMin;
            hasSelectedShow = true;

            // Pokazanie mapy dostępnych miejsc
            cinema.printSeatsByTitle(title, hallId, startMin);
            std::cout << "Seans został wybrany. Teraz użyj: buy <count> [miejsca...]\n\n";
        }
        return;
    }

    if (cmd == "buy") 
    {
        if (!hasSelectedShow) 
        {
            std::cout << "\nNajpierw wybierz seans: select_show <title> <hallId> <HH:MM>\n\n";
            return;
        }

        if (args.size() < 2) 
        {
            std::cout << "\nUżycie: buy <count> [lista_miejsc...]\n\n";
        } 
        else 
        {
            int count = std::atoi(args[1].c_str());

            std::vector<std::string> seats;
            
            for (size_t i = 2; i < args.size(); i++) 
            {
                seats.push_back(args[i]);
            }

            std::vector<int> ids = cinema.buyTickets(
                clientNickname,
                selectedTitle,
                selectedHallId,
                selectedStartTimeMin,
                count,
                seats
            );

            if (!ids.empty()) 
            {
                std::cout << "\nKupiono bilety, przypisane im numery ID: ";
            
                for (size_t i = 0; i < ids.size(); i++) 
                {
                    std::cout << ids[i] << " ";
                }
                
                std::cout << "\n\n";
            }
        }
        return;
    }   

    if (cmd == "mytickets") 
    {
        cinema.printClientTickets(clientNickname);
        return;
    }

    if (cmd == "refund") 
    {
        if (args.size() != 2) 
        {
            std::cout << "\nUżycie: refund <ticketId>\n\n";
        } 
        else 
        {
            int ticketId = std::atoi(args[1].c_str());
            cinema.requestRefund(clientNickname, ticketId);
        }
        return;
    }

    if (cmd == "rate") 
    {
        if (args.size() != 3) 
        {
            std::cout << "\nUżycie: rate <title> <1-5>\n\n";
        } 
        else 
        {
            std::string title = args[1];
            int stars = std::atoi(args[2].c_str());
            cinema.rateMovie(clientNickname, title, stars);
        }
        return;
    }

    if (cmd == "time") 
    {
        if (args.size() != 2) 
        {
            std::cout << "\nUżycie: time <minutes>\n\n";
        } 
        else 
        {
            int m = std::atoi(args[1].c_str());
            cinema.advanceTime(m);
            std::cout << "\nCzas (min): " << cinema.getCurrentTime() << "\n\n";
        }
        return;
    }

    std::cout << "\nNieznana komenda klienta. Użyj komendy -help.\n\n";
}

void CinemaApp::run()
{
    // Start aplikacji: wczytanie stanu z katalogu data/ (jeśli brak plików, start od zera)
    cinema.loadFromFiles("data");

    std::cout << "\nWitamy w symulatorze kina! \n";
    std::cout << "Aby otrzymać instrukcję, napisz help lub -help. \n\n";

    while (running) 
    {
        // prompt
        if (role == "guest") 
        {
            std::cout << "[guest]> ";
        } 
        else if (role == "admin") 
        {
            std::cout << "[admin]> ";
        } 
        else 
        {
            std::cout << "[client:" << clientNickname << "]> ";
        }

        std::string line;
        if (!std::getline(std::cin, line)) break;
        if (line.size() == 0) continue;

        std::vector<std::string> args = splitLine(line);
        if (args.empty()) continue;

        std::string cmd = args[0];

        if (cmd == "exit") 
        {
            running = false;
            break;
        }

        if (role == "guest") 
        {
            handleGuestCommand(cmd, args);
        } 
        else if (role == "admin") 
        {
            handleAdminCommand(cmd, args);
        } 
        else 
        {
            handleClientCommand(cmd, args);
        }
    }

    if (!cinema.saveToFiles("data")) 
    {
        std::cout << "\nUwaga: Nie udało się zapisać danych do folderu data.\n\n";
    }


    std::cout << "\nKoniec programu.\n\n";
}
