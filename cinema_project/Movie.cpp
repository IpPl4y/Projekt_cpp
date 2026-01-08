#include "Movie.h"

Movie::Movie() {
    id = 0;
    durationMin = 0;
    ageRating = 0;
}

Movie::Movie(int id_, const std::string &t, int dur, int age)
{
    id = id_;
    title = t;
    durationMin = dur;
    ageRating = age;
}

int Movie::getId() const {
    return id;
}

std::string Movie::getTitle() const {
    return title;
}

int Movie::getDuration() const {
    return durationMin;
}

int Movie::getAgeRating() const {
    return ageRating;
}

void Movie::addOrUpdateRating(const std::string &nickname, int stars)
{
    if (stars < 1) stars = 1;
    if (stars > 5) stars = 5;

    // szukanie, czy ten nick już oceniał
    for (size_t i = 0; i < ratings.size(); i++) {
        if (ratings[i].nickname == nickname) {
            ratings[i].stars = stars;
            return;
        }
    }

    Rating r;
    r.nickname = nickname;
    r.stars = stars;
    ratings.push_back(r);
}

double Movie::getAverageRating() const
{
    if (ratings.empty()) return 0.0;

    int sum = 0;
    for (size_t i = 0; i < ratings.size(); i++) {
        sum += ratings[i].stars;
    }
    return (double)sum / (double)ratings.size();
}

int Movie::getRatingsCount() const
{
    return (int)ratings.size();
}

const std::vector<Movie::Rating>& Movie::getRatings() const
{
    return ratings;
}
