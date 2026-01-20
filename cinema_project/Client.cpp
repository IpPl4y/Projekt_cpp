#include "Client.h"

Client::Client() {}

Client::Client(const std::string &nickname_)
{
    nickname = nickname_;
}

std::string Client::getNickname() const
{
    return nickname;
}

void Client::addTicketId(int ticketId)
{
    ticketIds.push_back(ticketId);
}

const std::vector<int>& Client::getTicketIds() const
{
    return ticketIds;
}
