#pragma once
#include "BackGround.h"

NS_BEGIN(Client)
class CPellBase;
class CPellHealthBar;
class CNeturalTypeIcon;

class CNeturalPellInfo : public CBackGround
{
public :
	typedef struct	NeturalPellDesc : public GAMEOBJECT_DESC
	{
		CPellBase*			pOwner;
		const char*			szAttachSocketName;
	}NETURAL_PELL_DESC;

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
	CPellBase*								m_pOwner = nullptr;

	//폰트는 나중에 구해서 적용
	CPellHealthBar*							m_pHealthBar = nullptr;
	CNeturalTypeIcon*						 m_pTypeIcon = nullptr;

	const _float4x4*						m_pSpineMatrix = nullptr;
private :
	HRESULT									ADD_Components();
	HRESULT									ADD_Childs();

	void									SetUpPellInfoData();

public:
	static			CNeturalPellInfo*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;

};
NS_END