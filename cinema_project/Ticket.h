#ifndef TICKET_H
#define TICKET_H

#include <string>

/*
Status biletu w procesie zakupu/zwrotu
Przechowywany jako liczba (0..3), co ułatwia zapis/odczyt do pliku
*/

enum TicketStatus {
    TICKET_PURCHASED        = 0,  // bilet aktywny (kupiony)
    TICKET_REFUND_REQUESTED = 1,  // klient poprosił o zwrot (czeka na decyzję admina)
    TICKET_REFUNDED         = 2,  // admin zaakceptował zwrot (pieniądze oddane, miejsce zwolnione)
    TICKET_REFUND_REJECTED  = 3   // admin odrzucił zwrot
};

/*
 Pojedynczy bilet kupiony na konkretny seans i miejsce
 seatLabel to etykieta z UI (np. "1A"), a screeningId łączy bilet z obiektem Screening
 purchaseTimeMin to czas zakupu (minuty od 00:00 w ramach symulowanego dnia)
 */

struct Ticket {
    int id;
    int screeningId;
    std::string seatLabel;
    double price;
    std::string ownerNickname;
    int purchaseTimeMin;
    TicketStatus status;

    // Konstruktor domyślny (wartości startowe, status ustawiony na PURCHASED)
    Ticket() 
    {
        id = 0;
        screeningId = 0;
        price = 0.0;
        purchaseTimeMin = 0;
        status = TICKET_PURCHASED;
    }
};

#endif

