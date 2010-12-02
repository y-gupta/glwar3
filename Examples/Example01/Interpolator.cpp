//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Interpolator.h"
#include "Model.h"


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
REFERENCE_OBJECT<INTERPOLATOR*, INTERPOLATOR*> INTERPOLATOR::InterpolatorNodes;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
INTERPOLATOR::INTERPOLATOR()
{

}


//+-----------------------------------------------------------------------------
//| Copy constructor
//+-----------------------------------------------------------------------------
INTERPOLATOR::INTERPOLATOR(CONST INTERPOLATOR &CopyObject)
{

}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
INTERPOLATOR::~INTERPOLATOR()
{

}


//+-----------------------------------------------------------------------------
//| Assignment operator
//+-----------------------------------------------------------------------------
CONST INTERPOLATOR& INTERPOLATOR::operator =(CONST INTERPOLATOR& CopyObject)
{
	NodeList = CopyObject.NodeList;

	GlobalSequenceId = CopyObject.GlobalSequenceId;
	Type = CopyObject.Type;
	InterpolationType = CopyObject.InterpolationType;

	Static = CopyObject.Static;
	StaticVector = CopyObject.StaticVector;
	DefaultVector = CopyObject.DefaultVector;

	//GlobalSequenceNode = CopyObject.GlobalSequenceNode;
	//InterpolatorNode = CopyObject.InterpolatorNode;

	return (*this);
}


//+-----------------------------------------------------------------------------
//| Clears the interpolator
//+-----------------------------------------------------------------------------
VOID INTERPOLATOR::Clear()
{
	NodeList.clear();

	Type = INTERPOLATOR_TYPE_NONE;
	InterpolationType = INTERPOLATION_TYPE_NONE;

	Static = TRUE;
	StaticVector = VECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
}


//+-----------------------------------------------------------------------------
//| Returns the mdx size of the interpolator
//+-----------------------------------------------------------------------------
INT INTERPOLATOR::GetSize()
{
	INT NodeSize = 0;
	INT Factor = 1;

	if (GetNrOfNodes() <= 0) return 0;

	switch (InterpolationType)
	{
	case INTERPOLATION_TYPE_HERMITE:
	case INTERPOLATION_TYPE_BEZIER:
		Factor = 3;
		break;
	}

	switch (Type)
	{
	case INTERPOLATOR_TYPE_SCALAR:
	case INTERPOLATOR_TYPE_SCALAR_INT:
		{
			NodeSize = (Factor * 4) + 4;
			break;
		}

	case INTERPOLATOR_TYPE_VECTOR2:
		{
			NodeSize = (Factor * 8) + 4;
			break;
		}

	case INTERPOLATOR_TYPE_VECTOR3:
		{
			NodeSize = (Factor * 12) + 4;
			break;
		}

	case INTERPOLATOR_TYPE_VECTOR4:
		{
			NodeSize = (Factor * 16) + 4;
			break;
		}
	}

	NodeSize *= GetNrOfNodes();

	return NodeSize + 16;
}


//+-----------------------------------------------------------------------------
//| Checks if the interpolator is static
//+-----------------------------------------------------------------------------
BOOL INTERPOLATOR::IsStatic()
{
	return Static;
}


//+-----------------------------------------------------------------------------
//| Returns the nr of nodes
//+-----------------------------------------------------------------------------
INT INTERPOLATOR::GetNrOfNodes()
{
	return static_cast<INT>(NodeList.size());
}


//+-----------------------------------------------------------------------------
//| Adds a new node to the interpolator
//+-----------------------------------------------------------------------------
VOID INTERPOLATOR::AddNode(CONST INTERPOLATOR_NODE& Node)
{
	NodeList.push_back(Node);
	Static = FALSE;
}


//+-----------------------------------------------------------------------------
//| Sets a new type
//+-----------------------------------------------------------------------------
VOID INTERPOLATOR::SetType(INTERPOLATOR_TYPE NewType)
{
	Type = NewType;
}


//+-----------------------------------------------------------------------------
//| Returns the type
//+-----------------------------------------------------------------------------
INTERPOLATOR_TYPE INTERPOLATOR::GetType()
{
	return Type;
}


//+-----------------------------------------------------------------------------
//| Sets a new interpoaltion type
//+-----------------------------------------------------------------------------
VOID INTERPOLATOR::SetInterpolationType(INTERPOLATION_TYPE NewInterpolationType)
{
	InterpolationType = NewInterpolationType;
}


//+-----------------------------------------------------------------------------
//| Returns the interpolation type
//+-----------------------------------------------------------------------------
INTERPOLATION_TYPE INTERPOLATOR::GetInterpolationType()
{
	return InterpolationType;
}


//+-----------------------------------------------------------------------------
//| Sets a new global sequence ID
//+-----------------------------------------------------------------------------
VOID INTERPOLATOR::SetGlobalSequenceId(INT NewGlobalSequenceId)
{
	GlobalSequenceId = NewGlobalSequenceId;
}


