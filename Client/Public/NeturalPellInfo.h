#pragma once
#include "BackGround.h"

NS_BEGIN(Client)
class CPellHealthBar;
class CTypeIcon;

class CNeturalPellInfo : public CBackGround
{
protected:
	CNeturalPellInfo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNeturalPellInfo(const CNeturalPellInfo& rhs);
	virtual ~CNeturalPellInfo() = default;

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
	//폰트는 나중에 구해서 적용
	CPellHealthBar*							m_pHealthBar = nullptr;
	CTypeIcon*								m_pTypeIcon = nullptr;

private :
	HRESULT									ADD_Components();

public:
	static			CNeturalPellInfo*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;

};
NS_END