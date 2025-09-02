#include "renderer.h"
#include <gl/gl.h>
#include "FastNoiseLite.h"

void Renderer::drawTerrain(const Terrain& terrain)
{
    glBegin(GL_TRIANGLES);

    float centerX = terrain.width / 2.0f;
    float centerY = terrain.height / 2.0f;

    for (int x = 0; x < terrain.height - 1; ++x)
    {
        for (int y = 0; y < terrain.width - 1; ++y)
        {
            float h1 = terrain.heightMap[x][y];
            float h2 = terrain.heightMap[x + 1][y];
            float h3 = terrain.heightMap[x][y + 1];
            float h4 = terrain.heightMap[x + 1][y + 1];

            float avg = (h1 + h2 + h3 + h4) / 4.0f;
            float t = 0.0f;

            // Elevation-based coloring
            if (avg < 10.0f) {
                glColor3f(0.0f, 0.3f, 0.8f); // Deep water
            }
            else if (avg < 20.0f) {
                t = (avg - 10.0f) / 10.0f;
                float r = (1.0f - t) * 0.0f + t * 0.2f;
                float g = (1.0f - t) * 0.3f + t * 0.7f;
                float b = (1.0f - t) * 0.8f + t * 0.2f;
                glColor3f(r, g, b); // Blend water to grass
            }
            else if (avg < 30.0f) {
                glColor3f(0.2f, 0.7f, 0.2f); // Grass
            }
            else if (avg < 45.0f) {
                glColor3f(0.5f, 0.5f, 0.5f); // Rock
            }
            else {
                glColor3f(1.0f, 1.0f, 1.0f); // Snow
            }

            // First triangle
            glVertex3f(x - centerX, h1, y - centerY);
            glVertex3f(x + 1 - centerX, h2, y - centerY);
            glVertex3f(x - centerX, h3, y + 1 - centerY);

            // Second triangle
            glVertex3f(x + 1 - centerX, h2, y - centerY);
            glVertex3f(x + 1 - centerX, h4, y + 1 - centerY);
            glVertex3f(x - centerX, h3, y + 1 - centerY);
        }
    }

    glEnd();

    // Water plane beneath the island
    glBegin(GL_QUADS);
    glColor4f(0.0f, 0.4f, 0.9f, 0.6f);
    float waterLevel = -20.0f;
    float size = 2000.0f;
    glVertex3f(-size, waterLevel, -size);
    glVertex3f(size, waterLevel, -size);
    glVertex3f(size, waterLevel, size);
    glVertex3f(-size, waterLevel, size);
    glEnd();

    
}