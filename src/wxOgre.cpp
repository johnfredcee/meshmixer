#include "wxOgre.h"
#include "OgreLog.h"
#include "OgreLogManager.h"

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
	params["externalWindowHandle"] = handle;

#elif defined(__WXGTK__)
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);

	GtkWidget* privHandle = GetHandle();
    
	// prevents flickering
	gtk_widget_set_double_buffered(privHandle, FALSE);


    // this doesn't work w. Ogre 1.6.1 maybe this will fix it?
    gtk_widget_realize(privHandle);
   
	// grab the window object
	GdkWindow* gdkWin = GDK_PIZZA(privHandle)->bin_window;
	Display* display = GDK_WINDOW_XDISPLAY(gdkWin);
	Window wid = GDK_WINDOW_XWINDOW(gdkWin);

    XSync(display,wid);


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

void wxOgre::createOgreRenderResources()
{
	//////////////////////////////////////////////////////////////
	//  Debug Overlay
	//////////////////////////////////////////////////////////////
	mDebugPanelOverlay = Ogre::OverlayManager::getSingleton().getByName("CameraInfoPanel");
	mRoot->addFrameListener(this);
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
	static const wxString camCalcPosCaption(wxT("CalcCamPos:X:%08.4f, Y:%08.4f, Z:%08.4f"));
	static const wxString camRealPosCaption(wxT("CalcCamPos:X:%08.4f, Y:%08.4f, Z:%08.4f"));

	// TODO: Position camera somwehere sensible relative to object
	if (target)
	{
		Ogre::LogManager* logMgr = Ogre::LogManager::getSingletonPtr();
		Ogre::Log* log(logMgr->getDefaultLog());
		mTarget = target;
		Ogre::Vector3 size( target->getAttachedObject(0)->getBoundingBox().getSize() );
		Ogre::Vector3 camPos(target->getPosition() + size * 1.2f); 
		setZoomScale(size.length() / 30.0f);
		mCameraNode->setPosition(camPos);
		mCamera->lookAt(target->getPosition());
		mDebugPanelOverlay->show();
		wxString msg(wxString::Format(camCalcPosCaption, camPos.x, camPos.y, camPos.x));
		log->logMessage(Ogre::String(msg.mb_str(wxConvUTF8)));
		Ogre::Vector3 camRealPos(mCamera->getRealPosition());
		msg = wxString(wxString::Format(camCalcPosCaption, camRealPos.x, camRealPos.y, camRealPos.x));
		log->logMessage(Ogre::String(msg.mb_str(wxConvUTF8)));
	} else {
		mTarget = NULL;
	}

}

void wxOgre::toggleTimerRendering()
{
	// Toggle Start/Stop
	if (mTimer.IsRunning())
		mTimer.Stop();
	mTimer.Start(10);
}

void wxOgre::destroyOgreRenderWindow()
{
	mTimer.Stop();

	// destroy Viewport and RenderWindow
	if (mViewPort)
	{
		mRenderWindow->removeViewport(mViewPort->getZOrder());
	}

	Ogre::Root::getSingleton().detachRenderTarget(mRenderWindow);
	mViewPort = NULL;
	mRenderWindow = NULL;
	mRoot->shutdown();
    delete mRoot;
	mRoot = NULL;
}

