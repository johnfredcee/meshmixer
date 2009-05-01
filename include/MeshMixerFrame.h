#ifndef MESHMIXERFRAME_H
#define MESHMIXERFRAME_H

#include <wx/wx.h>
#include <wx/image.h>


#include "OgreRenderSystem.h"
#include "OgreRoot.h"
#include "OgreMesh.h"

class wxAuiManager;
class wxAuiNotebook;
class wxNotebook;
class wxPropertyGridManager;
class wxTreeCtrl;

struct aiScene;

namespace Ogre
{
    class Entity;
    class SceneNode;
    class RenderSystem;
    class Root;
}

class DocPanel;
class EventArgs;
class LogPanel;
class OptionsPanel;
class ScenePanel;
class PropertiesPanel;
class ResourcePanel;
class WorkspacePanel;
class wxOgre;
class MeshMaker;
class MeshMixerApp;


class MeshMixerFrame: public wxFrame
{
public:

    MeshMixerFrame(wxWindow* parent, int id, const wxString& title,
				   const wxPoint& pos=wxDefaultPosition,
				   const wxSize& size=wxDefaultSize,
				   long style=wxDEFAULT_FRAME_STYLE);

    friend class MeshMixerApp;
        
protected:
    void createFileMenu();
    void createViewMenu();        
    void createMenuBar();        
    void createAuiManager(void);
    bool createOgrePane(void);
    void createInformationPane(void);
    void createOptionsPane(void);
    void createOgreRenderWindow(void);
    void updateOgre(void);

    void OnFileNew(wxCommandEvent& event);
    void OnFileOpen(wxCommandEvent& event);
    void OnFileSave(wxCommandEvent& event);
    void OnFileSaveAs(wxCommandEvent& event);
    void OnFileClose(wxCommandEvent& event);
    void OnFileExit(wxCommandEvent& event);
    void OnViewFreeCamera(wxCommandEvent& event);
    void OnViewWireframe(wxCommandEvent& event);
	void OnSceneChange(wxCommandEvent& event);

    wxMenuBar* mMenuBar;
    wxMenu* mFileMenu;        
    wxMenu* mViewMenu;        
    wxAuiNotebook* mInformationNotebook;
    wxAuiNotebook* mOptionsNotebook;
    LogPanel *mLogPanel;
    LogPanel *mImportLogPanel;
    OptionsPanel* mOptionsPanel;
    ScenePanel *mScenePanel;	

    wxAuiManager *mAuiManager;
        
    wxOgre* mOgreControl;
        
    Ogre::Root* mRoot;
    Ogre::Entity* mEntity;
    Ogre::SceneNode *mMeshNode;
	Ogre::MeshPtr mMesh;

    const aiScene *mScene;

	wxString mWorkingDir;

#ifndef __unix__
    Ogre::RenderSystem* mDirectXRenderSystem;
#endif
    Ogre::RenderSystem* mOpenGLRenderSystem;
        
    DECLARE_EVENT_TABLE();
        
};


#endif // MESHMIXERFRAME_H
