#pragma once

#include <Client/UI/Util/AffectFlagContainer.h>
#include <Client/Map/MapType.h>

#include <map>

class CInstanceBase;

class CInstanceBaseEffect
{
	public:
		explicit CInstanceBaseEffect(CInstanceBase& rkOwner);

		void AddDamageEffect(uint32_t damage, uint8_t flag, int32_t bSelf, int32_t bTarget);
		void AttachSpecialEffect(uint32_t effect);
		void AttachTextTail();
		static void ClearPVPKeySystem();
		void CreateSpecialEffect(uint32_t iEffectIndex);
		void DeleteBlendOut();
		void DetachTextTail();
		static const D3DXCOLOR& GetIndexedNameColor(uint32_t eNameColor);
		const D3DXCOLOR& GetNameColor();
		uint32_t GetNameColorIndex();
		const D3DXCOLOR& GetTitleColor();
		static void InsertDUELKey(uint32_t dwSrcVID, uint32_t dwDstVID);
		static void InsertGVGKey(uint32_t dwSrcGuildVID, uint32_t dwDstGuildVID);
		static void InsertPVPKey(uint32_t dwSrcVID, uint32_t dwDstVID);
		static void InsertPVPReadyKey(uint32_t dwSrcVID, uint32_t dwDstVID);
		bool IsPVPInstance(CInstanceBase& rkInstVictim);
		bool IsPossibleEmoticon();
		void LevelUp();
		void ProcessDamage();
		void RefreshTextTail();
		void RefreshTextTailTitle();
		static bool RegisterEffect(uint32_t eEftType, const char* c_szEftAttachBone, const char* c_szEftName, bool isCache);
		static bool RegisterNameColor(uint32_t uIndex, uint32_t r, uint32_t g, uint32_t b);
		static bool RegisterTitleColor(uint32_t uIndex, uint32_t r, uint32_t g, uint32_t b);
		static void RegisterTitleName(int iIndex, const char* c_szTitleName);
		static void RemoveGVGKey(uint32_t dwSrcGuildVID, uint32_t dwDstGuildVID);
		static void RemovePVPKey(uint32_t dwSrcVID, uint32_t dwDstVID);
		void SCRIPT_SetAffect(uint32_t eAffect, bool isVisible);
		void SetAffectFlagContainer(const CAffectFlagContainer& c_rkAffectFlagContainer);
		void SetAlpha(float fAlpha);
		static void SetDustGap(float fDustGap);
		void SetEmoticon(uint32_t eEmoticon);
		static void SetEmpireNameMode(bool isEnable);
		void SetFishEmoticon();
		static void SetHorseDustGap(float fDustGap);
		void SkillUp();
		bool UpdateDeleting();
		void UpdateTextTailLevel(uint32_t level);
		void __Assassin_SetEunhyeongAffect(bool isVisible);
		uint32_t __AttachEffect(uint32_t eEftType);
		void __AttachEmpireEffect(uint32_t eEmpire);
		void __AttachSelectEffect();
		void __AttachTargetEffect();
		void __ClearAffectFlagContainer();
		void __ClearAffects();
		void __ComboProcess();
		void __DetachEffect(uint32_t dwEID);
		void __DetachSelectEffect();
		void __DetachTargetEffect();
		uint32_t __EffectContainer_AttachEffect(uint32_t eEffect);
		void __EffectContainer_Destroy();
		void __EffectContainer_DetachEffect(uint32_t eEffect);
		std::map<uint32_t, uint32_t>& __EffectContainer_GetDict();
		void __EffectContainer_Initialize();
		static bool __FindDUELKey(uint32_t dwSrcGuildID, uint32_t dwDstGuildID);
		static bool __FindGVGKey(uint32_t dwSrcGuildID, uint32_t dwDstGuildID);
		static bool __FindPVPKey(uint32_t dwSrcVID, uint32_t dwDstVID);
		static bool __FindPVPReadyKey(uint32_t dwSrcVID, uint32_t dwDstVID);
		static uint32_t __GetPVPKey(uint32_t dwSrcVID, uint32_t dwDstVID);
		void __SetAffect(uint32_t eAffect, bool isVisible);
		void __SetNormalAffectFlagContainer(const CAffectFlagContainer& c_rkAffectFlagContainer);
		void __SetReviveInvisibilityAffect(bool isVisible);
		void __SetStoneSmokeFlagContainer(const CAffectFlagContainer& c_rkAffectFlagContainer);
		void __Shaman_SetParalysis(bool isParalysis);
		void __StoneSmoke_Create(uint32_t eSmoke);
		void __StoneSmoke_Destroy();
		void __StoneSmoke_Inialize();
		void __Warrior_SetGeomgyeongAffect(bool isVisible);

	private:
		CInstanceBase& m_rkOwner;
};
