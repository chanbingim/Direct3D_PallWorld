#pragma once
#include "Client_Define.h"
#include "Base.h"

NS_BEGIN(Engine)
class CTransform;
NS_END

NS_BEGIN(Client)
class CObjectAnim : public CBase
{
public :
	typedef struct ObejctAnimDesc
	{
		const CTransform*			OwnerTransform;
	}OBJECT_ANIM_DESC;

protected :
	CObjectAnim();
	virtual ~CObjectAnim() = default;

public :
	virtual		HRESULT				Initialize_Prototype();
	virtual		HRESULT				Initialize(void* pArg);

	virtual		_bool				Update(_float fDeletaTiem);


private :
	const CTransform*				m_pOwnerTransform = nullptr;
	_bool							m_bIsAnimation = false; 


public :
	virtual			CObjectAnim*	Clone(void* pArg);
	virtual			void			Free() override;

};
NS_END