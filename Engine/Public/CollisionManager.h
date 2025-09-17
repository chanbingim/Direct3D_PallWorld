#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CCollision;

typedef struct PairHash
{
	size_t operator () (pair<CCollision*, CCollision*> const& v) const {

		CCollision* MinData = min(v.first, v.second);
		CCollision* MaxData = max(v.first, v.second);

		return hash<CCollision*>()(MinData) ^ (hash<CCollision*>()(MaxData) << 1 );
	}
}PAIR_HASH;

typedef struct ColCompare
{
	_bool operator() (const pair<CCollision*, CCollision*>& Src, const pair<CCollision*, CCollision*>& Dest) const
	{
		if ((Src.first == Dest.first && Src.second == Dest.second) ||
			(Src.second == Dest.first && Src.first == Dest.second))
			return true;

		return false;
	}

}COL_COMPARE;


class CCollisionManager final : public CBase
{
private:
	CCollisionManager();
	virtual ~CCollisionManager() = default;

public:
	void							ADD_CollisionList(CCollision* pObject);
	void							Compute_Collision();

private:
	unordered_set<pair<CCollision*, CCollision*>, PAIR_HASH, COL_COMPARE>	m_ColHash;
	list<CCollision*>														m_ColList = {};

public :
	static		CCollisionManager*	Create();
	virtual		void				Free() override;

};
NS_END