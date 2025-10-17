#pragma once

#include  "Client_Define.h"
#include  "BackGround.h"

NS_BEGIN(Client)
class CPellInfo;
class CPellCharacterIcon;

class CBattlePell : public CBackGround
{
protected:
	CBattlePell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBattlePell(const CBattlePell& rhs);
	virtual ~CBattlePell() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;
	 
private :
	_float4x4								m_GroupMat[2] = {};
	CPellCharacterIcon*						m_PellCharacterIcon[3] = {};

private :
	HRESULT									ADD_Components();
	HRESULT									ADD_Childs();

public:
	static		CBattlePell*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CGameObject*				Clone(void* pArg) override;
	virtual		void						Free() override;
};
NS_END