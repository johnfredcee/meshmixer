
#if !defined(H_MESHMAKER)
#define H_MESHMAKER

struct aiScene;
struct aiMaterial;

namespace Ogre {
    class ManualObject;
    class SceneManager;
}

class MeshMaker
{

    public:
        MeshMaker(Ogre::SceneManager* sceneMgr);
        ~MeshMaker();
        bool create(const aiMesh *sc, aiMaterial **mat);
        void destroy();
        Ogre::MeshPtr convertToMesh(const Ogre::String& name);
        Ogre::MeshPtr getMesh();
        void setName(const std::string& name)	{ mName = name;	}
    private:
        Ogre::MaterialPtr createMaterial(aiMaterial* mat);        
        Ogre::SceneManager *mSceneMgr;
        Ogre::MeshPtr mMesh;
        Ogre::String mName;
        Ogre::AxisAlignedBox mAAB;        
        static unsigned mCount;
};

#endif

