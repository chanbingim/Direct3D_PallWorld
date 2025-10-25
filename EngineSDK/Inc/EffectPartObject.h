#pragma once
#include "GameObject.h"
#include "EffectStruct.h"

NS_BEGIN(Engine)
class CVIBuffer;
class CTexture;
class CShader;

class ENGINE_DLL CEffectPartObject : public CGameObject
{
public:
	typedef struct PartObject_Desc : public GAMEOBJECT_DESC
	{
		const _float4x4* SocketMatrix;

	}PARTOBJECT_DESC;

#ifdef _DEBUG
	typedef struct EffectPartObject_Desc
	{
		const char*				szFilePath;
	}EFFECT_PARTOBJECT_DESC;
#endif // _DEBUG

protected:
	CEffectPartObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffectPartObject(const CEffectPartObject& rhs);
	virtual ~CEffectPartObject() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initalize_Prototype(const char* pFilePath);

	virtual		HRESULT						Initialize(void* pArg) override;

	//업데이트	
	virtual		void						Priority_Update(_float fDeletaTime) override;
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;

	// 랜더
	virtual		HRESULT						Render() override;
	
	void									ResetEffect();
	const _float4x4*						GetPartObejctWorldMat() { return &m_CombinedWorldMatrix; }
	const EFFECT_NETWORK_DESC&				GetEffectPartData() { return m_EffectData; }
	
	void									EffectDead();

#ifdef _DEBUG
	void									SetPartEffectData(EFFECT_NETWORK_DESC& Data);
	virtual void							ExportData(void* pArg) override;
#endif // DEBUG

protected:
	EFFECT_NETWORK_DESC						m_EffectData = {};

	_bool									m_bIsDissolve = false;
	_float									m_fLifeAccTime = {};
	_float2									m_fDistotionAccTime = {};

	_float4x4								m_CombinedWorldMatrix = {};

	CComponent*								m_pVIBufferCom = nullptr;
	CTexture*								m_pTextures[5] = { nullptr, nullptr, nullptr, nullptr, nullptr };
	CShader*								m_pShaderCom = nullptr;

protected:
	virtual     HRESULT						Apply_ConstantShaderResources();
	HRESULT									Apply_ConstantShaderResources(_uInt iMeshIndex);

	void									UpdateCombinedMatrix();

private :
	HRESULT									ReadFileData(const char* pFilePath);
	void									DeadDissolve();

public:
	static			CEffectPartObject*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	static			CEffectPartObject*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const char* pFilePath);

	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;

};
NS_END