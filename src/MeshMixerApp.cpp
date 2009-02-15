
#include "MeshMixerApp.h"

#include <wx/splash.h>

#include "OgreMaterialManager.h"
#include "OgreConfigFile.h"
#include "OgreLog.h"
#include "OgreLogManager.h"
#include "OgreResourceManager.h"


#include "MeshMixerFrame.h"

using Ogre::ConfigFile;
using Ogre::LogManager;
using Ogre::ResourceGroupManager;

MeshMixerApp::~MeshMixerApp()
{
}

bool MeshMixerApp::OnInit()
{
	wxInitAllImageHandlers();

#ifdef SHOW_SPLASH_SCREEN
	wxBitmap bitmap;
	if(bitmap.LoadFile(wxT("splash.png"), wxBITMAP_TYPE_PNG))
	{
		wxSplashScreen* splash = new wxSplashScreen(bitmap, wxSPLASH_CENTRE_ON_SCREEN | wxSPLASH_TIMEOUT,
			2000, NULL, -1, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER | wxSTAY_ON_TOP);
	}
#endif

    MeshMixerFrame* mainFrame = new MeshMixerFrame(NULL, wxID_ANY, wxT("Mesh Mixer"), wxDefaultPosition,  wxSize(800, 600));
    SetTopWindow(mainFrame);
    mainFrame->Show();    
#ifdef __unix__
	// when using gtk we want to delay the first render event until the window is shown
	// because that is when a gtk window is created
    mainFrame->createOgreRenderWindow();
#endif
    mainFrame->updateOgre();
	return true;
}

int MeshMixerApp::OnExit()
{
	return 0;
}


