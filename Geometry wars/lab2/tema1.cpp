// Bogdan Stefan 333 CA

#include "Framework/DrawingWindow.h"
#include "Framework/Visual2D.h"
#include "Framework/Transform2D.h"
#include "Framework/Line2D.h"
#include "Framework/Rectangle2D.h"
#include "Framework/Circle2D.h"
#include "Framework/Polygon2D.h"
#include <iostream>
#include <windows.h>


#define PI 3.14159265358979323846

using namespace std;

Visual2D *v2d1,*v2d2;
Rectangle2D *rect;
Text *text, *text2, *score;
Circle2D *playerCircle, *playerCircle2, *enemyCircle2, *enemyCircle3, *enemyCircle41, *enemyCircle42, *enemyCircle43, *enemyCircle44,
	*weaponCircle1, *weaponCircle2, *smartEnemies[50], *heart1, *heart2, *heart;
float c1x, c1y, u1, u2 = 0, delta = 5;
Polygon2D *player, *player2, *enemy21, *enemy22, *enemy31, *enemy32, *enemy41, *enemy42, *enemy43, *enemy44, *weapon, *newLive,
	*enemy51, *enemy52;
float tx = 0, ty = 0, ty4 = 0, tx5 = 0, ty5 = 0;
bool poligon4 = true, activeWeapon = false;
float raza1, raza2, raza3, raza4, razaArma1, razaArma2, razaHeart, razaHeart1, razaHeart2, raza5;
float centru1x, centru2x, centru3x, centru4x[4], centruArma1x, centruArma2x, centruSmartEnemiesx[50], centruHeartx, centruHeart1x,
	centruHeart2x, centru5x[2];
float centru1y, centru2y, centru3y, centru4y[4], centruArma1y, centruArma2y, centruSmartEnemiesy[50], centruHearty, centruHeart1y,
	centruHeart2y, centru5y[2];
int numar = 0, smartCount = 0;
bool smart = false;
float smartTranslateX[50] = {0}, smartTranslateY[50] = {0};
bool activeEnemy2 = false, activeEnemy3 = false, activeEnemy4 = false, activeHeart = false, activeEnemy5;
bool activeSmartEnemies[50] = {false};
int myScore = 0;
int lives = 3;
float count2, count3, count4, count5;
Circle2D *smallEnemies[200];
int stateEnemy5;

bool* keyStates = new bool[256];
bool* keySpecialStates = new bool[246]; // Create an array of boolean values of length 256 (0-255)


