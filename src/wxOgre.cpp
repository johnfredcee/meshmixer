#include "wxOgre.h"

#ifdef __WXGTK__
#include <gdk/gdkx.h>
#include <gdk/gdk.h>
#include <gtk/gtk.h> 
#include <wx/gtk/win_gtk.h>
#include <GL/glx.h>
#endif

// Required for the timer
const long ID_RENDERTIMER = wxNewId();

// Required for WX
IMPLEMENT_CLASS(wxOgre, wxControl)

// Required for WX
BEGIN_EVENT_TABLE(wxOgre, wxControl)
	EVT_SIZE(wxOgre::OnSize)
	// EVT_PAINT(wxOgre::OnPaint) // Produces flickers and runs too fast!
	EVT_ERASE_BACKGROUND(wxOgre::OnEraseBackground)
	EVT_TIMER(ID_RENDERTIMER, wxOgre::OnRenderTimer)
	EVT_LEFT_DOWN(wxOgre::OnMouseDown)
	EVT_MIDDLE_DOWN(wxOgre::OnMouseDown)
	EVT_RIGHT_DOWN(wxOgre::OnMouseDown)
	EVT_MOTION(wxOgre::OnMouseMotion)
	EVT_MOUSEWHEEL(wxOgre::OnMouseWheel)
END_EVENT_TABLE()

wxOgre::wxOgre(wxFrame* parent) : 
		wxControl(parent, -1),
		mTimer(this, ID_RENDERTIMER),
		mRoot(0),
		mViewPort(0),
		mCamera(0),
		mSceneMgr(0),
		mRenderWindow(0),
		mCameraNode(0),
		mZoomScale(1)
{
}

void wxOgre::createOgreRenderWindow()
{
	// See if an Ogre::Root already exists
	mRoot = Ogre::Root::getSingletonPtr();  
	mRenderWindow = mRoot->initialise(false);

	// --------------------
	// Create a new parameters list according to compiled OS
	Ogre::NameValuePairList params;
	Ogre::String handle;
#ifdef __WXMSW__
	handle = Ogre::StringConverter::toString((size_t)((HWND)GetHandle()));
#elif defined(__WXGTK__)
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);

	GtkWidget* privHandle = m_wxwindow;
    
	// prevents flickering
	gtk_widget_set_double_buffered(privHandle, FALSE);


    // this doesn't work w. Ogre 1.6.1 maybe this will fix it?
    //gtk_widget_realize(privHandle);
    

	// grab the window object
	GdkWindow* gdkWin = GTK_PIZZA(privHandle)->bin_window;
	Display* display = GDK_WINDOW_XDISPLAY(gdkWin);
	Window wid = GDK_WINDOW_XWINDOW(gdkWin);


    //XSync(display,wid);

	std::stringstream str;

	// display
	str << (unsigned long)display << ':';

	// screen (returns "display.screen")
	std::string screenStr = DisplayString(display);
	std::string::size_type dotPos = screenStr.find(".");
	screenStr = screenStr.substr(dotPos+1, screenStr.size());
	str << screenStr << ':';

	// XID
	str << wid << ':';

	// retrieve XVisualInfo
	int attrlist[] = { GLX_RGBA, GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, 16, GLX_STENCIL_SIZE, 8, None };
	XVisualInfo* vi = glXChooseVisual(display, DefaultScreen(display), attrlist);
	str << (unsigned long)vi;

	handle = str.str();    
    
#else
#error Not supported on this platform.
#endif

	params["externalWindowHandle"] = handle;

	// Get wx control window size
	int width;
	int height;
	GetSize(&width, &height);
	// Create the render window
	mRenderWindow = Ogre::Root::getSingleton().createRenderWindow("OgreRenderWindow", width, height, false, &params);

    // line is required, otherwise the screen does not update
    mRenderWindow->setActive(true);
    
	// --------------------
	// Create the SceneManager, in this case a generic one
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "ExampleSMInstance");
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
	
	// --------------------
	// Create the camera
	mCamera = mSceneMgr->createCamera("EntityCamera");
	// Set up the SceneNodes to control the camera
	mCameraNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mCameraNode->attachObject(mCamera);
	mCamera->setNearClipDistance(0.1);
	// Set the viewport
	mViewPort = mRenderWindow->addViewport(mCamera); 
	// Set the background to match the wxWindow background color
	mViewPort->setBackgroundColour(Ogre::ColourValue(212.0f/255.0f, 208.0f/255.0f, 200.0f/255.0f, 1.0f)); 

	//mLightNode = mCameraNode->createChildSceneNode();

    mXLight = mSceneMgr->createLight("XLight");
    mXLight->setType(Ogre::Light::LT_DIRECTIONAL);
    mXLight->setDiffuseColour(1.0f, 1.0f,1.0f);
    mXLight->setDirection(-1.0f, 0.0f, 0.0f); 

    mYLight = mSceneMgr->createLight("YLight");
    mYLight->setType(Ogre::Light::LT_DIRECTIONAL);
    mYLight->setDiffuseColour(1.0f, 1.0f,1.0f);
    mYLight->setDirection(0.0f, -1.0f, 0.0f); 

    mZLight = mSceneMgr->createLight("ZLight");
    mZLight->setType(Ogre::Light::LT_DIRECTIONAL);
    mZLight->setDiffuseColour(1.0f, 1.0f,1.0f);
    mZLight->setDirection(0.0f, 0.0f, -1.0f); 



}

