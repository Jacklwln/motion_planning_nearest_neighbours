#include <GL/glew.h>
#include <GL/gl.h>

#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

#include "mpnn.hpp"

#include <cmath>
#include <armadillo>

using namespace arma;
using namespace std;

GLFWwindow * window = nullptr;

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

vec mouse_x = zeros<vec>(2);
int win_x = 640;
int win_y = 640;

static void cursor_callback(GLFWwindow* window, double x, double y)
{
    mouse_x(0) = x / win_x;
    mouse_x(1) = 1.0 - y / win_y;
}


int init_gl_window()
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(win_x,win_y,"Nearest Neighbor on Torus",nullptr,nullptr);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewInit();

    glfwSwapInterval(1);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_callback);
    return 0;
}

void recurse(kd_node * & node)
{
    if(node == nullptr) return;
    glBegin(GL_LINE_LOOP);
    glColor3d(0.0,0.0,0.0);
    glVertex2d(2.0*node->l_x(0)-1.0,2.0*node->l_x(1)-1.0);
    glVertex2d(2.0*node->h_x(0)-1.0,2.0*node->l_x(1)-1.0);
    glVertex2d(2.0*node->h_x(0)-1.0,2.0*node->h_x(1)-1.0);
    glVertex2d(2.0*node->l_x(0)-1.0,2.0*node->h_x(1)-1.0);
    glEnd();
    if(node->l)
        recurse(node->l);
    if(node->r)
        recurse(node->r);
}

int main(int argc,char ** argv)
{
    srand(time(nullptr));
    if(init_gl_window()<0) return -1;

    float t = 0.;

    vec  mu   = ones<vec>(2);
    ivec topo = ones<ivec>(2);

    kd_tree T(ones<vec>(2),ones<ivec>(2),zeros<vec>(2),ones<vec>(2));

    mat X = randu<mat>(2,atoi(argv[1]));
    for(int i=0;i<X.n_cols;i++)
    {
        T.insert(X.col(i));
    }

    mat Y = 2.0*X-1.0;

    while (!glfwWindowShouldClose(window))
    {
        glfwGetFramebufferSize(window, &win_x, &win_y);
        glViewport(0, 0, win_x, win_y);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0,1.0,1.0,1.0);

        recurse(T.root);

        glPointSize(5);

        glBegin(GL_POINTS);
        glColor3d(0.0,1.0,0.0);
        glVertex2d(2.0*mouse_x(0)-1.0,2.0*mouse_x(1)-1.0);
        uvec n_idx = zeros<uvec>(20);
        T.searchK(mouse_x,n_idx);
        for(uword i=0;i<X.n_cols;i++)
        {
	    int idx = -1;
	    for(uword k=0;k<n_idx.n_elem;k++)
		if(n_idx[k] == i)
		{
		   idx = k;
		   break;		
		}
            if(idx < 0)
                glColor3d(0.0,0.0,1.0);
            else if(idx == 0)
	        glColor3d(1.0,0.0,0.0);
            else
		glColor3d(1.0,1.0,0.0);
	    glVertex2d(Y(0,i),Y(1,i));
        }
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
