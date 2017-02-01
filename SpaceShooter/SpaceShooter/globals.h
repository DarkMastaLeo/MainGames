#pragma once //hey, only load this once, een if a bunch of files use it

const int WIDTH = 800;
const int HEIGHT = 500;


enum ID { PLAYER, ENEMY, BULLET, BORDER, MISC, EXPLOSION };
enum STATE { TITLE, PLAYING, LOST };