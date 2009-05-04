#ifndef __WXOGRE_H__
#define __WXOGRE_H__

#include "Ogre.h"
#include "wx/wx.h"

/**
@brief WX widget for and Ogre rendering window

This WX widget is a self-contained SINGLETON Ogre rendering window;
meaning it contains all Ogre objects necessary to create a rendering
window and currently supports only one rendering window at a time!
This is due to the limitation of the self contained class.

@usage Simply create a new wxOgre object and pass a wxFrame as its
parent window. Then work with it just like ay other wxControl object.
It can even be passed to an wxAUI pane.
*/
class wxOgre :
	public wxControl,
	public Ogre::FrameListener,
	public Ogre::Singleton<wxOgre>
{
	DECLARE_CLASS(wxOgre)
public:
	/** A new wxOgre must receive a parent frame to which to attach
	itself to */
	wxOgre (wxFrame* parent);
	~wxOgre();

	/** Renders a single Ogre frame */
	void update();

	/** Returns the currently used camera */
	inline Ogre::Camera* getCamera() const
	{
		return (mCamera);
	}
	/** Sets a new camera for rendering */
	inline void setCamera(Ogre::Camera* camera)
	{
		mCamera = camera;
	}

	inline Ogre::SceneManager* getSceneManager() const
	{
		return (mSceneMgr);
	}
	void setZoomScale(Ogre::Real zoomScale)
	{
		mZoomScale = zoomScale;
	}

	void resetCamera();

	/** Creates an ogre rendering window and all other default objects
	such as the Ogre Root, default camera, default scene manager etc */
	void createOgreRenderWindow();
	void createOgreRenderResources();

	void destroyOgreRenderWindow();

	/** Toggles the rendering timer on/off */
	void toggleTimerRendering();

	/* toggle between wireframe and solid */
	void wireFrame(bool wireframe);

	/* set the camera to track this node */
	void cameraTrackNode(Ogre::SceneNode* target = NULL);

	Ogre::Real TrackballProjectToSphere(Ogre::Real r, Ogre::Real x, Ogre::Real y);


protected:
	DECLARE_EVENT_TABLE()

private:

	class ArcBall
	{
	private:
		Ogre::Vector2 mStartPoint;
		Ogre::Vector3 mStartVector;
		Ogre::Quaternion mStartRotation;
		Ogre::Real mWidth, mHeight;
		const float mRadius;
	public:  
		ArcBall(int width, int height) : mStartVector(0.0f, 0.0f, 1.0f),
										 mWidth((Ogre::Real) width),
										 mHeight((Ogre::Real) height),
										 mRadius(600.0f)

		{
		}

		void SetScreen(int width, int height)
		{
			mWidth = (Ogre::Real) width;
			mHeight =(Ogre::Real) height;			
		}
		
		void StartDrag(Ogre::Vector2 startPoint, Ogre::Quaternion rotation)
		{
			mStartPoint = startPoint;
			mStartVector = MapToSphere(startPoint).normalisedCopy();
			mStartRotation = rotation;
		}

		Ogre::Quaternion Update(Ogre::Vector2 currentPoint)
		{
			Ogre::Vector3 currentVector(MapToSphere(currentPoint).normalisedCopy());
			Ogre::Vector3 axis(mStartVector.crossProduct(currentVector).normalisedCopy());
			Ogre::Radian angle(mStartVector.dotProduct(currentVector));			
			Ogre::Quaternion delta(axis.x, axis.y, axis.z, angle.valueRadians());
			delta.normalise();
			return delta * mStartRotation;
		}

		
		Ogre::Vector3 MapToSphere(Ogre::Vector2 point)
		{
			Ogre::Vector3 result((point.x - (mWidth / 2.0f)) / mWidth,   ((mHeight / 2.0f) - point.y) / mHeight, 0.0f);
			if (result.length() > 1.0f)
			{
				result.normalise();
			} else {
				result.z = Ogre::Math::Sqrt(1.0f - result.length());
			}
			return result;
		}
	};

	/** WX Callbacks */
	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnEraseBackground(wxEraseEvent& event);
	void OnRenderTimer(wxTimerEvent& event);
	void OnMouseDown(wxMouseEvent& event);
	void OnMouseMotion(wxMouseEvent& event);
	void OnMouseWheel(wxMouseEvent& event);

	/** Ogre Callbacks */
	virtual bool frameEnded(const Ogre::FrameEvent& evt);

	/** Rendering timer */
	wxTimer mTimer;
	wxPoint mPrevPos;

	/* Ogre members */
	Ogre::Root* mRoot;
	Ogre::Viewport* mViewPort;
	Ogre::Camera* mCamera;
	Ogre::SceneManager* mSceneMgr;
	Ogre::RenderWindow* mRenderWindow;
	Ogre::SceneNode *mCameraNode;
	Ogre::Real mZoomScale;
	Ogre::Light* mXLight;
	Ogre::Light* mYLight;
	Ogre::Light* mZLight;
	Ogre::SceneNode* mTarget;
	Ogre::Overlay*   mDebugPanelOverlay;

	// used for tracking spherical cam
	Ogre::Real mRadius;
	Ogre::Vector3 mBasis;
	Ogre::Vector2 mDragPos;

	Ogre::Degree mPitch;
	Ogre::Degree mYaw;
public:
	// *****************************************************

	// -----------------------------------------------------
	/**
	@remarks
	Why do we do this? Well, it's because the Singleton
	implementation is in a .h file, which means it gets compiled
	into anybody who includes it. This is needed for the
	Singleton template to work, but we actually only want it
	compiled into the implementation of the class based on the
	Singleton, not all of them. If we don't change this, we get
	link errors when trying to use the Singleton-based class from
	an outside dll.
	@par
	This method just delegates to the template version anyway,
	but the implementation stays in this single compilation unit,
	preventing link errors.
	*/
	static wxOgre& getSingleton();
	/**
	@remarks
	Why do we do this? Well, it's because the Singleton
	implementation is in a .h file, which means it gets compiled
	into anybody who includes it. This is needed for the
	Singleton template to work, but we actually only want it
	compiled into the implementation of the class based on the
	Singleton, not all of them. If we don't change this, we get
	link errors when trying to use the Singleton-based class from
	an outside dll.
	@par
	This method just delegates to the template version anyway,
	but the implementation stays in this single compilation unit,
	preventing link errors.
	*/
	static wxOgre* getSingletonPtr();


};


#endif // __WXOGRE_H__
