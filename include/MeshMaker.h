
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
        Ogre::ManualObject* getManualMesh();
        Ogre::MeshPtr convertToMesh(const Ogre::String& name);

    private:
        Ogre::SceneManager *mSceneMgr;
        Ogre::ManualObject* mManual;
        Ogre::MeshPtr mMesh;
        Ogre::String mName;
        
        static unsigned mCount;
};

#endif

