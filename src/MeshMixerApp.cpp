
#include "MeshMixerApp.h"

#include <wx/splash.h>

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

#if 0
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

// for gtk we need to do this here *after* pane creation, not before (widget needs to be realized).
#ifdef __unix__    
    mainFrame->createOgreRenderWindow();
#endif

// fuck. I'm a moron.
    //ResourceGroupManager::getSingleton().addResourceLocation("../../../resources/packs/OgreCore.zip", "Zip", "Bootstrap");    
    //ResourceGroupManager::getSingleton().addResourceLocation("../../../media", "FileSystem", "General");    
    //ResourceGroupManager::getSingleton().addResourceLocation("../../../media/fonts", "FileSystem", "General");    
    //ResourceGroupManager::getSingleton().addResourceLocation("../../../media/sounds", "FileSystem", "General");    
    //ResourceGroupManager::getSingleton().addResourceLocation("../../../media/models", "FileSystem", "General");    
    //ResourceGroupManager::getSingleton().addResourceLocation("../../../media/materials", "FileSystem", "General");
    //ResourceGroupManager::getSingleton().addResourceLocation("../../../media/scripts", "FileSystem", "General");        
    //ResourceGroupManager::getSingleton().addResourceLocation("../../../media/textures", "FileSystem", "General");    
    //ResourceGroupManager::getSingleton().addResourceLocation("../../../media/overlays", "FileSystem", "General");    
    //ResourceGroupManager::getSingleton().addResourceLocation("../../../media/particle", "FileSystem", "General");    
    
    mainFrame->updateOgre();
	return true;
}

int MeshMixerApp::OnExit()
{

	return 0;
}


