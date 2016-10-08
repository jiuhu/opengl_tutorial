#include <Windows.h>
#include <gl/GL.h>

#pragma comment (lib, "OpenGL32.lib")

#define WINDOW_TITLE "OpenGL Window"
//--------------------------------------------------------------------

GLuint m_fontBase;
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

void buildFont(HDC hdc)
{
	HFONT font = CreateFont(24, 0, 0, 0,
						FW_BOLD, FALSE, FALSE, FALSE,
						ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
						ANTIALIASED_QUALITY, FF_DONTCARE|DEFAULT_PITCH, 
						"Courier New");

	SelectObject(hdc, font);					// Selects The Font We Want
	
	m_fontBase = glGenLists(96);					// Storage For 96 Characters
	wglUseFontBitmaps(hdc, 32, 96, m_fontBase);		// Builds 96 Characters Starting At Character 32	

	DeleteObject(font);
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
	glColor3f(0.8f, 0.8f, 0.0f);
	glRasterPos3f(x, y, 0.0f);	
	glListBase(m_fontBase - 32);							// Sets The Base Character to 32
	glCallLists(strlen(text), GL_BYTE, text);	// Draws The Display List Text
}
//--------------------------------------------------------------------

void render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0f, 0.5f, 1.0f, 1.0f);

	glPushMatrix();

		renderText(0.0f, 0.0f, "Hello World");
	
	glPopMatrix();

	glFlush();
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

	HWND hWnd = CreateWindow(	WINDOW_TITLE, "04 Cube", WS_OVERLAPPEDWINDOW,
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

	//--------------------------------
	//	Setup Projection
	//--------------------------------

	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();  
	//glOrtho(-1.0f, +1.0f, -1.0f, +1.0f, -1.0f, +1.0f);
	glFrustum(-1.0f, +1.0f, -1.0f, +1.0f, 1.0f, +100.0f); 
	glMatrixMode(GL_MODELVIEW);  
	glLoadIdentity(); 
	glTranslatef(0.0f, 0.0f, -5.0f);

	//--------------------------------
	//	End
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);
	
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

	deleteFont();

	UnregisterClass(WINDOW_TITLE, wc.hInstance);
	
	return true;
}
//--------------------------------------------------------------------