#include "level.h"
#include <SDL3_image/SDL_Image.h>

void Level_Init(Level *level, int width, int height) {
    if (!level) return;
    level->width = width;
    level->height = height;
    level->bgTexture = NULL;
    level->bounds.x = 0;
    level->bounds.y = 0;
    level->bounds.w = (float)width;
    level->bounds.h = (float)height;
}

void Level_Load(Level *level, SDL_Renderer *renderer, const char *bgPath) {
    if (!level || !renderer) return;
    // For now, we can create a simple checkerboard or load an image if provided.
    // Since we want a "big surface", let's create a large texture or load one.
    // For this example, we'll try to load, but if it fails (or path is NULL), we might want a fallback.
    
    if (bgPath) {
        level->bgTexture = IMG_LoadTexture(renderer, bgPath);
    }
}

void Level_Render(Level *level, SDL_Renderer *renderer, const Camera *camera) {
    if (!level || !renderer || !camera) return;

    // Render background relative to camera
    // If we have a texture, render it. 
    // Optimization: Only render the part visible by camera.
    
    if (level->bgTexture) {
        // Simple approach: Render the whole texture at -camera.x, -camera.y
        // Better approach: srcRect derived from camera, destRect is screen
        
        SDL_FRect dst = { -camera->x, -camera->y, (float)level->width, (float)level->height };
        SDL_RenderTexture(renderer, level->bgTexture, NULL, &dst);
    } else {
        // Fallback: Render a grid
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        
        // Draw grid lines
        int gridSize = 100;
        for (float x = 0; x <= level->width; x += gridSize) {
            float drawX = x - camera->x;
            if (drawX >= 0 && drawX <= camera->w) {
                 SDL_RenderLine(renderer, drawX, 0, drawX, camera->h);
            }
        }
        for (float y = 0; y <= level->height; y += gridSize) {
             float drawY = y - camera->y;
             if (drawY >= 0 && drawY <= camera->h) {
                 SDL_RenderLine(renderer, 0, drawY, camera->w, drawY);
             }
        }
        
        // Draw border
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_FRect border = { -camera->x, -camera->y, level->bounds.w, level->bounds.h };
        SDL_RenderRect(renderer, &border);
    }
}

void Level_Destroy(Level *level) {
    if (!level) return;
    if (level->bgTexture) {
        SDL_DestroyTexture(level->bgTexture);
    }
}

void Level_ClampEntity(const Level *level, SDL_FRect *entityBox) {
    if (!level || !entityBox) return;

    if (entityBox->x < 0) entityBox->x = 0;
    if (entityBox->y < 0) entityBox->y = 0;
    if (entityBox->x > level->width - entityBox->w) entityBox->x = level->width - entityBox->w;
    if (entityBox->y > level->height - entityBox->h) entityBox->y = level->height - entityBox->h;
}

// Camera Implementation
void Camera_Init(Camera *camera, float x, float y, float w, float h) {
    if (!camera) return;
    camera->x = x;
    camera->y = y;
    camera->w = w;
    camera->h = h;
}

void Camera_Follow(Camera *camera, const SDL_FRect *target, int mapWidth, int mapHeight) {
    if (!camera || !target) return;

    // Center camera on target
    camera->x = target->x + (target->w / 2) - (camera->w / 2);
    camera->y = target->y + (target->h / 2) - (camera->h / 2);

    // Clamp to map bounds
    if (camera->x < 0) camera->x = 0;
    if (camera->y < 0) camera->y = 0;
    if (camera->x > mapWidth - camera->w) camera->x = mapWidth - camera->w;
    if (camera->y > mapHeight - camera->h) camera->y = mapHeight - camera->h;
}
