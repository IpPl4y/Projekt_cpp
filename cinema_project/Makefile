CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

TARGET = cinema

SRCS = main.cpp \
       CinemaApp.cpp \
       Cinema.cpp \
       Movie.cpp \
       Hall.cpp \
       Screening.cpp \
       Client.cpp \
       Money.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean

