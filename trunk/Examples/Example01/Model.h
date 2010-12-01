#pragma once

//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Misc.h"


//+-----------------------------------------------------------------------------
//| Pre-declared classes
//+-----------------------------------------------------------------------------
class MODEL_BASE;
class MODEL_BONE;
class MODEL_GEOSET;
class MODEL_GEOSET_ANIMATION;
class MODEL_GEOSET_VERTEX;
class MODEL_GEOSET_FACE;
class MODEL_GEOSET_GROUP;
class MODEL_TEXTURE;
class MODEL_TEXTURE_ANIMATION;
class MODEL_MATERIAL;
class MODEL_MATERIAL_LAYER;
class MODEL_SEQUENCE;
class MODEL_GLOBAL_SEQUENCE;
class MODEL_LIGHT;
class MODEL_HELPER;
class MODEL_CAMERA;
class MODEL_ATTACHMENT;
class MODEL_EVENT_OBJECT;
class MODEL_COLLISION_SHAPE;
class MODEL_PARTICLE_EMITTER;
class MODEL_PARTICLE_EMITTER_2;
class MODEL_RIBBON_EMITTER;


//+-----------------------------------------------------------------------------
//| Model info data structure
//+-----------------------------------------------------------------------------
struct MODEL_INFO_DATA
{
	MODEL_INFO_DATA()
	{
		Version = MODEL_DEFAULT_VERSION;
		Name = "Name";
		AnimationFile = "";

		BlendTime = 150;
	}

	DWORD Version;
	std::string Name;
	std::string AnimationFile;

	EXTENT Extent;
	INT BlendTime;
};


//+-----------------------------------------------------------------------------
//| Model data structure
//+-----------------------------------------------------------------------------
struct MODEL_DATA
{
	MODEL_DATA()
	{
		//Empty
	}

	MODEL_INFO_DATA Info;

	SIMPLE_CONTAINER<MODEL_BASE*> BaseContainer;

	SIMPLE_CONTAINER<MODEL_ATTACHMENT*> AttachmentContainer;
	SIMPLE_CONTAINER<MODEL_BONE*> BoneContainer;
	SIMPLE_CONTAINER<MODEL_COLLISION_SHAPE*> CollisionShapeContainer;
	SIMPLE_CONTAINER<MODEL_EVENT_OBJECT*> EventObjectContainer;
	SIMPLE_CONTAINER<MODEL_HELPER*> HelperContainer;
	SIMPLE_CONTAINER<MODEL_LIGHT*> LightContainer;
	SIMPLE_CONTAINER<MODEL_PARTICLE_EMITTER*> ParticleEmitterContainer;
	SIMPLE_CONTAINER<MODEL_PARTICLE_EMITTER_2*> ParticleEmitter2Container;
	SIMPLE_CONTAINER<MODEL_RIBBON_EMITTER*> RibbonEmitterContainer;

	SIMPLE_CONTAINER<MODEL_CAMERA*> CameraContainer;
	SIMPLE_CONTAINER<MODEL_GEOSET*> GeosetContainer;
	SIMPLE_CONTAINER<MODEL_GEOSET_ANIMATION*> GeosetAnimationContainer;
	SIMPLE_CONTAINER<MODEL_GLOBAL_SEQUENCE*> GlobalSequenceContainer;
	SIMPLE_CONTAINER<MODEL_MATERIAL*> MaterialContainer;
	SIMPLE_CONTAINER<VECTOR3*> PivotPointContainer;
	SIMPLE_CONTAINER<MODEL_SEQUENCE*> SequenceContainer;
	SIMPLE_CONTAINER<MODEL_TEXTURE*> TextureContainer;
	SIMPLE_CONTAINER<MODEL_TEXTURE_ANIMATION*> TextureAnimationContainer;
};



//+-----------------------------------------------------------------------------
//| Model class
//+-----------------------------------------------------------------------------
class MODEL
{
public:
	CONSTRUCTOR MODEL();
	DESTRUCTOR ~MODEL();
	
	VOID Clear();

	MODEL_DATA& Data();

	BOOL AddSequence(MODEL_SEQUENCE* Sequence);
	BOOL AddGlobalSequence(MODEL_GLOBAL_SEQUENCE* GlobalSequence);
	BOOL AddTexture(MODEL_TEXTURE* Texture);

protected:

	MODEL_DATA ModelData;
	MODEL_BASE* Root;

	FLOAT BoundsRadius;
	VECTOR3 BoundsCenter;

	SEQUENCE_TIME AnimationTime;

	REFERENCE<MODEL*, MODEL_SEQUENCE*> SequenceNode;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern MODEL Model;


//+-----------------------------------------------------------------------------
//| Post-included files
//+-----------------------------------------------------------------------------
#include "Graphics.h"
#include "ModelSequence.h"
#include "ModelGlobalSequence.h"
#include "ModelTexture.h"
