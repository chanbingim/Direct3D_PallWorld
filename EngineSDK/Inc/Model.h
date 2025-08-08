#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class ENGINE_DLL CModel : public CComponent
{
private :
	const aiScene* m_pAIScene = nullptr;
	Importer m_pImporter;
};
NS_END