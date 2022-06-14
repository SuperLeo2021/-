#include <GL/glut.h> 
#include<stdio.h>
#include<iostream>
using namespace std;

int width = 400;//设置宽度
int height = 400;//设置高度
unsigned char* pixels;

void set_pixel(int x, int y) {
	int pianyi = 100;
	x = x + pianyi;
	y = y + pianyi;
	int index = y * width + x;
	pixels[index * 3 + 0] = 255;
	pixels[index * 3 + 1] = 0;
	pixels[index * 3 + 2] = 0;
}

void set_circle_eight_pixel(int x, int y) {//画出八个点的算法

	set_pixel(x, y);
	set_pixel(y, x);
	set_pixel(-x, y);
	set_pixel(y, -x);
	set_pixel(-x, -y);
	set_pixel(-y, x);
	set_pixel(x, -y);
	set_pixel(-y, -x);
}
void Midcircle_init(int R) {
	int x = 0;
	int y = R;
	float d = 1.25 - R;
	while (x <= y) {
		set_circle_eight_pixel(x, y);
		if (d < 0) {
			d += 2 * x + 3;
			x++;
		}
		else {
			d += 2 * (x - y) + 5;
			x++;
			y--;
		}
	}
}
void mydraw() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	glFlush();
}
int main(int argc, char* argv[])//主方法的调用
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(300, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("中点画圆的算法： ");
	pixels = new unsigned char[width * height * 3];
	int R;
	cout << "请输入半径的大小（整数）：  ";
	cin >> R;
	Midcircle_init(R);
	glutDisplayFunc(&mydraw);
	glutMainLoop();
	return 0;
}