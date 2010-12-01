#pragma once

//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------


//+-----------------------------------------------------------------------------
//| Pre-declared classes
//+-----------------------------------------------------------------------------
class MODEL_GLOBAL_SEQUENCE;


//+-----------------------------------------------------------------------------
//| Interpolator type enumeration
//+-----------------------------------------------------------------------------
enum INTERPOLATOR_TYPE
{
	INTERPOLATOR_TYPE_NONE,
	INTERPOLATOR_TYPE_SCALAR,
	INTERPOLATOR_TYPE_SCALAR_INT,
	INTERPOLATOR_TYPE_VECTOR2,
	INTERPOLATOR_TYPE_VECTOR3,
	INTERPOLATOR_TYPE_VECTOR4,
};


//+-----------------------------------------------------------------------------
//| Interpolation type enumeration
//+-----------------------------------------------------------------------------
enum INTERPOLATION_TYPE
{
	INTERPOLATION_TYPE_NONE,
	INTERPOLATION_TYPE_LINEAR,
	INTERPOLATION_TYPE_HERMITE,
	INTERPOLATION_TYPE_BEZIER,
};


//+-----------------------------------------------------------------------------
//| Interpolator node structure
//+-----------------------------------------------------------------------------
struct INTERPOLATOR_NODE
{
	INTERPOLATOR_NODE()
	{
		Time = 0;
		Vector = VECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
		InTan = VECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
		OutTan = VECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	INTERPOLATOR_NODE(INT NewTime, CONST VECTOR4& NewVector)
	{
		Time = 0;
		Vector = VECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
		InTan = VECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
		OutTan = VECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	INT Time;
	VECTOR4 Vector;
	VECTOR4 InTan;
	VECTOR4 OutTan;
};


//+-----------------------------------------------------------------------------
//| Interpolator class
//+-----------------------------------------------------------------------------
class INTERPOLATOR
{
public:
	CONSTRUCTOR INTERPOLATOR();
	CONSTRUCTOR INTERPOLATOR(CONST INTERPOLATOR& CopyObject);
	DESTRUCTOR ~INTERPOLATOR();

	CONST INTERPOLATOR& operator =(CONST INTERPOLATOR& CopyObject);

	VOID Clear();
	INT GetSize();

	BOOL IsStatic();
	INT GetNrOfNodes();

	VOID AddNode(CONST INTERPOLATOR_NODE& Node);

	VOID SetType(INTERPOLATOR_TYPE NewType);
	INTERPOLATOR_TYPE GetType();

	VOID SetInterpolationType(INTERPOLATION_TYPE NewInterpolationType);
	INTERPOLATION_TYPE GetInterpolationType();

	VOID SetGolbalSequenceId(INT NewGlobalSequenceId);
	INT GetGlobalSequenceId();


	FLOAT GetScalar(CONST SEQUENCE_TIME& Time = SEQUENCE_TIME());
	D3DXVECTOR2 GetVector2(CONST SEQUENCE_TIME& Time = SEQUENCE_TIME());
	D3DXVECTOR3 GetVector3(CONST SEQUENCE_TIME& Time = SEQUENCE_TIME());
	D3DXVECTOR4 GetVector4(CONST SEQUENCE_TIME& Time = SEQUENCE_TIME());

	VOID SetStaticScalar(FLOAT NewScalar, CONST std::string& NewName = "");
	VOID SetStaticScalarInt(INT NewScalar, CONST std::string& NewName = "");
	VOID SetStaticVector2(CONST D3DXVECTOR2& NewVector2, CONST std::string& NewName = "");
	VOID SetStaticVector3(CONST D3DXVECTOR3& NewVector3, CONST std::string& NewName = "");
	VOID SetStaticVector4(CONST D3DXVECTOR4& NewVector4, CONST std::string& NewName = "");

protected:
	VOID GetInterpolatedValue(D3DXVECTOR4& Vector, CONST SEQUENCE_TIME& Time);
	VOID GetQuaternionValue(D3DXVECTOR4& Vector, CONST SEQUENCE_TIME& Time);
	VOID GetSurroundingNodes(CONST SEQUENCE_TIME& Time, std::list<INTERPOLATOR_NODE>::iterator& Node1, std::list<INTERPOLATOR_NODE>::iterator& Node2);

	std::list<INTERPOLATOR_NODE> NodeList;

	std::string Name;
	INT GlobalSequenceId;
	INTERPOLATOR_TYPE Type;
	INTERPOLATION_TYPE InterpolationType;

	BOOL Static;
	VECTOR4 StaticVector;
	VECTOR4 DefaultVector;

	
};