void wxOgre::wireFrame(bool wireframe)
{
    if (wireframe)
        mCamera->setPolygonMode(Ogre::PM_WIREFRAME);
    else
        mCamera->setPolygonMode(Ogre::PM_SOLID);
}


void wxOgre::resetCamera()
{
    mCamera->setAutoTracking(false);
	mCameraNode->setPosition(Ogre::Vector3::ZERO);
	mCameraNode->setOrientation(Ogre::Quaternion::IDENTITY);
	mCamera->setPosition(Ogre::Vector3::ZERO);
}

void wxOgre::cameraTrackNode(Ogre::SceneNode* target)
{
    mTarget = target;
    if (target != NULL) {
        mCamera->setAutoTracking(true, target);
        Ogre::Vector3 delta(  mCamera->getPosition() - mTarget->getPosition() );
        mRadius = delta.length();
        mTheta = acos( delta.z / delta.length() );
        mRho = atan2(delta.x, delta.y);  
    }
    else
        mCamera->setAutoTracking(false);
}

void wxOgre::toggleTimerRendering()
{
	// Toggle Start/Stop
	if (mTimer.IsRunning())
		mTimer.Stop();
	mTimer.Start(10);
}

wxOgre::~wxOgre()
{
	// destroy Viewport and RenderWindow
	if (mViewPort)
	{
		mRenderWindow->removeViewport(mViewPort->getZOrder());
		mViewPort = 0;
	}

	Ogre::Root::getSingleton().detachRenderTarget(mRenderWindow);
	mRenderWindow = 0;

    delete mRoot;


}

void wxOgre::OnSize(wxSizeEvent& event)
{
	// Setting new size;
	int width;
	int height;
	GetSize(&width, &height);
    
    if (mRenderWindow) {
        mRenderWindow->resize( width, height );
        // Letting Ogre know the window has been resized;
        mRenderWindow->windowMovedOrResized();
    }

    if (mCamera) {
        // Set the aspect ratio for the new size;
        if (mCamera)
            mCamera->setAspectRatio(Ogre::Real(width) / Ogre::Real(height));
    }

	update();
}
void wxOgre::OnPaint(wxPaintEvent& event)
{
//	update(); // Produces flickers and runs too fast!
}
void wxOgre::OnEraseBackground(wxEraseEvent& event)
{
	update();
}
void wxOgre::OnRenderTimer(wxTimerEvent& event)
{
	update();
}

void wxOgre::OnMouseDown(wxMouseEvent& event)
{
	mPrevPos = event.GetPosition();
	event.Skip();
}

void wxOgre::OnMouseMotion(wxMouseEvent& event)
{
	if(event.Dragging())
	{
		wxPoint pos = event.GetPosition();
		wxPoint change = pos - mPrevPos;

		if(event.LeftIsDown())
		{
            mYaw += Ogre::Degree(-change.x / (event.ShiftDown() ? 20.0 : 2.0));
            mPitch += Ogre::Degree(-change.y / (event.ShiftDown() ? 20.0 : 2.0));
            mCameraNode->setOrientation(  Ogre::Quaternion( mPitch, Ogre::Vector3::UNIT_X ) * Ogre::Quaternion( mYaw,  Ogre::Vector3::UNIT_Z )  );

		}
		else if(event.MiddleIsDown())
		{
			int left, top, width, height;
			mCamera->getViewport()->getActualDimensions(left, top, width, height);

			float moveX = ((float)-change.x / (float)width) * mZoomScale * (event.ShiftDown() ? 0.1 : 1.0);
			float moveY = ((float)change.y / (float)height) * mZoomScale * (event.ShiftDown() ? 0.1 : 1.0);

			mCamera->moveRelative(Ogre::Vector3(moveX, moveY, 0));
		}
		//else if(event.RightIsDown())
		//{
		//	mLightYawNode->yaw(Ogre::Degree(change.x / (event.ShiftDown() ? 20.0 : 2.0)));
		//	mLightPitchNode->pitch(Ogre::Degree(change.y /(event.ShiftDown() ? 20.0 : 2.0)));
		//}

		mPrevPos = pos;
	}
}

void wxOgre::OnMouseWheel(wxMouseEvent& event)
{
	float moveZ = ((float)event.GetWheelRotation() / 120.0) / 100.0;
	moveZ = moveZ * mZoomScale * (event.ShiftDown() ? 1.0 : 10.0);

	mCamera->moveRelative(Ogre::Vector3(0, 0, moveZ));
}

void wxOgre::update()
{
 // ****************************************************
   // TODO: REMOVE THESE LINES! These are merely for test!

   // static float redTone = 0;
   // redTone += 0.01;
   // if(redTone>1.0)
   //     redTone=0;
   // ****************************************************
   if (mViewPort) {
       mViewPort->setBackgroundColour(Ogre::ColourValue(0.2, 0.2f, 0.2f, 1.0f));     
       Ogre::Root::getSingletonPtr()->renderOneFrame();
   }
}

template<> wxOgre* Ogre::Singleton<wxOgre>::ms_Singleton = 0;
wxOgre& wxOgre::getSingleton()
{
	return ( *ms_Singleton );
}

wxOgre* wxOgre::getSingletonPtr()
{
	return ms_Singleton;
}
