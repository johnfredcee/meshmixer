
#include "OgreRoot.h"
#include "OgreSceneManager.h"
#include "OgreManualObject.h"
#include "OgreMaterial.h"
#include "OgreMaterialManager.h"
#include "OgreMeshManager.h"
#include "OgreLog.h"
#include "OgreLogManager.h"

#include "wxOgre.h"


#include <string>
#include <sstream>

#include <aiMesh.h>
#include <aiMaterial.h>

#include <boost/format.hpp>

#include "MeshMaker.h"

unsigned MeshMaker::mCount = 0;

MeshMaker::MeshMaker(Ogre::SceneManager *sceneMgr)
{
    // create the manual object
    mManual = NULL; 
    mSceneMgr = sceneMgr;
    
}

MeshMaker::~MeshMaker()
{
}


bool MeshMaker::create(const aiMesh *m, aiMaterial** mats)
{
    static unsigned count = 0;

    std::ostringstream meshname;    
    meshname << "Mesh";
    meshname.width(4);
    meshname.fill('0');
    meshname << mCount;
    mCount++;
    mName = meshname.str();

    Ogre::Log *olog = Ogre::LogManager::getSingletonPtr()->getDefaultLog();    
    olog->logMessage( (boost::format("Creating %s") % mName).str() );

    std::ostringstream matname;    
    matname << "Material";
    matname.width(4);
    matname.fill('0');
    matname << count;
    count++;
    Ogre::String matName = matname.str();
    
    // create the material
    aiMaterial *mat = mats[m->mMaterialIndex];
    aiMaterialProperty **matprops = mat->mProperties;

    olog->logMessage( (boost::format("Creating %s") % matName).str() );

    Ogre::MaterialPtr omat =
        Ogre::MaterialManager::getSingleton().create(matName,
                                                     Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    omat->setReceiveShadows(false); 
    omat->getTechnique(0)->setLightingEnabled(true); 

    // ambient
    aiColor4D clr(0.0f, 0.0f, 0.0f, 1.0);    
    aiGetMaterialColor(mat, AI_MATKEY_COLOR_AMBIENT,  &clr);
    float r = clr.r; float g = clr.g; float b = clr.b; float a = clr.a;
    olog->logMessage( ( boost::format("Ambient %8.4f %8.4f %8.4f %8.4f") % r % g % b % a).str() );        

    omat->getTechnique(0)->getPass(0)->setAmbient(clr.r, clr.g, clr.b); 
    
    // diffuse
    clr = aiColor4D(0.0f, 0.0f, 0.0f, 1.0f);    
    aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &clr);
    r = clr.r;  g = clr.g;  b = clr.b;  a = clr.a;
    olog->logMessage( ( boost::format("Diffuse %8.4f %8.4f %8.4f %8.4f") % r % g % b % a).str() );
    omat->getTechnique(0)->getPass(0)->setDiffuse(r, g, b, a); 

    // specular
    //clr = aiColor4D(0.0f, 0.0f, 0.0f, 1.0f);
    //aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &clr);
//     r = clr.r;  g = clr.g;  b = clr.b;  a = clr.a;
//     olog->logMessage( ( boost::format("Specular %8.4f %8.4f %8.4f %8.4f") % r % g % b % a).str() );
//     omat->getTechnique(0)->getPass(0)->setSpecular(clr.r, clr.g, clr.b, clr.a);

    // emissive
    //clr = aiColor4D(0.0f, 0.0f, 0.0f, 1.0f);
    //aiGetMaterialColor(mat, AI_MATKEY_COLOR_EMISSIVE, &clr);
    //r = clr.r;  g = clr.g;  b = clr.b;  a = clr.a;

    //olog->logMessage( ( boost::format("Emissive %8.4f %8.4f %8.4f %8.4f") % r % g % b % a).str() );    
    //omat->getTechnique(0)->getPass(0)->setSelfIllumination(clr.r, clr.g, clr.b);

    omat->getTechnique(0)->getPass(0)->setShadingMode(Ogre::SO_GOURAUD);    
    omat->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
    omat->getTechnique(0)->getPass(0)->setLightingEnabled(true);
    omat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(true);

    if (mManual == NULL) {
        mManual = mSceneMgr->createManualObject(mName);
        mManual->setDynamic(false);
    }

    // now begin the object definition
    mManual->begin(matName, Ogre::RenderOperation::OT_TRIANGLE_LIST);

    aiVector3D *vec = m->mVertices;
    aiVector3D *norm = m->mNormals;
    aiVector3D *uv = m->mTextureCoords[0];
    aiColor4D *col = m->mColors[0]; 

    olog->logMessage((boost::format(" %d vertices ") % m->mNumVertices).str() );

    if (norm)
    {
        olog->logMessage((boost::format(" %d normals ") % m->mNumVertices).str() );
    }

    if (uv)
    {
        olog->logMessage((boost::format(" %d uvs ") % m->mNumVertices).str() );
    }

    if (col)
    {
        olog->logMessage((boost::format(" %d colours ") % m->mNumVertices).str() );
    }

    for(std::size_t i = 0; i < m->mNumVertices; i++)
    {
        mManual->position(Ogre::Vector3( vec->x, vec->y, vec->z ));
        vec++;

        if (norm) {
            mManual->normal(norm->x, norm->y, norm->z);
            //float x,y,z; x = norm->x; y = norm->y; z = norm->z;
            //olog->logMessage( ( boost::format("Normal %d %8.4f %8.4f %8.4f") % i % x % y % z).str() );            
            norm++;
        }

        if (uv) {
            mManual->textureCoord(uv->x, uv->y);
            uv++;
        }

        if (col) {
            mManual->colour(col->r, col->g, col->b, col->a);
            col++;
        }

    }

    olog->logMessage((boost::format(" %d faces ") % m->mNumFaces).str() );
    aiFace *f = m->mFaces;
    for(std::size_t i = 0; i < m->mNumFaces; i++)
    {        
        if(f->mNumIndices == 4) {
            mManual->quad(f->mIndices[0], f->mIndices[1], f->mIndices[2], f->mIndices[3]);
        } else
        if(f->mNumIndices == 3) {
            mManual->triangle(f->mIndices[0], f->mIndices[1], f->mIndices[2]);
        } else {
            olog->logMessage((boost::format("Face:%d (?) %d verts ") % i % f->mNumIndices).str());                          
        }
        f++;
    }
    
    mManual->end();


    return true;
    
}

void MeshMaker::destroy()
{
    wxASSERT(mManual != NULL);
    mSceneMgr->destroyManualObject(mManual);
    mManual = NULL;
}
        
Ogre::ManualObject* MeshMaker::getManualMesh()
{
    return mManual;
}

Ogre::MeshPtr MeshMaker::convertToMesh(const Ogre::String& name)
{
    if ((mMesh.isNull()) && (mManual != NULL))
    {
        mMesh = mManual->convertToMesh(name + ".mesh", "Converted") ;
    }
    return mMesh;
}
