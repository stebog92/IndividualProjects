Elemente de grafica pe calculator - Tema1

Geometry Wars
Bogdan Stefan 333 CA

Utilizand framework-ul de la laborator, a trebuit sa implementam acest joc. Pe un teren de joc(dreptunghi) se afla jucatorul pe care il controlez eu si inamici. Scopul jocului este sa distrug cat mai multi inamici pentru a acumula un punctaj cat mai mare.
Inamicii sunt distrusi folosind un burghiu care il activa cand dorim, acesta ingreunandu-ne deplasarea. 

Pentru a putea controla jucatorul, am folosit mai multe taste:
UP - pentru a merge inainte
LEFT - pentru a vira la stanga
RIGHT - pentru a vira la dreapta
SPACE - pentru activarea/dezactivarea burghiului
ESC - pentru a iesit din joc

Tema a fost implementata in Microsoft Visual Studio 2012, folosind sistemul de operare Windows8. Pentru aceasta tema, nu am folosit vreun algoritm complex, nici macar pentru inamicii inteligenti. Acestia pur si simplu imi stiu pozitia mea, iar directia lor de deplasare este in functie de diferenta de coordonate. 

Clasele folosite sunt :
+--tema1+----+DrawingWindow
	  |
	  |
	  +----+Transform2D
	  |
	  |
	  +----+Visual2D 

Cel mai dificil lucru in implementarea acestei probleme a fost detectarea coliziunilor. Am ales metoda mai simpla, aceea a incadrarii tuturor elementelor in cercuri. Dar nu am incadrat fiecare element doar intr-un cerc, deoarece era riscul sa atinga cercul si sa nu atinga inamicul. In functie de forma obiectelor, foloseam numarul de cercuri ce imi asigura incadrarea corecta a obiectelor. De exemplu, la burghiu am folosit un cerc mai mare pentru partea lata si un cerc mai mic pentru partea ascutita. 

Continutul arhivei: 
tema1.cpp - sursa principala a aplicatiei
Transform2D.cpp - clasa ce contine transformarile obiectelor
Transform2D.h - fisier header pentru functiile de mai sus
DrawingWindow.cpp - clasa ce contine afiasarea ferestrei
DrawingWindow.h - fisier header pentru functiile de mai sus
Visual2D.cpp - clasa ce contine transformarile fereastra poarta
Visual2D.h - fisier header pentru functiile de mai sus
Circle2D.h - header pentru definirea cercurilor
Color.h - header pentru definirea culorilor
Line2D.h - header pentru definirea liniilor
Object2D.h - header pentru definirea obiectelor
Point2D.h - header pentru definirea punctelor
Point3D.h - header pentru definirea punctelor
Polygon2D.h - header pentru definirea poligoanelor
Rectangle2d.h - header pentru definirea dreptunghiurilor
Text.h - header pentru definirea textelor

Pentru realizarea acestei teme, am creat 4 tipuri de inamici. Am creat inamici statici, inamici ce se misca aleator pe harta si inamici inteligenti ce ma urmaresc. Inamicii inteligenti nu apar de la inceput. Acestia apar pe parcurs iar dupa fiecare 2 inamici inteligenti aparuti, viteza acestora creste si automat nivelul de dificultate este mai ridicat deoarece acestia pot ajunge mai repede la jucator. De asemenea, primii inamici(cei care nu ma urmaresc) reapar dupa un timp dupa ce sunt distrusi.

Punctajele inamicilor sunt urmatoarele: cei 2 statici au 5 puncte, cel ce se misca doar pe axa oY are 8 puncte, cel ce se misca pe toata harta are 15 puncte, iar cei inteligenti pleaca de la 10 puncte, punctajul lor crescand proportional cu viteza pe care o au.

De asemenea, pentru bonus, am ales sa adaug apara pe ecran dupa un timp o imagine in forma de inima, care incrementeaza numarul vietilor cand este atinsa. 