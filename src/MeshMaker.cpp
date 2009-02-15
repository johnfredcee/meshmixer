
#include "OgreRoot.h"
#include "OgreSceneManager.h"
#include "OgreManualObject.h"
#include "OgreMaterial.h"
#include "OgreMaterialManager.h"
#include "OgreLog.h"
#include "OgreLogManager.h"
#include "OgreHardwareBuffer.h"
#include "OgreMesh.h"

#include "wxOgre.h"


#include <string>
#include <sstream>

#include <aiMesh.h>
#include <aiMaterial.h>

#include <boost/format.hpp>

#include "MeshMaker.h"

unsigned MeshMaker::mCount = 0;

MeshMaker::MeshMaker() :
mLog(NULL),    
mSceneMgr(NULL)
{

}

MeshMaker::~MeshMaker()
{
}

Ogre::MaterialPtr MeshMaker::createMaterial(int index, aiMaterial* mat)
{ 
	std::ostringstream matname; 

	matname << mName << "_" << "Mat";
	matname.width(4);
	matname.fill('0');
	matname << index;    
	Ogre::String matName = matname.str();

	mLog->logMessage( (boost::format("Creating %s") % matName.c_str()).str() );

	Ogre::MaterialPtr omat = Ogre::MaterialManager::getSingleton().create(matName, "Converted");

	omat->setReceiveShadows(false); 

	omat->getTechnique(0)->getPass(0)->setShadingMode(Ogre::SO_GOURAUD);    
	omat->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
	omat->getTechnique(0)->getPass(0)->setLightingEnabled(true);
	omat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(true);

	// ambient
	aiColor4D clr(0.0f, 0.0f, 0.0f, 1.0);    
	aiGetMaterialColor(mat, AI_MATKEY_COLOR_AMBIENT,  &clr);
	omat->getTechnique(0)->getPass(0)->setAmbient(clr.r, clr.g, clr.b); 

	// diffuse
	clr = aiColor4D(0.0f, 0.0f, 0.0f, 1.0f);    
	aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &clr);
	omat->getTechnique(0)->getPass(0)->setDiffuse(clr.r, clr.g, clr.b, clr.a); 

	// specular
	clr = aiColor4D(0.0f, 0.0f, 0.0f, 1.0f);
	aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &clr);
	omat->getTechnique(0)->getPass(0)->setSpecular(clr.r, clr.g, clr.b, clr.a);

	// emissive
	clr = aiColor4D(0.0f, 0.0f, 0.0f, 1.0f);
	aiGetMaterialColor(mat, AI_MATKEY_COLOR_EMISSIVE, &clr);
	omat->getTechnique(0)->getPass(0)->setSelfIllumination(clr.r, clr.g, clr.b);

	enum aiTextureType type = aiTextureType_DIFFUSE;
	static aiString path[4096];
	aiTextureMapping mapping;    // the mapping (should be uv for now)
	unsigned int uvindex;                 // the texture uv index channel
	float blend;                 // blend
	aiTextureOp op;              // op
	aiTextureMapMode mapmode;    // mapmode
	if (mat->GetTexture(type, index, path, &mapping, &uvindex, &blend, &op, &mapmode) == AI_SUCCESS)
	{
		mLog->logMessage( ( boost::format("Found texture %s for channel %d ") % path % uvindex).str() );
	}
	omat->load();
	return omat;
}


