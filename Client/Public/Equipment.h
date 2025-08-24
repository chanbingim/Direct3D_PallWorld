#pragma once
#include "UserInterface.h"

NS_BEGIN(Client)
class CEquipment : public CUserInterface
{
private :
	CEquipment(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEquipment(const CEquipment& rhs);
	virtual ~CEquipment() = default;

private :

public :
	static		CEquipment*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		void			Free() override;
};
NS_END