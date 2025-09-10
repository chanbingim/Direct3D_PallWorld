#pragma once
#include "Client_Define.h"
#include "Component.h"

NS_BEGIN(Engine)
class CGameObject;
NS_END

//여기서 이제 회복에 대한 이펙트 처리 및 다른 기타 처리를 여기서 하게될거임
NS_BEGIN(Client)
class CRecovery : public CComponent
{
public :
	typedef	 struct Recovery_Desc
	{
		CGameObject*				pOwner;
		_float						fRecoveryTime;
		_float						fPerTickRecovery;

	}RECOVERY_DESC;

private :
	CRecovery(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRecovery(const CRecovery& Prototype);
	virtual ~CRecovery() = default;

public:
	virtual HRESULT							Initialize_Prototype();
	virtual HRESULT							Initialize(void* pArg);

	// 회복량을 리턴한다.
	_float									Update(_float fTimeDeleta);

	// 회복 컴포넌트를 강제 리셋하는 함수
	void									ResetComponent();

	void									SetRecovery(_bool bIsFlag) { m_bIsPlay = bIsFlag; }
	_bool									GetRecovering() { return m_bIsPlay; }

private :							
	CGameObject*							m_pOwner = nullptr;

	_bool									m_bIsPlay = false;
	// 타임 델타 누적으로 회복 구현
	_float									m_fAccTime = {};
	
	//	몇초당 회복을 시킬건지
	_float									m_fRecoveryTime = {};

	//초당 얼마나 회복할지를 나타냄
	_float									m_fPerTickRecovery = {};

public:
	static		CRecovery*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CComponent*					Clone(void* pArg) override;
	virtual		void						Free() override;
};
NS_END