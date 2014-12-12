#include "Framework/DrawingWindow.h"
#include "Framework/Visual2D.h"
#include "Framework/Transform2D.h"
#include "Framework/Transform3D.h"
#include "Framework/Line2D.h"
#include "Framework/Rectangle2D.h"
#include "Framework/Circle2D.h"
#include "Framework/Polygon2D.h"
#include <iostream>
#include <windows.h>


#define PI 3.14159265358979323846
#define inf 1000000
using namespace std;

Visual2D *v2d1, *v2d2;
Object3D *drum,*cube21,*masina,*cube12,*cube22,*cube32,*banda1,*banda2,*banda3,*banda_stanga[12],*banda_dreapta[12],*masini[5],
	*obstacol[5], *piramida;
Text *text1, *score, *text2, *distanta_ramasa, *vieti_ramase, *text3, *game_over;

bool peSosea[5], obstacol_activ[5], piramida_activa;
float distances[5], distanta_obstacol[5], distanta_piramida;
int contor = 0, contor_obstacol = 0;
int banda_masina[5], banda_obstacol[5];
float z_masina[5], z_obstacol[5], z_piramida;

float centru_masina_x, centru_masina_z, centru_masini_x[5], centru_masini_z[5], centru_obstacol_x[5], centru_obstacol_z[5],
	centru_piramida_x, centru_piramida_z;
float raza_masina_x, raza_masina_z, raza_masini_x[5], raza_masini_z[5], raza_obstacol_x[5], raza_obstacol_z[5],
	raza_piramida_x, raza_piramida_z;

bool initial = true;
float n=40;
int vieti = 3;
float tz_banda[12];
float tx = 0;
int punctaj = 0, frames = 0, bonus = 0;

bool* keyStates = new bool[256];
bool* keySpecialStates = new bool[246];


