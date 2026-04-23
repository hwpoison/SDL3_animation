#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "entity.h"
#include "sprites.h"
#include "animator.h"
#include "player.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_Image.h>
#include "level.h"


// Global context
SDL_Window *main_window = NULL;
SDL_Renderer *main_renderer = NULL;

int main(){
	const int W_WIDTH = 1500, W_HEIGHT = 600;
	bool is_sdl_init = SDL_Init(SDL_INIT_VIDEO);
	
	if(!is_sdl_init){
		SDL_Log("Error to initialize SDL Video Subsystem.");
		return SDL_APP_FAILURE;
	}

	if(!SDL_CreateWindowAndRenderer("Cookie_SDL", W_WIDTH, W_HEIGHT, 0, &main_window, &main_renderer)){
		return SDL_APP_FAILURE;
	}

    // Initialize Player
    Player player;
    Player_Init(&player);

    // Initialize Level (Big surface: 3000x3000)
    Level level;
    Level_Init(&level, 3000, 3000);
    Level_Load(&level, main_renderer, NULL); // NULL for now, uses grid

    // Initialize Camera
    Camera camera;
    Camera_Init(&camera, 0, 0, (float)W_WIDTH, (float)W_HEIGHT);

	int lastTickTime = 0;

	bool game_running = true;
	while(game_running){

		SDL_Event event;
		int currentTickTime = SDL_GetTicks();
		float deltaTime = (currentTickTime - lastTickTime) / 1000.f;
		const bool *keyboard = SDL_GetKeyboardState(NULL);
		SDL_RenderClear(main_renderer);
		
        while(SDL_PollEvent(&event)){
			if(event.key.scancode == SDL_SCANCODE_ESCAPE){
				game_running = false;
				return SDL_APP_SUCCESS;
			}
			if(event.key.scancode == SDL_SCANCODE_N){
			}
			if(event.key.scancode == SDL_SCANCODE_B){
			}
            
            // Pass input to player (specifically for single press events, though currently player handles continuous state)
            // If we move to event-based input for player, we'd call it here.
            // For now, scale is handled in Player_HandleInput via keyboard state scan
		}
		

		// Player control & Update
        Player_HandleInput(&player, keyboard);
        Player_Update(&player, deltaTime);
        
        // Keep player in bounds
        Level_ClampEntity(&level, &player.entity.box);
        // Sync physics position with clamped box
        player.entity.physics.posX = player.entity.box.x;
        player.entity.physics.posY = player.entity.box.y;

        // Camera Update
        Camera_Follow(&camera, &player.entity.box, level.width, level.height);

		lastTickTime = currentTickTime;
		
		// Render
        // Clear screen
        SDL_SetRenderDrawColor(main_renderer, 0, 0, 0, 255);
		SDL_RenderClear(main_renderer);

        Level_Render(&level, main_renderer, &camera);
        
        // Render player with camera offset
        // We need to temporarily offset the player's box for rendering, OR pass camera to Player_Render.
        // For least intrusion into Player module right now (since Player_Render uses Entity_Render which uses box directly for now),
        // let's manually subtract camera before render and add back, OR better, update Entity_Render later.
        // Actually, let's just do the offset here for simplicity and safety without changing too many headers yet.
        
        float originalX = player.entity.box.x;
        float originalY = player.entity.box.y;
        
        player.entity.box.x -= camera.x;
        player.entity.box.y -= camera.y;
        
        Player_Render(&player, main_renderer);
        
        // Restore position
        player.entity.box.x = originalX;
        player.entity.box.y = originalY;

		SDL_RenderPresent(main_renderer);
		SDL_Delay(5);
	}
    
    Level_Destroy(&level);
    Player_Destroy(&player);
    SDL_DestroyRenderer(main_renderer);
    SDL_DestroyWindow(main_window);
    SDL_Quit();

	return 0;
}

