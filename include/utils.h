#pragma once

#include <random>

int numeroAleatorio(int min, int max) {
    static std::random_device rd;   // semilla
    static std::mt19937 gen(rd());  // generador Mersenne Twister
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}