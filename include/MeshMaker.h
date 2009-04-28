
#if !defined(H_MESHMAKER)
#define H_MESHMAKER

struct aiScene;
struct aiMaterial;

namespace Ogre {
    class ManualObject;
    class SceneManager;
    class Log;
}

class MeshMaker
{
    public:
        MeshMaker();
        ~MeshMaker();
		Ogre::MeshPtr createMesh(const aiScene* scene, aiNode *node);
        bool createSubMesh(int index, const aiMesh *sc, aiMaterial **mat);
        void destroy();
        void setName(const std::string& name)	{ mName = name;	}
        void setDirectory(const std::string& dir)	{ mDir = dir;	}
        void setLog(Ogre::Log *olog) { mLog = olog;  }
   	    Ogre::MeshPtr getMesh(void) { return mMesh; };
	    Ogre::MeshPtr finishMesh();

    private:
        Ogre::Log *mLog;
        Ogre::MaterialPtr createMaterial(int index, aiMaterial* mat);        
        Ogre::SceneManager *mSceneMgr;
        Ogre::MeshPtr mMesh;
        Ogre::String mName;
	    Ogre::String mDir;
        Ogre::AxisAlignedBox mAAB;        
        static unsigned mCount;
};

#endif


