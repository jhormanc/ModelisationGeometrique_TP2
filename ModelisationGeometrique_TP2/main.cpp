
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "struct.h"

/* au cas ou M_PI ne soit defini */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

#define WIDTH  600
#define HEIGHT 600

#define KEY_ESC 27
#define KEY_SPACE 32

float angleX = 0.0f; //angle de rotation en Y de la scene
float angleY = 0.0f; //angle de rotation en X de la scene

float pasDeplacement=1.25;


//position lumiere
float xLitePos=0;
float yLitePos=10;
float zLitePos=-100;

float tx=0.0;
float ty=0.0;
float tz=0.0;

int etape = 0;

void printCommandes()
{
	printf("=== Commandes ===\n\n");
	printf(" Espace : changer de forme\n");
}

/* initialisation d'OpenGL*/
static void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	// Si vous avez des choses à initialiser, c est ici.	
	glClearDepth(1.0f);                   // Set background depth to farthest
	glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
	glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
	glShadeModel(GL_SMOOTH);   // Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

void drawCircle(const point3 &centre, const double rayon, const int nb_meridiens, const point3 &color, bool inverse)
{
	double teta, teta2, x, y, x2, y2;
	const double step = 2. * M_PI / (double)nb_meridiens;
	int min = inverse ? nb_meridiens : 0;
	int max = inverse ? 0 : nb_meridiens;
	int pas = inverse ? -1 : 1;

	glBegin(GL_TRIANGLES);
	glColor3f(color.x, color.y, color.z);
	for (int i = min; i != max; i += pas)
	{
		teta = step * i;
		x = centre.x + rayon * cos(teta);
		y = centre.y + rayon * sin(teta);

		teta2 = step * (double)(i + pas);
		x2 = centre.x + rayon * cos(teta2);
		y2 = centre.y + rayon * sin(teta2);

		glVertex3f(centre.x, centre.y, centre.z);
		glVertex3f(x, y, centre.z);
		glVertex3f(x2, y2, centre.z);
	}
	glEnd();
}

void drawCylindre(const point3 &centre, const double rayon, const double hauteur, const int nb_meridiens)
{
	double teta, teta2, x, y, x2, y2;
	const double step = 2. * M_PI / (double)nb_meridiens;
	const double h = hauteur * 0.5;

	// Cercle haut
	drawCircle(point3(centre.x, centre.y, centre.z + h), rayon, nb_meridiens, point3(0., 0., 1.), false);

	// Facettes cylindre
	glBegin(GL_TRIANGLES);
	glColor3f(0.75, 0.75, 0.75);
	for(int i = 0; i < nb_meridiens; i++)
	{
		teta = step * (double)i;
		x = centre.x + rayon * cos(teta);
		y = centre.y + rayon * sin(teta);

		teta2 = step * (double)(i + 1.);
		x2 = centre.x + rayon * cos(teta2);
		y2 = centre.y + rayon * sin(teta2);

		glVertex3f(x, y, centre.z + h);
		glVertex3f(x, y, centre.z - h);
		glVertex3f(x2, y2, centre.z - h);

		glVertex3f(x, y, centre.z + h);
		glVertex3f(x2, y2, centre.z - h);
		glVertex3f(x2, y2, centre.z + h);
	}
	glEnd();

	// Cercle bas 
	drawCircle(point3(centre.x, centre.y, centre.z - h), rayon, nb_meridiens, point3(0., 1., 0.), true);
	
}

void drawSphere(const point3 &centre, const point3& color, const double rayon, const int nb_paralleles, const int nb_meridiens, const bool quads)
{
	double phi_i, teta_j, phi_i2, teta_j2, x, y, z, x2, y2, z2, x3, y3, z3, x4, y4, z4;
	double step_i = 2. * M_PI / (double)nb_paralleles;
	double step_j = 2. * M_PI / (double)nb_meridiens;

	if (quads)
		glBegin(GL_QUADS);
	else
		glBegin(GL_LINE_LOOP);
		
	glColor3f(color.x, color.y, color.z);
	for (int i = 0; i < nb_paralleles; i++)
	{
		for (int j = 0; j < nb_meridiens; j++)
		{
			phi_i = step_i * (double)i;
			teta_j = step_j * (double)j;
			phi_i2 = step_i * (double)(i + 1.);
			teta_j2 = step_j * (double)(j + 1.);

			x = centre.x + rayon * sin(phi_i) * cos(teta_j);
			y = centre.y + rayon * sin(phi_i) * sin(teta_j);
			z = centre.z + rayon * cos(phi_i);

			x2 = centre.x + rayon * sin(phi_i2) * cos(teta_j);
			y2 = centre.y + rayon * sin(phi_i2) * sin(teta_j);
			z2 = centre.z + rayon * cos(phi_i2);

			x3 = centre.x + rayon * sin(phi_i2) * cos(teta_j2);
			y3 = centre.y + rayon * sin(phi_i2) * sin(teta_j2);
			z3 = centre.z + rayon * cos(phi_i2);

			x4 = centre.x + rayon * sin(phi_i) * cos(teta_j2);
			y4 = centre.y + rayon * sin(phi_i) * sin(teta_j2);
			z4 = centre.z + rayon * cos(phi_i);

			glVertex3f(x, y, z);
			glVertex3f(x2, y2, z2);
			glVertex3f(x3, y3, z3);
			glVertex3f(x4, y4, z4);
		}
	}
	glEnd();
}

