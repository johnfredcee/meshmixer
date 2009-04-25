#ifndef SCENECLIENTDATA_H_INCLUDED
#define SCENECLIENTDATA_H_INCLUDED

#include <wx/event.h>
#include <wx/clntdata.h>

#include <assimp.h>
#include <aiScene.h>

extern const wxEventType SceneEventType;

class SceneClientData : public wxClientData
{	
public:
	aiNode  *meshNode;
};

#endif
