#pragma once
#include  "Client_Define.h"
#include  "BackGround.h"

NS_BEGIN(Engine)
class CHealthBar;
NS_END

NS_BEGIN(Client)
class CTypeIcon;

class CPellInfo : public CBackGround
{
protected :
	CPellInfo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPellInfo(const CPellInfo& rhs);
	virtual ~CPellInfo() = default;

public :
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

private :
	_wstring								m_szPellName = {};
	_uInt									m_iPellType = {};

	CHealthBar*								m_pHealthBar = nullptr;
	CTypeIcon*								m_pTypeIcon = nullptr;

private :
	HRESULT									ADD_Components();
	HRESULT									ADD_Childs();

public :
	static		CPellInfo*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CGameObject*				Clone(void* pArg) override;
	virtual		void						Free() override;
};
NS_END