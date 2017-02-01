#include "comet.h"

//contrsuctor, handles initializations
Comet::Comet(float x, float y, ALLEGRO_BITMAP *image, void(*TakeLife)(void)) {
	GameObject::Init(x, y, 5, 0, -1, 0, 35, 35);
	setID(ENEMY);
	maxFrame = 1;
	curFrame = 0;
	frameCount = 0;
	frameDelay = 1;
	frameWidth = 50;
	frameHeight = 50;
	animationColumns = 1;


	if (rand() % 2) //random number generator for which way asteroid spins
		animationDirection = 1;

	Comet::image = image;
	Comet::TakeLife = TakeLife;
}

//inherits the destroy function
void Comet::Destroy() {

	GameObject::Destroy();

}

void Comet::Update() {
	GameObject::Update();
	if (++frameCount >= frameDelay) {//animation update
		curFrame += animationDirection;
		if (curFrame >= maxFrame)
			curFrame = 0;
		else if (curFrame <= 0)
			curFrame = maxFrame;

		frameCount = 0;
	}
	if (x + frameWidth < 0) //bounds checking
		Collided(BORDER);
}

void Comet::Render() {
	GameObject::Render();
	int fx = (curFrame % animationColumns) *frameWidth;
	int fy = (curFrame / animationColumns) *frameHeight;

	//render
	al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x - frameWidth / 2, y - frameHeight / 2, 0);
}

void Comet::Collided(int objectID) {
	if (objectID == BORDER) {//if you've collided with a border
							 //take life
		TakeLife();
	}
	if (objectID != ENEMY)
		SetAlive(false);//get rekt comet
}