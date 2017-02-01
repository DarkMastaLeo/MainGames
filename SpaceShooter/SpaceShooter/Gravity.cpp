#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_acodec.h>
#include <iostream>

#include "GameObject.h"
#include "SpaceShip.h"
#include "bullet.h"
#include "comet.h"
#include "Explosion.h"
#include "Background.h"

using namespace std;

#include <list>
#include "globals.h"

bool keys[] = { false, false, false, false, false };
enum KEYS { UP, DOWN, LEFT, RIGHT, SPACE };

//globals
SpaceShip *ship;

list<GameObject *> objects;
list<GameObject *>::iterator iter;
list<GameObject *>::iterator iter2;

//function declarations

//cdecl is a calling convention; it allows us to declare these functions as pointers to functions
void __cdecl TakeLife();
void __cdecl ScorePoint();


int main() {
	bool done = false;
	bool render = false;

	float gameTime = 0;
	int frames = 0;
	int gameFPS = 0;

	ship = new SpaceShip();

	ALLEGRO_BITMAP *shipImage = NULL;
	ALLEGRO_BITMAP *cometImage = NULL;
	ALLEGRO_BITMAP *explimage = NULL;
	ALLEGRO_BITMAP *bgImage = NULL;
	ALLEGRO_BITMAP *mgImage = NULL;
	ALLEGRO_BITMAP *fgImage = NULL;

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer;
	ALLEGRO_FONT *font;


	al_init();

	display = al_create_display(WIDTH, HEIGHT);


	al_install_keyboard();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_install_audio();
	al_init_acodec_addon();



	font = al_load_font("barq.ttf", 18, 0);
	al_reserve_samples(15);

	bgImage = al_load_bitmap("Background.png");
	mgImage = al_load_bitmap("Planet1.png");
	al_convert_mask_to_alpha(mgImage, al_map_rgb(255, 255, 255));
	fgImage = al_load_bitmap("Planet2.png");

	Background *bg = new Background(bgImage, 2);
	objects.push_back(bg);
	Background *mg = new Background(mgImage, 3);
	objects.push_back(mg);
	Background *fg = new Background(fgImage, 1);
	objects.push_back(fg);

	shipImage = al_load_bitmap("ship.png");
	al_convert_mask_to_alpha(shipImage, al_map_rgb(255, 0, 255));
	ship->Init(shipImage);

	objects.push_back(ship);//pushback is a function that puts stuff into lists
	cometImage = al_load_bitmap("asteroid.png");
	al_convert_mask_to_alpha(mgImage, al_map_rgb(0, 0, 0));
	explimage = al_load_bitmap("Explosion.png");
	al_convert_mask_to_alpha(mgImage, al_map_rgb(0, 0, 0));

	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);

	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_start_timer(timer);
	gameTime = al_current_time();


	while (!done) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = true;
				//generate bullets
				Bullet *bullet = new Bullet(ship->GetX() + 17, ship->GetY(), &ScorePoint);
				objects.push_back(bullet);
				break;
			}//end switch
		}//end if
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_ESCAPE:
				done = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = false;
				break;
			}//end switch
		}//end else if
		else if (ev.type == ALLEGRO_EVENT_TIMER) {
			render = true;

			frames++;
			if (al_current_time() - gameTime >= 1) {
				gameTime = al_current_time();
				gameFPS = frames;
				frames = 0;


			}//end if
			 //moves ship
			if (keys[UP])
				ship->MoveUp();
			else if (keys[DOWN]) {
				ship->MoveDown();
			}
			else
				ship->ResetAnimation(1);

			if (keys[LEFT])
				ship->MoveLeft();
			else if (keys[RIGHT])
				ship->MoveRight();
			else
				ship->ResetAnimation(0);


			//comet generation
			//mess with mod to increase or decrease comet generation rate
			if (rand() % 150 == 0) {
				Comet *comet = new Comet(WIDTH, 30 + rand() % (HEIGHT - 60), cometImage, *TakeLife);
				objects.push_back(comet);
			}

			//update
			for (iter = objects.begin(); iter != objects.end(); ++iter)
				(*iter)->Update(); //include the "*" before the iter, to indicate 
								   //that you're updating what the iterator is pointing to, not the iterator itself.

								   //collision
			for (iter = objects.begin(); iter != objects.end(); ++iter) {

				if (!(*iter)->Collidable()) continue; //skip it if its not collidable
				for (iter2 = iter; iter2 != objects.end(); ++iter2) {
					if (!(*iter2)->Collidable()) continue; //skip if its not collidable
					if ((*iter)->GetID() == (*iter2)->GetID()) continue; //no comets hit each other

					if ((*iter)->CheckCollisions(*iter2)) {
						//if two objects collide, call each others collided function
						(*iter)->Collided((*iter2)->GetID());
						(*iter2)->Collided((*iter)->GetID());

						Explosion *explosion = new Explosion(((*iter)->GetX() + (*iter2)->GetX()) / 2, ((*iter)->GetY() + (*iter2)->GetY()) / 2, explimage);

						objects.push_back(explosion);
					}
				}

			}
			//cull the dead
			for (iter = objects.begin(); iter != objects.end();) {
				if (!(*iter)->GetAlive()) {//if its not alive
					delete (*iter);
					iter = objects.erase(iter);

				}
				else
					iter++;

			}//end loop
		}//end else if
		if (render && al_is_event_queue_empty(event_queue)) {
			render = false;



			for (iter = objects.begin(); iter != objects.end(); ++iter)
				(*iter)->Render();

			al_draw_textf(font, al_map_rgb(255, 0, 255), 5, 5, 0, "Player has %i lives left. Player has destroyed %i objects", ship->GetLives(), ship->GetScore());

			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}//end if




	}//end game loop
	for (iter = objects.begin(); iter != objects.end();) {
		(*iter)->Destroy();
		delete (*iter); //"delete" undoes "new"
		iter = objects.erase(iter);

	}//end loop

	al_destroy_font(font);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);

	return 0;
}//end main

void __cdecl TakeLife() {
	ship->LoseLife();
}
void __cdecl ScorePoint() {
	ship->AddPoint();
}