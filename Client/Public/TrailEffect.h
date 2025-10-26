#pragma once
#include "SpriteEffect.h"

NS_BEGIN(Engine)
class CComputeShader;
class CVIBuffer_Point_Instance;
class CTexture;
class CShader;
NS_END

NS_BEGIN(Client)
class CTrailEffect : public CSpriteEffect
{
public :
	struct TrailConstBufferData
	{
		_uInt			iHeadIndex;
		_uInt			iMaxIndex;

		_float			fDeletaTime;
		_float			fSize;
		_float4x4		SocketMatrix;
		_float			fLifeTime;		// 여기서 Lifetime보다 크면 
		_float			fSmoothRate;	// 얼마나 부드럽게 따라갈지 (0~1)

		_uInt			iPadding;
		_uInt			iPadding2;
	};

	typedef		struct TrailEffectDesc
	{
		const		WCHAR*			szTrailEffectName;
		const		_float4x4*		pSocketMatrix;
		_int3						TrailDisPatch;
	}TRAIL_EFFECT_DESC;

protected:
	CTrailEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTrailEffect(const CTrailEffect& rhs);
	virtual ~CTrailEffect() = default;

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

	void									SetRenderTrail(_bool bIsFlag) { m_bIsTrailRender = bIsFlag; }
	_bool									IsTrailRender() { return m_bIsTrailRender; }

protected :
	virtual HRESULT							Apply_ConstantShaderResources() override;

private :
	_bool									m_bIsTrailRender = false;
	_int3									m_vTrailDispatch = {};
	const	WCHAR*							m_szTrailTexTag = {};

	const	_float4x4*						m_pSocketMatrix = nullptr;
	_float4x4								m_CombinedMatrix = {};

	TrailConstBufferData					m_CBData = {};
	_Int									m_iTrailHead = {};
	_uInt									m_iNumData = {};

	ID3D11Buffer*							m_pResourceBuffer[2] = { nullptr, nullptr };
	ID3D11Buffer*							m_pReadBuffer = nullptr;
	ID3D11ShaderResourceView*				m_pTrailInputSRV = nullptr;
	ID3D11UnorderedAccessView*				m_pTrailOutUAV = nullptr;
	ID3D11Buffer*							m_pConstTrailBuffer = nullptr;

	CComputeShader*							m_pComputeShader = {};
	CTexture*								m_pTextureCom = nullptr;
	CVIBuffer_Point_Instance*				m_pInstanceBuffer = nullptr;
	CShader*								m_pShaderCom = nullptr;

private :
	void									ComputeMatrix();

	HRESULT									Apply_ComputeShaderResource();
	HRESULT									Setting_Compute_Shader();
	HRESULT									ADD_Components();
		
public:
	static			CTrailEffect*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;

};
NS_END