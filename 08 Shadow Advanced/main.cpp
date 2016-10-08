#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <cstdio>
#include <iostream>
#include <cmath>

using namespace std;

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define WINDOW_TITLE "OpenGL Window"

GLUquadricObj * qObj;
float x = 0.0f, z = 0.0f, angle = 0.0f, boxSize = 10.0f;
bool stencil = true;

GLfloat lightPosition[] = { 0.0, 0.0, 0.0, 1.0 };

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
		break;

		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE) PostQuitMessage(0);
			else if (wParam == VK_LEFT) angle -= 0.1f;
			else if (wParam == VK_RIGHT) angle += 0.1f;
			//else if (wParam == 'S') stencil = !stencil;
		break;

		default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
 
	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 1;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
 
	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------

void display()
{
//--------------------------------
//	OpenGL drawing
//--------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(0.2f, 0.2f, 0.6f, 1.0f);

	x = cos(angle) * 5.0f;
	z = sin(angle) * 5.0f;

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	
	glPushMatrix();

		glPushMatrix();
 
//--------------------------------
//	Render xz-plane
//	No shadow at here
//--------------------------------		
			glPushMatrix();
				glBegin(GL_QUADS);

				//	Bottom face
					float red[] = {0.8f, 0.0f, 0.0f, 1.0f};
					glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red);
					glNormal3f(0.0f, 1.0f, 0.0f);
					glVertex3f(-boxSize, -boxSize, -boxSize);
					glVertex3f(-boxSize, -boxSize, boxSize);
					glVertex3f(boxSize, -boxSize, boxSize);
					glVertex3f(boxSize, -boxSize, -boxSize);

				glEnd();
			glPopMatrix();
//--------------------------------
//	Done render for xz-plane.
//--------------------------------

			
//--------------------------------
//	Render for xy-plane
//--------------------------------
			glPushMatrix();
			
				if (stencil)
				{
					glEnable(GL_STENCIL_TEST); 
					glStencilFunc(GL_ALWAYS, 1, 1); 
					glStencilOp(GL_REPLACE,GL_REPLACE,GL_REPLACE); 
				}

				glPushMatrix();
					glBegin(GL_QUADS);
					//	Back face
						float green[] = {0.0f, 0.8f, 0.0f, 1.0f};
						glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, green);
						glNormal3f(0.0f, 0.0f, 1.0f);
						glVertex3f(boxSize, boxSize, -boxSize);
						glVertex3f(boxSize, -boxSize, -boxSize);
						glVertex3f(-boxSize, -boxSize, -boxSize);
						glVertex3f(-boxSize, boxSize, -boxSize);
					glEnd();
				glPopMatrix();

				if (stencil)
				{
					glStencilFunc(GL_EQUAL, 1, 1); 
					glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
				}

				glPushMatrix();
					GLfloat matrixXY[16];
					matrixXY[0] = lightPosition[2] + boxSize;
					matrixXY[1] = matrixXY[2] = matrixXY[3] = matrixXY[4] = 0.0f;
					matrixXY[5] = lightPosition[2] + boxSize;
					matrixXY[6] = matrixXY[7] = 0.0f;
					matrixXY[8] = -lightPosition[0];
					matrixXY[9] = -lightPosition[1];
					matrixXY[10] = 0.0f;
					matrixXY[11] = -1.0f;
					matrixXY[12] = matrixXY[13] = matrixXY[14] = 0.0f;
					matrixXY[15] = lightPosition[2] + boxSize;
		
					glTranslatef(0.0f, 0.0f, -boxSize);
					glMultMatrixf(matrixXY);
					glDisable(GL_LIGHTING);
					glDisable(GL_DEPTH_TEST);
					glColor3f(0.2f, 0.2f, 0.2f);
					glTranslatef(x, 0.0f, z + boxSize);
					gluSphere(qObj, 1.0, 50, 50);
					glEnable(GL_DEPTH_TEST);
					glEnable(GL_LIGHTING);
				glPopMatrix();

				if (stencil) glDisable(GL_STENCIL_TEST);
				glClear(GL_STENCIL_BUFFER_BIT);

			glPopMatrix();
//--------------------------------
//	Done
//--------------------------------
			
