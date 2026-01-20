#include "Screening.h"

Screening::Screening()
{
    id = 0;
    movieId = 0;
    startTimeMin = 0;
    price = 0.0;
}

Screening::Screening(int id_, int movieId_, const std::string &hallId_, int startTimeMin_, double price_)
{
    id = id_;
    movieId = movieId_;
    hallId = hallId_;
    startTimeMin = startTimeMin_;
    price = price_;
}

int Screening::getId() const { return id; }
int Screening::getMovieId() const { return movieId; }
std::string Screening::getHallId() const { return hallId; }
int Screening::getStartTimeMin() const { return startTimeMin; }
double Screening::getPrice() const { return price; }

bool Screening::isSeatTaken(int seatIndex) const
{
    if (seatIndex < 0) return false;
    if (seatIndex >= (int)takenSeatsFlags.size()) return false;
    return takenSeatsFlags[seatIndex] == 1;
}

void Screening::takeSeat(int seatIndex)
{
    if (seatIndex < 0) return;
    
    if (seatIndex >= (int)takenSeatsFlags.size()) 
    {
        takenSeatsFlags.resize(seatIndex + 1, 0);
    }
    
    takenSeatsFlags[seatIndex] = 1;
}

void Screening::freeSeat(int seatIndex)
{
    if (seatIndex < 0) return;
    if (seatIndex >= (int)takenSeatsFlags.size()) return;
    takenSeatsFlags[seatIndex] = 0;
}

int Screening::getTakenSeatsCount() const
{
    int count = 0;
    
    for (size_t i = 0; i < takenSeatsFlags.size(); i++) 
    {
        if (takenSeatsFlags[i] == 1) count++;
    }
    
    return count;
}

void Screening::clearSeats()
{
    takenSeatsFlags.clear();
}