bool MeshMaker::createSubMesh(int index, const aiMesh *m, aiMaterial** mats)
{
	wxASSERT(mLog != NULL);
	wxASSERT(!mMesh.isNull());

	mLog->logMessage( (boost::format("Creating %s") % mName).str() );

	// create the material
	aiMaterial *mat = mats[m->mMaterialIndex];

	Ogre::MaterialPtr matptr( createMaterial(m->mMaterialIndex, mat) );

	// now begin the object definition
	// We create a submesh per material
	Ogre::SubMesh* submesh = mMesh->createSubMesh();

	// prime pointers to vertex related data
	aiVector3D *vec = m->mVertices;
	aiVector3D *norm = m->mNormals;
	aiVector3D *uv = m->mTextureCoords[0];
	aiColor4D *col = m->mColors[0]; 

	// We must create the vertex data, indicating how many vertices there will be
	submesh->useSharedVertices = false;
	submesh->vertexData = new Ogre::VertexData();
	submesh->vertexData->vertexStart = 0;
	submesh->vertexData->vertexCount = m->mNumVertices;

	// We must now declare what the vertex data contains
	Ogre::VertexDeclaration* declaration = submesh->vertexData->vertexDeclaration;
	static const unsigned short source = 0;
	size_t offset = 0;
	offset += declaration->addElement(source,offset,Ogre::VET_FLOAT3,Ogre::VES_POSITION).getSize();

	mLog->logMessage((boost::format(" %d vertices ") % m->mNumVertices).str());
	if (norm)
	{
		mLog->logMessage((boost::format(" %d normals ") % m->mNumVertices).str() );
		offset += declaration->addElement(source,offset,Ogre::VET_FLOAT3,Ogre::VES_NORMAL).getSize();
	}

	if (uv)
	{
		mLog->logMessage((boost::format(" %d uvs ") % m->mNumVertices).str() );
		offset += declaration->addElement(source,offset,Ogre::VET_FLOAT2,Ogre::VES_TEXTURE_COORDINATES).getSize();		
	}

	if (col)
	{
		mLog->logMessage((boost::format(" %d colours ") % m->mNumVertices).str() );
		offset += declaration->addElement(source,offset,Ogre::VET_FLOAT3,Ogre::VES_DIFFUSE).getSize();
	}

	// We create the hardware vertex buffer
	Ogre::HardwareVertexBufferSharedPtr vbuffer =
		Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(declaration->getVertexSize(source), // == offset
		submesh->vertexData->vertexCount,   // == nbVertices
		Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

	// Now we get access to the buffer to fill it.  During so we record the bounding box.
	float* vdata = static_cast<float*>(vbuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD));
	for (size_t i=0;i < m->mNumVertices; ++i)
	{
		// Position
		Ogre::Vector3 position( vec->x, vec->y, vec->z );		
		*vdata++ = vec->x;
		*vdata++ = vec->y;
		*vdata++ = vec->z;
		mAAB.merge(position);
		vec++;

		// Normal
		if (norm)
		{
			*vdata++ = norm->x;
			*vdata++ = norm->y;
			*vdata++ = norm->z;
			norm++;
		}

		// uvs
		if (uv)
		{
			*vdata++ = uv->x;
			*vdata++ = uv->y;
			uv++;
		}

		if (col)
		{
			*vdata++ = col->r;
			*vdata++ = col->g;
			*vdata++ = col->b;
			// here be dragons *vdata++ = col->a;
		}
	}

	vbuffer->unlock();
	submesh->vertexData->vertexBufferBinding->setBinding(source,vbuffer);

	mLog->logMessage((boost::format(" %d faces ") % m->mNumFaces).str() );
	aiFace *f = m->mFaces;

	// Creates the index data
	submesh->indexData->indexStart = 0;
	submesh->indexData->indexCount = m->mNumFaces * 3;
	submesh->indexData->indexBuffer =
		Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(Ogre::HardwareIndexBuffer::IT_16BIT,
		submesh->indexData->indexCount,
		Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
	Ogre::uint16* idata = static_cast<Ogre::uint16*>(submesh->indexData->indexBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD));

	// poke in the face data
	for (size_t i=0; i < m->mNumFaces;++i)
	{
		wxASSERT(f->mNumIndices == 3);
		*idata++ = f->mIndices[0];
		*idata++ = f->mIndices[1];
		*idata++ = f->mIndices[2];						
		f++;
	}
	submesh->indexData->indexBuffer->unlock();

	// Finally we set a material to the submesh
	submesh->setMaterialName(matptr->getName());  

	return true;

}

bool MeshMaker::createMesh()
{
	wxASSERT(mLog != NULL);
	std::ostringstream meshname;    
	meshname << mName;
	meshname.width(4);
	meshname.fill('0');
	meshname << mCount;
	mCount++;
	mName = meshname.str();

	wxASSERT(mMesh.isNull());
	if (mMesh.isNull()) {
		mMesh = Ogre::MeshManager::getSingleton().createManual(mName + ".mesh","Converted");
	}
	mSceneMgr = wxOgre::getSingleton().getSceneManager();
	return true;
}

void MeshMaker::destroy()
{
	mMesh->unload();
	mMesh.setNull(); 
}

Ogre::MeshPtr MeshMaker::finishMesh()
{
	// We must indicate the bounding box
	mMesh->_setBounds(mAAB);  
	mMesh->_setBoundingSphereRadius((mAAB.getMaximum()- mAAB.getMinimum()).length()/2.0);  

	// And we load the mesh
	mMesh->load();

	return mMesh;
}

