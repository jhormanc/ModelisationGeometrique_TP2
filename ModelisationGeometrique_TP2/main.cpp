
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

float angleX=0.0f; //angle de rotation en Y de la scene
float angleY=0.0f; //angle de rotation en X de la scene

float pasDeplacement=1.25;


//position lumiere
float xLitePos=0;
float yLitePos=10;
float zLitePos=-100;

float tx=0.0;
float ty=0.0;
float tz=0.0;

/* initialisation d'OpenGL*/
static void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	// Si vous avez des choses à initialiser, c est ici.	
}

void drawCylindre(const point3 &centre, const float &rayon, const float &hauteur, const int &nb_meridiens)
{
	float teta, teta2, x, y, x2, y2;

	for(int i = 0; i < nb_meridiens; i++)
	{
		//glBegin(GL_LINE_LOOP);
		glBegin(GL_TRIANGLES); 

		glColor3f(0.7, 0.7, 0.7);

		teta = 2. * M_PI * (float)i / (float)nb_meridiens;

		x = centre.x + rayon * cos(teta);
		y = centre.y + rayon * sin(teta);

		teta2 = 2. * M_PI * (float)(i + 1.) / (float)nb_meridiens;

		x2 = centre.x + rayon * cos(teta2);
		y2 = centre.y + rayon * sin(teta2);

		glColor3f(0., 0., 1.);
		glVertex3f(x2, y2, centre.z + hauteur / 2.);
		glVertex3f(centre.x, centre.y, centre.z + hauteur / 2.);
		glVertex3f(x, y, centre.z + hauteur / 2.);

		glColor3f(0.7, 0.7, 0.7);
		glVertex3f(x, y, centre.z + hauteur / 2.);
		glVertex3f(x2, y2, centre.z - hauteur / 2.);
		glVertex3f(x2, y2, centre.z + hauteur / 2.);

		glColor3f(0., 1., 0.);
		glVertex3f(x2, y2, centre.z - hauteur / 2.);
		glVertex3f(centre.x, centre.y, centre.z - hauteur / 2.);
		glVertex3f(x, y, centre.z - hauteur / 2.);

		glColor3f(0.7, 0.7, 0.7);
		glVertex3f(x, y, centre.z - hauteur / 2.);
		glVertex3f(x2, y2, centre.z - hauteur / 2.);
		glVertex3f(x, y, centre.z + hauteur / 2.);
		
		glEnd();
	}
}

//void drawSphere(const point3 &centre, const float &rayon, const 

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
			
			//glRotatef(-90,1.0f,0.0f,0.0f);
			// C'est ici que l'on dessine notre cylindre
			// Il n'y a pour l'instant qu'un triangle gris ...

			// Exemple d'appel à Point3
			//point3 p1 = point3(-1.0,1.0,0.0);
			//point3 p2 = point3(1,1,0);
			//point3 p3 = point3(1,2,0);
			////tableau de points
			//point3 tab[2];

			//tab[0]=p1;
			//tab[1]=p2;

			//glColor3f(0.7,0.7,0.7);
			//glBegin(GL_TRIANGLES);	
			//	glVertex3f(tab[0].x,tab[0].y, tab[0].z);
			//	glVertex3f(tab[1].x,tab[1].y, tab[1].z);
			//	glVertex3f(p3.x,p3.y,p3.z);
			//glEnd();

	drawCylindre(point3(0., 0., 5.), 10., 20., 6);

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
   glOrtho(-20, 20, -20, 20, -10, 10);
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

//sortie
	case KEY_ESC:
		exit(0);
		break;

	default:
		printf ("La touche %d non active.\n", key);
	break;
	}
glutPostRedisplay();
}

int main(int argc, char **argv)
{
   glutInitWindowSize(400, 400);
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutCreateWindow("Mon cylindre");
   init();
   glutReshapeFunc(reshape);
   glutKeyboardFunc(&window_key_down);
   glutDisplayFunc(display);
   glutPassiveMotionFunc(gestionSouris);
   glutSpecialFunc(&gestionFleche);
   glutMainLoop();
   return 0;
}
