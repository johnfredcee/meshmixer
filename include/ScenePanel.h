
#ifndef SCENE_PANEL_H_INCLUDED
#define SCENE_PANEL_H_INCLUDED

#include <aiscene.h>
#include <wx/panel.h>

class wxTreeCtrl;
class wxFlexGridSizer;
class wxPanel;
struct aiScene;

class ScenePanel : public wxPanel
{
    DECLARE_CLASS(ScenePanel);
 public:
	class SceneTreeItemData : public wxTreeItemData
	{
	public:
		SceneTreeItemData(aiNode* node) : mNode(node)
		{

		}

		aiNode* GetData()
		{
			return mNode;
		}

	private:
		aiNode* mNode;
	};

	ScenePanel(wxWindow* parent, wxWindowID id = wxID_ANY,
                 const wxPoint& pos = wxDefaultPosition, 
		 const wxSize& size = wxDefaultSize,
                 long style = wxTAB_TRAVERSAL, 
		 const wxString& name = wxT("ScenePanel"));
    virtual ~ScenePanel();
    bool SetScene(const aiScene*);
 private:
    wxTreeCtrl *mSceneTree;
	bool AddChildren(aiNode* node, wxTreeItemId treeItem);
};

#endif
