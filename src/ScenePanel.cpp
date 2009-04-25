
#include <wx/treectrl.h>
#include <wx/string.h>
#include <wx/strconv.h>
#include <wx/sizer.h>
#include <aiscene.h>

#include "SceneClientData.h"
#include "ScenePanel.h"


wxWindowID SCENE_TREE_ID = wxNewId();

IMPLEMENT_CLASS(ScenePanel, wxPanel);

BEGIN_EVENT_TABLE(ScenePanel, wxPanel)
	EVT_TREE_ITEM_GETTOOLTIP(SCENE_TREE_ID, SceneTooltipSet)
	EVT_TREE_ITEM_ACTIVATED(SCENE_TREE_ID, SelectedNode)
END_EVENT_TABLE()

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

bool ScenePanel::AddChildren(aiNode* node, wxTreeItemId treeItem, bool root)
{
	wxTreeItemId item = treeItem;
	if (!root)
	{
		item = mSceneTree->AppendItem(treeItem, wxString(&node->mName.data[0]), -1, -1, new SceneTreeItemData(node));
	}
	for(int i = node->mNumChildren - 1; i >= 0; i--)
	{
		aiNode *child = node->mChildren[i];
		AddChildren(child, item);
	}
	return true;
}

bool ScenePanel::SetScene(const aiScene* scene)
{
	mSceneTree->Hide();
	mSceneTree->DeleteAllItems();
	aiNode* root = scene->mRootNode;
	wxTreeItemId rootId = mSceneTree->AddRoot(wxString(&root->mName.data[0]), -1, -1, new SceneTreeItemData( root ));
	AddChildren(root, rootId, true);
	mSceneTree->Show();
	return true;	
}


void ScenePanel::SceneTooltipSet(wxTreeEvent& evt)
{
	wxTreeItemId item = evt.GetItem();
	SceneTreeItemData *sceneData = dynamic_cast<SceneTreeItemData*>(mSceneTree->GetItemData(item));
	if (sceneData != NULL)
	{		
		evt.SetToolTip(wxString::Format("%d meshes", sceneData->GetData()->mNumMeshes)); 
	}

	return;
	
}

void ScenePanel::SelectedNode(wxTreeEvent& evt)
{
	wxTreeItemId item = evt.GetItem();
	SceneTreeItemData *sceneData = dynamic_cast<SceneTreeItemData*>(mSceneTree->GetItemData(item));
	if (sceneData != NULL)
	{
		if (sceneData->GetData()->mNumMeshes > 0)
		{
			wxCommandEvent sceneEvent(SceneEventType, GetId());
			SceneClientData* nodeObject = new SceneClientData;
			nodeObject->meshNode = (sceneData->GetData());
			sceneEvent.SetClientObject(nodeObject);
			ProcessEvent(sceneEvent);
		}
	}
	return;
}
