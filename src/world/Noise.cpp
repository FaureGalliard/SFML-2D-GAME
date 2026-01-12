//
// Created by angel on 9/01/2026.
//
#include "Noise.h"
#include <algorithm>
#include <random>
static int fastfloor(float x) {
    int xi = (int)x;
    return x < xi ? xi - 1 : xi;
}

static float dot(const int* g, float x, float y) {
    return g[0] * x + g[1] * y;
}
static const int grad3[12][2] = {
    {1,1},{-1,1},{1,-1},{-1,-1},
    {1,0},{-1,0},{1,0},{-1,0},
    {0,1},{0,-1},{0,1},{0,-1}
};
Noise::Noise(uint32_t seed) {
    std::array<int, 256> p;
    for (int i = 0; i < 256; ++i)
        p[i] = i;

    std::mt19937 rng(seed);
    std::shuffle(p.begin(), p.end(), rng);

    for (int i = 0; i < 512; ++i)
        perm[i] = p[i & 255];
}

float Noise::simplex(float xin, float yin) const {

    const float F2 = 0.366025403f;
    const float G2 = 0.211324865f;

    float n0, n1, n2;

    float s = (xin + yin) * F2;
    int i = fastfloor(xin + s);
    int j = fastfloor(yin + s);

    float t = (i + j) * G2;
    float X0 = i - t;
    float Y0 = j - t;
    float x0 = xin - X0;
    float y0 = yin - Y0;

    int i1, j1;
    if (x0 > y0) { i1 = 1; j1 = 0; }
    else         { i1 = 0; j1 = 1; }

    float x1 = x0 - i1 + G2;
    float y1 = y0 - j1 + G2;
    float x2 = x0 - 1.0f + 2.0f * G2;
    float y2 = y0 - 1.0f + 2.0f * G2;

    int ii = perm[i & 255];
    int jj = perm[j & 255];

    float t0 = 0.5f - x0*x0 - y0*y0;
    if (t0 < 0) n0 = 0.0f;
    else {
        t0 *= t0;
        n0 = t0 * t0 * dot(grad3[perm[ii + perm[jj]] % 12], x0, y0);
    }

    float t1 = 0.5f - x1*x1 - y1*y1;
    if (t1 < 0) n1 = 0.0f;
    else {
        t1 *= t1;
        n1 = t1 * t1 * dot(grad3[perm[ii + i1 + perm[jj + j1]] % 12], x1, y1);
    }

    float t2 = 0.5f - x2*x2 - y2*y2;
    if (t2 < 0) n2 = 0.0f;
    else {
        t2 *= t2;
        n2 = t2 * t2 * dot(grad3[perm[ii + 1 + perm[jj + 1]] % 12], x2, y2);
    }

    return 70.0f * (n0 + n1 + n2);
}