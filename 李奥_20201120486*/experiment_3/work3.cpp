#include <windows.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
//Bresenham算法
void lineBres(int x0, int y0, int xEnd, int yEnd)
{
	int dx = fabs(xEnd - x0);
	int dy = fabs(yEnd - y0);
	int p = 2 * dy - dx;
	int twoDy = 2 * dy;
	int twoDyMinusDx = 2 * (dy - dx);
	int x, y;
	if (x0 > xEnd) {
		x = xEnd;
		y = yEnd;
		xEnd = x0;
	}
	else {
		x = x0;
		y = y0;
	}
	SetPixel(x, y);
	while (x < xEnd) {
		x++;
		if (p < 0)
			p += twoDy;
		else {
			y++;
			p += twoDyMinusDx;
		}
		SetPixel(x, y);

	}
}
void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(100, 100, 500, 500);
	lineBres(30, 35, 140, 200);
}

void Init() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glPointSize(5.0);
	glColor3f(0.0, 0.0, 0.0);
	gluOrtho2D(0.0, 600.0, 0.0, 600.0);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Bresenham算法");
	glutDisplayFunc(display);
	Init();
	glutMainLoop();

	return 0;
}