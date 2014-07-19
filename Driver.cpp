/******************************************************************************/
/*!
\file Driver.cpp
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #01
\date 21/02/2012
\brief
Driver for the project
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#include <cstdlib>
#include <GL/glew.h>
#include <GL/wglew.h>
#include <sstream>		
#include <iostream>
#include "Logger.h"
#include "FileWrapper.h"
#include "MatrixStack.h"
#include "ObjectWrapper.h"
#include "TGALoader.h"
#include <string.h>

/// stringstream typedef for use in logging while initializing openGL Render Context
typedef std::stringstream StringStream_t;

///Constants
const LPCTSTR LP_WINDOW_NAME = TEXT("CS1050_a.anshumanwadhera_1" ); /*! Window Name */
const LPCTSTR LP_CLASS_NAME = TEXT("Window_ClassName"); /*! Window Class */
const LPCTSTR LP_CREATE_WINDOW_ERROR = TEXT("Cannot create window"); /*! Window Error Message */

///Globals
HDC g_hDC = NULL; /*! Device context Handle */
HGLRC g_hGLRC = NULL; /*! openGL render context */
SIZE g_WinSize = { 0 }; /*! Viewport specific variable */
int flag = 1; /*! flag is used for handling WM_CLOSE message */
MatrixStack mv_s; /*! ModelView stack */
MatrixStack pr_s; /*! Projection stack */

