
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
	Ogre::MaterialPtr createMaterial(aiMaterial* mat);
	Ogre::MeshPtr getMesh();
	void setName(const std::string& name) 
	{ 
		mName = name;
	}
private:
	Ogre::AxisAlignedBox mAAB;
	Ogre::MeshPtr mMesh;
	Ogre::SceneManager *mSceneMgr;
	Ogre::String mName;
	static unsigned mCount;
};

#endif											\
 
