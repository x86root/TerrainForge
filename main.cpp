#include <windows.h>
#include <gl/gl.h>
#include <GL/glu.h>
#include "terrain.h"
#include "renderer.h"

// Global instances
Terrain terrain(400, 400);			// Sets size of our generated Land. Bigger will fill the screen more and so on. 
Renderer renderer;

float camX = 0.0f;
float camY = 60.0f;			// Global Camera offsets to be able to maneuver the camera
float camZ = 150.0f;

// Function declarations
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void EnableOpenGL(HWND hWnd, HDC* hDC, HGLRC* hRC);
void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
    WNDCLASS wc = {};
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszClassName = "GLSample";
    RegisterClass(&wc);

    HWND hWnd = CreateWindow("GLSample", "Procedural Terrain Generation ~~~ Trevor Belt",
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT, 800, 700,
                             NULL, NULL, hInstance, NULL);

    if (!hWnd) return 1;

    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);

    HDC hDC;
    HGLRC hRC;
    EnableOpenGL(hWnd, &hDC, &hRC);

    glViewport(0, 0, 800, 700);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(45.0, 800.0 / 700.0, 1.0, 1000.0);
    gluPerspective(60.0, 800.0 / 700.0, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);


    MSG msg;
    BOOL bQuit = FALSE;
    float theta = 0.0f;

    while (!bQuit)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                bQuit = TRUE;
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
        	//this centers the terrain around the camera. as you pan with arrow keys, the terrain shifts and regenerates.
        	terrain.terraGenerator(camX - terrain.width / 2, camZ - terrain.height / 2);
        	
        	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        	// Sky gradient background
			glDisable(GL_DEPTH_TEST);
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(-1, 1, -1, 1, -1, 1); // 2D screen space

			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();

			glBegin(GL_QUADS);
    			glColor3f(0.5f, 0.7f, 1.0f); // Top sky
    			glVertex2f(-1.0f, 1.0f);
    			glVertex2f(1.0f, 1.0f);
    			glColor3f(0.9f, 0.9f, 1.0f); // Horizon
    			glVertex2f(1.0f, -1.0f);
    			glVertex2f(-1.0f, -1.0f);
			glEnd();

glPopMatrix();
glMatrixMode(GL_PROJECTION);
glPopMatrix();
glMatrixMode(GL_MODELVIEW);
glEnable(GL_DEPTH_TEST);
            
            glLoadIdentity();

            gluLookAt(camX, camY, camZ, camX, 0.0f, camZ - 100.0f, 0, 1, 0);
            glRotatef(theta, 0.0f, 1.0f, 0.0f);			// Automatically rotates the terrain. Comment out to keep static 
            //theta += 0.1f;

            renderer.drawTerrain(terrain);

            SwapBuffers(hDC);
        }
    }

    DisableOpenGL(hWnd, hDC, hRC);
    DestroyWindow(hWnd);

    return 0;
}

// Window procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_LEFT:  camX -= 5.0f; break;
        case VK_RIGHT: camX += 5.0f; break;
        case VK_UP:    camZ -= 5.0f; break;
        case VK_DOWN:  camZ += 5.0f; break;
        case VK_OEM_PLUS: // '+' key
    		camY -= 5.0f;
    		camZ -= 5.0f;
    		break;

		case VK_OEM_MINUS: // '-' key
    		camY += 5.0f;
    		camZ += 5.0f;
    		break;
        case VK_ESCAPE:
            PostQuitMessage(0);
            return 0;
        }
        return 0;  // ? Ensures WM_KEYDOWN always returns something. Fixed compiler 

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}



// OpenGL setup
void EnableOpenGL(HWND hWnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    *hDC = GetDC(hWnd);
    int iFormat = ChoosePixelFormat(*hDC, &pfd);
    SetPixelFormat(*hDC, iFormat, &pfd);

    *hRC = wglCreateContext(*hDC);
    wglMakeCurrent(*hDC, *hRC);
    glEnable(GL_DEPTH_TEST);
    
    // Fog setup
	glEnable(GL_FOG);
	GLfloat fogColor[] = { 0.5f, 0.7f, 1.0f, 1.0f }; // Match sky
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_START, 200.0f);
	glFogf(GL_FOG_END, 600.0f);
}

void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hWnd, hDC);
}