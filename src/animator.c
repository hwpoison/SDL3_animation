#include "animator.h"
#include <stdlib.h>

Animation Animation_Create(
		char *name, 
		SDL_Texture *sheetTexture,
		SpriteDefinition *sheet,
		int length,
		float fps,
		bool in_loop) {
	
	SDL_FRect *frames = Sprites_GenerateGridFrames(sheetTexture, sheet, length);
	
	Animation newAnimation;
   	newAnimation.name = name;
	newAnimation.texture = sheetTexture;
	newAnimation.frames = frames;
	newAnimation.length = length;
	newAnimation.in_loop = in_loop;
	newAnimation.speed = 1.0f / fps; // stores period (seconds per frame)

	return newAnimation;
}

void Animation_Destroy(Animation *anim) {
    if (anim && anim->frames) {
        free(anim->frames);
        anim->frames = NULL;
    }
}

void Animator_Init(Animator *animator) {
    animator->currentAnimation = NULL;
    animator->currentFrame = NULL;
    animator->indexFrame = 0;
    animator->animationTime = 0.0f;
    animator->isPlaying = false;
    animator->flip = SDL_FLIP_NONE;
}

void Animator_Play(Animator *animator, Animation *anim) {
    if (animator->currentAnimation != anim) {
        animator->currentAnimation = anim;
        animator->indexFrame = 0;
        animator->animationTime = 0.0f;
        animator->isPlaying = true;
        
        if (anim && anim->frames) {
            animator->currentFrame = &anim->frames[0];
        } else {
            animator->currentFrame = NULL;
        }
    }
}

void Animator_Update(Animator *animator, float dt) {
    if (!animator->isPlaying || !animator->currentAnimation) {
        return;
    }

    Animation *anim = animator->currentAnimation;
    animator->animationTime += dt;

    while (animator->animationTime >= anim->speed) {
        animator->animationTime -= anim->speed;
        
        // Advance frame
        if (anim->in_loop) {
            animator->indexFrame = (animator->indexFrame + 1) % anim->length;
        } else {
            if (animator->indexFrame + 1 < anim->length) {
                animator->indexFrame++;
            }
            // else stay at last frame or stop? 
            // Original code stayed at last frame.
        }
    }
    
    animator->currentFrame = &anim->frames[animator->indexFrame];
}

SDL_FRect *Animator_GetCurrentFrame(Animator *animator) {
    return animator->currentFrame;
}

SDL_Texture *Animator_GetCurrentTexture(Animator *animator) {
    if (animator->currentAnimation) {
        return animator->currentAnimation->texture;
    }
    return NULL;
}

