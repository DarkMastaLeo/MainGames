#include "SpaceShip.h"

#include<iostream>
using namespace std;

SpaceShip::SpaceShip() { //constructor
						 //don't initialize here, we made it before allegro image library is initialized
}

void SpaceShip::Destroy() {
	GameObject::Destroy();
}

void SpaceShip::Init(ALLEGRO_BITMAP *image) {

	GameObject::Init(20, 200, 6, 6, 0, 0, 10, 12);

	setID(PLAYER); //pulls from enum in Globals.h
	SetAlive(true);

	lives = 3;
	score = 0;

	//sprite stuff: review spries if lost
	maxFrame = 3;
	curFrame = 0;
	frameWidth = 46;
	frameHeight = 41;
	animationColumns = 3;
	animationDirection = 1;
	animationRow = 1;
	if (image != NULL) //only give it an image if it doesn't already have one!
		SpaceShip::image = image;
}

void SpaceShip::Update() {
	GameObject::Update();

	//don't let the ship fly offscreen
	if (x < 0)
		x = 0;
	else if (x > WIDTH)
		x = WIDTH;

	if (y < 0)
		y = 0;
	else if (y > HEIGHT)
		y = HEIGHT;

}

void SpaceShip::Render() {

	GameObject::Render(); //currently does nothing

						  //sprie animation code
	int fx = (curFrame % animationColumns) * frameWidth;
	int fy = animationRow * frameHeight;
	al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight,
		x - frameWidth / 2, y - frameHeight / 2, 0);

}

void SpaceShip::MoveUp() {
	animationRow = 0;
	dirY = -1;
	//cout<<"inside Moveup"<<endl;

}

void SpaceShip::MoveDown() {
	animationRow = 2;
	dirY = 1;
	//cout<<"inside movedown"<<endl;

}

void SpaceShip::MoveLeft() {
	curFrame = 2;
	dirX = -1;

}

void SpaceShip::MoveRight() {
	curFrame = 1;
	dirX = 1;

}

void SpaceShip::ResetAnimation(int position) {
	if (position == 1) {
		animationRow = 1;
		dirY = 0;

	}
	else {
		curFrame = 0;
		dirX = 0;

	}
}

void SpaceShip::Collide(int objectID) {
	if (objectID == ENEMY) //only subtract lives if it's an enemy, not if it's your own bullet
		lives--;
}