///Function Prototypes
LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
void InitializeOpenGLRenderContext( HWND hWnd, HDC & hDC, HGLRC & hGLRC );
void InitializeOpenGLState( HWND hWnd );
void Update( HWND hWnd, HDC hDC );
void LoadGeometry( HWND hWnd );
void DestroyGeometry( HWND hWnd);
void UploadGeometryIntoGPU( HWND hWnd );

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE /*hPrevInstance*/,
	LPSTR /*lpCmdLine*/,
	int nShowCmd )
{
	WNDCLASS wndClass = { 0 };
	MSG msg = { 0 };
	HWND hWnd;
	// Initialize window class
	wndClass.style			= CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc	= WndProc;
	wndClass.cbClsExtra		= 0;
	wndClass.cbWndExtra		= 0;
	wndClass.hInstance		= hInstance;
	wndClass.hIcon			= LoadIcon( NULL, IDI_APPLICATION );
	wndClass.hCursor		= LoadCursor( NULL, IDC_ARROW );
	wndClass.hbrBackground	= NULL;
	wndClass.lpszMenuName	= NULL;
	wndClass.lpszClassName	= LP_CLASS_NAME;
	// Register class name
	RegisterClass( &wndClass );
	///Create object of Singleton:Logger
	Logger::create();
	/// Create the mesh objects and call their constructors.
	/// This does not create or upload geometry to the GPU. Also upload transforms
	Wrapper::create();
	Wrapper &wrapper = Wrapper::Instance();
	wrapper.state = 2;
	RECT WindowRect;                            // Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;                    // Set Left Value To 0
	WindowRect.right=(long)800;                 // Set Right Value To Requested Width
	WindowRect.top=(long)0;                     // Set Top Value To 0
	WindowRect.bottom=(long)600;                // Set Bottom Value To Requested Height
	wrapper.nodesGenerated = "";
	wrapper.timeTaken = "";
	wrapper.solutionSteps = "";
	wrapper.stressTestStr = "";
	/// Create the window 
	hWnd = CreateWindow(
		LP_CLASS_NAME,
		LP_WINDOW_NAME,
		/*WS_OVERLAPPEDWINDOW*/WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WindowRect.right-WindowRect.left,
		WindowRect.bottom-WindowRect.top, 
		NULL,
		NULL,
		hInstance,
		NULL );
	/// If we were not able to create a window, display an error message and exit
	if( hWnd == NULL )
	{
		MessageBox( NULL, LP_CREATE_WINDOW_ERROR, LP_WINDOW_NAME, MB_OK );
		return FALSE;
	}
	/// Display the window
	/// Sets the specified window's show state
	ShowWindow( hWnd, nShowCmd );
	LARGE_INTEGER freq, oldCount, newCount;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&oldCount);
	/// Window's main loop
	while( IsWindow( hWnd ) )
	{
		while( PeekMessage( &msg, hWnd, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		if(flag)
		{  
			QueryPerformanceCounter(&newCount);
			wrapper.dt = ((newCount.QuadPart - oldCount.QuadPart)*1.0f/freq.QuadPart);
			oldCount = newCount;
			if(wrapper.gameInSlidingState!=0)
			{
				wrapper.slideTile();
			}	
			else if(wrapper.gameinAIMode == 1)
			{
				if(wrapper.it2!=wrapper.sol.end())
				{
					if(wrapper.state == 2)
					{
						unsigned int *temp = *(wrapper.it1);
						wrapper.zeroCellPos = temp[4];
						temp = *(wrapper.it2);
						wrapper.slidingTileIndex = temp[4];
						(wrapper.it1)++;
						(wrapper.it2)++;
					}
					if(wrapper.state == 3)
					{
						unsigned int *temp = *(wrapper.it1);
						wrapper.zeroCellPos = temp[9];
						temp = *(wrapper.it2);
						wrapper.slidingTileIndex = temp[9];
						(wrapper.it1)++;
						(wrapper.it2)++;
					}
					wrapper.moveTileToEmptyCell();
				}
				else
				{
					wrapper.gameinAIMode = 0;
				}
			}
			Update( hWnd, g_hDC );
		}
	}
	/// Destroy the wrapper and logger instances of respective Singletons
	wrapper.destroyTiles();
	Wrapper::destroy();
	Logger::destroy();
	return static_cast<int>( msg.wParam );
}


/******************************************************************************/
/*!
The WndProc function is an application-defined function that processes
messages sent to a window

\param [in] hWnd
Handle to the window. 

\param [in] msg
Specifies the message. 

\param [in] wParam
Specifies additional message information. The contents of this parameter
depend on the value of the uMsg parameter. 

\param [in] lParam
Specifies additional message information. The contents of this parameter
depend on the value of the uMsg parameter. 

\return
The return value is the result of the message processing and depends on
the message sent.
*/
/******************************************************************************/
LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	static GLenum mode = GL_FILL;
	static bool isBackFaceEnabled = false;
	static enum PROJTYPE{ ORTHO, PERSP};
	static PROJTYPE proj;
	RECT rect;
	float aspectRatio;
	float posX, posY, posZ, winZ;
	///Return instance of type:Wrapper
	Wrapper &wrapper = Wrapper::Instance();
	switch( msg )
	{
	case WM_CREATE:
		///Set the projection type
		proj = ORTHO;
		///Set the initial camera position
		wrapper.camera.setCamera(0.0f,0.0f,10.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);
		///Initialize openGL rendre context
		InitializeOpenGLRenderContext( hWnd, g_hDC, g_hGLRC );
		///Initialize openGL state
		InitializeOpenGLState( hWnd );
		/// Create the geometries 
		LoadGeometry(hWnd);
		/// Upload the geometries to GPU
		UploadGeometryIntoGPU(hWnd);
		wrapper.createTiles();
		wrapper.InitializeTiles();
		break;

	case WM_SIZE:
		// If the window is minimized, do not do anything
		if( wParam == SIZE_MINIMIZED )
			break;
		// Get the client rectangle
		GetClientRect( hWnd, &rect );
		// Compute the size of the window
		g_WinSize.cx = rect.right - rect.left;
		g_WinSize.cy = rect.bottom - rect.top;
		// Update viewport size within the client rectangle
		glViewport( 0, 0, g_WinSize.cx, g_WinSize.cy );
		(Logger::Instance()).checkAndReportGLError("Failed to set Viewport",'e',4,__LINE__,__FILE__);
		// If the height of the window is zero, we do not do anything
		if( g_WinSize.cy == 0 ) 
			break;
		/// Calculate aspect ratio
		aspectRatio = static_cast<float>( g_WinSize.cx ) / g_WinSize.cy;
		/// set the viewport width and height for camera
		wrapper.camera.vpWidth = g_WinSize.cx;
		wrapper.camera.vpHeight = g_WinSize.cy;
		// Set the projection matrix
		glMatrixMode( GL_PROJECTION );
		(Logger::Instance()).checkAndReportGLError("Failed to set Matrix mode to GL_PROJECTION",'e',4,__LINE__,__FILE__);
		// Set the projection matrix stack to IDENTITY and recalculate the projection matrix		
		pr_s.LoadIdentity();
		pr_s.MultMatrix(my_Ortho(-8.0f,8.0f,-8.0f * ( 1.0f / aspectRatio ),8.0f * ( 1.0f / aspectRatio ), 0.0f, 20.0f));
		/// Load the projection matrix on top of stack
		glLoadMatrixf(pr_s.last());
		(Logger::Instance()).checkAndReportGLError("Failed to load matrix on top of openGL Projection stack",'e',4,__LINE__,__FILE__);
		break;
	case WM_CHAR:
		switch( wParam ) 
		{
		case 0x1b: // ESC
			PostMessage( hWnd, WM_CLOSE, 0, 0 );
			break;
		case 'e':
		case 'E':
			wrapper.gameinAIMode = (wrapper.gameinAIMode == 1)?0:wrapper.gameinAIMode;
			break;
		}
		break;
	case WM_LBUTTONUP: 
		wrapper.camera.mCurr.x = LOWORD( lParam );
		wrapper.camera.mCurr.y = HIWORD( lParam );
		wrapper.camera.mCurr.y = wrapper.camera.vpHeight - wrapper.camera.mCurr.y;
		/// Calculate Depth values for window coordinates
		glReadPixels( wrapper.camera.mCurr.x, wrapper.camera.mCurr.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
		(Logger::Instance()).checkAndReportGLError("Failed to read depth component of window coordinate",'e',4,__LINE__,__FILE__);
		unProject( float(wrapper.camera.mCurr.x), float(wrapper.camera.mCurr.y), winZ, mv_s.top(), pr_s.top(),wrapper.camera.vpWidth,wrapper.camera.vpHeight, &posX, &posY, &posZ);

		if(wrapper.gameinAIMode == 1)
		{
			if((posX>=3.3f && posX<=7.3f) && (posY>=-2.1f && posY<=-1.1f))
			{
				wrapper.speed = (wrapper.speed == 3.0f)?15.0f:3.0f;
				wrapper.animationSpeed = (wrapper.speed == 3.0f)?"Animation Speed:Slow":"Animation Speed:Fast";
			}
		}
		else if(wrapper.gameInSlidingState==0)
		{
			wrapper.processInput(posX,posY);
		}
		break;
	case WM_CLOSE:
		flag = 0;
		/// Unbind buffers and destroy vertex and indices array for objects
		DestroyGeometry(hWnd);
		/// Delete the OpenGL Render Context
		if( wglMakeCurrent( NULL, NULL ) == 0 ) /// Unselecting the render context
		{
			(Logger::Instance()).log("Failed to unselect the openGL render context",'e',4,__LINE__,__FILE__);
		}
		if( wglDeleteContext( g_hGLRC ) == 0 ) /// Deleting the render context
		{
			(Logger::Instance()).log("Failed to delete the openGL render context",'e',4,__LINE__,__FILE__);
		}
		/// Destroy the Device Context
		ReleaseDC( hWnd, g_hDC ); 
		/// Destroy the window
		DestroyWindow( hWnd ); 
		break;
	case WM_DESTROY:
		PostQuitMessage( EXIT_SUCCESS );
		break;
	default:
		return DefWindowProc( hWnd, msg, wParam, lParam );
	}
	return 0;
}

/******************************************************************************/
/*!
Creates a Window/OpenGL render context

\param [in] hWnd
Handle to this window

\param [out] hDC
Handle to the application device context

\param [out] hGLRC
Handle to the application render context
*/
/******************************************************************************/
void InitializeOpenGLRenderContext( HWND hWnd, HDC & hDC, HGLRC & hGLRC )
{
	const PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,								// version number
		PFD_DRAW_TO_WINDOW |			// support window
		PFD_SUPPORT_OPENGL |			// support OpenGL
		PFD_DOUBLEBUFFER,				// double buffered
		PFD_TYPE_RGBA,					// RGBA type
		32,								// 32-bit color depth
		0, 0, 0, 0, 0, 0,				// color bits ignored
		0,								// no alpha buffer
		0,								// shift bit ignored
		0,								// no accumulation buffer
		0, 0, 0, 0,						// accum bits ignored
		24,								// 24-bit z-buffer
		8,								// 8-bit stencil buffer
		0,								// no auxiliary buffer
		0,								// main layer
		0,								// reserved
		0, 0, 0							// layer masks ignored
	};

	BOOL bResult = FALSE;
	HGLRC tmpHGLRC = NULL;
	/// Get device context only once.
	hDC = GetDC( hWnd );
	/// Pixel format.
	int nPixelFormat = ChoosePixelFormat( hDC, &pfd );
	if( nPixelFormat == 0 )
	{
		(Logger::Instance()).log("Failed to specify the pixel format",'e',4,__LINE__,__FILE__);
	}
	bResult = SetPixelFormat( hDC, nPixelFormat, &pfd );
	if( bResult == FALSE )
	{
		(Logger::Instance()).log("Failed to set pixel format",'e',4,__LINE__,__FILE__);
	}
	/// Create the OpenGL Rendering Context, which is suitable for drawing on
	/// the device referenced by hDC. The rendering context has the same
	/// pixel format as the device context.
	tmpHGLRC = wglCreateContext( hDC );
	if( tmpHGLRC == NULL )
	{
		(Logger::Instance()).log("Failed to create the openGL render context",'e',4,__LINE__,__FILE__);
	}
	// Makes a specified OpenGL rendering context the calling thread's
	// current rendering context. All subsequent OpenGL calls made by the
	// thread are drawn on the device identified by hDC
	bResult = wglMakeCurrent( hDC, tmpHGLRC );
	if( bResult == FALSE )
	{
		(Logger::Instance()).log("Failed to make the specified openGL render context as current context",'e',4,__LINE__,__FILE__);
	}
	/// Initializing OpenGL extensions
	GLenum err = glewInit();
	if( err != GLEW_OK )
	{
		(Logger::Instance()).log("Failed to initialize GLEW",'e',4,__LINE__,__FILE__);
	}
	/// Stream used to display OpenGL version
	StringStream_t ss;
	/// Check if the create context for the new OpenGL is available
	if( wglewIsSupported( "WGL_ARB_create_context" ) == 1 )
	{
		const int attribs[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 3, 
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			/*WGL_CONTEXT_CORE_PROFILE_BIT_ARB*/
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
			0
		};
		hGLRC = wglCreateContextAttribsARB( hDC,0, attribs );
		if( !hGLRC )
		{
			(Logger::Instance()).log("Failed to create context Attrib ARB",'e',4,__LINE__,__FILE__);
		}
		wglMakeCurrent( NULL, NULL );
		wglDeleteContext( tmpHGLRC );
		wglMakeCurrent( hDC, hGLRC );
		/// Checking the OpenGL version number the OpenGL 3 way!
		int OpenGLVersion[2] = { 0 };
		glGetIntegerv( GL_MAJOR_VERSION, &OpenGLVersion[0] );
		(Logger::Instance()).checkAndReportGLError("Failed to get GL_MAJOR_VERSION",'e',4,__LINE__,__FILE__);
		glGetIntegerv( GL_MINOR_VERSION, &OpenGLVersion[1] );
		(Logger::Instance()).checkAndReportGLError("Failed to get GL_MINOR_VERSION",'e',4,__LINE__,__FILE__);
		/// Display the OpenGL version we are using
		ss << "OpenGL version:"
			<< OpenGLVersion[0]
		<< "."
			<< OpenGLVersion[1];
	}
	else
	{
		// It's not possible to make a GL 3.x context.
		// Use the old style context (GL 2.1 and before)
		hGLRC = tmpHGLRC;
		// Checking the OpenGL version number the OpenGL old way!
		const std::string strGLVersion = 
			reinterpret_cast<const char *>( glGetString( GL_VERSION ) );
		ss 	<< "WGL_ARB_create_context not supported!"
			<< "Cannot specify the OpenGL version to be used!"
			<< "OpenGL version:"
			<< strGLVersion;
	}
	(Logger::Instance()).log(ss.str().c_str(),'i',1,__LINE__,__FILE__);
	if( hGLRC == NULL )
	{
		(Logger::Instance()).log("openGL render context was not created",'e',4,__LINE__,__FILE__);
	}
	SelectObject (hDC, GetStockObject (SYSTEM_FONT));
	wglUseFontBitmaps (hDC, 0, 255, 1000); 
}

