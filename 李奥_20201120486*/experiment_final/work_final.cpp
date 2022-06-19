#include<stdlib.h>
#include<GL/glut.h>
#include<GL/GL.h>
#include<math.h>
float theta[] = { 0, 0, 0, 0 };
int axis = 3;
float step = 2.0;
int win_w, win_h, mx, my;
#define TEXTUREWIDTH 64
GLubyte Texture[3 * TEXTUREWIDTH];
//定义紫色，绿色，紫绿色的渐变纹理
void makeTexture(void)
{
	int i;
	for (i = 0; i < TEXTUREWIDTH; i++)
	{
		Texture[3 * i] = 255;
		Texture[3 * i + 1] = 255 - 2 * i;
		Texture[3 * i + 2] = 255;
	}
}
GLfloat sgenparams[] = { 1.0,1.0,1.0,0.0 };
void Light(void)
{
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };//材质的镜面反射系数
	GLfloat mat_shininess[] = { 50.0 };//材质的镜面光指数
	// 光源 0
	GLfloat light_position[] = { -50.0, 100.0, 100.0, 0.0 };//光源位置
	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };//环境光
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };//漫反射
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };//镜面光
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);//光暗处理
	glEnable(GL_LIGHT0);//开启0光源
	//设置材质
	glMaterialf(GL_FRONT, GL_SHININESS, 64.0);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glEnable(GL_LIGHTING);//开启光照效果
	//设置光照材质与位置
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

}
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
	glEnable(GL_DEPTH_TEST);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	my = win_h / 2;
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(500, 500);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	//  创建纹理
	makeTexture();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//  控制纹理
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage1D(GL_TEXTURE_1D, 0, 3, TEXTUREWIDTH, 0,
		GL_RGB, GL_UNSIGNED_BYTE, Texture);
	//  唯一与前面例子不同的地方：启用纹理坐标自动产生，生成环境纹理
	//  纹理的方向S
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_S, GL_OBJECT_PLANE, sgenparams);
	//  启用纹理
	glEnable(GL_TEXTURE_1D);
	glEnable(GL_TEXTURE_GEN_S);
	//  启用消隐
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthFunc(GL_LESS);
	//  一些绘图控制
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glMaterialf(GL_FRONT, GL_SHININESS, 64.0);
	//  glShadeModel(GL_FLAT);
}
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(500, 500);
	glutInitWindowSize(1000, 700);
	glutCreateWindow("利用茶壶的例子,添加纹理光照和消隐贴图");
	myinit();
	glutDisplayFunc(display);
	Light();
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(move);
	glutMainLoop();
	return 0;
}