#!/bin/bash
g++ src/main.cpp src/cryptography.cpp -lbotan-3 -lsfml-system -lsfml-window -lsfml-network -I./deps/include