void drawCone(const point3 &centre, const double rayon, const int nb_meridiens, const double hauteur, const double hauteur_coupe)
{
	double teta, teta2, x, y, z, x2, y2, z2, x3, y3, z3, x4, y4, z4;
	double h = hauteur_coupe > 0. ? hauteur_coupe : hauteur;
	double step = 2. * M_PI / (double)nb_meridiens;
	double rayon_haut = (rayon - rayon * h / hauteur);

	// Cercle haut
	if (hauteur_coupe != -1)
		drawCircle(point3(centre.x, centre.y, centre.z + h), rayon_haut, nb_meridiens, point3(1., 0., 0.), false);

	glBegin(GL_QUADS);
	glColor3f(0.75, 0.75, 0.75);
	for (int i = 0; i < nb_meridiens; i++)
	{
		teta = step * (double)i;
		x = centre.x + rayon * cos(teta);
		y = centre.y + rayon * sin(teta);
		z = centre.z;

		teta2 = step * (double)(i + 1.);
		x2 = centre.x + rayon * cos(teta2);
		y2 = centre.y + rayon * sin(teta2);
		z2 = centre.z;

		x3 = centre.x + rayon_haut * cos(teta);
		y3 = centre.y + rayon_haut * sin(teta);
		z3 = centre.z + h;

		x4 = centre.x + rayon_haut * cos(teta2);
		y4 = centre.y + rayon_haut * sin(teta2);
		z4 = centre.z + h;

		glVertex3f(x, y, z);
		glVertex3f(x2, y2, z2);
		glVertex3f(x4, y4, z4);
		glVertex3f(x3, y3, z3);
	}
	glEnd();

	// Cercle bas
	drawCircle(centre, rayon, nb_meridiens, point3(1., 0., 0.), true);
}

void drawCone(const point3 &centre, const double rayon, const int nb_meridiens, const double hauteur)
{
	drawCone(centre, rayon, nb_meridiens, hauteur, -1);
}

/* Dessin */
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //effacement du buffer

	//Description de la scene
	glLoadIdentity();
	//rotation de la scene suivant les mouvements de la souris
	glRotatef(-angleY,0.0f,0.0f,1.0f);
	glRotatef(angleX,0.0f,1.0f,0.0f);
	glTranslatef(tx,ty,tz);

	glRotatef(-angleY,0.0f,0.0f,1.0f);
	glRotatef(angleX,0.0f,1.0f,0.0f);

	// Voir plus tard les push et Pop
	glPushMatrix();			
	
	switch (etape)
	{
	case 0:
		drawCylindre(point3(0., 0., 0.), 2., 20., 6);
		break;
	case 1:
		drawCylindre(point3(0., 0., 0.), 4., 15., 60);
		break;
	case 2:
		drawCylindre(point3(0., 0., 0.), 10., 2., 100);
		break;
	case 3:
		drawSphere(point3(0., 0., 0.), point3(1., 1., 1.), 10., 100, 100, true);
		break;
	case 4:
		drawSphere(point3(0., 0., 0.), point3(0., 1., 0.5), 8., 10, 10, false);
		break;
	case 5:
		drawSphere(point3(0., 0., 0.), point3(0., 1., 1.), 4., 50, 50, false);
		break;
	case 6:
		drawCone(point3(0., 0., 0.), 5., 50., 15.);
		break;
	case 7:
		drawCone(point3(0., 0., 0.), 8., 5., 20., 12.);
		break;
	case 8:
		drawCone(point3(0., 0., 0.), 10., 50., 20., 5.);
		break;
	default:
		break;
	}

	glPopMatrix();			

	//affiche les axes du repere
	glColor3f(0.0,1.0,0.0); //Y vert
	glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(0,20,0);
	glEnd();

	glColor3f(0.0,0.0,1.0); //Z bleu
	glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(0,0,20);
	glEnd();
		
	glColor3f(1.0,0.0,0.0); //X rouge
	glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(20,0,0);
	glEnd();

	glutSwapBuffers();// echange des buffers
}

/* Au cas ou la fenetre est modifiee ou deplacee */
void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-20, 20, -20, 20, -40, 40);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

GLvoid gestionSouris(int x, int y)
{
	angleX=x*720/WIDTH; //gere l'axe Oy
	angleY=-(y*180/HEIGHT-90)*4; //gere l'axe Ox

	glutPostRedisplay();
}

GLvoid gestionFleche(int key, int x, int y) 
{
	switch (key) {
		case GLUT_KEY_UP :
			tz+=int(pasDeplacement);glutPostRedisplay();
		break;

		case GLUT_KEY_DOWN :
			tz-=int(pasDeplacement);glutPostRedisplay();
		break;
	glutPostRedisplay();
	}

}

GLvoid window_key_down(unsigned char key, int x, int y)  //appuie des touches
{  
switch (key) 
{

//deplacement de la camera
	case 'z':
		ty+=int(pasDeplacement);glutPostRedisplay();
	break;

	case 's':
		ty-=int(pasDeplacement);glutPostRedisplay();
	break;

	case 'q':
		tx-=int(pasDeplacement);glutPostRedisplay();
	break;

	case 'd':
		tx+=int(pasDeplacement);glutPostRedisplay();
	break;

	// espace : change la forme
	case KEY_SPACE:
		if (etape >= 8)
			etape = 0;
		else
			etape++;
	break;
//sortie
	case KEY_ESC:
		exit(0);
		break;

	default:
		printf ("La touche %d non active.\n", key);
		printCommandes();
	break;
	}
glutPostRedisplay();
}

void windowMenu(int value)
{
	window_key_down((unsigned char)value, 0, 0);
}

int main(int argc, char **argv)
{
	printCommandes();
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(500, 400);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("TP2");
	init();
	glutReshapeFunc(reshape);
	glutKeyboardFunc(&window_key_down);
	glutDisplayFunc(display);
	glutPassiveMotionFunc(gestionSouris);
	glutSpecialFunc(&gestionFleche);

	glutCreateMenu(windowMenu);
	glutAddMenuEntry("Changer de forme", KEY_SPACE);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
	return 0;
}
