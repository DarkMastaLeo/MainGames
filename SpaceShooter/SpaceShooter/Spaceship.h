#pragma once
#include "GameObject.h"
#include <iostream>
using namespace std;

class SpaceShip : public GameObject {

private:
	int lives;
	int score;
	int animationRow;
	int Opposite;
public:
	SpaceShip(); //constructor
	void Destroy();
	void Init(ALLEGRO_BITMAP *image = NULL);

	void Update();
	void Render();

	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();

	void ResetAnimation(int position);

	int GetLives() { return lives; }
	int GetScore() { return score; }
	void LoseLife() { lives--; }
	void AddPoint() { score++; }

	void Collide(int objectID);

};