//functia care permite adaugarea de obiecte
void DrawingWindow::init()
{
	
	v2d1 = new Visual2D(-200,-100,200,100,0,DrawingWindow::height/4,DrawingWindow::width,DrawingWindow::height); 
	v2d2 = new Visual2D(-20,-10,20,10,0,0,DrawingWindow::width,DrawingWindow::height/4); 
//	v2d1->tipTran(true);
//	v2d2->tipTran(true);
	v2d1->cadruFereastra(Color (1,1,0));
	v2d2->cadruFereastra(Color (0,1,1));
	addVisual2D(v2d1);
	addVisual2D(v2d2);

	
	//se deseneaza un cub
	
	vector <Point3D*> vertices;
	vector <Face*> faces;
	
	//varfurile de jos
	vertices.push_back(new Point3D(0,0,0));
	vertices.push_back(new Point3D(n,0,0));
	vertices.push_back(new Point3D(n,0,n));
	vertices.push_back(new Point3D(0,0,n));
	//varfurile de sus
	vertices.push_back(new Point3D(0,0,0));
	vertices.push_back(new Point3D(n,0,0));
	vertices.push_back(new Point3D(n,0,n));
	vertices.push_back(new Point3D(0,0,n));
	
	//cele 6 fete
	vector <int> contour;
	//fata jos
	contour.clear();
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(3);
	faces.push_back(new Face(contour));
	//fata sus
	contour.clear();
	contour.push_back(4);
	contour.push_back(5);
	contour.push_back(6);
	contour.push_back(7);
	faces.push_back(new Face(contour));
	//fata fata
	contour.clear();
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(5);
	contour.push_back(4);
	faces.push_back(new Face(contour));
	//fata dreapta
	contour.clear();
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(6);
	contour.push_back(5);
	faces.push_back(new Face(contour));
	//fata spate
	contour.clear();
	contour.push_back(2);
	contour.push_back(3);
	contour.push_back(7);
	contour.push_back(6);
	faces.push_back(new Face(contour));
	//fata stanga
	contour.clear();
	contour.push_back(3);
	contour.push_back(0);
	contour.push_back(4);
	contour.push_back(7);
	faces.push_back(new Face(contour));
	
	drum = new Object3D(vertices,faces,Color(1,0,0),false);
	addObject3D_to_Visual2D(drum,v2d1);
	

	cube21 = new Object3D(vertices,faces,Color(0,1,0),false);
//	addObject3D_to_Visual2D(cube21,v2d1);
	
	
	cube12 = new Object3D(vertices,faces,Color(1,0,0),false);
	//addObject3D_to_Visual2D(cube12,v2d2);
	
	cube22 = new Object3D(vertices,faces,Color(0,1,0),false);
	//addObject3D_to_Visual2D(cube22,v2d2);
	
	cube32 = new Object3D(vertices,faces,Color(0,0,1),false);
	//addObject3D_to_Visual2D(cube32,v2d2);


	banda1 = new Object3D(vertices,faces,Color(1,1,1),true);
//	addObject3D_to_Visual2D(banda1,v2d1);

	banda2 = new Object3D(vertices,faces,Color(1,1,1),true);
//	addObject3D_to_Visual2D(banda2,v2d1);

	banda3 = new Object3D(vertices,faces,Color(1,1,1),true);
//	addObject3D_to_Visual2D(banda3,v2d1);

	for (int i = 0; i < 12; i++) {
		banda_stanga[i] = new Object3D(vertices,faces,Color(1,1,1),true);
		addObject3D_to_Visual2D(banda_stanga[i],v2d1);
		banda_dreapta[i] = new Object3D(vertices,faces,Color(1,1,1),true);
		addObject3D_to_Visual2D(banda_dreapta[i],v2d1);

		tz_banda[i] = 0;
	}


	vertices.clear();
	faces.clear();
	//varfurile de jos
	vertices.push_back(new Point3D(0,0,0));
	vertices.push_back(new Point3D(n,0,0));
	vertices.push_back(new Point3D(n,0,n));
	vertices.push_back(new Point3D(0,0,n));
	//varfurile de sus
	vertices.push_back(new Point3D(0,n,0));
	vertices.push_back(new Point3D(n,n,0));
	vertices.push_back(new Point3D(n,n,n));
	vertices.push_back(new Point3D(0,n,n));
	
	//cele 6 fete
	//fata jos
	contour.clear();
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(3);
	faces.push_back(new Face(contour));
	//fata sus
	contour.clear();
	contour.push_back(4);
	contour.push_back(5);
	contour.push_back(6);
	contour.push_back(7);
	faces.push_back(new Face(contour));
	//fata fata
	contour.clear();
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(5);
	contour.push_back(4);
	faces.push_back(new Face(contour));
	//fata dreapta
	contour.clear();
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(6);
	contour.push_back(5);
	faces.push_back(new Face(contour));
	//fata spate
	contour.clear();
	contour.push_back(2);
	contour.push_back(3);
	contour.push_back(7);
	contour.push_back(6);
	faces.push_back(new Face(contour));
	//fata stanga
	contour.clear();
	contour.push_back(3);
	contour.push_back(0);
	contour.push_back(4);
	contour.push_back(7);
	faces.push_back(new Face(contour));

	masina = new Object3D(vertices,faces,Color(0,0,1),false);
	addObject3D_to_Visual2D(masina,v2d1);

	centru_masina_x = 0;
	centru_masina_z = 0;
	raza_masina_x = n/2 * 1.414 * 0.3;
	raza_masina_z = n/2 * 1.414 * 0.1;
//    printf("raza x %f raza z %f\n", raza_masina_x*0.3, raza_masina_z*0.1);

	for (int i = 0; i < 5; i++) {
		masini[i] = new Object3D(vertices,faces,Color(i % 2,(i + 1) % 2,i % 2),false);
		peSosea[i] = false;
		distances[i] = 0;
		centru_masini_x[i] = n/2;
		centru_masini_z[i] = n/2;
		raza_masini_x[i] = n/2 * 1.414 * 0.3;
		raza_masini_z[i] = n/2 * 1.414 * 0.1;
	}

	for (int i = 0; i < 5; i++) {
		obstacol[i] = new Object3D(vertices,faces,Color((i + 1) % 2,i % 2,i % 2),true);
		obstacol_activ[i] = false;
		distanta_obstacol[i] = 0;
		centru_obstacol_x[i] = n/2;
		centru_obstacol_z[i] = n/2;
		raza_obstacol_x[i] = n/2 * 1.414 * 0.3;
		raza_obstacol_z[i] = n/2 * 1.414 * 0.1;		
	}


	text1 = new Text("Score:", Point2D(-13,0),Color(1,1,0),BITMAP_TIMES_ROMAN_24);
	addText_to_Visual2D(text1,v2d2);
	score = new Text("0", Point2D(-10,0),Color(1,1,0),BITMAP_TIMES_ROMAN_24);
	addText_to_Visual2D(score,v2d2);

	text2 = new Text("Distanta ramasa:", Point2D(0,0),Color(1,1,0),BITMAP_TIMES_ROMAN_24);
	addText_to_Visual2D(text2,v2d2);
	distanta_ramasa = new Text("150000", Point2D(7,0),Color(1,1,0),BITMAP_TIMES_ROMAN_24);
	addText_to_Visual2D(distanta_ramasa,v2d2);
	
	text3 = new Text("Vieti:", Point2D(12,0),Color(1,1,0),BITMAP_TIMES_ROMAN_24);
	addText_to_Visual2D(text3,v2d2);
	vieti_ramase = new Text(to_string(vieti), Point2D(15,0),Color(1,1,0),BITMAP_TIMES_ROMAN_24);
	addText_to_Visual2D(vieti_ramase,v2d2);

	game_over = new Text("GAME OVER", Point2D(0,0),Color(1,0,1),BITMAP_TIMES_ROMAN_24);

	// Initializare booleani de key
	for (int i = 0; i < 246; i++) {
		keyStates[i] = false;
		keySpecialStates[i] = false;
	}

	for (int i = 246; i < 256; i++) {
		keySpecialStates[i] = false;
	}


	vertices.clear();
	faces.clear();

	//varfurile de jos
	vertices.push_back(new Point3D(0,0,0));
	vertices.push_back(new Point3D(n,0,0));
	vertices.push_back(new Point3D(n,0,n));
	vertices.push_back(new Point3D(0,0,n));
	//varfurile de sus
	vertices.push_back(new Point3D(n/2,n,n/2));
	
	//cele 6 fete
	//fata jos
	contour.clear();
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(3);
	faces.push_back(new Face(contour));
	//fata fata
	contour.clear();
	contour.push_back(0);
	contour.push_back(1);
	contour.push_back(4);
	faces.push_back(new Face(contour));
	//fata dreapta
	contour.clear();
	contour.push_back(1);
	contour.push_back(2);
	contour.push_back(4);
	faces.push_back(new Face(contour));
	//fata spate
	contour.clear();
	contour.push_back(2);
	contour.push_back(3);
	contour.push_back(4);
	faces.push_back(new Face(contour));
	//fata stanga
	contour.clear();
	contour.push_back(3);
	contour.push_back(0);
	contour.push_back(4);
	faces.push_back(new Face(contour));

	piramida = new Object3D(vertices,faces,Color(1,0,0),false);
	raza_piramida_x = n/2 * 1.414 * 0.3;
	raza_piramida_z = n/2 * 1.414 * 0.1;		
	
}


