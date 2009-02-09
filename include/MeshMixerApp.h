
#if !defined(H_MESHMIXERAPP)
#define H_MESHMIXERAPP

#include <wx/wx.h>

#include "OgreRoot.h"

using Ogre::Root;

class MeshMixerApp: public wxApp
{
    public:
        virtual ~MeshMixerApp();
        
    private:
        virtual bool OnInit();
        virtual int  OnExit();

        Root *m_root;
        
        
};

IMPLEMENT_APP(MeshMixerApp)

#endif


