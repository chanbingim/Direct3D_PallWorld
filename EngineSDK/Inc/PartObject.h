#pragma once
#include "GameObject.h"

NS_BEGIN(Engine)

class CModel;
class CShader;

class ENGINE_DLL CPartObject : public CGameObject
{
public :
	typedef struct PartObject_Desc : public GAMEOBJECT_DESC
	{
		const _float4x4*		SocketMatrix;
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
	void									SetAnimIndex(_uInt iIndex, _bool bIsLoop = true);
	_uInt									GetAnimIndex(const char* szName);

	const _float4x4*						GetPartObejctWorldMat() { return &m_CombinedWorldMatrix; }
	const _float4x4*						GetBoneMatrix(const char* szBone) const;


protected :
	_float4x4								m_CombinedWorldMatrix = {};
	const _float4x4*						m_SocketMatrix = nullptr;

#pragma region Animation
	CModel*									m_pVIBufferCom = nullptr;
	CShader*								m_pShaderCom = nullptr;
	ID3DX11EffectShaderResourceVariable*	m_pSRVEffect = nullptr;
	ID3DX11EffectMatrixVariable*			m_pBoneMatrixEffect = nullptr;

	_uInt									m_iAnimIndex = {};
	_bool									m_bIsAnimLoop = true;
#pragma endregion

protected :
	virtual     HRESULT						Bind_ShaderResources();
	virtual     HRESULT						Apply_ConstantShaderResources(_uInt iMeshIndex);

	void									UpdateCombinedMatrix();

public:
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END