//--------------------------------
//	Render for yz-plane
//--------------------------------
			glPushMatrix();

				if (stencil)
				{
					glEnable(GL_STENCIL_TEST); 
					glStencilFunc(GL_ALWAYS, 1, 1); 
					glStencilOp(GL_REPLACE,GL_REPLACE,GL_REPLACE); 
				}
			
				glPushMatrix();
					glBegin(GL_QUADS);
					//	Left face
						float blue[] = {0.0f, 0.0f, 0.8f, 1.0f};
						glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blue);
						glNormal3f(1.0f, 0.0f, 0.0f);
						glVertex3f(-boxSize, -boxSize, boxSize);
						glVertex3f(-boxSize, boxSize, boxSize);
						glVertex3f(-boxSize, boxSize, -boxSize);
						glVertex3f(-boxSize, -boxSize, -boxSize);
					//	Right face
						float yellow[] = {0.8f, 0.8f, 0.0f, 1.0f};
						glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, yellow);
						glNormal3f(-1.0f, 0.0f, 0.0f);
						glVertex3f(boxSize, -boxSize, boxSize);
						glVertex3f(boxSize, boxSize, boxSize);
						glVertex3f(boxSize, boxSize, -boxSize);
						glVertex3f(boxSize, -boxSize, -boxSize);
					glEnd();
				glPopMatrix();

				if (stencil)
				{
					glStencilFunc(GL_EQUAL, 1, 1); 
					glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
				}

				glPushMatrix();
					GLfloat matrixYZ[16];
					matrixYZ[0] = matrixYZ[4] = matrixYZ[6] = matrixYZ[7] = 
						matrixYZ[8] = matrixYZ[9] = matrixYZ[11] = matrixYZ[12] = 
						matrixYZ[13] = matrixYZ[14] = 0.0f;
		
					glPushMatrix();
						matrixYZ[1] = -lightPosition[1];
						matrixYZ[2] = -lightPosition[2];
						matrixYZ[3] = -1.0f;
						matrixYZ[5] = lightPosition[0] + boxSize;
						matrixYZ[10] = lightPosition[0] + boxSize;
						matrixYZ[15] = lightPosition[0] + boxSize;

						glTranslatef(-boxSize, 0.0f, 0.0f);
						glMultMatrixf(matrixYZ);
						glDisable(GL_LIGHTING);
						glDisable(GL_DEPTH_TEST);
						glColor3f(0.2f, 0.2f, 0.2f);
						glTranslatef(x + boxSize, 0.0f, z);
						gluSphere(qObj, 1.0, 50, 50);
						glEnable(GL_DEPTH_TEST);
						glEnable(GL_LIGHTING);
					glPopMatrix();
		
					glPushMatrix();
						matrixYZ[1] = lightPosition[1];
						matrixYZ[2] = lightPosition[2];
						matrixYZ[3] = 1.0f;
						matrixYZ[5] = -lightPosition[0] + boxSize;
						matrixYZ[10] = -lightPosition[0] + boxSize;
						matrixYZ[15] = -lightPosition[0] + boxSize;

						glTranslatef(boxSize, 0.0f, 0.0f);
						glMultMatrixf(matrixYZ);
						glDisable(GL_LIGHTING);
						glDisable(GL_DEPTH_TEST);
						glColor3f(0.2f, 0.2f, 0.2f);
						glTranslatef(x - boxSize, 0.0f, z);
						gluSphere(qObj, 1.0, 50, 50);
						glEnable(GL_DEPTH_TEST);
						glEnable(GL_LIGHTING);
					glPopMatrix();

				glPopMatrix();

				if (stencil) glDisable(GL_STENCIL_TEST);
				glClear(GL_STENCIL_BUFFER_BIT);

			glPopMatrix();
//--------------------------------
//	Done
//--------------------------------
			
//--------------------------------
//	Render sphere
//--------------------------------
			glPushMatrix();
				glTranslatef(x, 0.0f, z);
				float white[] = {1.0f, 1.0f, 1.0f, 1.0f};
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
				gluSphere(qObj, 1.0, 50, 50);
			glPopMatrix();
//--------------------------------
//	Done
//--------------------------------

		glPopMatrix();

	glPopMatrix();

	glFlush();
		
//--------------------------------
//	End of OpenGL drawing
//--------------------------------
}
//--------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
//int main()
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.hInstance		= GetModuleHandle(NULL);
	wc.lpfnWndProc		= WindowProcedure;
	wc.lpszClassName	= WINDOW_TITLE;
    wc.style			= CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(	WINDOW_TITLE, "08 Shadow Advanced", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
								CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
								NULL, NULL, wc.hInstance, NULL );
	
	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;
	
	//--------------------------------
	//	End initialization
	//--------------------------------

	glEnable(GL_DEPTH_TEST);

	qObj = gluNewQuadric();
	gluQuadricNormals(qObj, GLU_SMOOTH);
	gluQuadricTexture(qObj, GLU_TRUE);

	glEnable(GL_LIGHTING);

	GLfloat diffuse[] = { 1.0, 1.0, 1.0, 0.5 };

	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	glEnable(GL_LIGHT0);


	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	glFrustum(-1.0f, +1.0f, -1.0f, +1.0f, 1.0f, +100.0f);

	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -20.0f);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while(true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();
		
		SwapBuffers(hdc);
	}

	gluDeleteQuadric(qObj);

	hglrc = wglGetCurrentContext();
	if (hglrc) {
		wglDeleteContext(hglrc);
		hglrc = NULL;
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);
	
	return true;
}
//--------------------------------------------------------------------