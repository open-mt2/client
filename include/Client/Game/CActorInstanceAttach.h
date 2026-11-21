#pragma once

#include <Neoeter/Model/Grn/ThingInstance.h>
#include <Client/Map/MapType.h>

#include <cstdint>
#include <Client/Game/RaceData.h>

class CActorInstance;
class CItemData;

class CActorInstanceAttach
{
	public:
		explicit CActorInstanceAttach(CActorInstance& rkOwner);

		uint32_t AttachEffectByID(uint32_t dwParentPartIndex, const char* c_pszBoneName, uint32_t dwEffectID, const D3DXVECTOR3* c_pv3Position = NULL);
		uint32_t AttachEffectByName(uint32_t dwParentPartIndex, const char* c_pszBoneName, const char* c_pszEffectFileName);
		uint32_t AttachSmokeEffect(uint32_t eSmoke);
		void AttachWeapon(uint32_t dwItemIndex, uint32_t dwParentPartIndex = CRaceData::PART_MAIN, uint32_t dwPartIndex = CRaceData::PART_WEAPON);
		void AttachWeapon(uint32_t dwParentPartIndex, uint32_t dwPartIndex, CItemData* pItemData);
		void DettachEffect(uint32_t dwEID);
		int32_t GetAttachingBoneName(uint32_t dwPartIndex, const char** c_szBoneName);
		void HideAllAttachingEffect();
		void RefreshActorInstance();
		void SetWeaponTraceTexture(const char* szTextureName);
		void ShowAllAttachingEffect();
		void UpdateAttachingInstances();
		void UseAlphaWeaponTrace();
		void UseTextureWeaponTrace();
		void __ClearAttachingEffect();
		bool __GetWeaponAttachTransform(uint32_t dwPartIndex, D3DXMATRIX* pMat);
		bool __IsLeftHandWeapon(uint32_t type);
		bool __IsRightHandWeapon(uint32_t type);
		bool __IsWeaponTrace(uint32_t weaponType);

	private:
		CActorInstance& m_rkOwner;
};
