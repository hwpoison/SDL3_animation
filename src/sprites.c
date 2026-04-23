#include "sprites.h"

// External renderer reference - in a real engine this would be handled better (e.g. passing renderer to loadTexture)
extern SDL_Renderer *main_renderer;

/*
 * Load an image using SDL_Image lib and return texture pointer
 * @path 		: image path
 * @alpha       : remove alpha background (PNG images)
*/
SDL_Texture *loadTexture(const char *path, bool alpha){
	SDL_Surface *image = IMG_Load(path);
    if(!image) {
        SDL_Log("IMG_Load: %s\n", SDL_GetError());
        return NULL;
    }

    if(alpha){	
		// Remove colorkey from the sprite sheet
		int color_key = SDL_MapRGB( SDL_GetPixelFormatDetails(image->format), NULL, 255, 255, 255); // currently justcurrently just  empty bg for PNG images
		SDL_SetSurfaceColorKey(image, true, color_key);
	}

	SDL_Texture *texture = SDL_CreateTextureFromSurface(main_renderer, image);
	SDL_DestroySurface(image);
	if(texture ==  NULL){
		SDL_Log("Problem loading image.");
		return NULL; // todo: return misc texture
	}
	return texture;
}

SDL_FRect *Sprites_GenerateGridFrames(SDL_Texture *texture, SpriteDefinition *rect, int length) {
	int hframes_c = (int)SDL_ceil((float)texture->w/(float)rect->w);
	int vframes_c = (int)SDL_ceil((float)texture->h/(float)rect->h);

	SDL_FRect *all_frames = (SDL_FRect*)malloc(sizeof(SDL_FRect)*(hframes_c*vframes_c));
	if(all_frames == NULL) return NULL;

	int h_idx, v_idx, frame_idx = 0;
	for(v_idx = 0; v_idx < vframes_c; v_idx++){ // for each column 
		for(h_idx = 0; h_idx < hframes_c; h_idx++){ // for each horizontal tile
			all_frames[frame_idx].y = (float)((rect->h*v_idx)+(rect->h * rect->row));
			all_frames[frame_idx].x = (float)((rect->w*h_idx)+(rect->w*rect->column));
			all_frames[frame_idx].w = (float)rect->w;
			all_frames[frame_idx].h = (float)rect->h;
			frame_idx++;
		}
		if(length != 0){
			if(frame_idx > length) break;
		}
	}
	return all_frames;
}
