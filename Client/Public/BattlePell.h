#pragma once

#include  "Client_Define.h"
#include  "BackGround.h"

NS_BEGIN(Client)
class CPellInfo;

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
	_float4x4										m_GroupMat[2] = {};

	//	펠정보를 가지고 있을 데이터
	//	소유한 펠 리스트를 참조 하고 있다가
	//	선택된 펠정보를 표시한다.
	// 현재선택된 Index의 값을 통해서 좌우 펠을 선택
	vector<CPellInfo*>*								m_pPellInfo = nullptr;
	_uInt											m_SelectIndex = {};

private :
	HRESULT									ADD_Components();
	HRESULT									ADD_Childs();

public:
	static		CBattlePell*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CGameObject*				Clone(void* pArg) override;
	virtual		void						Free() override;
};
NS_END