/******************************************************************************/
/*!
Initialize OpenGL states
*/
/******************************************************************************/
void InitializeOpenGLState( HWND /*hWnd*/ )
{
	/// Set the background color
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	(Logger::Instance()).checkAndReportGLError("Failed to clear background color",'e',4,__LINE__,__FILE__);
	/// Specify the vertex point size on screen
	glEnable(GL_PROGRAM_POINT_SIZE);
	(Logger::Instance()).checkAndReportGLError("Failed to enable GL_PROGRAM_POINT_SIZE",'e',4,__LINE__,__FILE__);
	GLfloat *currPointSize = new GLfloat;
	glGetFloatv(GL_POINT_SIZE_MAX,currPointSize);
	(Logger::Instance()).checkAndReportGLError("Failed to get GL_POINT_SIZE_MAX",'e',4,__LINE__,__FILE__);
	glPointSize(3.0);
	(Logger::Instance()).checkAndReportGLError("Failed to specify point size",'e',4,__LINE__,__FILE__);
	delete currPointSize;
	/// Enabling depth test
	glEnable( GL_DEPTH_TEST );	/// Enable Z-buffering
	(Logger::Instance()).checkAndReportGLError("Failed to enable GL_DEPTH_TEST",'e',4,__LINE__,__FILE__);
	glClearDepth( 1.0f );		/// Depth Buffer Setup
	(Logger::Instance()).checkAndReportGLError("Failed to setup depth buffer",'e',4,__LINE__,__FILE__);
	glDepthFunc( GL_LEQUAL );
	/// Set the value used to clear the Stencil Buffer
	glClearStencil( 0 );
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	(Logger::Instance()).checkAndReportGLError("Failed to clear stencil buffer",'e',4,__LINE__,__FILE__);
	/// Enabling primitive restart
	glPrimitiveRestartIndex( PRIMITIVE_RESTART_INDEX );
	glEnable( GL_PRIMITIVE_RESTART );
	(Logger::Instance()).checkAndReportGLError("Failed to enable PRIMITIVE_RESTART_INDEX",'e',4,__LINE__,__FILE__);
	/// Set back face culling
	glFrontFace( GL_CCW );
	glCullFace( GL_BACK );
	(Logger::Instance()).checkAndReportGLError("Failed to setup culling",'e',4,__LINE__,__FILE__);
}

