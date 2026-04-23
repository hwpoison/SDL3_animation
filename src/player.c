#include "player.h"

void Player_Init(Player *self) {
    if (!self) return;

    // Configuration
    self->width = 180;
    self->height = 180;
    float x = 100.f;
    float y = 100.f;

    // Initialize Entity
    Entity_Init(&self->entity, x, y, self->width, self->height);
    self->entity.name = "cookie";
    
    // Physics setup
    // Physics setup
    // Physics setup
    self->entity.physics.acceleration = 2000.f;
    self->entity.physics.friction     = 900.f;
    self->entity.physics.mSpeed       = 650.f;
    self->entity.physics.speedX       = 0.f;
    self->entity.physics.speedY       = 0.f;
    self->entity.scale                = 1.0f;

    // Load assets
    char *cookie_spritesheet = "./assets/ss_cookie.png";
    self->texture = loadTexture(cookie_spritesheet, true);

    // Stay animation
	SpriteDefinition cookie_stay = {
		.w 	  = 180,
		.h    = 180,
		.column = 0,
		.row = 0,
	};
	self->entity.animations[COOKIE_STAY] = Animation_Create(
			"stay", 
			self->texture,
			&cookie_stay,
			4,
			10.0, 
			true
	);

	// Walk animation
	SpriteDefinition cookie_walk = {
		.w 	  = 180,
		.h    = 180,
		.column = 1,
		.row = 1,
	};
   	self->entity.animations[COOKIE_WALKING] = Animation_Create(
			"walk", 
			self->texture,
			&cookie_walk,
			3,
			15.0, 
			true
	);

	// Dragging animation
	SpriteDefinition cookie_drag = {
		.w 	  = 180,
		.h    = 180,
		.column = 2,
		.row = 4,
	};
   	self->entity.animations[COOKIE_DRAGGING] = Animation_Create(
			"dragging", 
			self->texture,
			&cookie_drag,
			1,
			10.0, 
			false
	);

	// crouch animation
	SpriteDefinition cookie_crouch = {
		.w 	  = 180,
		.h    = 180,
		.column = 2,
		.row = 4,
	};
   	self->entity.animations[COOKIE_CROUCH] = Animation_Create(
			"crouch", 
			self->texture,
			&cookie_crouch,
			1,
			10.0, 
			false
	);
    
    // Set initial animation
    Animator_Play(&self->entity.animator, &self->entity.animations[COOKIE_STAY]);
}

void Player_HandleInput(Player *self, const bool *keyboard) {
    if (!self || !keyboard) return;

    if(keyboard[SDL_SCANCODE_LCTRL]){
        Animator_Play(&self->entity.animator, &self->entity.animations[COOKIE_CROUCH]);
        self->entity.physics.moveX = 0;
        self->entity.physics.moveY = 0;
    } else {
       // Get player movement direction from X,Y [ -1, 0, 1 ]
       self->entity.physics.moveX = (float)keyboard[SDL_SCANCODE_D] - (float)keyboard[SDL_SCANCODE_A];
       self->entity.physics.moveY = (float)keyboard[SDL_SCANCODE_S] - (float)keyboard[SDL_SCANCODE_W];
       
       if(self->entity.physics.moveX == 0.0 && self->entity.physics.moveY == 0.0){
            Animator_Play(&self->entity.animator, &self->entity.animations[COOKIE_STAY]);
       } else {
            // Simple direction check for animation (favor X axis for now)
            if(self->entity.physics.moveX > 0) self->entity.animator.flip = SDL_FLIP_NONE;
            if(self->entity.physics.moveX < 0) self->entity.animator.flip = SDL_FLIP_HORIZONTAL;
            
            Animator_Play(&self->entity.animator, &self->entity.animations[COOKIE_WALKING]);
       }
    }
    
    // Scale control (debug/test features)
    if(keyboard[SDL_SCANCODE_X]){
        self->entity.scale += 0.01f; // Slowed down from original because this is called every frame vs every event
    }
    if(keyboard[SDL_SCANCODE_Z]){
        self->entity.scale -= 0.01f;
    }
}

void Player_Update(Player *self, float dt) {
    if (!self) return;

    Entity_Update(&self->entity, dt);

    // Update dimensions based on scale
    self->entity.box.w = self->width * self->entity.scale;
    self->entity.box.h = self->height * self->entity.scale;
}

void Player_Render(Player *self, SDL_Renderer *renderer) {
    if (!self) return;
    Entity_Render(&self->entity, renderer);
}

void Player_Destroy(Player *self) {
    if (!self) return;
    Entity_Destroy(&self->entity);
    // Texture is managed by SDL_DestroyRenderer but good practice to clean up if we had resource manager
    // SDL_DestroyTexture(self->texture); 
}
