#define GLUT_DISABLE_ATEXIT_HACK
#include <gl/glut.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#define ZVALUE 20.0f
int nearplane_width = 300;//窗口的大小
int nearplane_height = 300;//窗口的高度
int nearplane_distance = 50;
int farplane_distance = nearplane_distance + 300;
int eye_x = 50; //视点的x坐标

struct my_v_homogeneous
{
	float x;
	float y;
	float z;
	float ratio;
};

struct my_v_homogeneous box[8];

void init(void)
{
	box[0].x = 0;
	box[0].y = 0;
	box[0].z = 0;
	box[0].ratio = 1;
	box[1].x = 80;
	box[1].y = 0;
	box[1].z = 0;
	box[1].ratio = 1;
	box[2].x = 80;
	box[2].y = 40;
	box[2].z = 0;
	box[2].ratio = 1;
	box[3].x = 0;
	box[3].y = 40;
	box[3].z = 0;
	box[3].ratio = 1;
	//后面四个顶点
	box[4].x = 0;
	box[4].y = 0;
	box[4].z = -50;
	box[4].ratio = 1;
	box[5].x = 80;
	box[5].y = 0;
	box[5].z = -50;
	box[5].ratio = 1;
	box[6].x = 80;
	box[6].y = 40;
	box[6].z = -50;
	box[6].ratio = 1;
	box[7].x = 0;
	box[7].y = 40;
	box[7].z = -50;
	box[7].ratio = 1;
}

void draw_coordinate()
{
	glBegin(GL_LINES);
	//红色x轴
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(nearplane_width, 0.0, 0.0);
	glVertex3f(-nearplane_width, 0.0, 0.0);
	//绿色y轴
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, nearplane_height, 0.0);
	glVertex3f(0.0, -nearplane_height, 0.0);
	//蓝色z轴	
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, nearplane_height);
	glVertex3f(0.0, 0.0, -nearplane_height);
	glEnd();
}

struct my_v_homogeneous matrix_multiply_vector(float matrix[][4], struct my_v_homogeneous input_v)
{
	struct my_v_homogeneous translated_v;
	translated_v.x = matrix[0][0] * input_v.x + matrix[0][1] * input_v.y + matrix[0][2] * input_v.z + matrix[0][3] * input_v.ratio;
	translated_v.y = matrix[1][0] * input_v.x + matrix[1][1] * input_v.y + matrix[1][2] * input_v.z + matrix[1][3] * input_v.ratio;
	translated_v.z = matrix[2][0] * input_v.x + matrix[2][1] * input_v.y + matrix[2][2] * input_v.z + matrix[2][3] * input_v.ratio;
	translated_v.ratio = matrix[3][0] * input_v.x + matrix[3][1] * input_v.y + matrix[3][2] * input_v.z + matrix[3][3] * input_v.ratio;
	return translated_v;
}

void my_translate_homogeneous(struct my_v_homogeneous* polygon, int vertex_count, float tx, float ty, float tz)
{
	//装配生成平移矩阵
	float translate_matrix[4][4];
	memset(translate_matrix, 0, sizeof(int) * 16);
	translate_matrix[0][0] = translate_matrix[1][1] = translate_matrix[2][2] = translate_matrix[3][3] = 1;//对角线赋值为1
	translate_matrix[0][3] = tx;
	translate_matrix[1][3] = ty;
	translate_matrix[2][3] = tz;
	//遍历并平移多边形每个顶点
	for (int vIndex = 0; vIndex < vertex_count; vIndex++)
	{
		struct my_v_homogeneous input_v;
		input_v.x = polygon[vIndex].x;
		input_v.y = polygon[vIndex].y;
		input_v.z = polygon[vIndex].z;
		input_v.ratio = 1;
		input_v = matrix_multiply_vector(translate_matrix, polygon[vIndex]);
		polygon[vIndex].x = input_v.x;
		polygon[vIndex].y = input_v.y;
		polygon[vIndex].z = input_v.z;
		polygon[vIndex].ratio = input_v.ratio;
	}
}

