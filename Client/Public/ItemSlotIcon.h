#pragma once
#include "Client_Define.h"
#include "UserInterface.h"

NS_BEGIN(Engine)
class CVIBuffer;
class CShader;
class CTexture;
NS_END

NS_BEGIN(Client)
class CItemSlotIcon : public CUserInterface
{
public :
	typedef struct ItemSlotIconDesc : GAMEOBJECT_DESC
	{
		const CTransform*		pParentTransform;

	}ITEM_SLOT_ICON_DESC;

private:
	CItemSlotIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItemSlotIcon(const CItemSlotIcon& rhs);
	virtual ~CItemSlotIcon() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

	

	void									SetTexture(const CTexture* pTexture);
protected :
	HRESULT									Bind_ShaderResources() override;

private:
	const		CTransform*					m_pParentTransform = nullptr;
	const		CTexture*					m_pItemIcon = nullptr;
	CShader*								m_pShaderCom = nullptr;
	CVIBuffer*								m_pVIBufferCom = nullptr;

private:
	HRESULT									ADD_Components();

public:
	static		CItemSlotIcon*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CUserInterface*				Clone(void* pArg) override;
	virtual		void						Free() override;



};
NS_END