//+-----------------------------------------------------------------------------
//| Returns the global sequence ID
//+-----------------------------------------------------------------------------
INT INTERPOLATOR::GetGlobalSequenceId()
{
	return GlobalSequenceId;
}


//+-----------------------------------------------------------------------------
//| Retrieves the scalar at a certain timepoint
//+-----------------------------------------------------------------------------
FLOAT INTERPOLATOR::GetScalar(const SEQUENCE_TIME &Time)
{
	VECTOR4 TempVector;

	if (Static) return StaticVector.x;
	if (NodeList.size() <= 0) return 0.0f;

	GetInterpolatedValue(TempVector, Time);

	return TempVector.x;
}


//+-----------------------------------------------------------------------------
//| Retrieves the vector at a certain timpoint
//+-----------------------------------------------------------------------------
VECTOR2 INTERPOLATOR::GetVector2(const SEQUENCE_TIME &Time)
{
	VECTOR2 Vector2;
	VECTOR4 TempVector;

	if (Static) return VECTOR2(StaticVector.x, StaticVector.y);
	if (NodeList.size() <= 0) return VECTOR2(0.0f, 0.0f);

	GetInterpolatedValue(TempVector, Time);

	Vector2.x = TempVector.x;
	Vector2.y = TempVector.y;

	return Vector2;
}


//+-----------------------------------------------------------------------------
//| Retrieves the vector at a certain timepoint
//+-----------------------------------------------------------------------------
VECTOR3 INTERPOLATOR::GetVector3(CONST SEQUENCE_TIME& Time)
{
	VECTOR3 Vector3;
	VECTOR4 TempVector;

	if(Static) return VECTOR3(StaticVector.x, StaticVector.y, StaticVector.z);
	if(NodeList.size() <= 0) return VECTOR3(0.0f, 0.0f, 0.0f);

	GetInterpolatedValue(TempVector, Time);

	Vector3.x = TempVector.x;
	Vector3.y = TempVector.y;
	Vector3.z = TempVector.z;

	return Vector3;
}


//+-----------------------------------------------------------------------------
//| Retrieves the vector at a certain timepoint
//+-----------------------------------------------------------------------------
VECTOR4 INTERPOLATOR::GetVector4(CONST SEQUENCE_TIME& Time)
{
	VECTOR4 Vector4;

	if(Static) return StaticVector;
	if(NodeList.size() <= 0) return VECTOR4(0.0f, 0.0f, 0.0f, 1.0f);

	GetQuaternionValue(Vector4, Time);

	return Vector4;
}


//+-----------------------------------------------------------------------------
//| Sets a static scalar
//+-----------------------------------------------------------------------------
VOID INTERPOLATOR::SetStaticScalar(FLOAT NewScalar, CONST std::string& NewName)
{
	Clear();

	Type = INTERPOLATOR_TYPE_SCALAR;
	StaticVector = VECTOR4(NewScalar, 0.0f, 0.0f, 0.0f);
	Static = TRUE;

	if(NewName != "")
	{
		Name = NewName;
		DefaultVector = StaticVector;
	}
}


//+-----------------------------------------------------------------------------
//| Sets a static integer scalar
//+-----------------------------------------------------------------------------
VOID INTERPOLATOR::SetStaticScalarInt(INT NewScalar, CONST std::string& NewName)
{
	Clear();

	Type = INTERPOLATOR_TYPE_SCALAR_INT;
	StaticVector = VECTOR4(static_cast<FLOAT>(NewScalar), 0.0f, 0.0f, 0.0f);
	Static = TRUE;

	if(NewName != "")
	{
		Name = NewName;
		DefaultVector = StaticVector;
	}
}


//+-----------------------------------------------------------------------------
//| Sets a static vector
//+-----------------------------------------------------------------------------
VOID INTERPOLATOR::SetStaticVector2(CONST VECTOR2& NewVector2, CONST std::string& NewName)
{
	Clear();

	Type = INTERPOLATOR_TYPE_VECTOR2;
	StaticVector = VECTOR4(NewVector2.x, NewVector2.y, 0.0f, 0.0f);
	Static = TRUE;

	if(NewName != "")
	{
		Name = NewName;
		DefaultVector = StaticVector;
	}
}


//+-----------------------------------------------------------------------------
//| Sets a static vector
//+-----------------------------------------------------------------------------
VOID INTERPOLATOR::SetStaticVector3(CONST VECTOR3& NewVector3, CONST std::string& NewName)
{
	Clear();

	Type = INTERPOLATOR_TYPE_VECTOR3;
	StaticVector = VECTOR4(NewVector3.x, NewVector3.y, NewVector3.z, 0.0f);
	Static = TRUE;

	if(NewName != "")
	{
		Name = NewName;
		DefaultVector = StaticVector;
	}
}


