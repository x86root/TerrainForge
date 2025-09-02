#include "terrain.h"
#include "FastNoiseLite.h"

Terrain::Terrain(int h, int w) : height(h), width(w)
{
    for (int i = 0; i < w; i++)
    {
        heightMap.push_back(vector<float>(h, 0.0f));
    }
}

void Terrain::terraGenerator(float offsetX, float offsetY)
{
    FastNoiseLite noise; 
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetFrequency(0.02f); // Lower frequency sets larger features
    noise.SetFractalType(FastNoiseLite::FractalType_FBm);		// Layered noise for a more natural variation
	noise.SetFractalOctaves(3); // More octaves = more detail


    for (int x = 0; x < height; ++x)
    {
        for (int y = 0; y < width; ++y)
        {
        	
        	float worldX = (x + offsetX) * 0.5f;
            float worldY = (y + offsetY) * 0.5f;
            // Ensures terrain transitions smoothly as you move 
        	
			float heightvalue = noise.GetNoise(worldX, worldY);
			float elevation = pow((heightvalue + 1.0f) / 2.0f, 2.0f) * 60.0f; // Exaggerate the elevation to build mountains, trees etc. 
			elevation += 10.0f;
			heightMap[x][y] = elevation;
			
			float edgeFalloff = 1.0f;

			float distX = abs(x - width / 2.0f) / (width / 2.0f);
			float distY = abs(y - height / 2.0f) / (height / 2.0f);
			float edgeFactor = 1.0f - pow(max(distX, distY), edgeFalloff); // Smooth falloff

			elevation *= edgeFactor;
													//pow( , 2.0) squares values which compresses short elevations and preserves higher. Pow simply allows you to 'exaggerate' elevation contrast

        }
    }
}