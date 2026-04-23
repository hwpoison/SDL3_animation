#include "entity.h"

void calculateMovement(PhysicsBody *p, double dt) {

		// Friction and acceleration
		if (p->moveY == 0) { // If there is not Y movement, decrease speed
			if (SDL_fabs(p->speedY) < p->friction * dt) p->speedY = 0;
			else p->speedY -= (p->speedY > 0 ? 1 : -1) * p->friction * dt;
		} else { // If there is Y movement increase graduality the speed
			p->speedY += p->moveY * p->acceleration * dt; 
		}
		if (p->moveX == 0) {
			if (SDL_fabs(p->speedX) < p->friction * dt) p->speedX = 0;
			else p->speedX -= (p->speedX > 0 ? 1 : -1) * p->friction * dt;
		} else {
			p->speedX += p->moveX * p->acceleration * dt;
		}

		float length = SDL_sqrtf(p->moveX * p->moveX + p->moveY * p->moveY);

		// Normalize move acceleration
		if (length > 1.0f) { 
			p->moveX /= length;
			p->moveY /= length;
		}

		// Calculate the movement
		p->posX += p->speedX * dt;
		p->posY += p->speedY * dt;
}

void Entity_Init(Entity *self, float x, float y, float w, float h) {
    if (!self) return;

    self->box.x = x;
    self->box.y = y;
    self->box.w = w;
    self->box.h = h;
    self->scale = 1.0;
    self->name = "Entity";

    // Initialize Physics
    SDL_memset(&self->physics, 0, sizeof(PhysicsBody));
    self->physics.posX = x;
    self->physics.posY = y;
    // Default physics values - can be overridden
    self->physics.friction = 10.0;
    self->physics.acceleration = 20.0;
    self->physics.mSpeed = 100.0;

    // Initialize Animator
    Animator_Init(&self->animator);
    self->animationCount = 0;
    // Clear animations array
    SDL_memset(self->animations, 0, sizeof(self->animations));
}

void Entity_Update(Entity *self, double dt) {
    if (!self) return;

    // Physics
    calculateMovement(&self->physics, dt);
    
    // Sync box position with physics position
    self->box.x = (float)self->physics.posX;
    self->box.y = (float)self->physics.posY;

    // Animator
    Animator_Update(&self->animator, (float)dt);
}

void Entity_Render(Entity *self, SDL_Renderer *renderer) {
    if (!self || !renderer) return;

    SDL_Texture *tex = Animator_GetCurrentTexture(&self->animator);
    SDL_FRect *frame = Animator_GetCurrentFrame(&self->animator);
    
    if (tex && frame) {
        SDL_RenderTextureRotated( 
                renderer, 
                tex,
                frame,
                &self->box,
                0.0, NULL, self->animator.flip);
    }
}

void Entity_Destroy(Entity *self) {
    if (!self) return;

    // Destroy all animations
    for (int i = 0; i < self->animationCount; i++) {
        Animation_Destroy(&self->animations[i]);
    }
    self->animationCount = 0;
}

int Entity_AddAnimation(Entity *self, Animation anim) {
    if (!self || self->animationCount >= MAX_ANIMATIONS) return -1;
    
    self->animations[self->animationCount] = anim;
    int index = self->animationCount;
    self->animationCount++;
    return index;
}
