#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <stdio.h>
#include <iostream>

using namespace std;
#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define WINDOW_TITLE "OpenGL Window"

GLUquadricObj * qObj;
float x = 0.0f;
float z = -1.0f;
float y = 2.0f;
float cutoff = 0.0f;
bool stencil = false;

GLfloat lightPosition[] = { -10.0, 10.0, 0.0, 1.0 };

GLuint m_fontBase;

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
		break;

		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE) PostQuitMessage(0);
			else if (wParam == VK_LEFT) lightPosition[0] -= 0.5f;
			else if (wParam == VK_RIGHT) lightPosition[0] += 0.5f;
			else if (wParam == VK_UP) lightPosition[2] -= 0.5f;
			else if (wParam == VK_DOWN) lightPosition[2] += 0.5f;
			else if (wParam == VK_PRIOR) z += 0.1f;
			else if (wParam == VK_NEXT) z -= 0.1f;
			else if (wParam == 'W') y += 0.1f;
			else if (wParam == 'A') y -= 0.1f;
			else if (wParam == 'S') stencil = !stencil;
			else if (wParam == 'C') 
			{
				cutoff += 1.0f;
				glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, cutoff);
			}
			else if (wParam == 'X') 
			{
				cutoff -= 1.0f;
				glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, cutoff);
			}
		break;

		//case WM_MOUSEMOVE:
		//	cout << wParam << lParam << endl;
		//	break;

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

void buildFont(HDC hdc)
{
	HFONT	font;										// Windows Font ID
	HFONT	oldfont;									// Used For Good House Keeping

	m_fontBase = glGenLists(96);								// Storage For 96 Characters

	font = CreateFont(	24,							// Height Of Font
						0,								// Width Of Font
						0,								// Angle Of Escapement
						0,								// Orientation Angle
						FW_BOLD,						// Font Weight
						FALSE,							// Italic
						FALSE,							// Underline
						FALSE,							// Strikeout
						ANSI_CHARSET,					// Character Set Identifier
						OUT_TT_PRECIS,					// Output Precision
						CLIP_DEFAULT_PRECIS,			// Clipping Precision
						ANTIALIASED_QUALITY,			// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
						"Courier New");					// Font Name

	oldfont = (HFONT)SelectObject(hdc, font);           // Selects The Font We Want
	wglUseFontBitmaps(hdc, 32, 96, m_fontBase);			// Builds 96 Characters Starting At Character 32
	SelectObject(hdc, oldfont);							// Selects The Font We Want
	DeleteObject(font);									// Delete The Font
}
//--------------------------------------------------------------------

void deleteFont()
{
	if (m_fontBase) {
		glDeleteLists(m_fontBase, 96);
		m_fontBase = 0;
	}
}
//--------------------------------------------------------------------

void renderText(GLfloat x, GLfloat y, const char * const text)
{
	glPushMatrix();
	glColor3f(0.8f, 0.8f, 0.0f);
	glRasterPos3f(x, y, 0.0f);	
	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(m_fontBase - 32);							// Sets The Base Character to 32
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits
	glPopMatrix();
}
//--------------------------------------------------------------------

void renderFormattedText(GLfloat x, GLfloat y, const char * const format, ...)
{
    va_list args;
    va_start(args, format);
	char strBuffer[256];
    vsprintf_s( strBuffer, 256, format, args );
    strBuffer[255] = '\0';
    va_end(args);

	renderText(x, y, strBuffer);
}
//--------------------------------------------------------------------

void display()
{
//--------------------------------
//	OpenGL drawing
//--------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(0.2f, 0.2f, 0.6f, 1.0f);


	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	glPushMatrix();

		glTranslatef(0.0f, 0.0f, z);
 
		if (stencil)
		{
			glEnable(GL_STENCIL_TEST); 
			glStencilFunc(GL_ALWAYS, 1, 1); 
			glStencilOp(GL_REPLACE,GL_REPLACE,GL_REPLACE); 
		}

		glPushMatrix();
			glTranslatef(0.0f, 0.0f, 0.0f);
			glBegin(GL_QUADS);

				glNormal3f(0.0f, 1.0f, 0.0f);

				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(-5.0f, 0.0f, -5.0f);

				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(-5.0f, 0.0f, 5.0f);

				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(5.0f, 0.0f, 5.0f);

				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(5.0f, 0.0f, -5.0f);

			glEnd();
		glPopMatrix();

		if (stencil)
		{
			glStencilFunc(GL_EQUAL, 1, 1); 
			glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
		}

		// now draw the shadow
		glPushMatrix();

			GLfloat matrix[16];
			matrix[0] = lightPosition[1];									// setup the projection matrix for the shadow (see lecture notes)
			matrix[1] = matrix[2] = matrix[3] = 0.0f;
			matrix[4] = -lightPosition[0];
			matrix[5] = 0.0f;
			matrix[6] = -lightPosition[2];
			matrix[7] = -1.0f;
			matrix[8] = matrix[9] = 0.0f;
			matrix[10] = lightPosition[1];
			matrix[11] = matrix[12] = matrix[13] = matrix[14] = 0.0f;
			matrix[15] = lightPosition[1];
		
			glTranslatef(0.0f, 0.01f, 0.0f);
			glMultMatrixf(matrix);									// multiply the current matrix on the stack with the shadow squish matrix
			glTranslatef(0.0f, y, 0.0f);

			glDisable(GL_LIGHTING);
			//glDisable(GL_DEPTH_TEST);
			glColor3f(0.2f, 0.2f, 0.2f);
			gluSphere(qObj, 0.75, 50, 50);
			//glEnable(GL_DEPTH_TEST);
			glEnable(GL_LIGHTING);

		glPopMatrix();

		if (stencil) glDisable(GL_STENCIL_TEST);

		glPushMatrix();
			glTranslatef(0.0f, y, 0.0f);
			glRotatef(x, 0.0f, 1.0f, 0.0f);
			gluSphere(qObj, 0.75, 50, 50);
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

	HWND hWnd = CreateWindow(	WINDOW_TITLE, "08 Shadow", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
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

	buildFont(hdc);

	//--------------------------------
	//	End initialization
	//--------------------------------

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

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

	gluLookAt(0.0f, 5.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

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

	deleteFont();

	hglrc = wglGetCurrentContext();
	if (hglrc) {
		wglDeleteContext(hglrc);
		hglrc = NULL;
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);
	
	return true;
}
//--------------------------------------------------------------------