//functia care permite adaugarea de obiecte
void DrawingWindow::init()
{
	// Adaugare cadru1
	v2d1 = new Visual2D(0,0,DrawingWindow::width,DrawingWindow::height,100,200,DrawingWindow::width - 100,DrawingWindow::height - 100);  
	addVisual2D(v2d1);
	v2d1->tipTran(true);
	v2d1->cadruFereastra(Color(0,1,1));


	// Adaugare cadru2
	v2d2 = new Visual2D(0,0,DrawingWindow::width,DrawingWindow::height,100,0,DrawingWindow::width - 100,200);  
	addVisual2D(v2d2);
	v2d2->tipTran(true);

	
	text = new Text("Score", Point2D(200,300),Color(1,1,0),BITMAP_TIMES_ROMAN_24);
	addText_to_Visual2D(text,v2d2);

	score = new Text("0", Point2D(450,300),Color(1,1,0),BITMAP_TIMES_ROMAN_24);
	addText_to_Visual2D(score,v2d2);

	text2 = new Text("3", Point2D(700,300),Color(1,1,0),BITMAP_TIMES_ROMAN_24);
	addText_to_Visual2D(text2,v2d2);

	// Adaugare player
	playerCircle = new Circle2D(Point2D(DrawingWindow::width/2, DrawingWindow::height/2), 40, Color (1,0,0), false);
	addObject2D_to_Visual2D(playerCircle,v2d1);

	c1x = DrawingWindow::width/2;
	c1y = DrawingWindow::height/2;
	u1 = 0;

	centru1x = c1x;
	centru1y = c1y;
	raza1 = 40;

	player = new Polygon2D(Color (1,0,0), false);
	player->addPoint(Point2D (DrawingWindow::width/2 - 30, DrawingWindow::height/2));
	player->addPoint(Point2D (DrawingWindow::width/2, DrawingWindow::height/2 + 30));
	player->addPoint(Point2D (DrawingWindow::width/2 + 30, DrawingWindow::height/2 + 20));
	player->addPoint(Point2D (DrawingWindow::width/2 + 12, DrawingWindow::height/2 + 20));
	player->addPoint(Point2D (DrawingWindow::width/2 - 5, DrawingWindow::height/2));
	player->addPoint(Point2D (DrawingWindow::width/2 + 12, DrawingWindow::height/2 - 20));
	player->addPoint(Point2D (DrawingWindow::width/2 + 30, DrawingWindow::height/2 - 20));
	player->addPoint(Point2D (DrawingWindow::width/2, DrawingWindow::height/2 - 30));

	addObject2D_to_Visual2D(player,v2d1);


	player2 = new Polygon2D(Color (1,0,0), false);
	player2->addPoint(Point2D (DrawingWindow::width/2 - 30, DrawingWindow::height/2));
	player2->addPoint(Point2D (DrawingWindow::width/2, DrawingWindow::height/2 + 30));
	player2->addPoint(Point2D (DrawingWindow::width/2 + 30, DrawingWindow::height/2 + 20));
	player2->addPoint(Point2D (DrawingWindow::width/2 + 12, DrawingWindow::height/2 + 20));
	player2->addPoint(Point2D (DrawingWindow::width/2 - 5, DrawingWindow::height/2));
	player2->addPoint(Point2D (DrawingWindow::width/2 + 12, DrawingWindow::height/2 - 20));
	player2->addPoint(Point2D (DrawingWindow::width/2 + 30, DrawingWindow::height/2 - 20));
	player2->addPoint(Point2D (DrawingWindow::width/2, DrawingWindow::height/2 - 30));

	playerCircle2 = new Circle2D(Point2D(DrawingWindow::width/2, DrawingWindow::height/2), 40, Color (1,0,0), false);
	addObject2D_to_Visual2D(playerCircle2,v2d2);


	addObject2D_to_Visual2D(player2,v2d2);
	Transform2D::loadIdentityMatrix();
	Transform2D::translateMatrix(125,-48);
	Transform2D::applyTransform(player2);
	Transform2D::applyTransform(playerCircle2);


	// Arma player
	weapon = new Polygon2D(Color (1,0,0), false);
	weapon->addPoint(Point2D (DrawingWindow::width/2 + 45, DrawingWindow::height/2 - 30));
	weapon->addPoint(Point2D (DrawingWindow::width/2 + 130, DrawingWindow::height/2));
	weapon->addPoint(Point2D (DrawingWindow::width/2 + 45, DrawingWindow::height/2 + 30));	


	// Cercurile care incadreaza arma pentru a putea face coliziunile
	weaponCircle1 = new Circle2D(Point2D(DrawingWindow::width/2 + 55, DrawingWindow::height/2), 32, Color(1,0,0), false);
	centruArma1x = c1x + 55;
	centruArma1y = c1y;
	razaArma1 = 32;

	weaponCircle2 = new Circle2D(Point2D(DrawingWindow::width/2 + 110, DrawingWindow::height/2), 20, Color(1,0,0), false);
	centruArma2x = c1x + 110;
	centruArma2y = c1y;
	razaArma2 = 20;


	enemy21 = new Polygon2D(Color (0,1,0), false);
	enemy21->addPoint(Point2D (150,150));
	enemy21->addPoint(Point2D (150,220));
	enemy21->addPoint(Point2D (220,220));
	enemy21->addPoint(Point2D (220,150));

	addObject2D_to_Visual2D(enemy21,v2d1);

	enemy22 = new Polygon2D(Color (0,1,0), false);
	enemy22->addPoint(Point2D (150,185));
	enemy22->addPoint(Point2D (185,220));
	enemy22->addPoint(Point2D (220,185));
	enemy22->addPoint(Point2D (185,150));

	addObject2D_to_Visual2D(enemy22,v2d1);

	activeEnemy2 = true;

	centru2x = 185;
	centru2y = 185;
	raza2 = 35 * 1.42;
	enemyCircle2 = new Circle2D(Point2D(centru2x, centru2y), raza2, Color (1,0,0), false);

	enemy31 = new Polygon2D(Color (0,1,0), false);
	enemy31->addPoint(Point2D (150,550));
	enemy31->addPoint(Point2D (150,620));
	enemy31->addPoint(Point2D (220,620));
	enemy31->addPoint(Point2D (220,550));

	addObject2D_to_Visual2D(enemy31,v2d1);

	enemy32 = new Polygon2D(Color (0,1,0), false);
	enemy32->addPoint(Point2D (150,585));
	enemy32->addPoint(Point2D (185,620));
	enemy32->addPoint(Point2D (220,585));
	enemy32->addPoint(Point2D (185,550));

	addObject2D_to_Visual2D(enemy32,v2d1);

	activeEnemy3 = true;

	centru3x = 185;
	centru3y = 585;
	raza3 = 35 * 1.42;
	enemyCircle3 = new Circle2D(Point2D(centru3x, centru3y), raza3, Color (1,0,0),false);


	enemy41 = new Polygon2D(Color (0,0,1), false);
	enemy41->addPoint(Point2D(900,100));
	enemy41->addPoint(Point2D(950,100));
	enemy41->addPoint(Point2D(950,150));
	enemy41->addPoint(Point2D(900,150));

	addObject2D_to_Visual2D(enemy41, v2d1);

	enemy42 = new Polygon2D(Color (0,0,1), false);
	enemy42->addPoint(Point2D(950,100));
	enemy42->addPoint(Point2D(1000,100));
	enemy42->addPoint(Point2D(1000,150));
	enemy42->addPoint(Point2D(950,150));

	addObject2D_to_Visual2D(enemy42, v2d1);

	enemy43 = new Polygon2D(Color (0,0,1), false);
	enemy43->addPoint(Point2D(1000,100));
	enemy43->addPoint(Point2D(1050,100));
	enemy43->addPoint(Point2D(1050,150));
	enemy43->addPoint(Point2D(1000,150));

	addObject2D_to_Visual2D(enemy43, v2d1);

	enemy44 = new Polygon2D(Color (0,0,1), false);
	enemy44->addPoint(Point2D(900,50));
	enemy44->addPoint(Point2D(950,50));
	enemy44->addPoint(Point2D(950,100));
	enemy44->addPoint(Point2D(900,100));

	addObject2D_to_Visual2D(enemy44, v2d1);

	activeEnemy4 = true;

	centru4x[0] = 925;
	centru4y[0] = 125;
	centru4x[1] = 975;
	centru4y[1] = 125;
	centru4x[2] = 1025;
	centru4y[2] = 125;
	centru4x[3] = 925;
	centru4y[3] = 75;
	raza4 = 35.5;
	enemyCircle41 = new Circle2D(Point2D(centru4x[0], centru4y[0]), raza4, Color (1,0,0), false);
	enemyCircle42 = new Circle2D(Point2D(centru4x[1], centru4y[1]), raza4, Color (1,0,0), false);
	enemyCircle43 = new Circle2D(Point2D(centru4x[2], centru4y[2]), raza4, Color (1,0,0), false);
	enemyCircle44 = new Circle2D(Point2D(centru4x[3], centru4y[3]), raza4, Color (1,0,0), false);


	enemy51 = new Polygon2D(Color (1,0,1), false);
	enemy51->addPoint(Point2D (DrawingWindow::width/2 + 300,DrawingWindow::height/2 + 200));
	enemy51->addPoint(Point2D (DrawingWindow::width/2 + 350,DrawingWindow::height/2 + 200));
	enemy51->addPoint(Point2D (DrawingWindow::width/2 + 350,DrawingWindow::height/2 + 250));
	enemy51->addPoint(Point2D (DrawingWindow::width/2 + 300,DrawingWindow::height/2 + 250));

	addObject2D_to_Visual2D(enemy51,v2d1);

	enemy52 = new Polygon2D(Color (1,0,1), false);
	enemy52->addPoint(Point2D (DrawingWindow::width/2 + 325,DrawingWindow::height/2 + 225));
	enemy52->addPoint(Point2D (DrawingWindow::width/2 + 375,DrawingWindow::height/2 + 225));
	enemy52->addPoint(Point2D (DrawingWindow::width/2 + 375,DrawingWindow::height/2 + 275));
	enemy52->addPoint(Point2D (DrawingWindow::width/2 + 325,DrawingWindow::height/2 + 275));

	addObject2D_to_Visual2D(enemy52,v2d1);
	
	centru5x[0] = DrawingWindow::width / 2 + 325;
	centru5y[0] = DrawingWindow::height / 2 + 225;
	centru5x[1] = DrawingWindow::width / 2 + 350;
	centru5y[1] = DrawingWindow::height / 2 + 250;

	activeEnemy5 = true;
	raza5 = 25 * 1.42;

	stateEnemy5 = 0;

	// Creating new live icon
	heart1 = new Circle2D(Point2D(DrawingWindow::width/2 - 15, DrawingWindow::height/2), 15, Color (1,0,0), true);
	heart2 = new Circle2D(Point2D(DrawingWindow::width/2 + 15, DrawingWindow::height/2), 15, Color (1,0,0), true);
	newLive = new Polygon2D(Color(1,0,0), true);
	newLive->addPoint(Point2D(DrawingWindow::width/2 -30, DrawingWindow::height/2));
	newLive->addPoint(Point2D(DrawingWindow::width/2 +30, DrawingWindow::height/2));
	newLive->addPoint(Point2D(DrawingWindow::width/2 , DrawingWindow::height/2 - 40));

	heart = new Circle2D(Point2D(DrawingWindow::width/2, DrawingWindow::height/2 - 20), 20, Color (0,1,1), false);

	razaHeart = 20;
	razaHeart1 = 15;
	razaHeart2 = 15;

	centruHeartx = DrawingWindow::width/2;
	centruHearty = DrawingWindow::height/2 - 20;

	centruHeart1x = DrawingWindow::width/2 - 15;
	centruHeart1y = DrawingWindow::height;

	centruHeart2x = DrawingWindow::width/2 + 15;
	centruHeart2y = DrawingWindow::height;

	activeHeart = false;


	for (int i = 0; i < 246; i++) {
		keyStates[i] = false;
		keySpecialStates[i] = false;
	}

	for (int i = 246; i < 256; i++) {
		keySpecialStates[i] = false;
	}

}


