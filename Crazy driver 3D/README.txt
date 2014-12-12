Elemente de grafica pe calculator - Tema2

Crazy Driver
Bogdan Stefan 333 CA

Utilizand framework-ul de la laborator, a trebuit sa implementam acest joc 3D. Pentru aceasta tema, am ales sa folosesc proiectiile perspective. Terenul de joc este format din paralelipipede cu inaltimea 0 : drumul si benzile. Masina ce se afla in cadrul cel mai apropiat la inceputul jocului (de altfel singura care apare la inceput) este cea pe care noi trebuie sa o controlam. Pentru obstacole, am creat diferite paralelipipede. Unele sunt obstacole fixe, iar altele sunt obstacole mobile (masini ca cea pe care o controlez eu) ce merg in acelasi sens cu masina mea. Scopul acestui joc este ajungerea la final. Initial avem 3 vieti, iar la fiecare coliziune cu unul din obstacole, numarul vietilor este decrementat. 

De asemenea, am incercat sa ating si o parte a bonusului : power-ups. Din cand in cand, pe drum apar niste piramide rosii. Acestea nu sunt obstacole, coliziunea cu acestea fiind una benefica : aduce o noua viata. 

In joc veti observa ca daca nu apasam pe niciuna din taste masina merge constant cu o viteza redusa. Daca viteza este mai mare, punctajul pe care il acumulam este mai mare. 

Pentru a putea controla jucatorul, am folosit mai multe taste:
UP - pentru a merge cu viteza mai mare
LEFT - pentru a vira la stanga
RIGHT - pentru a vira la dreapta
DOWN - pentru a ma opri
ESC - pentru a iesit din joc

Tema a fost implementata in Microsoft Visual Studio 2012, folosind sistemul de operare Windows8. Pentru aceasta tema, nu am folosit niciun algoritm complex. 

Clasele folosite sunt :
+--main+----+DrawingWindow
	  |
	  |
	  +----+Transform2D
	  |
	  |
	  +----+Visual2D
	  |
	  |
	  +----+Transform3D 

Detectarea coliziunilor este mai usoara decat la tema precedenta, deoarece obstacolele care apar nu isi schimba banda. Deci pur si simplu pentru detectarea coliziunilor este de ajuns sa verificam daca diferenta dintre x_masina si x_obstacol este mai mica decat latime_masina / 2 + latime_obstacol / 2. Analog pentru z_masina, z_obstacol. Y nu ne intereseaza deoarece este inaltimea. 

Continutul arhivei: 
main.cpp - sursa principala a aplicatiei
Transform2D.cpp - clasa ce contine transformarile obiectelor
DrawingWindow.cpp - clasa ce contine afiasarea ferestrei
Visual2D.cpp - clasa ce contine transformarile fereastra poarta
Transform3D.cpp - clasa ce contine transformarile 3D ale obiectelor

Punctajele obstacolelor sunt urmatoarele: atunci cand depasim un obstacol static punctajul creste cu 7500, iar cand intram in coliziune cu acesta ni se scade 7500. Pentru masinile care merg in acelasi sens cu noi, punctajul este de 15000. De asemenea, la lovirea unei masini ni se scad 15000.

La prima vedere, cel mai greu de implementat parea efectul vizual la schimbarea benzii de circulatie : botul masinii rotit. Dar problema aceasta am rezolvat-o cu schimbarea coordonatei proiectiei perspective pe axa Ox. Astfel efectul este similar cu cel din primul exemplu din enuntul temei.