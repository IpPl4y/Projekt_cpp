#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <vector>

/*
Reprezentuje klienta kina w systemie.

- klient jest identyfikowany po nickname'ie (string).
- obiekt przechowuje tylko listę ID biletów przypisanych do klienta;
- szczegóły biletów znajdują się w strukturze Ticket i wektorze tickets w Cinema.
*/

class Client {
    public:
        Client();
        Client(const std::string &nickname_);

        // Zwraca nickname klienta (identyfikator w systemie)
        std::string getNickname() const;

        // Dodaje ID biletu do historii klienta
        void addTicketId(int ticketId);
        
        // Zwraca listę ID biletów klienta (tylko do odczytu)
        const std::vector<int>& getTicketIds() const;

    private:
        std::string nickname;
        std::vector<int> ticketIds;
};

#endif