wxOgre::~wxOgre()
{
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

Ogre::Real wxOgre::TrackballProjectToSphere(Ogre::Real r, Ogre::Real x, Ogre::Real y)
{
     float d, t, z;
 
	 d = Ogre::Math::Sqrt(x*x + y*y);
     if (d < r * 0.70710678118654752440f) {    /* Inside sphere */
		 z = Ogre::Math::Sqrt(r*r - d*d);
     } else {           /* On hyperbola */
         t = r / 1.41421356237309504880f;
         z = t*t / d;
     }
     return z;
}

void wxOgre::OnMouseMotion(wxMouseEvent& event)
{
	static float speed = 0.0f;
	static bool dragStart = true;
	static const wxString posInfo(wxT("Pos:X:%03d, Y:%03d Change X:%03d Y:%03d"));
	static Ogre::Quaternion startRot(Ogre::Quaternion::IDENTITY);

	Ogre::LogManager* logMgr = Ogre::LogManager::getSingletonPtr();
	Ogre::Log* log(logMgr->getDefaultLog());
	int left, top, width, height;	
	mCamera->getViewport()->getActualDimensions(left, top, width, height);

	wxPoint pos = event.GetPosition();
	wxPoint change = pos - mPrevPos;

	Ogre::Vector2 changeNorm((Ogre::Real) change.x / (Ogre::Real) width,
							 (Ogre::Real) -change.y / (Ogre::Real) height);
	
	if ((!dragStart) && ((!event.Dragging()) || (!event.LeftIsDown()))) {
 			wxString msg(wxT("Drag End"));
			log->logMessage(Ogre::String(msg.mb_str(wxConvUTF8)));
			dragStart = true;
	}

	if(event.Dragging())
	{
		if (event.LeftIsDown())
		{
			if (mTarget) {
				Ogre::Vector3 objectCentre(mTarget == NULL ? Ogre::Vector3::ZERO : mTarget->getPosition());
				if (dragStart) {
					Ogre::Vector3 cam2Object(mCameraNode->getPosition() - objectCentre);
					mDirection =  cam2Object.normalisedCopy();
					mRadius = cam2Object.length();
					mChangePos = Ogre::Vector2::ZERO;
					dragStart = false;					
					wxString msg(wxT("Drag Start"));
					log->logMessage(Ogre::String(msg.mb_str(wxConvUTF8)));
				}  else {
					Ogre::Vector3 across;
					Ogre::Vector3 up;
					Ogre::Vector3 forward;
					mCamera->getRealOrientation().ToAxes(across, up, forward);
					mChangePos += changeNorm;
					Ogre::Quaternion roty(Ogre::Radian(mChangePos.y * Ogre::Math::PI / 12.0f), across);
					Ogre::Quaternion rotx(Ogre::Radian(mChangePos.x * Ogre::Math::PI / 12.0f), up);					
					Ogre::Quaternion rot(roty * rotx);
					rot.normalise();
					mCameraNode->setPosition(objectCentre + ((rot * mDirection) * mRadius));
 					mCamera->lookAt(objectCentre);
				}
			//} 
			}
	//	else if(event.MiddleIsDown())
	//	{
	//		int left, top, width, height;
	//		mCamera->getViewport()->getActualDimensions(left, top, width, height);

	//		float speed = 1.0f;
	//		if (event.ShiftDown())
	//			speed = 0.1f;
	//		if (event.ControlDown())
	//			speed = 10.0f;
	//		float moveX = ((float)-change.x / (float)width) * mZoomScale * speed;
	//		float moveY = ((float)change.y / (float)height) * mZoomScale * speed;

	//		Ogre::Vector3 delta(mCamera->getRealOrientation().xAxis() * moveX);
	//		delta += mCamera->getRealOrientation().yAxis() * moveY;
	//		mCamera->setPosition(mCamera->getRealPosition() + delta);
		}
	}
	mPrevPos = pos;
}

void wxOgre::OnMouseWheel(wxMouseEvent& event)
{
	float speed = 1.0f;
	if (event.ShiftDown())
		speed = 0.1f;
	if (event.ControlDown())
		speed = 10.0f;

	float moveZ = ((float)event.GetWheelRotation() / 120.0) / 100.0;
	
	moveZ = moveZ * mZoomScale * speed;

	mCamera->moveRelative(Ogre::Vector3(0, 0, moveZ));
}

// -- ogre rendering

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

bool wxOgre::frameEnded(const Ogre::FrameEvent& evt)
{
	static const wxString camPosCaption(wxT("CamPos:X:%06.3f, Y:%06.3f, Z:%06.3f"));
	Ogre::OverlayElement* guiCurr  = Ogre::OverlayManager::getSingleton().getOverlayElement("TextAreaOverlayElement");
	Ogre::Vector3 camPos(mCamera->getRealPosition());
	wxString camText(wxString::Format(camPosCaption, camPos.x, camPos.y, camPos.z));
	guiCurr->setCaption(Ogre::String(camText.mb_str(wxConvUTF8)));
	return true;
		
}

// -- singleton

template<> wxOgre* Ogre::Singleton<wxOgre>::ms_Singleton = 0;
wxOgre& wxOgre::getSingleton()
{
	return ( *ms_Singleton );
}

wxOgre* wxOgre::getSingletonPtr()
{
	return ms_Singleton;
}
