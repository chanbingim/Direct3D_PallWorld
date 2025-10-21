#pragma once
#include "Base.h"
#include "EffectStruct.h"

NS_BEGIN(Engine)
class CGameObject;
class CComponent;
class CTexture;
class CShader;

class CEffectManager : public CBase
{
public :
	enum class EFFECT_TYPE { PART_OBJECT, CONTATINER, END };

private :
	CEffectManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CEffectManager() = default;

public :
	HRESULT							Initalize();

	CTexture*						GetTextureResource(const WCHAR* TextureTag);
	CShader*						GetShaderResource(const WCHAR* ShaderTag);
	CComponent*						GetModelResource(const WCHAR* ModelTag);

	CGameObject*					Clone_Object(EFFECT_TYPE eEffectType, const WCHAR* szEffectTag, void* pArg);

#pragma region _DEBUG
	HRESULT								ADD_Effects(EFFECT_TYPE eEffectType, const WCHAR* szEffectTag, CGameObject* pEffect);
	HRESULT								Remove_Effect(EFFECT_TYPE eEffectType, const WCHAR* szEffectTag);

	map<const _wstring, CTexture*>*		GetALLTextureResource()	{ return &m_pTextures; }
	map<const _wstring, CShader*>*		GetALLShaderResource()	{ return &m_pShaders; }
	map<const _wstring, CComponent*>*	GetALLModelResource()	{ return &m_pVIBuffers; }
	map<const _wstring, CGameObject*>*  GetALLEffects(EFFECT_TYPE eEffectType) { return &m_pEffects[ENUM_CLASS(eEffectType)]; }
#pragma endregion

private :
	ID3D11Device*						m_pDevice = nullptr;
	ID3D11DeviceContext*				m_pContext = nullptr;

	map<const _wstring, CTexture*>		m_pTextures = {};
	map<const _wstring, CComponent*>	m_pVIBuffers = {};
	map<const _wstring, CShader*>		m_pShaders = {};

	map<const _wstring, CGameObject*>	m_pEffects[ENUM_CLASS(EFFECT_TYPE::END)];

private :
	HRESULT							LoadEffectTexture();
	HRESULT							LoadEffectShader();
	HRESULT							LoadEffectModel();

	HRESULT							LoadPartEffect();
	HRESULT							LoadContainerEffect();

public :
	static		CEffectManager*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		void				Free();

};
NS_END