//functia care permite animatia
void DrawingWindow::onIdle()
{
	numar++;

	if (numar % 2001 == 2000) {
		addObject2D_to_Visual2D(heart1, v2d1);
		addObject2D_to_Visual2D(heart2, v2d1);
		addObject2D_to_Visual2D(newLive, v2d1);
		activeHeart	= true;
	}

	if (!activeEnemy2) {
		cout<<count2<<endl;
		count2++;
		if (count2 == 450) {
			activeEnemy2 = true;
			addObject2D_to_Visual2D(enemy21,v2d1);
			addObject2D_to_Visual2D(enemy22,v2d1);
			count2 = 0;
		}
	}

	if (!activeEnemy3) {
		count3++;
		if (count3 == 450) {
			activeEnemy3 = true;
			addObject2D_to_Visual2D(enemy31,v2d1);
			addObject2D_to_Visual2D(enemy32,v2d1);
			count3 = 0;
		}
	}

	if (!activeEnemy4) {
		count4++;
		if (count4 == 450) {
			activeEnemy4 = true;
			addObject2D_to_Visual2D(enemy41,v2d1);
			addObject2D_to_Visual2D(enemy42,v2d1);
			addObject2D_to_Visual2D(enemy43,v2d1);
			addObject2D_to_Visual2D(enemy44,v2d1);
			count4 = 0;
		}
	}

	if (!activeEnemy5) {
		count5++;
		if (count5 == 450) {
			activeEnemy5 = true;
			addObject2D_to_Visual2D(enemy51,v2d1);
			addObject2D_to_Visual2D(enemy52,v2d1);
			count5 = 0;
		}
	}

	if (numar % 501 == 500) {
		smartEnemies[smartCount] = new Circle2D(Point2D(DrawingWindow::width - 20, DrawingWindow::height - 20), 15, Color(1,0,1), true);
		centruSmartEnemiesx[smartCount] = DrawingWindow::width - 20;
		centruSmartEnemiesy[smartCount] = DrawingWindow::height - 20;
	
		addObject2D_to_Visual2D(smartEnemies[smartCount], v2d1);
		activeSmartEnemies[smartCount] = true;
		smartCount++;
		smart = true;
	}

	if (smart = true) {

		for (int i = 0; i < smartCount; i++ ) {
		
			if (centru1x > centruSmartEnemiesx[i]) {
				smartTranslateX[i] = smartTranslateX[i] + i/2 + 1;
				centruSmartEnemiesx[i] = centruSmartEnemiesx[i] + i/2 + 1;
			}
			else {
				smartTranslateX[i] = smartTranslateX[i] - i/2 - 1;
				centruSmartEnemiesx[i] = centruSmartEnemiesx[i] - i/2 - 1;
			}

			if (centru1y > centruSmartEnemiesy[i]) {
				smartTranslateY[i] = smartTranslateY[i] + i/2 + 1;
				centruSmartEnemiesy[i] = centruSmartEnemiesy[i] + i/2 + 1;
			}
			else {
				smartTranslateY[i] = smartTranslateY[i] - i/2 - 1;
				centruSmartEnemiesy[i] = centruSmartEnemiesy[i] - i/2 - 1;
			}

		Transform2D::loadIdentityMatrix();
		Transform2D::translateMatrix(smartTranslateX[i], smartTranslateY[i]);
		Transform2D::applyTransform(smartEnemies[i]);


		}
	} // end if smart == true


	//miscarea poligonului enemy4
	//poligon4 = true inseamna ca urca, false inseamna ca coboara
	if (poligon4) {
		if (centru4y[0] < (float) DrawingWindow::height - 50) {
			ty4 = ty4 + 5;
			Transform2D::loadIdentityMatrix();
			Transform2D::translateMatrix(0, ty4);
			Transform2D::applyTransform(enemy41);
			Transform2D::applyTransform(enemy42);
			Transform2D::applyTransform(enemy43);
			Transform2D::applyTransform(enemy44);
			centru4y[0] = centru4y[0] + 5;
			centru4y[1] = centru4y[1] + 5;
			centru4y[2] = centru4y[2] + 5;
			centru4y[3] = centru4y[3] + 5;
		}
		else {
			poligon4 = false;
		}
	}
	else {
		if (centru4y[0] > 80) {
			ty4 = ty4 - 5;
			Transform2D::loadIdentityMatrix();
			Transform2D::translateMatrix(0, ty4);
			Transform2D::applyTransform(enemy41);
			Transform2D::applyTransform(enemy42);
			Transform2D::applyTransform(enemy43);
			Transform2D::applyTransform(enemy44);
			centru4y[0] = centru4y[0] - 5;
			centru4y[1] = centru4y[1] - 5;
			centru4y[2] = centru4y[2] - 5;
			centru4y[3] = centru4y[3] - 5;
		}
		else {
			poligon4 = true;
		}
	}


	if (activeEnemy2) {

		if (sqrt((centru1x - centru2x) * (centru1x - centru2x) + (centru1y - centru2y) * (centru1y - centru2y)) <= raza1 + raza2)
				if (lives == 0)
					exit(0);
				else {
					lives--;
					removeObject2D_from_Visual2D(enemy21,v2d1);
					removeObject2D_from_Visual2D(enemy22,v2d1);
					activeEnemy2 = false;
					count2 = 0;
					removeText_from_Visual2D(text2,v2d2);
					text2 = new Text(to_string(lives), Point2D(700,300),Color(1,1,0),BITMAP_TIMES_ROMAN_24);
					addText_to_Visual2D(text2,v2d2);
				}

		if (activeWeapon)
			if (sqrt((centruArma1x - centru2x) * (centruArma1x - centru2x) + (centruArma1y - centru2y) * (centruArma1y - centru2y)) <= razaArma1 + raza2 
				|| sqrt((centruArma2x - centru2x) * (centruArma2x - centru2x) + (centruArma2y - centru2y) * (centruArma2y - centru2y)) <= razaArma2 + raza2)
				{
					removeObject2D_from_Visual2D(enemy21,v2d1);
					removeObject2D_from_Visual2D(enemy22,v2d1);
					activeEnemy2 = false;
					myScore = myScore + 5;
					removeText_from_Visual2D(score,v2d2);
					score = new Text(to_string(myScore), Point2D(450,300),Color(1,1,0),BITMAP_TIMES_ROMAN_24);
					addText_to_Visual2D(score,v2d2);
				}
	}

	if (activeEnemy3) {
		if (sqrt((centru1x - centru3x) * (centru1x - centru3x) + (centru1y - centru3y) * (centru1y - centru3y)) <= raza1 + raza3)
			if (lives == 0)
				exit(0);
			else {
				lives--;
				removeObject2D_from_Visual2D(enemy31,v2d1);
				removeObject2D_from_Visual2D(enemy32,v2d1);
				activeEnemy3 = false;
				count3 = 0;
				removeText_from_Visual2D(text2,v2d2);
				text2 = new Text(to_string(lives), Point2D(700,300),Color(1,1,0),BITMAP_TIMES_ROMAN_24);
				addText_to_Visual2D(text2,v2d2);
			}

		if (activeWeapon)
			if (sqrt((centruArma1x - centru3x) * (centruArma1x - centru3x) + (centruArma1y - centru3y) * (centruArma1y - centru3y)) <= razaArma1 + raza3 
				|| sqrt((centruArma2x - centru3x) * (centruArma2x - centru3x) + (centruArma2y - centru3y) * (centruArma2y - centru3y)) <= razaArma2 + raza3)
				{
					removeObject2D_from_Visual2D(enemy31,v2d1);
					removeObject2D_from_Visual2D(enemy32,v2d1);
					activeEnemy3 = false;
					myScore = myScore + 5;
					removeText_from_Visual2D(score,v2d2);
					score = new Text(to_string(myScore), Point2D(450,300),Color(1,1,0),BITMAP_TIMES_ROMAN_24);
					addText_to_Visual2D(score,v2d2);
				}
	}

	if (activeEnemy4) {
		for (int i = 0; i < 4; i++) 
			if (sqrt((centru1x - centru4x[i]) * (centru1x - centru4x[i]) + (centru1y - centru4y[i]) * (centru1y - centru4y[i])) <= raza1 + raza4)
				if (lives == 0)
					exit(0);
				else {
					lives--;
					removeObject2D_from_Visual2D(enemy41,v2d1);
					removeObject2D_from_Visual2D(enemy42,v2d1);
					removeObject2D_from_Visual2D(enemy43,v2d1);
					removeObject2D_from_Visual2D(enemy44,v2d1);
					activeEnemy4 = false;
					count4 = 0;
					removeText_from_Visual2D(text2,v2d2);
					text2 = new Text(to_string(lives), Point2D(700,300),Color(1,1,0),BITMAP_TIMES_ROMAN_24);
					addText_to_Visual2D(text2,v2d2);
				}

		if (activeWeapon) {
			for (int i = 0; i < 4; i++) 
				if (sqrt((centruArma1x - centru4x[i]) * (centruArma1x - centru4x[i]) + (centruArma1y - centru4y[i]) * (centruArma1y - centru4y[i])) <= razaArma1 + raza4 
					|| sqrt((centruArma2x - centru4x[i]) * (centruArma2x - centru4x[i]) + (centruArma2y - centru4y[i]) * (centruArma2y - centru4y[i])) <= razaArma2 + raza4)
					{
						removeObject2D_from_Visual2D(enemy41,v2d1);
						removeObject2D_from_Visual2D(enemy42,v2d1);
						removeObject2D_from_Visual2D(enemy43,v2d1);
						removeObject2D_from_Visual2D(enemy44,v2d1);
						activeEnemy4 = false;
						myScore = myScore + 8;
						removeText_from_Visual2D(score,v2d2);
						score = new Text(to_string(myScore), Point2D(450,300),Color(1,1,0),BITMAP_TIMES_ROMAN_24);
						addText_to_Visual2D(score,v2d2);
					}
		}
	}

	for (int i = 0; i < smartCount; i++) {
		if (activeSmartEnemies[i]) {
			if (sqrt((centru1x - centruSmartEnemiesx[i]) * (centru1x - centruSmartEnemiesx[i]) + (centru1y - centruSmartEnemiesy[i]) * (centru1y - centruSmartEnemiesy[i])) <= raza1 + 15)
				if (lives == 0)
					exit(0);
				else {
					lives--;
					removeObject2D_from_Visual2D(smartEnemies[i],v2d1);
					activeSmartEnemies[i] = false;
					removeText_from_Visual2D(text2,v2d2);
					text2 = new Text(to_string(lives), Point2D(700,300),Color(1,1,0),BITMAP_TIMES_ROMAN_24);
					addText_to_Visual2D(text2,v2d2);
				}

			if (activeWeapon) {
				if (sqrt((centruArma1x - centruSmartEnemiesx[i]) * (centruArma1x - centruSmartEnemiesx[i]) + (centruArma1y - centruSmartEnemiesy[i]) * (centruArma1y - centruSmartEnemiesy[i])) <= razaArma1 + 15 
					|| sqrt((centruArma2x - centruSmartEnemiesx[i]) * (centruArma2x - centruSmartEnemiesx[i]) + (centruArma2y - centruSmartEnemiesy[i]) * (centruArma2y - centruSmartEnemiesy[i])) <= razaArma2 + 15)
					{
						removeObject2D_from_Visual2D(smartEnemies[i],v2d1);
						activeSmartEnemies[i] = false;
						myScore = myScore + (i/2 + 1) * 10;
						removeText_from_Visual2D(score,v2d2);
						score = new Text(to_string(myScore), Point2D(450,300),Color(1,1,0),BITMAP_TIMES_ROMAN_24);
						addText_to_Visual2D(score,v2d2);
					}
			}
		}
	}

	if (activeEnemy5) {

		if (sqrt((centru1x - centru5x[0]) * (centru1x - centru5x[0]) + (centru1y - centru5y[0]) * (centru1y - centru5y[0])) <= raza1 + raza5
			|| sqrt((centru1x - centru5x[1]) * (centru1x - centru5x[1]) + (centru1y - centru5y[1]) * (centru1y - centru5y[1])) <= raza1 + raza5)
				if (lives == 0)
					exit(0);
				else {
					lives--;
					removeObject2D_from_Visual2D(enemy51,v2d1);
					removeObject2D_from_Visual2D(enemy52,v2d1);
					activeEnemy5 = false;
					count5 = 0;
					removeText_from_Visual2D(text2,v2d2);
					text2 = new Text(to_string(lives), Point2D(700,300),Color(1,1,0),BITMAP_TIMES_ROMAN_24);
					addText_to_Visual2D(text2,v2d2);
				}

		if (activeWeapon)
			for (int i = 0; i < 2; i++)
			if (sqrt((centruArma1x - centru5x[i]) * (centruArma1x - centru5x[i]) + (centruArma1y - centru5y[i]) * (centruArma1y - centru5y[i])) <= razaArma1 + raza5 
				|| sqrt((centruArma2x - centru5x[i]) * (centruArma2x - centru5x[i]) + (centruArma2y - centru5y[i]) * (centruArma2y - centru5y[i])) <= razaArma2 + raza5)
				{
					removeObject2D_from_Visual2D(enemy51,v2d1);
					removeObject2D_from_Visual2D(enemy52,v2d1);
					activeEnemy5 = false;
					myScore = myScore + 15;
					removeText_from_Visual2D(score,v2d2);
					score = new Text(to_string(myScore), Point2D(450,300),Color(1,1,0),BITMAP_TIMES_ROMAN_24);
					addText_to_Visual2D(score,v2d2);
				}
	}

	if (activeHeart) {
		if (sqrt((centru1x - centruHeartx) * (centru1x - centruHeartx) + (centru1y - centruHearty) * (centru1y - centruHearty)) <= raza1 + razaHeart
			|| sqrt((centru1x - centruHeart1x) * (centru1x - centruHeart1x) + (centru1y - centruHeart1y) * (centru1y - centruHeart1y)) <= raza1 + razaHeart1
			|| sqrt((centru1x - centruHeart2x) * (centru1x - centruHeart2x) + (centru1y - centruHeart2y) * (centru1y - centruHeart2y)) <= raza1 + razaHeart2) {
			lives++;
			activeHeart = false;
			removeObject2D_from_Visual2D(heart1, v2d1);
			removeObject2D_from_Visual2D(heart2, v2d1);
			removeObject2D_from_Visual2D(newLive, v2d1);
			removeText_from_Visual2D(text2,v2d2);
			text2 = new Text(to_string(lives), Point2D(700,300),Color(1,1,0),BITMAP_TIMES_ROMAN_24);
			addText_to_Visual2D(text2,v2d2);
		}
	}

	// Miscarea inamicului 21,22
	Transform2D::loadIdentityMatrix();
	Transform2D::translateMatrix(-185,-185);
	Transform2D::rotateMatrix(u2);
	Transform2D::translateMatrix(185, 185);
	Transform2D::applyTransform(enemy21);
	Transform2D::applyTransform(enemy22);
	
	// Miscarea inamicului 31,32
	Transform2D::loadIdentityMatrix();
	Transform2D::translateMatrix(-185,-585);
	Transform2D::rotateMatrix(u2);
	Transform2D::translateMatrix(185, 585);
	Transform2D::applyTransform(enemy31);
	Transform2D::applyTransform(enemy32);

	u2 = u2 + PI/32;

	// Miscarea inamicului 51,52
	if (stateEnemy5 == 0) { // coboara stanga jos
		if (centru5x[0] > 35 && centru5y[0] > 35) {
			tx5 = tx5 - 2;
			ty5 = ty5 - 2;
			centru5x[0] = centru5x[0] - 2;
			centru5y[0] = centru5y[0] - 2;
			Transform2D::loadIdentityMatrix();
			Transform2D::translateMatrix(tx5, ty5);
			Transform2D::applyTransform(enemy51);
			Transform2D::applyTransform(enemy52);
		}
		else {
			stateEnemy5 = 1;
		}
	}

	if (stateEnemy5 == 1) { // urca
		if (centru5x[0] > 35 && centru5y[0] < DrawingWindow::height - 35) {
			ty5 = ty5 + 2;
			centru5y[0] = centru5y[0] + 2;
			Transform2D::loadIdentityMatrix();
			Transform2D::translateMatrix(tx5, ty5);
			Transform2D::applyTransform(enemy51);
			Transform2D::applyTransform(enemy52);
		}
		else {
			stateEnemy5 = 2;
		}
	}

	if (stateEnemy5 == 2) { // coboara dreapta jos
		if (centru5x[0] < DrawingWindow::width - 35 && centru5y[0] > 35) {
			tx5 = tx5 + 2;
			ty5 = ty5 - 2;
			centru5x[0] = centru5x[0] + 2;
			centru5y[0] = centru5y[0] - 2;
			Transform2D::loadIdentityMatrix();
			Transform2D::translateMatrix(tx5, ty5);
			Transform2D::applyTransform(enemy51);
			Transform2D::applyTransform(enemy52);
		}
		else {
			stateEnemy5 = 3;
		}
	}

	if (stateEnemy5 == 3) { // urca
		if (centru5x[0] < DrawingWindow::width - 35 && centru5y[0] < DrawingWindow::height - 35) {
			ty5 = ty5 + 2;
			centru5y[0] = centru5y[0] + 2;
			Transform2D::loadIdentityMatrix();
			Transform2D::translateMatrix(tx5, ty5);
			Transform2D::applyTransform(enemy51);
			Transform2D::applyTransform(enemy52);
		}
		else {
			stateEnemy5 = 0;
		}
	}


	// Verificarea tastelor care sunt apasate

	if (keyStates[27]) {
		exit(0);
	}

	if (keySpecialStates[KEY_UP]) {
			Transform2D::loadIdentityMatrix();
			Transform2D::translateMatrix(-c1x,-c1y);
			Transform2D::rotateMatrix(u1);
			Transform2D::translateMatrix(c1x, c1y);
			if (!activeWeapon) {
				if (c1x + tx  + delta * cos(u1) >= 40 && c1x + tx + delta * cos(u1) <= DrawingWindow::width - 40)
					tx = tx + delta * cos(u1);
				if (c1y + ty  + delta * sin(u1) >= 40 && c1y + ty + delta * sin(u1) <= DrawingWindow::height - 20)
					ty = ty + delta * sin(u1);
			}
			else {
				if (c1x + tx  + delta * cos(u1) + 130 * cos(u1) >= 0 && c1x + tx + delta * cos(u1) + 130 * cos(u1) - 40 <= DrawingWindow::width - 40)
					tx = tx + delta * cos(u1);
				if (c1y + ty  + delta * sin(u1) + 130 * sin(u1) >= 0 && c1y + ty + delta * sin(u1) + 130 * sin(u1) - 40 <= DrawingWindow::height - 20)
					ty = ty + delta * sin(u1);
			}
			Transform2D::translateMatrix(tx, ty);
			Transform2D::applyTransform(playerCircle);
			Transform2D::applyTransform(player);
			Transform2D::applyTransform(weapon);
			Transform2D::applyTransform(weaponCircle1);
			Transform2D::applyTransform(weaponCircle2);
			centru1x = c1x + tx;
			centru1y = c1y + ty;
			centruArma1x = c1x + 55 * cos(u1) + tx;
			centruArma1y = c1y + 55 * sin(u1) + ty;
			centruArma2x = c1x + 110 * cos(u1) + tx;
			centruArma2y = c1y + 110 * sin(u1) + ty;
	}
	if (keySpecialStates[KEY_LEFT]) {
			u1 = u1 + PI/24;
			if (activeWeapon)
			if (c1x + tx + 130 * cos(u1) < 0 || c1x + tx + 130 * cos(u1) - 40 > DrawingWindow::width - 40 ||
				c1y + ty  + 130 * sin(u1) < 0 || c1y + ty + 130 * sin(u1) - 40 > DrawingWindow::height - 20)
				u1 = u1 - PI/24;
			Transform2D::loadIdentityMatrix();
			Transform2D::translateMatrix(-c1x,-c1y);
			Transform2D::rotateMatrix(u1);
			Transform2D::translateMatrix(c1x,c1y);
			Transform2D::translateMatrix(tx, ty);
			Transform2D::applyTransform(playerCircle);
			Transform2D::applyTransform(player);
			Transform2D::applyTransform(weapon);
			Transform2D::applyTransform(weaponCircle1);
			Transform2D::applyTransform(weaponCircle2);
	}
	if (keySpecialStates[KEY_RIGHT]) {
			u1 = u1 - PI/24;	
			if (activeWeapon)
			if (c1x + tx + 130 * cos(u1) < 0 || c1x + tx + 130 * cos(u1) - 40 > DrawingWindow::width - 40 ||
				c1y + ty  + 130 * sin(u1) < 0 || c1y + ty + 130 * sin(u1) - 40 > DrawingWindow::height - 20)
				u1 = u1 + PI/24;
			Transform2D::loadIdentityMatrix();
			Transform2D::translateMatrix(-c1x, -c1y);
			Transform2D::rotateMatrix(u1);
			Transform2D::translateMatrix(c1x, c1y);
			Transform2D::translateMatrix(tx, ty);
			Transform2D::applyTransform(playerCircle);
			Transform2D::applyTransform(player);
			Transform2D::applyTransform(weapon);
			Transform2D::applyTransform(weaponCircle1);
			Transform2D::applyTransform(weaponCircle2);
	}

}