void my_rotate_x_homogeneous(struct my_v_homogeneous* polygon, int vertex_count, float angle)
{
	//装配生成旋转矩阵
	float translate_matrix[4][4];
	memset(translate_matrix, 0, sizeof(int) * 16);
	float pai = 3.14159;
	angle = angle * pai / 180;
	translate_matrix[0][0] = 1;
	translate_matrix[1][1] = cos(angle);
	translate_matrix[1][2] = -sin(angle);
	translate_matrix[2][1] = sin(angle);
	translate_matrix[2][2] = cos(angle);
	translate_matrix[3][3] = 1;
	//遍历并旋转多边形每个顶点
	for (int vIndex = 0; vIndex < vertex_count; vIndex++)
	{
		struct my_v_homogeneous input_v;
		input_v.x = polygon[vIndex].x;
		input_v.y = polygon[vIndex].y;
		input_v.z = polygon[vIndex].z;
		input_v.ratio = 1;
		input_v = matrix_multiply_vector(translate_matrix, input_v);
		polygon[vIndex].x = input_v.x;
		polygon[vIndex].y = input_v.y;
		polygon[vIndex].z = input_v.z;
		polygon[vIndex].ratio = input_v.ratio;
	}
}

void my_rotate_y_homogeneous(struct my_v_homogeneous* polygon, int vertex_count, float angle)
{//装配生成旋转矩阵
	float translate_matrix[4][4];
	memset(translate_matrix, 0, sizeof(int) * 16);
	float pai = 3.14159;
	angle = angle * pai / 180;
	translate_matrix[0][0] = cos(angle);
	translate_matrix[1][1] = 1;
	translate_matrix[0][2] = sin(angle);
	translate_matrix[2][0] = -sin(angle);
	translate_matrix[2][2] = cos(angle);
	translate_matrix[3][3] = 1;//遍历并旋转多边形每个顶点
	for (int vIndex = 0; vIndex < vertex_count; vIndex++)
	{
		struct my_v_homogeneous input_v;
		input_v.x = polygon[vIndex].x;
		input_v.y = polygon[vIndex].y;
		input_v.z = polygon[vIndex].z;
		input_v.ratio = 1;
		input_v = matrix_multiply_vector(translate_matrix, input_v);
		polygon[vIndex].x = input_v.x;
		polygon[vIndex].y = input_v.y;
		polygon[vIndex].z = input_v.z;
		polygon[vIndex].ratio = input_v.ratio;
	}
}
void my_rotate_z_homogeneous(struct my_v_homogeneous* polygon, int vertex_count, float angle)
{//装配生成旋转矩阵
	float translate_matrix[4][4];
	memset(translate_matrix, 0, sizeof(int) * 16);
	float pai = 3.14159;
	angle = angle * pai / 180;
	translate_matrix[0][0] = 1;
	translate_matrix[1][1] = sin(angle);
	translate_matrix[1][2] = 1;
	translate_matrix[2][1] = -sin(angle);
	translate_matrix[2][2] = cos(angle);
	translate_matrix[3][3] = 1;//遍历并旋转多边形每个顶点
	for (int vIndex = 0; vIndex < vertex_count; vIndex++)
	{
		struct my_v_homogeneous input_v;
		input_v.x = polygon[vIndex].x;
		input_v.y = polygon[vIndex].y;
		input_v.z = polygon[vIndex].z;
		input_v.ratio = 1;
		input_v = matrix_multiply_vector(translate_matrix, input_v);
		polygon[vIndex].x = input_v.x;
		polygon[vIndex].y = input_v.y;
		polygon[vIndex].z = input_v.z;
		polygon[vIndex].ratio = input_v.ratio;
	}
}
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case'r':
	case'R':
	{
		my_translate_homogeneous(box, 8, 30, 0, 0);
		glutPostRedisplay();
		break;
	}
	case'l':
	case'L':
	{
		my_translate_homogeneous(box, 8, -30, 0, 0);
		glutPostRedisplay();
		break;
	}
	case'X':
	case'x':
	{
		my_rotate_x_homogeneous(box, 8, 6);
		glutPostRedisplay();
		break;
	}
	case'Y':
	case'y':
	{
		my_rotate_y_homogeneous(box, 8, 6);
		glutPostRedisplay();
		break;
	}
	case'Z':
	case'z':
	{
		my_rotate_y_homogeneous(box, 8, 6);
		glutPostRedisplay();
		break;
	}
	case 27:
		exit(0);
		break;
	}
}
void display(void) {
	glClearColor(1.f, 1.f, 1.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw_coordinate();
	//绘制坐标系
	//绘制box,默认顶点之间通过直线段相连
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	//前面4条边
	glVertex3i((floor)(box[0].x + 0.5), (floor)(box[0].y + 0.5), (floor)(box[0].z + 0.5));
	glVertex3i((floor)(box[1].x + 0.5), (floor)(box[1].y + 0.5), (floor)(box[1].z + 0.5));
	glVertex3i((floor)(box[1].x + 0.5), (floor)(box[1].y + 0.5), (floor)(box[1].z + 0.5));
	glVertex3i((floor)(box[2].x + 0.5), (floor)(box[2].y + 0.5), (floor)(box[2].z + 0.5));
	glVertex3i((floor)(box[2].x + 0.5), (floor)(box[2].y + 0.5), (floor)(box[2].z + 0.5));
	glVertex3i((floor)(box[3].x + 0.5), (floor)(box[3].y + 0.5), (floor)(box[3].z + 0.5));
	glVertex3i((floor)(box[3].x + 0.5), (floor)(box[3].y + 0.5), (floor)(box[3].z + 0.5));
	glVertex3i((floor)(box[0].x + 0.5), (floor)(box[0].y + 0.5), (floor)(box[0].z + 0.5));
	//后面4条边
	glVertex3i((floor)(box[4].x + 0.5), (floor)(box[4].y + 0.5), (floor)(box[4].z + 0.5));
	glVertex3i((floor)(box[5].x + 0.5), (floor)(box[5].y + 0.5), (floor)(box[5].z + 0.5));
	glVertex3i((floor)(box[5].x + 0.5), (floor)(box[5].y + 0.5), (floor)(box[5].z + 0.5));
	glVertex3i((floor)(box[6].x + 0.5), (floor)(box[6].y + 0.5), (floor)(box[6].z + 0.5));
	glVertex3i((floor)(box[6].x + 0.5), (floor)(box[6].y + 0.5), (floor)(box[6].z + 0.5));
	glVertex3i((floor)(box[7].x + 0.5), (floor)(box[7].y + 0.5), (floor)(box[7].z + 0.5));
	glVertex3i((floor)(box[7].x + 0.5), (floor)(box[7].y + 0.5), (floor)(box[7].z + 0.5));
	glVertex3i((floor)(box[4].x + 0.5), (floor)(box[4].y + 0.5), (floor)(box[4].z + 0.5));
	glVertex3i((floor)(box[0].x + 0.5), (floor)(box[0].y + 0.5), (floor)(box[0].z + 0.5));
	glVertex3i((floor)(box[4].x + 0.5), (floor)(box[4].y + 0.5), (floor)(box[4].z + 0.5));
	glVertex3i((floor)(box[7].x + 0.5), (floor)(box[7].y + 0.5), (floor)(box[7].z + 0.5));
	glVertex3i((floor)(box[3].x + 0.5), (floor)(box[3].y + 0.5), (floor)(box[3].z + 0.5));
	//右面补两条边
	glVertex3i((floor)(box[1].x + 0.5), (floor)(box[1].y + 0.5), (floor)(box[1].z + 0.5));
	glVertex3i((floor)(box[5].x + 0.5), (floor)(box[5].y + 0.5), (floor)(box[5].z + 0.5));
	glVertex3i((floor)(box[6].x + 0.5), (floor)(box[6].y + 0.5), (floor)(box[6].z + 0.5));
	glVertex3i((floor)(box[2].x + 0.5), (floor)(box[2].y + 0.5), (floor)(box[2].z + 0.5));
	glEnd();
	glutSwapBuffers();
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION); ///将当前矩阵指定为投影矩阵,指定哪一个矩阵是当前矩阵，而它的参数代表要操作的目标，GL_PROJECTION是对投影矩阵操作
	glLoadIdentity();	//然后把矩阵设为单位矩阵：
	if (w <= h)    //平行投影
		glOrtho(-0.5 * nearplane_width, 0.5 * nearplane_width, -0.5 * nearplane_height * (GLfloat)nearplane_height / (GLfloat)nearplane_width, 0.5 * nearplane_height * (GLfloat)nearplane_height / (GLfloat)nearplane_width, -nearplane_distance, farplane_distance);
	//相对于视点
	else
		glOrtho(-0.5 * nearplane_width, 0.5 * nearplane_width, -0.5 * nearplane_height * (GLfloat)nearplane_width / (GLfloat)nearplane_height, 0.5 * nearplane_height * (GLfloat)nearplane_width / (GLfloat)nearplane_height, -nearplane_distance, farplane_distance);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye_x, 80, 80, 0, 0, 0, 0, 1, 0);
	//视点转换
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(nearplane_width, nearplane_height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("三维几何变换");
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
