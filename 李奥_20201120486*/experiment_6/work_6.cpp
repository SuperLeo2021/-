#include<GL\glut.h>
#include<Windows.h>
#include<math.h>

void init()
{
    glClearColor(1, 1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 500.0, 0, 500.0);
}
void display()//显示函数
{
    glClear(GL_COLOR_BUFFER_BIT);
    //平移
    glColor3f(0, 0, 1);
    glRecti(100, 100, 200, 200);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(200, 0, 0);
    glColor3f(0, 1, 0);
    glRecti(100, 100, 200, 200);


    //缩放
    glColor3f(1, 0, 0);
    glRecti(100, 100, 200, 200);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(0.5, 0.5, 0);
    //绘制变换后的矩形
    glColor3f(0, 1, 0);
    glRecti(100, 100, 200, 200);

    //旋转
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //逆旋转矩阵，即把物体移动回原来的位置
    glTranslatef(150, 150, 0);
    //复合旋转矩阵
    glRotatef(45, 0, 0, 1);
    //构建平移矩阵将物体中心移动到原点
    glTranslatef(-150, -150, 0);
    glColor3f(1, 0, 0);
    glRecti(100, 100, 200, 200);
    
        //对称
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //画出该轴作为参考
    glColor3f(0, 0, 1);
    glBegin(GL_LINES);
    glVertex2i(0, -50);
    glVertex2i(500, 450);
    glEnd();
    //画出原矩形
    glColor3f(1, 0, 0);
    glRecti(400, 200, 450, 250);
    // 逆平移回原位置
    glTranslatef(50, 0, 0);
    //逆旋转45°
    glRotatef(-45, 0, 0, 1);
    //乘上对称矩阵
    GLfloat arr[16] =
    {
        -1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1

    };
    glMultMatrixf(arr);
    //旋转45度使其重合于y轴(也可以重合于x轴，只要之后进行对应的逆变换即可)
    glRotatef(45, 0, 0, 1);
    //先把对称轴平移到原点
    glTranslatef(-50, 0, 0);
    glColor3f(0, 1, 0);
    glRecti(400, 200, 450, 250);
    
    glEnd();

    glFlush();
}

    int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(500, 500);
    glutInitWindowSize(500, 500);
    glutCreateWindow("二维几何图形的各种变换 ");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}