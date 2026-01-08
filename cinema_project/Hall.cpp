#include "Hall.h"
#include <cctype>

Hall::Hall() 
{
    rows = 0;
    cols = 0;
}

Hall::Hall(const std::string &id_, int rows_, int cols_)
{
    id = id_;
    rows = rows_;
    cols = cols_;
}

std::string Hall::getId() const 
{
    return id;
}

int Hall::getRows() const 
{
    return rows;
}

int Hall::getCols() const 
{
    return cols;
}

int Hall::getCapacity() const 
{
    return rows * cols;
}

bool Hall::isSeatLabelValid(const std::string &label) const
{
    if (label.size() < 2) return false;

    int i = 0;
    
    while (i < (int)label.size() && std::isdigit((unsigned char)label[i])) 
    {
        i++;
    }
    
    if (i == 0 || i >= (int)label.size()) return false;

    int row = 0;
    
    for (int j = 0; j < i; j++) 
    {
        if (!std::isdigit((unsigned char)label[j])) return false;
        row = row * 10 + (label[j] - '0');
    }
    
    char letter = label[i];
    if (!std::isalpha((unsigned char)letter)) return false;

    int col = std::toupper(letter) - 'A' + 1;
    if (row < 1 || row > rows) return false;
    if (col < 1 || col > cols) return false;

    return true;
}

int Hall::seatLabelToIndex(const std::string &label) const
{
    // Założenie: isSeatLabelValid zostało sprawdzone
    int i = 0;
    int row = 0;
    
    while (i < (int)label.size() && std::isdigit((unsigned char)label[i])) 
    {
        row = row * 10 + (label[i] - '0');
        i++;
    }
    
    char letter = label[i];
    int col = std::toupper(letter) - 'A' + 1;

    int row0 = row - 1;
    int col0 = col - 1;
    return row0 * cols + col0;
}

std::string Hall::indexToSeatLabel(int index) const
{
    if (index < 0 || index >= rows * cols) return "";

    int row0 = index / cols;
    int col0 = index % cols;

    int row = row0 + 1;
    char letter = (char)('A' + col0);

    return std::to_string(row) + letter;
}