//functia care se apeleaza la redimensionarea ferestrei
void DrawingWindow::onReshape(int width,int height)
{
	
	v2d1->poarta(100,200,width-100,height-100); 
	v2d2->poarta(100,0,width-100,200);

}

//functia care defineste ce se intampla cand se apasa pe tastatura
void DrawingWindow::onKey(unsigned char key)
{
	keyStates[key] = true;

	// activarea si dezactivarea armei
	if (keyStates[KEY_SPACE]) 
			if (!activeWeapon) {
				if (c1x + tx + 130 * cos(u1) < 0 || c1x + tx + 130 * cos(u1) - 40 > DrawingWindow::width - 40 ||
					c1y + ty  + 130 * sin(u1) < 0 || c1y + ty + 130 * sin(u1) - 40 > DrawingWindow::height - 20) {
				}
				else {
					addObject2D_to_Visual2D(weapon, v2d1);
					activeWeapon = true;
					Transform2D::loadIdentityMatrix();
					Transform2D::translateMatrix(-c1x,-c1y);
					Transform2D::rotateMatrix(u1);
					Transform2D::translateMatrix(c1x, c1y);
					Transform2D::translateMatrix(tx, ty);
					Transform2D::applyTransform(weapon);	
					delta = 2.5;
				}
			}
			else {
				removeObject2D_from_Visual2D(weapon, v2d1);
				activeWeapon = false;
				delta = 5;
			}

}

void DrawingWindow::onKeyUp(unsigned char key)
{
	keyStates[key] = false;
}

void DrawingWindow::onSpecialKey(int key)
{
	keySpecialStates[key] = true;
}

void DrawingWindow::onSpecialKeyUp(int key)
{
	keySpecialStates[key] = false;
}




//functia care defineste ce se intampla cand se da click pe mouse
void DrawingWindow::onMouse(int button,int state,int x, int y)
{
	cout<<"coords are "<<x<<y<<endl;
}


int main(int argc, char** argv)
{
	//creare fereastra
	DrawingWindow dw(argc, argv, 1366, 768, 0, 0, "Laborator EGC");
	//se apeleaza functia init() - in care s-au adaugat obiecte
	dw.init();
	//se intra in bucla principala de desenare - care face posibila desenarea, animatia si procesarea evenimentelor
	dw.run();
	return 0;

}