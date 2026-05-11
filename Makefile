# Simple Makefile — alternative to CMake
# Usage:  make          (needs raylib installed)
#         make clean

CXX      = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Iinclude
SRCS     = src/main.cpp src/ui.cpp src/screens.cpp src/logic.cpp
TARGET   = library

# ── Platform detection ────────────────────────────────────────────────────────
UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
    # macOS — installed via: brew install raylib
    LDFLAGS = -lraylib -framework IOKit -framework Cocoa -framework OpenGL
else
    # Linux — installed via: sudo apt install libraylib-dev
    LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
