#pragma once
#include "GameObject.h"

NS_BEGIN(Engine)

class CModel;
class CShader;

class ENGINE_DLL CPartObject : public CGameObject
{

public :
	typedef struct PartObject_Desc
	{
		CTransform* m_pParentTransform;
	}PARTOBJECT_DESC;

protected:
	CPartObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPartObject(const CPartObject& rhs);
	virtual ~CPartObject() = default;

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
	void									SetAnimIndex(_uInt iIndex);

protected :
	_float4x4								m_CombinedWorldMatrix = {};
#pragma region Animation
	CModel*									m_pVIBufferCom = nullptr;
	CShader*								m_pShaderCom = nullptr;
	ID3DX11EffectShaderResourceVariable*	m_pSRVEffect = nullptr;
	ID3DX11EffectMatrixVariable*			m_pBoneMatrixEffect = nullptr;

	_uInt									m_iAnimIndex = {};
#pragma endregion

protected :
	virtual     HRESULT						Bind_ShaderResources();
	virtual     HRESULT						Apply_ConstantShaderResources(_uInt iMeshIndex);

public:
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END