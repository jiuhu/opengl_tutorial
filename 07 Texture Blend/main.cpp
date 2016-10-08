#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <string>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "glu32.lib")

#define WINDOW_TITLE "OpenGL Window"

bool isBlend = true;

GLfloat x = 0.0f, y = 0.0f, z = 0.0f, horizontal = 0.0f, vertical = 0.0f;

GLUquadricObj * quadric = NULL;
GLUquadricObj * lightSource = NULL;

GLfloat position[] = { 0.0f, 0.0f, 3.0f, 1.0f };

GLuint texture1 = 0, texture2 = 0;

enum Geometry
{
	CUBE = 0,
	SPHERE,
	CYLINDER,

	NUM_GEOMETRY
};

Geometry type = SPHERE;

enum PolygonMode
{
	DOT = 0,
	LINE,
	FILL,

	NUM_POLYGONMODE
};

PolygonMode mode = DOT;

enum NormalMode
{
	FLAT = 0,
	SMOOTH,

	NUM_NORMALMODE
};

NormalMode normal = FLAT;
//--------------------------------------------------------------------

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
		break;

		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE) PostQuitMessage(0);

			switch (wParam)
			{
			case VK_LEFT:
				x -= 0.1f;
				break;
			case VK_RIGHT:
				x += 0.1f;
				break;

			case VK_UP:
				y += 0.1f;
				break;
			case VK_DOWN:
				y -= 0.1f;
				break;

			case VK_PRIOR:
				z += 0.1f;
				break;
			case VK_NEXT:
				z -= 0.1f;
				break;

			case 'A':
				horizontal += 10.0f;
				break;
			case 'D':
				horizontal -= 10.0f;
				break;

			case 'S':
				vertical += 10.0f;
				break;
			case 'W':
				vertical -= 10.0f;
				break;

			case 'J':
				position[0] -= 1.0f;
				glLightfv(GL_LIGHT0, GL_POSITION, position);
				break;
			case 'L':
				position[0] += 1.0f;
				glLightfv(GL_LIGHT0, GL_POSITION, position);
				break;
			case 'I':
				position[1] += 1.0f;
				glLightfv(GL_LIGHT0, GL_POSITION, position);
				break;
			case 'K':
				position[1] -= 1.0f;
				glLightfv(GL_LIGHT0, GL_POSITION, position);
				break;
			case 'U':
				position[2] += 1.0f;
				glLightfv(GL_LIGHT0, GL_POSITION, position);
				break;
			case 'O':
				position[2] -= 1.0f;
				glLightfv(GL_LIGHT0, GL_POSITION, position);
				break;

			case 'M':

				switch (mode)
				{
				case DOT:
					gluQuadricDrawStyle(quadric, GLU_POINT);
					glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
					mode = LINE;
					break;

				case LINE:
					gluQuadricDrawStyle(quadric, GLU_LINE);
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					mode = FILL;
					break;

				case FILL:
					gluQuadricDrawStyle(quadric, GLU_FILL);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					mode = DOT;
					break;

				}

				break;

			case 'N':

				switch (normal)
				{
				case FLAT:
					gluQuadricNormals(quadric, GLU_FLAT);
					glShadeModel(GL_FLAT);
					normal = SMOOTH;
					break;

				case SMOOTH:
					gluQuadricNormals(quadric, GLU_SMOOTH);
					glShadeModel(GL_SMOOTH);
					normal = FLAT;
					break;

				}
				
				break;

			case 'T':
				type = (type == CYLINDER) ? CUBE : (Geometry)((int)type + 1);
				break;

			case 'B':
				isBlend = !isBlend;
				break;

			}

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
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW;

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

