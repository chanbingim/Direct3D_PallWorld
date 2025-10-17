#pragma once
#include "BackGround.h"

NS_BEGIN(Client)
class CPellCharacterIcon : public CBackGround
{
public :
	typedef struct PellCharacterIcon : public GAMEOBJECT_DESC
	{
		const _float4x4*				pParentMat;
	}PELL_CHARACTER_ICON;

protected:
	CPellCharacterIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPellCharacterIcon(const CPellCharacterIcon& rhs);
	virtual ~CPellCharacterIcon() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;
	void									SetSelectPellIndex(_uInt iNumSelectPellIndex);

private :
	const		_float4x4*					m_pSocketMatrix = nullptr;
	const		CTexture*					m_pTexture = nullptr;

	//_float4x4								m_pCombindedMatrix = {};

private :
	HRESULT									ADD_Components();

public:
	static		CPellCharacterIcon*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CGameObject*				Clone(void* pArg);
	virtual		void						Free() override;

};
NS_END