//+-----------------------------------------------------------------------------
//| Sets a static vector
//+-----------------------------------------------------------------------------
VOID INTERPOLATOR::SetStaticVector4(CONST VECTOR4& NewVector4, CONST std::string& NewName)
{
	Clear();

	Type = INTERPOLATOR_TYPE_VECTOR4;
	StaticVector = NewVector4;
	Static = TRUE;

	if(NewName != "")
	{
		Name = NewName;
		DefaultVector = StaticVector;
	}
}


//+-----------------------------------------------------------------------------
//| Retrieves the interpolated value
//+-----------------------------------------------------------------------------
VOID INTERPOLATOR::GetInterpolatedValue(VECTOR4& Vector, CONST SEQUENCE_TIME& Time)
{
	FLOAT Factor;
	FLOAT InverseFactor;
	SEQUENCE_TIME TempTime;
	MODEL_GLOBAL_SEQUENCE* GlobalSequence;
	std::list<INTERPOLATOR_NODE>::iterator Node1;
	std::list<INTERPOLATOR_NODE>::iterator Node2;

	if(GlobalSequenceNode.IsAttached())
	{
		GlobalSequence = GlobalSequenceNode.GetObjectData();
		TempTime.IntervalStart = 0;
		TempTime.IntervalEnd = GlobalSequence->Data().Duration;
		TempTime.Time = GlobalSequence->Data().Time;
	}
	else
	{
		TempTime = Time;
	}

	GetSurroundingNodes(TempTime, Node1, Node2);

	if(Node1 == NodeList.end())
	{
		Vector = DefaultVector;
		return;
	}

	if(Node2 == NodeList.end())
	{
		Vector = Node1->Vector;
		return;
	}

	if(Node1->Time >= Node2->Time)
	{
		Vector = Node1->Vector;
		return;
	}

	Factor = static_cast<FLOAT>(TempTime.Time - Node1->Time) / static_cast<FLOAT>(Node2->Time - Node1->Time);
	InverseFactor = (1.0f - Factor);

	switch(InterpolationType)
	{
	case INTERPOLATION_TYPE_NONE:
		{
			Vector = Node1->Vector;
			break;
		}

	case INTERPOLATION_TYPE_LINEAR:
		{
			Vector = (  Node1->Vector * InverseFactor) + (Node2->Vector * Factor );
			break;
		}

	case INTERPOLATION_TYPE_HERMITE:
		{
			FLOAT Factor1;
			FLOAT Factor2;
			FLOAT Factor3;
			FLOAT Factor4;
			FLOAT FactorTimesTwo;

			FactorTimesTwo = Factor * Factor;

			Factor1 = FactorTimesTwo * (2.0f * Factor - 3.0f) + 1;
			Factor2 = FactorTimesTwo * (Factor - 2.0f) + Factor;
			Factor3 = FactorTimesTwo * (Factor - 1.0f);
			Factor4 = FactorTimesTwo * (3.0f - 2.0f * Factor);

			Vector = (Node1->Vector * Factor1) + (Node1->OutTan * Factor2) + (Node2->InTan * Factor3) + (Node2->Vector * Factor4);

			break;
		}

	case INTERPOLATION_TYPE_BEZIER:
		{
			FLOAT Factor1;
			FLOAT Factor2;
			FLOAT Factor3;
			FLOAT Factor4;
			FLOAT FactorTimesTwo;
			FLOAT InverseFactorTimesTwo;

			FactorTimesTwo = Factor * Factor;
			InverseFactorTimesTwo = InverseFactor * InverseFactor;

			Factor1 = InverseFactorTimesTwo * InverseFactor;
			Factor2 = 3.0f * Factor * InverseFactorTimesTwo;
			Factor3 = 3.0f * FactorTimesTwo * InverseFactor;
			Factor4 = FactorTimesTwo * Factor;

			Vector = (Node1->Vector * Factor1) + (Node1->OutTan * Factor2) + (Node2->InTan * Factor3) + (Node2->Vector * Factor4);

			break;
		}
	}
}


//+-----------------------------------------------------------------------------
//| Retrieves the interpolated quaternion value
//+-----------------------------------------------------------------------------
VOID INTERPOLATOR::GetQuaternionValue(VECTOR4& Vector, CONST SEQUENCE_TIME& Time)
{

}

//+-----------------------------------------------------------------------------
//| Retrieves the surrounding nodes
//+-----------------------------------------------------------------------------
VOID INTERPOLATOR::GetSurroundingNodes(CONST SEQUENCE_TIME& Time, std::list<INTERPOLATOR_NODE>::iterator& Node1, std::list<INTERPOLATOR_NODE>::iterator& Node2)
{
	Node1 = NodeList.end();
	Node2 = NodeList.begin();

	while(TRUE)
	{
		if(Node2 == NodeList.end()) break;
		if(Time.Time < Node2->Time) break;

		Node1 = Node2;
		Node2++;
	}

	if((Node1 != NodeList.end()) && (Node1->Time < Time.IntervalStart)) Node1 = NodeList.end();
	if((Node2 != NodeList.end()) && (Node2->Time > Time.IntervalEnd)) Node2 = NodeList.end();
}