void renderCube()
{
	glBegin(GL_QUADS); 

		// Face 1 - bottom
		//glColor3f(1.0f, 1.0f, 1.0f);
		glNormal3f(0.0f, -1.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		 
		// Face 2 - top
		//glColor3f(1.0f, 1.0f, 1.0f);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		 
		// Face 3 - left
		//glColor3f(0.0f, 0.0f, 0.0f);
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		 
		// Face 4 - right
		//glColor3f(0.0f, 0.0f, 1.0f);
		glNormal3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
		 
		// Face 5 - front
		//glColor3f(0.0f, 1.0f, 0.0f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		 
		// Face 6 - back
		//glColor3f(1.0f, 0.0f, 0.0f);
		glNormal3f(0.0f, 0.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);

	glEnd();  
}
//--------------------------------------------------------------------

void renderBlend()
{
	if (isBlend)
	{
		//glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
	//glEnable(GL_TEXTURE_2D); 	
	//glBindTexture(GL_TEXTURE_2D, texture1); 

	static GLfloat color[] = { 1.0f, 0.0f, 0.0f, 0.5f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, color);

	renderCube();
	glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 3.0f);
		glVertex3f(1.0f, -1.0f, 3.0f);
		glVertex3f(1.0f, 1.0f, 3.0f);
		glVertex3f(-1.0f, 1.0f, 3.0f);
	glEnd();

	//glDisable(GL_TEXTURE_2D);
	if (isBlend)
	{
		glDisable(GL_BLEND);
		//glEnable(GL_CULL_FACE);
	}
	else
	{
		glEnable(GL_CULL_FACE);
	}
}
//--------------------------------------------------------------------

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.5f, 1.0f, 1.0f);


	glPushMatrix();

		glTranslatef(x, y, z);
		glRotatef(horizontal, 0.0f, 1.0f, 0.0f);
		glRotatef(vertical, 1.0f, 0.0f, 0.0f);
	
		renderBlend();
		
		static GLfloat color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT, GL_DIFFUSE, color);

		switch (type)
		{
		case CUBE:
			glScalef(0.5f, 0.5f, 0.5f);
			renderCube();
			break;

		case SPHERE:
			glEnable(GL_TEXTURE_2D); 			
			glBindTexture(GL_TEXTURE_2D, texture2); 
			gluSphere(quadric, 1.0f, 10, 10);
			glDisable(GL_TEXTURE_2D);
			break;

		case CYLINDER:
			glEnable(GL_TEXTURE_2D); 			
			glBindTexture(GL_TEXTURE_2D, texture2); 
			gluCylinder(quadric, 1.0f, 1.0f, 2.0f, 10, 10);
			glDisable(GL_TEXTURE_2D);
			break;
		}

		
	glPopMatrix();

	//glPushMatrix();

	//	glTranslatef(position[0], position[1], position[2]);
	//	glRotatef(horizontal, 0.0f, 1.0f, 0.0f);
	//	glRotatef(vertical, 1.0f, 0.0f, 0.0f);

	//	glDisable(GL_LIGHTING);
	//	glColor3f(1.0f, 1.0f, 0.0f);
	//	gluSphere(lightSource, 0.2f, 10, 10);
	//	//gluCylinder(lightSource, 0.0f, 0.1f, 0.2f, 10, 10);
	//	glEnable(GL_LIGHTING);

	//glPopMatrix();

	glFlush();
}
//--------------------------------------------------------------------

GLuint loadTexture(const std::string &name)
{
	HBITMAP hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), name.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	
	BITMAP BMP; 
	GetObject(hBMP, sizeof(BMP), &BMP); 

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); 

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP.bmWidth, BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);
	
	DeleteObject(hBMP); 

	return texture;
}
//--------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.hInstance		= GetModuleHandle(NULL);
	wc.lpfnWndProc		= WindowProcedure;
	wc.lpszClassName	= WINDOW_TITLE;
    wc.style			= CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(	WINDOW_TITLE, "07 Texture Blend", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
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

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//--------------------------------
	//	End initialization
	//--------------------------------

	//--------------------------------
	//	Setup Projection
	//--------------------------------

	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();  
	//glOrtho(-1.0f, +1.0f, -1.0f, +1.0f, -1.0f, +1.0f);
	float aspect = 800.0f / 600.0f;
	glFrustum(-aspect, +aspect, -1.0f, +1.0f, 1.0f, +100.0f); 
	glTranslatef(0.0f, 0.0f, -5.0f);
	glMatrixMode(GL_MODELVIEW);  
	glLoadIdentity(); 

	//--------------------------------
	//	End
	//--------------------------------

	//--------------------------------
	//	Setup quadric
	//--------------------------------

	quadric = gluNewQuadric(); 
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluQuadricTexture(quadric, true);

	lightSource = gluNewQuadric();

	//--------------------------------
	//	End
	//--------------------------------

	//--------------------------------
	//	Setup Light
	//--------------------------------

	glEnable(GL_LIGHTING);

	GLfloat ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glEnable(GL_LIGHT0);

	//--------------------------------
	//	End
	//--------------------------------

	//--------------------------------
	//	Setup Texture
	//--------------------------------

	texture1 = loadTexture("texture/star.bmp");
	texture2 = loadTexture("texture/earth.bmp");

	//--------------------------------
	//	End
	//--------------------------------

	
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

		
	//--------------------------------
	//	OpenGL drawing
	//--------------------------------

		render();
		
	//--------------------------------
	//	End of OpenGL drawing
	//--------------------------------

		SwapBuffers(hdc);

	}

	//--------------------------------
	//	Clean up quadric
	//--------------------------------

	gluDeleteQuadric(quadric); 
	gluDeleteQuadric(lightSource); 

	//--------------------------------
	//	End
	//--------------------------------

	//--------------------------------
	//	Clean up texture
	//--------------------------------

	glDeleteTextures(1, &texture1); 
	glDeleteTextures(1, &texture2); 

	//--------------------------------
	//	End
	//--------------------------------

	UnregisterClass(WINDOW_TITLE, wc.hInstance);
	
	return true;
}
//--------------------------------------------------------------------