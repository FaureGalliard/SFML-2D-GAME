//
// Created by angel on 9/01/2026.
//
#include "Noise.h"

float Noise::value(int x, int y, int seed) {
    int n = x + y * 57 + seed * 131;
    n = (n << 13) ^ n;
    int nn = (n * (n * n * 15731 + 789221) + 1376312589);
    return 1.f - ((nn & 0x7fffffff) / 1073741824.f);
}
