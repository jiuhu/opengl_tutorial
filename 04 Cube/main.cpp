#include <Windows.h>
#include <gl/GL.h>

#pragma comment (lib, "OpenGL32.lib")

#define WINDOW_TITLE "OpenGL Window"

float x = 0.0f, y = 0.0f, z = 0.0f, horizontal = 0.0f, vertical = 0.0f;
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

		// Face 1
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		 
		// Face 2 
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		 
		// Face 3 
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		 
		// Face 4 
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
		 
		// Face 5 
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		 
		// Face 6 
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);

	glEnd();  
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

		renderCube();
	
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

	glEnable(GL_DEPTH_TEST);

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

	UnregisterClass(WINDOW_TITLE, wc.hInstance);
	
	return true;
}
//--------------------------------------------------------------------