/******************************************************************************/
/*!
Update function - Runs every frame and updates the states
\param [in] hDC
Handle to the application device context
*/
/******************************************************************************/
void Update( HWND /*hWnd*/, HDC hDC)
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
	(Logger::Instance()).checkAndReportGLError("Failed to clear buffers",'e',4,__LINE__,__FILE__);
	/// Set the matrix mode to modelview
	glMatrixMode( GL_MODELVIEW );
	(Logger::Instance()).checkAndReportGLError("Failed to setup matrix mode to GL_MODELVIEW",'e',4,__LINE__,__FILE__);
	/// Update the camera related specifics
	(Wrapper::Instance()).camera.lookAt();
	/// Load Identity on model view matrix stack
	mv_s.LoadIdentity();
	/// Multiply the camera view matrix with top of model view matrix stack
	mv_s.MultMatrix((Wrapper::Instance()).camera.viewMtx);
	/// Draw the geometry
	if(Wrapper::Instance().state == 2)
	{
		for(int i = 0;i < 4; ++i)
		{
			(Wrapper::Instance()).grid_2x2[i].drawTransformedGeometry(mv_s);
		}
	}
	else if(Wrapper::Instance().state == 3)
	{
		for(int i = 0;i < 9; ++i)
		{
			(Wrapper::Instance()).grid_3x3[i].drawTransformedGeometry(mv_s);
		}
	}
	for(int i = 0;i < 8; ++i)
	{
		(Wrapper::Instance()).grid_misc[i].drawTransformedGeometry(mv_s);
	}

	glDisable(GL_TEXTURE_2D);
	glLoadIdentity();
	glTranslatef(4.3f,3.2f,0.0f);
	glRasterPos2f(0.0f,0.0f);
	glColor3f(1.0f,1.0f,1.0f);
	glListBase (1000);
	const char *strToChar = (Wrapper::Instance()).algorithmUsed.c_str();
	glCallLists((Wrapper::Instance()).algorithmUsed.length(), GL_UNSIGNED_BYTE, LPCTSTR(strToChar));	

	std::string displayStr("Randomize");
	glLoadIdentity();
	glTranslatef(4.3f,2.0f,0.0f);
	glRasterPos2f(0.0f,0.0f);
	glColor3f(1.0f,1.0f,1.0f);
	glListBase (1000);
	strToChar = displayStr.c_str();
	glCallLists (displayStr.length(), GL_UNSIGNED_BYTE, LPCTSTR(strToChar));	

	displayStr = "Reset";
	glLoadIdentity();
	glTranslatef(4.3f,0.8f,0.0f);
	glRasterPos2f(0.0f,0.0f);
	glColor3f(1.0f,1.0f,1.0f);
	glListBase (1000);
	strToChar = displayStr.c_str();
	glCallLists(displayStr.length(), GL_UNSIGNED_BYTE, LPCTSTR(strToChar));

	displayStr = "Solve";
	glLoadIdentity();
	glTranslatef(4.3f,-0.4f,0.0f);
	glRasterPos2f(0.0f,0.0f);
	glColor3f(1.0f,1.0f,1.0f);
	glListBase (1000);
	strToChar = displayStr.c_str();
	glCallLists(displayStr.length(), GL_UNSIGNED_BYTE, LPCTSTR(strToChar));

	glLoadIdentity();
	glTranslatef(4.0f,-1.6f,0.0f);
	glRasterPos2f(0.0f,0.0f);
	glColor3f(1.0f,1.0f,1.0f);
	glListBase (1000);
	strToChar = (Wrapper::Instance()).animationSpeed.c_str();
	glCallLists ((Wrapper::Instance()).animationSpeed.length(), GL_UNSIGNED_BYTE, LPCTSTR(strToChar));

	displayStr = "Stress Test";
	glLoadIdentity();
	glTranslatef(4.3f,-2.8f,0.0f);
	glRasterPos2f(0.0f,0.0f);
	glColor3f(1.0f,1.0f,1.0f);
	glListBase (1000);
	strToChar = displayStr.c_str();
	glCallLists (displayStr.length(), GL_UNSIGNED_BYTE, LPCTSTR(strToChar));

	displayStr = "2x2";
	glLoadIdentity();
	glTranslatef(-1.3f,-4.0f,0.0f);
	glRasterPos2f(0.0f,0.0f);
	glColor3f(1.0f,1.0f,1.0f);
	glListBase (1000);
	strToChar = displayStr.c_str();
	glCallLists (displayStr.length(), GL_UNSIGNED_BYTE, LPCTSTR(strToChar));

	displayStr = "3x3";
	glLoadIdentity();
	glTranslatef(0.7f,-4.0f,0.0f);
	glRasterPos2f(0.0f,0.0f);
	glColor3f(1.0f,1.0f,1.0f);
	glListBase (1000);
	strToChar = displayStr.c_str();
	glCallLists (displayStr.length(), GL_UNSIGNED_BYTE, LPCTSTR(strToChar));

	glLoadIdentity();
	glTranslatef(-4.0f,4.0f,0.0f);
	glRasterPos2f(0.0f,0.0f);
	glColor3f(1.0f,1.0f,1.0f);
	glListBase (1000);
	strToChar = (Wrapper::Instance()).nodesGenerated.c_str();
	glCallLists ((Wrapper::Instance()).nodesGenerated.length(), GL_UNSIGNED_BYTE, LPCTSTR(strToChar));

	glLoadIdentity();
	glTranslatef(-4.0f,3.7f,0.0f);
	glRasterPos2f(0.0f,0.0f);
	glColor3f(1.0f,1.0f,1.0f);
	glListBase (1000);
	strToChar = (Wrapper::Instance()).timeTaken.c_str();
	glCallLists ((Wrapper::Instance()).timeTaken.length(), GL_UNSIGNED_BYTE, LPCTSTR(strToChar));

	glLoadIdentity();
	glTranslatef(-4.0f,3.4f,0.0f);
	glRasterPos2f(0.0f,0.0f);
	glColor3f(1.0f,1.0f,1.0f);
	glListBase (1000);
	strToChar = (Wrapper::Instance()).solutionSteps.c_str();
	glCallLists ((Wrapper::Instance()).solutionSteps.length(), GL_UNSIGNED_BYTE, LPCTSTR(strToChar));

	const char *tempStr;
	strToChar = (Wrapper::Instance()).stressTestStr.c_str();
	std::stringstream ss(strToChar);
	std::string s;
	float ycoord = 5.7f;
	while (getline(ss, s, '\n')) 
	{
		tempStr = s.c_str();
		glLoadIdentity();
		glTranslatef(-8.0f,ycoord,0.0f);
		glRasterPos2f(0.0f,0.0f);
		glColor3f(1.0f,1.0f,1.0f);
		glListBase (1000);
		glCallLists (s.length(), GL_UNSIGNED_BYTE, LPCTSTR(tempStr));
		tempStr = s.c_str();
		ycoord-=0.3f;
	}
	glEnable(GL_TEXTURE_2D);

	/// Swap the buffers
	if( SwapBuffers( hDC ) == 0 )
	{
		(Logger::Instance()).log("Failed to swap buffers",'e',4,__LINE__,__FILE__);
	}
}

/******************************************************************************/
/*!
Creates the geometries
*/
/******************************************************************************/
void LoadGeometry( HWND )
{ 
	(Wrapper::Instance()).createGeometries();
}

/******************************************************************************/
/*!
Upload the geometries to the GPU
*/
/******************************************************************************/
void UploadGeometryIntoGPU( HWND )
{
	(Wrapper::Instance()).uploadGeometries();
}

/******************************************************************************/
/*!
Destroys the geometries
*/
/******************************************************************************/
void DestroyGeometry(HWND )
{
	(Wrapper::Instance()).destroyGeometries();
}