//functia care permite animatia
void DrawingWindow::onIdle()
{

	if (distanta_ramasa == 0) {
		Sleep(2500);
		exit(0);
	}

	if (initial) {
		for (int i = 0; i < 12; i++) {
			tz_banda[i] = - 6 * i;
			
			Transform3D::loadIdentityModelMatrix();
			Transform3D::translateMatrix(-n/2,-n/2,-n/2);
			Transform3D::scaleMatrix(0.1,0.2,0.1);
			Transform3D::translateMatrix(n/2,n/2,n/2);
			Transform3D::translateMatrix(-18,-50,3 + tz_banda[i]);

			Transform3D::loadIdentityProjectionMatrix();
			Transform3D::perspectiveProjectionMatrix(20,30,50);
			Transform3D::applyTransform(banda_stanga[i]);

			Transform3D::loadIdentityModelMatrix();
			Transform3D::translateMatrix(-n/2,-n/2,-n/2);
			Transform3D::scaleMatrix(0.1,0.2,0.1);
			Transform3D::translateMatrix(n/2,n/2,n/2);
			Transform3D::translateMatrix(18,-50,3 + tz_banda[i]);

			Transform3D::loadIdentityProjectionMatrix();
			Transform3D::perspectiveProjectionMatrix(20,30,50);
			Transform3D::applyTransform(banda_dreapta[i]);
		}
		initial = false;
	}
	

	//verificarea tastelor ce sunt apasate
	if (keyStates[27]) {
		exit(0);
	}



	//translatarea benzilor

	if (keySpecialStates[KEY_UP]) 
		for (int i = 0; i < 12; i++) {
			tz_banda[i] += 0.5;
			if (tz_banda[i] > 7) {
				tz_banda[i] = -66;
			}

			Transform3D::loadIdentityModelMatrix();
			Transform3D::translateMatrix(-n/2,-n/2,-n/2);
			Transform3D::scaleMatrix(0.1,0.2,0.1);
			Transform3D::translateMatrix(n/2,n/2,n/2);
			Transform3D::translateMatrix(-18,-50,3 + tz_banda[i]);
		
			Transform3D::loadIdentityProjectionMatrix();
			Transform3D::perspectiveProjectionMatrix(20,30,50);
			Transform3D::applyTransform(banda_stanga[i]);
		
			Transform3D::loadIdentityModelMatrix();
			Transform3D::translateMatrix(-n/2,-n/2,-n/2);
			Transform3D::scaleMatrix(0.1,0.2,0.1);
			Transform3D::translateMatrix(n/2,n/2,n/2);
			Transform3D::translateMatrix(18,-50,3 + tz_banda[i]);

			Transform3D::loadIdentityProjectionMatrix();
			Transform3D::perspectiveProjectionMatrix(20,30,50);
			Transform3D::applyTransform(banda_dreapta[i]);


			// Afisarea punctajului
			punctaj++;
			punctaj++;
			removeText_from_Visual2D(score,v2d2);
			score = new Text(to_string(punctaj + bonus), Point2D(-10,0),Color(1,1,0),BITMAP_TIMES_ROMAN_24);
			addText_to_Visual2D(score,v2d2);

			removeText_from_Visual2D(distanta_ramasa,v2d2);
			distanta_ramasa = new Text(to_string(150000-punctaj), Point2D(7,0),Color(1,1,0),BITMAP_TIMES_ROMAN_24);
			addText_to_Visual2D(distanta_ramasa,v2d2);

		}
	else if (!keySpecialStates[KEY_DOWN]) {
		for (int i = 0; i < 12; i++) {
			tz_banda[i] += 0.3;
			if (tz_banda[i] > 7) {
				tz_banda[i] = -66;
			}

			Transform3D::loadIdentityModelMatrix();
			Transform3D::translateMatrix(-n/2,-n/2,-n/2);
			Transform3D::scaleMatrix(0.1,0.2,0.1);
			Transform3D::translateMatrix(n/2,n/2,n/2);
			Transform3D::translateMatrix(-18,-50,3 + tz_banda[i]);
		
			Transform3D::loadIdentityProjectionMatrix();
			Transform3D::perspectiveProjectionMatrix(20,30,50);
			Transform3D::applyTransform(banda_stanga[i]);
		
			Transform3D::loadIdentityModelMatrix();
			Transform3D::translateMatrix(-n/2,-n/2,-n/2);
			Transform3D::scaleMatrix(0.1,0.2,0.1);
			Transform3D::translateMatrix(n/2,n/2,n/2);
			Transform3D::translateMatrix(18,-50,3 + tz_banda[i]);

			Transform3D::loadIdentityProjectionMatrix();
			Transform3D::perspectiveProjectionMatrix(20,30,50);
			Transform3D::applyTransform(banda_dreapta[i]);


			// Afisarea punctajului
			punctaj++;
			removeText_from_Visual2D(score,v2d2);
			score = new Text(to_string(punctaj + bonus), Point2D(-10,0),Color(1,1,0),BITMAP_TIMES_ROMAN_24);
			addText_to_Visual2D(score,v2d2);

			removeText_from_Visual2D(distanta_ramasa,v2d2);
			distanta_ramasa = new Text(to_string(150000-punctaj), Point2D(7,0),Color(1,1,0),BITMAP_TIMES_ROMAN_24);
			addText_to_Visual2D(distanta_ramasa,v2d2);

		}
	}

	if (!keySpecialStates[KEY_LEFT] && !keySpecialStates[KEY_RIGHT]) {

		Transform3D::loadIdentityModelMatrix();
		Transform3D::translateMatrix(-n/2,-n/2,-n/2);
		Transform3D::scaleMatrix(0.3,0.1,0.1);
		Transform3D::translateMatrix(n/2,n/2,n/2);
		Transform3D::translateMatrix(tx,-50,3);

		Transform3D::loadIdentityProjectionMatrix();
		Transform3D::perspectiveProjectionMatrix(20,30,50);
	
			
		Transform3D::applyTransform(masina);

	}


	if (keySpecialStates[KEY_LEFT]) {

		if (-10 + tx > -45) {

			Transform3D::loadIdentityModelMatrix();
			Transform3D::translateMatrix(-n/2,-n/2,-n/2);
			Transform3D::scaleMatrix(0.3,0.1,0.1);
			Transform3D::translateMatrix(n/2,n/2,n/2);
			Transform3D::translateMatrix(-10 + tx,-50,3);

			Transform3D::loadIdentityProjectionMatrix();
			Transform3D::perspectiveProjectionMatrix(0,30,50);
	

			Transform3D::applyTransform(masina);

			centru_masina_x -= 0.5;
			tx -= 0.5;
		}

	}

	if (keySpecialStates[KEY_RIGHT]) {

		if (-10 + tx < 25) {

			Transform3D::loadIdentityModelMatrix();
			Transform3D::translateMatrix(-n/2,-n/2,-n/2);
			Transform3D::scaleMatrix(0.3,0.1,0.1);
			Transform3D::translateMatrix(n/2,n/2,n/2);
			Transform3D::translateMatrix(10 + tx,-50,3);

			Transform3D::loadIdentityProjectionMatrix();
			Transform3D::perspectiveProjectionMatrix(40,30,50);
	

			Transform3D::applyTransform(masina);

			centru_masina_x += 0.5;
			tx += 0.5;
		}

	}

	frames++;
	if (frames % 150 == 0) {
		addObject3D_to_Visual2D(masini[contor % 5],v2d1);
		peSosea[contor % 5] = true;
		banda_masina[contor % 5] = rand() %3;
		z_masina[contor % 5] = -66;
		centru_masini_x[contor % 5] = (banda_masina[contor % 5] - 1) * 30;
		centru_masini_z[contor % 5] = z_masina[contor % 5];


		Transform3D::loadIdentityModelMatrix();
		Transform3D::translateMatrix(-n/2,-n/2,-n/2);
		Transform3D::scaleMatrix(0.3,0.1,0.1);
		Transform3D::translateMatrix(n/2,n/2,n/2);
		Transform3D::translateMatrix( (banda_masina[contor % 5] - 1) * 30,-50,z_masina[contor % 5]);


		Transform3D::loadIdentityProjectionMatrix();
		Transform3D::perspectiveProjectionMatrix(20,30,50);
	
		Transform3D::applyTransform(masini[contor % 5]);

		contor++;

	}


	// Activarea obstacolelor fixe
	if (frames % 150 == 75) {
		addObject3D_to_Visual2D(obstacol[contor_obstacol % 5],v2d1);
		obstacol_activ[contor_obstacol % 5] = true;
		banda_obstacol[contor_obstacol % 5] = rand() %3;
		z_obstacol[contor_obstacol % 5] = -66;
		centru_obstacol_x[contor_obstacol % 5] = (banda_obstacol[contor_obstacol % 5] - 1) * 30;
		centru_obstacol_z[contor_obstacol % 5] = z_obstacol[contor_obstacol % 5];


		Transform3D::loadIdentityModelMatrix();
		Transform3D::translateMatrix(-n/2,-n/2,-n/2);
		Transform3D::scaleMatrix(0.3,0.3,0.1);
		Transform3D::translateMatrix(n/2,n/2,n/2);
		Transform3D::translateMatrix( (banda_obstacol[contor_obstacol % 5] - 1) * 30,-50,z_obstacol[contor_obstacol % 5]);


		Transform3D::loadIdentityProjectionMatrix();
		Transform3D::perspectiveProjectionMatrix(20,30,50);
	
		Transform3D::applyTransform(obstacol[contor_obstacol % 5]);

		contor_obstacol++;

	}

	// Apare piramida care ne da o noua viata
	if (frames % 1000 == 980) {
		addObject3D_to_Visual2D(piramida, v2d1);
		piramida_activa = true;
		z_piramida = -66;
		centru_piramida_x = 0;
		centru_piramida_z = -66;

		Transform3D::loadIdentityModelMatrix();
		Transform3D::translateMatrix(-n/2,-n/2,-n/2);
		Transform3D::scaleMatrix(0.3,0.3,0.1);
		Transform3D::translateMatrix(n/2,n/2,n/2);
		Transform3D::translateMatrix( 0,-50,z_piramida);


		Transform3D::loadIdentityProjectionMatrix();
		Transform3D::perspectiveProjectionMatrix(20,30,50);
	
		Transform3D::applyTransform(piramida);

	}

	// Detectam daca am trecut de masinile care merg in acelasi sens pentru a actualiza punctajul
	// Detectam coliziunile
	for (int i = 0; i < 5; i++) {

		if (peSosea[i]) {

			if (keySpecialStates[KEY_UP]) {
				z_masina[i] += 0.3;
				centru_masini_z[i] += 0.3;
			}
			else {
				z_masina[i] -= 0.3;
				centru_masini_z[i] -= 0.3;
			}

			if (z_masina[i] > 7 || z_masina[i] < -66) {
				bonus += 1500; //adaugam 1500 la punctaj pentru ca a trecut de masina
				peSosea[i] = false;
				removeObject3D_from_Visual2D(masini[i],v2d1);
				continue;
			}

			if ( abs(centru_masina_x - centru_masini_x[i]) < raza_masina_x + raza_masini_x[i] - 2 
				&& abs(centru_masina_z - centru_masini_z[i]) < raza_masina_z + raza_masini_z[i] - 4) {
				Sleep(500);

				if (vieti == 0) { 
					exit(0);
				}

				vieti--;
				removeText_from_Visual2D(vieti_ramase,v2d2);
				vieti_ramase = new Text(to_string(vieti), Point2D(15,0),Color(1,1,0),BITMAP_TIMES_ROMAN_24);
				addText_to_Visual2D(vieti_ramase,v2d2);


				removeObject3D_from_Visual2D(masini[i],v2d1);
				peSosea[i] = false;
				bonus -= 15000;
			}


			Transform3D::loadIdentityModelMatrix();
			Transform3D::translateMatrix(-n/2,-n/2,-n/2);
			Transform3D::scaleMatrix(0.3,0.1,0.1);
			Transform3D::translateMatrix(n/2,n/2,n/2);
			Transform3D::translateMatrix( (banda_masina[i] - 1) * 30,-50,z_masina[i]);

			Transform3D::loadIdentityProjectionMatrix();
			Transform3D::perspectiveProjectionMatrix(20,30,50);
	
			Transform3D::applyTransform(masini[i]);
		}

	}


	// Detectam daca am trecut de obstacole pentru a actualiza punctajul
	// Detectam coliziunile
	for (int i = 0; i < 5; i++) {
		if (obstacol_activ[i]) {

			if (keySpecialStates[KEY_UP]) {
				z_obstacol[i] += 0.5;
				centru_obstacol_z[i] += 0.5;
			}
			else if (!keySpecialStates[KEY_DOWN]) {
				z_obstacol[i] += 0.3;
				centru_obstacol_z[i] += 0.3;
			}

			if (z_obstacol[i] > 7) {
				bonus += 750; //adaugam 7500 la punctaj pentru ca a trecut de obstacol
				removeObject3D_from_Visual2D(obstacol[i],v2d1);
				obstacol_activ[i] = false;
				continue;
			}

			if ( abs(centru_masina_x - centru_obstacol_x[i]) < raza_masina_x + raza_obstacol_x[i] - 2 
				&& abs(centru_masina_z - centru_obstacol_z[i]) < raza_masina_z + raza_obstacol_z[i] - 2) {
				Sleep(500);

				if (vieti == 0) { 
					exit(0);
				}

				vieti--;
				removeText_from_Visual2D(vieti_ramase,v2d2);
				vieti_ramase = new Text(to_string(vieti), Point2D(15,0),Color(1,1,0),BITMAP_TIMES_ROMAN_24);
				addText_to_Visual2D(vieti_ramase,v2d2);


				removeObject3D_from_Visual2D(obstacol[i],v2d1);
				obstacol_activ[i] = false;
				bonus -= 7500;
			}

			Transform3D::loadIdentityModelMatrix();
			Transform3D::translateMatrix(-n/2,-n/2,-n/2);
			Transform3D::scaleMatrix(0.3,0.3,0.1);
			Transform3D::translateMatrix(n/2,n/2,n/2);
			Transform3D::translateMatrix( (banda_obstacol[i] - 1) * 30,-50,z_obstacol[i]);

			Transform3D::loadIdentityProjectionMatrix();
			Transform3D::perspectiveProjectionMatrix(20,30,50);
	
			Transform3D::applyTransform(obstacol[i]);

		}
	}

	// Detectam daca atingem piramida si luam o viata in plus
	if (piramida_activa) {
		if (keySpecialStates[KEY_UP]) {
			z_piramida += 0.5;
			centru_piramida_z += 0.5;
		}
		else if(!keySpecialStates[KEY_DOWN]) {
			z_piramida += 0.3;
			centru_piramida_z += 0.3;
		}

		if (z_piramida > 7) {
			removeObject3D_from_Visual2D(piramida, v2d1);
			piramida_activa = false;
		}
		else {

			if ( abs(centru_masina_x - centru_piramida_x) < raza_masina_x + raza_piramida_x - 2 
				&& abs(centru_masina_z - centru_piramida_z) < raza_masina_z + raza_piramida_z - 2) {

				vieti++;
				removeText_from_Visual2D(vieti_ramase,v2d2);
				vieti_ramase = new Text(to_string(vieti), Point2D(15,0),Color(1,1,0),BITMAP_TIMES_ROMAN_24);
				addText_to_Visual2D(vieti_ramase,v2d2);
				
				piramida_activa = false;
				removeObject3D_from_Visual2D(piramida, v2d1);
			}
			else {
				Transform3D::loadIdentityModelMatrix();
				Transform3D::translateMatrix(-n/2,-n/2,-n/2);
				Transform3D::scaleMatrix(0.3,0.3,0.1);
				Transform3D::translateMatrix(n/2,n/2,n/2);
				Transform3D::translateMatrix(0,-50,z_piramida);

				Transform3D::loadIdentityProjectionMatrix();
				Transform3D::perspectiveProjectionMatrix(20,30,50);
	
				Transform3D::applyTransform(piramida);
			}

		}
	}

	Transform3D::loadIdentityModelMatrix();
	Transform3D::loadIdentityProjectionMatrix();
	Transform3D::perspectiveProjectionMatrix(20,30,50);
	
	Transform3D::applyTransform(drum);
	
}

//functia care se apeleaza la redimensionarea ferestrei
void DrawingWindow::onReshape(int width,int height)
{
	
	v2d1->poarta(0,height/4,width,height); 
	v2d2->poarta(0,0,width,height/4);

}

//functia care defineste ce se intampla cand se apasa pe tastatura
void DrawingWindow::onKey(unsigned char key)
{
	keyStates[key] = true;
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
	
}


int main(int argc, char** argv)
{
	//creare fereastra
	DrawingWindow dw(argc, argv, 1000, 700, 50, 0, "TEMA 2");
	//se apeleaza functia init() - in care s-au adaugat obiecte
	dw.init();
	//se intra in bucla principala de desenare - care face posibila desenarea, animatia si procesarea evenimentelor
	dw.run();
	return 0;

}