// ============================================================================
// Source:
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
// ============================================================================

#ifndef			THREEDS_HH
# define		THREEDS_HH

#include		<vector>
#include		"vector3d.hh"

//>------ Primary Chunk, at the beginning of each file
#define PRIMARY		0x4D4D

//>------ Main Chunks
#define OBJECTINFO	0x3D3D		// Version of the mesh
#define VERSION_3DS	0x0002		// Version of the .3ds file
#define EDITKEYFRAME	0xB000		// Header for all of the key frame info

//>------ sub defines of OBJECTINFO
#define MATERIAL	0xAFFF	// Stored the texture info
#define OBJECT		0x4000	// Faces, vertices, etc...

//>------ sub defines of MATERIAL
#define MATNAME		0xA000		// Material name
#define MATDIFFUSE	0xA020		// Color of the object/material
#define MATMAP		0xA200		// Header for a new material
#define MATMAPFILE	0xA300		// Texture filename

#define OBJECT_MESH	0x4100		// Reading a new object

//>------ sub defines of OBJECT_MESH
#define OBJECT_VERTICES	0x4110		// Vertices
#define OBJECT_FACES	0x4120		// Faces
#define OBJECT_MATERIAL	0x4130		// Found if the object has a material
#define OBJECT_UV	0x4140		// UV texture coordinates


// Used to store the UV coordinates.
class CVector2
{
public:
  float x, y;
};

// This is our face structure.  This is is used for indexing into the
// vertex and texture coordinate arrays.  From this information we
// know which vertices from our vertex array go to which face, along
// with the correct texture coordinates.
struct tFace
{
  int vertIndex[3];
  int coordIndex[3];	// Texture coords
};

// This holds the information for a material.  It may be a texture map
// of a color.  Some of these are not used, but I left them because
// you will want to eventually read in the UV tile ratio and the UV
// tile offset for some models.
struct tMaterialInfo
{
  char strName[255];	// Texture name
  char strFile[255];	// Texture filename
  unsigned char color[3];
  int texureId;
  float uTile;		// Currently not used
  float vTile;		// Currently not used
  float uOffset;	// Currently not used
  float vOffset;	// Currently not used
} ;

// This holds all the information for our model/scene.
// You should eventually turn into a robust class that
// has loading/drawing/querying functions like:
// LoadModel(...); DrawObject(...); DrawModel(...); DestroyModel(...);
struct t3DObject
{
  int numOfVerts;	// Number of verts in the model
  int numOfFaces;	// Number of faces in the model
  int numTexVertex;	// Number of texture coordinates
  int materialID;	// Texture ID to use, index into our texture array
  bool bHasTexture;	// True if there is a texture map for this object
  char strName[255];	// Name of the object
  Vector3D  *pVerts;	// Vertices
  Vector3D  *pNormals;	// Normals
  CVector2  *pTexVerts;	// Texture UV coordinates
  tFace *pFaces;	// Faces information of the object
};

// This holds our model information.  This should also turn into a
// robust class.  We use STL's (Standard Template Library) vector
// class to ease our link list burdens. :)
struct t3DModel
{
  int numOfObjects;	// Number of objects in the model
  int numOfMaterials;	// Number of materials for the model
  std::vector<tMaterialInfo> pMaterials;	// List of material information (Textures and colors)
  std::vector<t3DObject> pObject;	// Object list for our model
};



// Structure for 3DS indicies
struct tIndices
{
  unsigned short a;
  unsigned short b;
  unsigned short c;
  unsigned short bVisible;		// visible flag
};

// Chunk info
struct tChunk
{
  unsigned short int ID;
  unsigned int length;
  unsigned int bytesRead;
};

// This class handles all of the loading code
class			CLoad3DS
{
public:
  CLoad3DS ();

  bool		Import3DS (t3DModel *pModel, char * strFileName);

private:
  // This reads in a string and saves it in the char array passed in
  int		GetString (char *);

  // This reads the next chunk
  void		ReadChunk (tChunk *);

  void		ProcessNextChunk (t3DModel *, tChunk *);
  void		ProcessNextObjectChunk (t3DModel *, t3DObject *, tChunk *);
  void		ProcessNextMaterialChunk (t3DModel *, tChunk *);

  void		ReadColorChunk (tMaterialInfo *, tChunk *);
  void		ReadVertices (t3DObject *, tChunk *);
  void		ReadVertexIndices (t3DObject *, tChunk *);
  void		ReadUVCoordinates (t3DObject *, tChunk *);
  void		ReadObjectMaterial (t3DModel *, t3DObject *, tChunk *);

  void		ComputeNormals (t3DModel *);

  // This frees memory and closes the file
  void		CleanUp ();

  FILE *	m_FilePointer;

  // These are used through the loading process to hold the
  // chunk information
  tChunk *	m_CurrentChunk;
  tChunk *	m_TempChunk;
};

#endif			// THREEDS_HH
