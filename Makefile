# simpleI Makefile
# Run 'make' to build, 'make clean' to remove binary, 'make install' to install

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = simpleI
SRC = simpleI.cpp
PREFIX = /usr/local

.PHONY: all clean install uninstall

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

install: $(TARGET)
	install -d $(DESTDIR)$(PREFIX)/bin
	install -m 755 $(TARGET) $(DESTDIR)$(PREFIX)/bin/$(TARGET)

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(TARGET)
