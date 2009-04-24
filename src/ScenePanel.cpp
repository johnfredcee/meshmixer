
#include <wx/treectrl.h>
#include <wx/string.h>
#include <wx/strconv.h>
#include <wx/sizer.h>
#include <aiscene.h>

#include "ScenePanel.h"

IMPLEMENT_CLASS(ScenePanel, wxPanel);

wxWindowID SCENE_TREE_ID = wxNewId();

ScenePanel::ScenePanel(wxWindow* parent, wxWindowID id,
		       const wxPoint& pos, 
		       const wxSize& size,
		       long style, 
			   const wxString& name) : wxPanel(parent, id, pos, size, style, name)
{
	wxBoxSizer* mSizer = new wxBoxSizer(wxVERTICAL);
	mSceneTree = new wxTreeCtrl(this, SCENE_TREE_ID);
	mSizer->Add(mSceneTree, 1, wxEXPAND);
	SetSizer(mSizer);
	Layout();
}

ScenePanel::~ScenePanel()
{
}

bool ScenePanel::AddChildren(aiNode* node, wxTreeItemId treeItem)
{
	for(int i = node->mNumChildren - 1; i > 0; i--)
	{
		aiNode *child = node->mChildren[i];
		wxTreeItemId item = mSceneTree->AppendItem(treeItem, wxString(&child->mName.data[0], wxConvUTF8), -1, -1, new SceneTreeItemData(child));
		AddChildren(child, item);
	}
	return true;
}

bool ScenePanel::SetScene(const aiScene* scene)
{
	mSceneTree->Hide();
	mSceneTree->DeleteAllItems();
	aiNode* root = scene->mRootNode;
	wxTreeItemId rootId = mSceneTree->AddRoot(wxString(&root->mName.data[0],wxConvUTF8), -1, -1, new SceneTreeItemData( root ));
	AddChildren(root, rootId);
	mSceneTree->Show();
	return true;	
}
