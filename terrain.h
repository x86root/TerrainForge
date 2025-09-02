#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
using namespace std;

class Terrain {
public:
    int height, width;
    vector<vector<float>> heightMap;

    Terrain(int h, int w);
    void terraGenerator(float offsetX, float offsetY);
};

#endif