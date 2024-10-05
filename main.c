#include <GL/glut.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <math.h>

#define number_of_particles 40

typedef struct particle
{
    float x, y, z;
    float vx, vy, vz;
    float mass;
}particle;

int init = 1;
float ground_height = 100;
float ground_width = 4800;
float ground_depth = 1000;
float r = 7;
float air = 0;
float gravity = -1;
float dt = 1;
struct particle p[number_of_particles];

GLfloat tetrahedron[4][3] = {
        {0, 0, 1},
        {0, 0.942809, -0.33333},
        {-0.816497, -0.471405, -0.333333},
        {0.816497, -0.471405, -0.333333}
};

void initialize()
{
    glClearColor(0, 0, 0, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2400, 2400,-100 , 3100,-10000,10000);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
}

void normalize(float* v)
{
    float d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    v[0] /= d;
    v[1] /= d;
    v[2] /= d;
}

void recursive_subdivision(float a[], float b[], float c[], int step)
{
    if (step > 0) {
        float ab[3], ac[3], bc[3];
        for (int i = 0; i < 3; i++){
            ab[i] = a[i] + b[i];
            ac[i] = a[i] + c[i];
            bc[i] = b[i] + c[i];
        }
        normalize(ab);
        normalize(ac);
        normalize(bc);

        recursive_subdivision(a, ab, ac, step - 1);
        recursive_subdivision(b, bc, ab, step - 1);
        recursive_subdivision(c, ac, bc, step - 1);
        recursive_subdivision(ab, bc, ac, step - 1);
    }
    else {
        glBegin(GL_POLYGON);
        glVertex3fv(a);
        glVertex3fv(b);
        glVertex3fv(c);
        glEnd();
    }
}

void drawParticle(float x, float y, float z)
{
    glColor3ub(255,0,0);
    glPushMatrix();
    glTranslatef(x,y,z);
    glScalef(r,r,r);
    recursive_subdivision(tetrahedron[0], tetrahedron[2], tetrahedron[1], 4);
    recursive_subdivision(tetrahedron[0], tetrahedron[3], tetrahedron[2], 4);
    recursive_subdivision(tetrahedron[0], tetrahedron[1], tetrahedron[3], 4);
    recursive_subdivision(tetrahedron[1], tetrahedron[2], tetrahedron[3], 4);
    glPopMatrix();
}

void calculateNewPosition(int i)
{
    p[i].x += dt * p[i].vx;
    p[i].vx += dt * p[i].mass * air;
    p[i].z += dt * p[i].vz;
    p[i].vz += dt * p[i].mass * 0;
    if (p[i].y + dt * p[i].vy + r > ground_height/2)
    {
        p[i].y += dt * p[i].vy;
        p[i].vy += dt * p[i].mass * gravity;
    }
    else
    {
        p[i].vy *= -1;
        p[i].y += dt * p[i].vy;
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Create particles
    if (init)
    {
        for (int i = 0; i < number_of_particles; i++)
        {
            p[i].x = 0;
            p[i].y = 2000;
            p[i].z = 0;
            p[i].vx = (float)rand()/(float)(RAND_MAX)*10.0 - 5;
            p[i].vy = (float)rand()/(float)(RAND_MAX)*10.0 - 5;
            p[i].vz = (float)rand()/(float)(RAND_MAX)*10.0 - 5;
            p[i].mass = 1;
            drawParticle(p[i].x, p[i].y, p[i].z);
        }
        init = 0;
    }
    else
        for (int i = 0; i < number_of_particles; i++)
        {
            calculateNewPosition(i);
            drawParticle(p[i].x, p[i].y, p[i].z);
        }

    //Create floor
    glColor3ub(0,255,0);
    glBegin(GL_POLYGON);
    glVertex3f(-ground_width/2,ground_height/2,-ground_depth/2);
    glVertex3f(ground_width/2,ground_height/2,-ground_depth/2);
    glVertex3f(ground_width/2,ground_height/2,ground_depth/2);
    glVertex3f(-ground_width/2,ground_height/2,ground_depth/2);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(-ground_width/2,-ground_height/2,-ground_depth/2);
    glVertex3f(ground_width/2,-ground_height/2,-ground_depth/2);
    glVertex3f(ground_width/2,-ground_height/2,ground_depth/2);
    glVertex3f(-ground_width/2,-ground_height/2,ground_depth/2);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(-ground_width/2,-ground_height/2,-ground_depth/2);
    glVertex3f(ground_width/2,-ground_height/2,-ground_depth/2);
    glVertex3f(ground_width/2,ground_height/2,-ground_depth/2);
    glVertex3f(-ground_width/2,ground_height/2,-ground_depth/2);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(-ground_width/2,-ground_height/2,ground_depth/2);
    glVertex3f(ground_width/2,-ground_height/2,ground_depth/2);
    glVertex3f(ground_width/2,ground_height/2,ground_depth/2);
    glVertex3f(-ground_width/2,ground_height/2,ground_depth/2);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(-ground_width/2,-ground_height/2,-ground_depth/2);
    glVertex3f(-ground_width/2,ground_height/2,-ground_depth/2);
    glVertex3f(-ground_width/2,ground_height/2,ground_depth/2);
    glVertex3f(-ground_width/2,-ground_height/2,ground_depth/2);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(ground_width/2,-ground_height/2,-ground_depth/2);
    glVertex3f(ground_width/2,ground_height/2,-ground_depth/2);
    glVertex3f(ground_width/2,ground_height/2,ground_depth/2);
    glVertex3f(ground_width/2,-ground_height/2,ground_depth/2);
    glEnd();

    glFlush();
    glutSwapBuffers();
}

void idle()
{
    glutPostRedisplay();
}

//Click to reset the particle system
void mouseCallback(int button, int state, int x, int y){
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        init = 1;
}
int main(int argc, char** argv) {

    glutInit(&argc,argv);
    glutInitDisplayMode (GLUT_DOUBLE| GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200,800);
    glutInitWindowPosition(400,100);
    glutCreateWindow("Particle System");
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutMouseFunc(mouseCallback);
    initialize();
    display();
    glutSwapBuffers();
    glutMainLoop();

    return 0;
}