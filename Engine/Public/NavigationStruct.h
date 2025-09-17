#pragma once
#include "Engine_Defines.h"

typedef struct Navi_Edge
{
	_float3 A;
	_float3 B;

	Navi_Edge() {}

	Navi_Edge(_float3 PointA, _float3 PointB) :
		A(PointA), B(PointB) {}

	Navi_Edge(const Navi_Edge& rhs) :
		A(rhs.A), B(rhs.B) {}

	_bool operator== (const Navi_Edge& rhs) const
	{
		if (XMVector3Equal(XMLoadFloat3(&A), XMLoadFloat3(&rhs.A)) && XMVector3Equal(XMLoadFloat3(&B), XMLoadFloat3(&rhs.B)) ||
			XMVector3Equal(XMLoadFloat3(&B), XMLoadFloat3(&rhs.A)) && XMVector3Equal(XMLoadFloat3(&A), XMLoadFloat3(&rhs.B)))
			return true;

		return false;
	}

}NAVI_EDGE;

typedef struct Navi_Triangle
{
	_float3 A;
	_float3 B;
	_float3 C;

	//외심
	_float3 Circumcenter = {};
	_float	Radius = { -1 };

	//삼각형의 변
	list<NAVI_EDGE> TriEdge;

	Navi_Triangle(_float3 PointA, _float3 PointB, _float3 PointC) :
		A(PointA), B(PointB), C(PointC)
	{
		_float POINT_A = PointB.x - PointA.x;
		_float POINT_B = PointB.z - PointA.z;
		_float POINT_C = PointC.x - PointA.x;
		_float POINT_D = PointC.z - PointA.z;

		_float POINT_E = POINT_A * (PointA.x + PointB.x) + POINT_B * (PointA.z + PointB.z);
		_float POINT_F = POINT_C * (PointA.x + PointC.x) + POINT_D * (PointA.z + PointC.z);
		_float POINT_G = 2.0f * (POINT_A * (PointC.z - PointB.z) - POINT_B * (PointC.x - PointB.x));

		TriEdge.emplace_back(A, B);
		TriEdge.emplace_back(B, C);
		TriEdge.emplace_back(C, A);

		if (FLT_EPSILON > fabsf(POINT_G))
			return;
		else
			Circumcenter = { (POINT_D * POINT_E - POINT_B * POINT_F) / POINT_G, 0.f, (POINT_A * POINT_F - POINT_C * POINT_E) / POINT_G };

		PointA.y = 0;
		Radius = XMVectorGetX(XMVector3Length(XMLoadFloat3(&PointA) - XMLoadFloat3(&Circumcenter)));
	}

	Navi_Triangle(const Navi_Triangle& rhs) :
		A(rhs.A), B(rhs.B), C(rhs.C), Circumcenter(rhs.Circumcenter), Radius(rhs.Radius),
		TriEdge(rhs.TriEdge) {}

	_bool operator== (const Navi_Triangle& rhs) const 
	{
		// A, B, C를 비교
		bool match1 = XMVector3Equal(XMLoadFloat3(&A), XMLoadFloat3(&rhs.A));
		bool match2 = XMVector3Equal(XMLoadFloat3(&A), XMLoadFloat3(&rhs.B));
		bool match3 = XMVector3Equal(XMLoadFloat3(&A), XMLoadFloat3(&rhs.C));

		bool Bmatch1 = XMVector3Equal(XMLoadFloat3(&B), XMLoadFloat3(&rhs.A));
		bool Bmatch2 = XMVector3Equal(XMLoadFloat3(&B), XMLoadFloat3(&rhs.B));
		bool Bmatch3 = XMVector3Equal(XMLoadFloat3(&B), XMLoadFloat3(&rhs.C));

		bool Cmatch1 = XMVector3Equal(XMLoadFloat3(&C), XMLoadFloat3(&rhs.A));
		bool Cmatch2 = XMVector3Equal(XMLoadFloat3(&C), XMLoadFloat3(&rhs.B));
		bool Cmatch3 = XMVector3Equal(XMLoadFloat3(&C), XMLoadFloat3(&rhs.C));

		// 모든 점이 서로 다른 점에 대응되는지 확인
		if ((match1 && Bmatch2 && Cmatch3) ||
			(match1 && Bmatch3 && Cmatch2) ||
			(match2 && Bmatch1 && Cmatch3) ||
			(match2 && Bmatch3 && Cmatch1) ||
			(match3 && Bmatch1 && Cmatch2) ||
			(match3 && Bmatch2 && Cmatch1))
			return true;
			
		
		return false;
	}

	_bool PointInCircumeCircle(_vector vPoint)
	{
		if (-1 == Radius)
			return false;

		_float fLength = XMVectorGetX(XMVector3Length(vPoint - XMLoadFloat3(&Circumcenter)));
		return fLength <= Radius;
	};
}NAVI_TRIANGLE;