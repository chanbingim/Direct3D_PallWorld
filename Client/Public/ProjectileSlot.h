#pragma once
#include "Client_Define.h"
#include "PartObject.h"

NS_BEGIN(Client)
class CProjectileSlot : public CPartObject
{
public :
	typedef struct ProjecttileDesc : PARTOBJECT_DESC
	{
		const _float4x4*				fLeftSocket;
	}PROJECTILE_DESC;

protected:
	CProjectileSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CProjectileSlot(const CProjectileSlot& rhs);

	virtual ~CProjectileSlot() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;

	//업데이트	
	virtual		void						Priority_Update(_float fDeletaTime) override;
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;

	// 랜더
	virtual		HRESULT						Render() override;

	void									SetItemIndex(_uInt iItemID) { m_iProjecTileItemID = iItemID; }
	void									SetLeftSocket(_bool bFlag) { m_bIsLeftSocket = bFlag; }

protected :
	virtual     HRESULT						Bind_ShaderResources() override;
	virtual     HRESULT						Apply_ConstantShaderResources(_uInt iMeshIndex) override;

private :
	_Int									m_iProjecTileItemID = {};
	_bool									m_bIsLeftSocket = false;
	_bool									m_bIsAnimationPlay = false;

	CShader*								m_pNoneAimShader = nullptr;
	const _float4x4*						m_pLeftSocketMatrix = nullptr;

private:
	HRESULT									ADD_Components();

public:
	static			CProjectileSlot*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;

};
NS_END