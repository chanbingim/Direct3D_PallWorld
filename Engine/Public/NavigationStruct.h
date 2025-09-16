#pragma once
#include "Engine_Defines.h"

typedef struct Navi_Edge
{
	_float3 A;
	_float3 B;

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
	_float	Radius = { -1};

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

		if (0 == POINT_G)
			return;

		PointA.y = 0;
		Circumcenter = {(POINT_D  * POINT_E - POINT_B * POINT_F) / POINT_G, 0.f, (POINT_A * POINT_F - POINT_C * POINT_E) / POINT_G };
		Radius = XMVectorGetX(XMVector3Length(XMLoadFloat3(&PointA) - XMLoadFloat3(&Circumcenter)));
	}
	//	if (XMVector3Equal(XMLoadFloat3(&A), XMLoadFloat3(&B)) ||
	//		XMVector3Equal(XMLoadFloat3(&B), XMLoadFloat3(&C)) ||
	//		XMVector3Equal(XMLoadFloat3(&C), XMLoadFloat3(&A))) // 같은 점이 있음. 삼각형 아님. 외접원 구할 수 없음.
	//	{
	//		Radius = -1;
	//		return;
	//	}

	//	//직선 AB의 기울기
	//	_float fABGradient = (B.x - A.x) / (B.z - A.z) * -1.0f;

	//	// 수직인 직선과 밑변의 교점
	//	_float3 CrossPointA = { (B.x + A.x) / 2.f, 0.f, (B.z + A.z) /2.f };

	//	//직선 BC의 기울기
	//	_float fBCGradient = (B.x - C.x) / (B.z - C.z) * -1.0f;

	//	// 수직인 직선과 밑변의 교점
	//	_float3 CrossPointB = { (B.x + C.x) / 2.f, 0.f, (B.z + C.z) / 2.f };

	//	A.y = B.y = C.y = 0;

	//	TriEdge.emplace_back(A, B);
	//	TriEdge.emplace_back(B, C);
	//	TriEdge.emplace_back(C, A);

	//	//외심의 중점과 반지름 구하는 공식
	//	if (fABGradient == fBCGradient)
	//	{
	//		Radius = -1;
	//		return;
	//	}

	//	Circumcenter.x = (fABGradient * CrossPointA.x - fBCGradient * CrossPointB.x + CrossPointB.z - CrossPointA.z) / (fABGradient - fBCGradient);
	//	Circumcenter.z = fABGradient * (Circumcenter.x - CrossPointA.x) + CrossPointA.z;

	//	if (PointB.x == PointA.x)     // 수직이등분선의 기울기가 0인 경우(수평선)
	//	{
	//		Circumcenter.x = (CrossPointB.x + CrossPointA.z - CrossPointB.z) / fBCGradient;
	//		Circumcenter.z = CrossPointA.z;
	//	}

	//	if (PointB.z == PointA.z)     // 수직이등분선의 기울기가 무한인 경우(수직선)
	//	{
	//		Circumcenter.x = CrossPointA.x;
	//		if (0.0f == fBCGradient)
	//			Circumcenter.z = CrossPointB.z;
	//		else
	//			Circumcenter.z = fBCGradient * (CrossPointA.x - CrossPointB.x) + CrossPointB.z;
	//	}

	//	if (PointB.x == PointC.x)     // 수직이등분선의 기울기가 0인 경우(수평선)
	//	{
	//		Circumcenter.x = (CrossPointB.x + CrossPointA.z - CrossPointB.z) / fABGradient;
	//		Circumcenter.z = CrossPointA.z;
	//	}

	//	if (PointB.z == PointC.z)     // 수직이등분선의 기울기가 무한인 경우(수직선)
	//	{
	//		Circumcenter.x = CrossPointB.x;
	//		if (0.0f == fABGradient)
	//			Circumcenter.z = CrossPointA.z;
	//		else
	//			Circumcenter.z = fABGradient * (CrossPointA.x - CrossPointB.x) + CrossPointB.z;
	//	}
	//
	//	Radius = XMVectorGetX(XMVector3Length(XMLoadFloat3(&A) - XMLoadFloat3(&Circumcenter)));
	//}

	Navi_Triangle(const Navi_Triangle& rhs) :
		A(rhs.A), B(rhs.B), C(rhs.C), Circumcenter(rhs.Circumcenter), Radius(rhs.Radius),
		TriEdge(rhs.TriEdge) {}

	_bool operator== (const Navi_Triangle& rhs) const 
	{
		if (XMVector3Equal(XMLoadFloat3(&A), XMLoadFloat3(&rhs.A)) &&
			XMVector3Equal(XMLoadFloat3(&B), XMLoadFloat3(&rhs.B)) &&
			XMVector3Equal(XMLoadFloat3(&C), XMLoadFloat3(&rhs.C)))
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