# Compiler
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -O2 -I.

# Project
TARGET = zentop
SRC_DIRS = . core utils ui monitors
SOURCES = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))
OBJECTS = $(SOURCES:.cpp=.o)

# Install paths
PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin

.PHONY: all clean run install uninstall install-user uninstall-user setup

all: $(TARGET)

# Link
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJECTS) $(TARGET)

# Run local binary
run: all
	./$(TARGET)

# System wide install 
install: all
	install -d $(DESTDIR)$(BINDIR)
	install -m 755 $(TARGET) $(DESTDIR)$(BINDIR)/$(TARGET)

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/$(TARGET)

# User local install
install-user: all
	install -d $(HOME)/.local/bin
	install -m 755 $(TARGET) $(HOME)/.local/bin/$(TARGET)

uninstall-user:
	rm -f $(HOME)/.local/bin/$(TARGET)

# One command setup for most users
setup: install-user