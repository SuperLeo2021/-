//鼠标、键盘执行茶壶的交互实验。
#include<stdlib.h>
#include<GL/glut.h>
float theta[] = { 0, 0, 0, 0 };
int axis = 3;
float step = 2.0;
int win_w, win_h, mx, my;
void display()
{
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(theta[0], 1.0, 0, 0);
	glRotatef(theta[1], 0, 1.0, 0);
	glRotatef(theta[2], 0, 0, 1.0);
	glutWireTeapot(2);
	glutSwapBuffers();
}
void keyboard(unsigned char key, int x, int y)
{
	//该步骤是利用键盘输入x，y，z
	switch (key)
	{
	case 'x':
		axis = 0;
		step = 2.0;
		break;
	case 'X':
		axis = 0;
		step = -2.0;
		break;
	case 'y':
		axis = 1;
		step = 2.0;
		break;
	case 'Y':
		axis = 1;
		step = -2.0;
		break;
	case 'z':
		axis = 2;
		step = 2.0;
		break;
	case 'Z':
		axis = 2;
		step = -2.0;
		break;
	case 27:
		exit(0);
	}
}
void idle()
{
	theta[axis] += step;
	if (theta[axis] >= 360)
		theta[axis] -= 360;
	if (theta[axis] < 0)
		theta[axis] += 360;
	glutPostRedisplay();
}
void move(int x, int y)
{
	if (x < mx)
		theta[1] += 2.0;
	else if (x > mx)
		theta[1] -= 2.0;
	if (y < my)
		theta[0] -= 2.0;
	else if (y > my)
		theta[0] += 2.0;
	mx = x, my = y;
	glutPostRedisplay();
}
void reshape(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-2.0, 2.0, -2.0 * h / w, 2.0 * h / w, -2.0, 2.0);
	else
		glOrtho(-2.0 * w / h, 2.0 * w / h, -2.0, 2.0, -2.0, 2.0);
	glViewport(0, 0, w, h);
	win_w = w, win_h = h;
	glMatrixMode(GL_MODELVIEW);
}
void myinit()
{
	glClearColor(0.0,1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	my = win_h / 2;
}
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 350);
	glutCreateWindow("利用茶壶的例子实现键鼠交互");
	myinit();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(move);

	glutMainLoop();
	return 0;
}