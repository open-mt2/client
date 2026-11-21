#pragma once

#include <Client/Protocol/Decode.h>

#include <cstdint>

class CPythonNetworkStream;

enum class ENetworkStreamPhaseResult
{
	NotHandled,
	Failed,
	Handled,
};

class CNetworkStreamPhase
{
	public:
		explicit CNetworkStreamPhase(CPythonNetworkStream& rkStream) : m_rkStream(rkStream) {}
		virtual ~CNetworkStreamPhase() {}

		virtual ENetworkStreamPhaseResult Handle(uint8_t header, const protocol::Packet& value) = 0;

	protected:
		CPythonNetworkStream& m_rkStream;
		friend class CPythonNetworkStream;
};

class CNetworkStreamOfflinePhase : public CNetworkStreamPhase
{
	public:
		explicit CNetworkStreamOfflinePhase(CPythonNetworkStream& rkStream);
		ENetworkStreamPhaseResult Handle(uint8_t header, const protocol::Packet& value) override;

		void OffLinePhase();
};

class CNetworkStreamHandShakePhase : public CNetworkStreamPhase
{
	public:
		explicit CNetworkStreamHandShakePhase(CPythonNetworkStream& rkStream);
		ENetworkStreamPhaseResult Handle(uint8_t header, const protocol::Packet& value) override;

		bool RecvHandshakeOKPacket(const protocol::Packet& value);
};

class CNetworkStreamSelectPhase : public CNetworkStreamPhase
{
	public:
		explicit CNetworkStreamSelectPhase(CPythonNetworkStream& rkStream);
		ENetworkStreamPhaseResult Handle(uint8_t header, const protocol::Packet& value) override;

		bool __RecvChangeName(const protocol::Packet& value);
		bool __RecvPlayerCreateFailurePacket(const protocol::Packet& value);
		bool __RecvPlayerCreateSuccessPacket(const protocol::Packet& value);
		bool __RecvPlayerDestroyFailurePacket(const protocol::Packet& value);
		bool __RecvPlayerDestroySuccessPacket(const protocol::Packet& value);
};

class CNetworkStreamLoginPhase : public CNetworkStreamPhase
{
	public:
		explicit CNetworkStreamLoginPhase(CPythonNetworkStream& rkStream);
		ENetworkStreamPhaseResult Handle(uint8_t header, const protocol::Packet& value) override;

		bool __RecvChinaMatrixCardPacket(const protocol::Packet& value);
		bool __RecvEmpirePacket(const protocol::Packet& value);
		bool __RecvLoginFailurePacket(const protocol::Packet& value);
		bool __RecvLoginKeyPacket(const protocol::Packet& value);
		bool __RecvLoginSuccessPacket3(const protocol::Packet& value);
		bool __RecvLoginSuccessPacket4(const protocol::Packet& value);
		bool __RecvNEWCIBNPasspodFailurePacket(const protocol::Packet& value);
		bool __RecvNEWCIBNPasspodRequestPacket(const protocol::Packet& value);
		bool __RecvRunupMatrixQuizPacket(const protocol::Packet& value);
};

class CNetworkStreamLoadingPhase : public CNetworkStreamPhase
{
	public:
		explicit CNetworkStreamLoadingPhase(CPythonNetworkStream& rkStream);
		ENetworkStreamPhaseResult Handle(uint8_t header, const protocol::Packet& value) override;

		bool RecvMainCharacter(const protocol::Packet& value);
		bool RecvMainCharacter2_EMPIRE(const protocol::Packet& value);
		bool RecvMainCharacter3_BGM(const protocol::Packet& value);
		bool RecvMainCharacter4_BGM_VOL(const protocol::Packet& value);
		bool __RecvPlayerPoints(const protocol::Packet& value);
};

class CNetworkStreamGameActorPhase : public CNetworkStreamPhase
{
	public:
		explicit CNetworkStreamGameActorPhase(CPythonNetworkStream& rkStream);
		ENetworkStreamPhaseResult Handle(uint8_t header, const protocol::Packet& value) override;

		bool RecvCharacterAdditionalInfo(const protocol::Packet& value);
		bool RecvCharacterAppendPacket(const protocol::Packet& value);
		bool RecvCharacterAppendPacketNew(const protocol::Packet& value);
		bool RecvCharacterDeletePacket(const protocol::Packet& value);
		bool RecvCharacterMovePacket(const protocol::Packet& value);
		bool RecvCharacterUpdatePacket(const protocol::Packet& value);
		bool RecvCharacterUpdatePacketNew(const protocol::Packet& value);
		bool RecvOwnerShipPacket(const protocol::Packet& value);
};

class CNetworkStreamGameItemPhase : public CNetworkStreamPhase
{
	public:
		explicit CNetworkStreamGameItemPhase(CPythonNetworkStream& rkStream);
		ENetworkStreamPhaseResult Handle(uint8_t header, const protocol::Packet& value) override;

		bool RecvDragonSoulRefine(const protocol::Packet& value);
		bool RecvItemGroundAddPacket(const protocol::Packet& value);
		bool RecvItemGroundDelPacket(const protocol::Packet& value);
		bool RecvItemOwnership(const protocol::Packet& value);
		bool RecvItemSetPacket(const protocol::Packet& value);	  // Alarm to python
		bool RecvItemSetPacket2(const protocol::Packet& value);	  // Alarm to python
		bool RecvItemUpdatePacket(const protocol::Packet& value); // Alarm to python
		bool RecvItemUsePacket(const protocol::Packet& value);	  // Alarm to python
		bool RecvMallItemDelPacket(const protocol::Packet& value);
		bool RecvMallItemSetPacket(const protocol::Packet& value);
		bool RecvMallOpenPacket(const protocol::Packet& value);
		bool RecvQuickSlotAddPacket(const protocol::Packet& value);	 // Alarm to python
		bool RecvQuickSlotDelPacket(const protocol::Packet& value);	 // Alarm to python
		bool RecvQuickSlotMovePacket(const protocol::Packet& value); // Alarm to python
		bool RecvSafeBoxDelPacket(const protocol::Packet& value);
		bool RecvSafeBoxMoneyChangePacket(const protocol::Packet& value);
		bool RecvSafeBoxSetPacket(const protocol::Packet& value);
		bool RecvSafeBoxSizePacket(const protocol::Packet& value);
		bool RecvSafeBoxWrongPasswordPacket(const protocol::Packet& value);
		bool RecvSpecialEffect(const protocol::Packet& value);
		bool RecvSpecificEffect(const protocol::Packet& value);
};

class CNetworkStreamGamePhase : public CNetworkStreamPhase
{
	public:
		explicit CNetworkStreamGamePhase(CPythonNetworkStream& rkStream);
		ENetworkStreamPhaseResult Handle(uint8_t header, const protocol::Packet& value) override;

		bool RecvAddFlyTargetingPacket(const protocol::Packet& value);
		bool RecvAffectAddPacket(const protocol::Packet& value);
		bool RecvAffectRemovePacket(const protocol::Packet& value);
		bool RecvChangeSkillGroupPacket(const protocol::Packet& value);
		bool RecvChangeSpeedPacket(const protocol::Packet& value);
		bool RecvChannelPacket(const protocol::Packet& value);
		bool RecvCharacterPositionPacket(const protocol::Packet& value);
		bool RecvCreateFlyPacket(const protocol::Packet& value);
		bool RecvDamageInfoPacket(const protocol::Packet& value);
		bool RecvDeadPacket(const protocol::Packet& value);
		bool RecvDigMotionPacket(const protocol::Packet& value);
		bool RecvExchangePacket(const protocol::Packet& value);
		bool RecvFishing(const protocol::Packet& value);
		bool RecvFlyTargetingPacket(const protocol::Packet& value);
		bool RecvLovePointUpdatePacket(const protocol::Packet& value);
		bool RecvLoverInfoPacket(const protocol::Packet& value);
		bool RecvMotionPacket(const protocol::Packet& value);
		bool RecvMountPacket(const protocol::Packet& value);
		bool RecvPVPPacket(const protocol::Packet& value);
		bool RecvPartyAdd(const protocol::Packet& value);
		bool RecvPartyInvite(const protocol::Packet& value);
		bool RecvPartyLink(const protocol::Packet& value);
		bool RecvPartyParameter(const protocol::Packet& value);
		bool RecvPartyRemove(const protocol::Packet& value);
		bool RecvPartyUnlink(const protocol::Packet& value);
		bool RecvPartyUpdate(const protocol::Packet& value);
		bool RecvPointChange(const protocol::Packet& value); // Alarm to python
		bool RecvQuestConfirmPacket(const protocol::Packet& value);
		bool RecvRefineInformationPacket(const protocol::Packet& value);
		bool RecvRefineInformationPacketNew(const protocol::Packet& value);
		bool RecvRequestMakeGuild(const protocol::Packet& value);
		bool RecvShopSignPacket(const protocol::Packet& value);
		bool RecvSkillLevel(const protocol::Packet& value);
		bool RecvSkillLevelNew(const protocol::Packet& value);
		bool RecvStunPacket(const protocol::Packet& value);
		bool RecvTargetCreatePacketNew(const protocol::Packet& value);
		bool RecvTargetDeletePacket(const protocol::Packet& value);
		bool RecvTargetPacket(const protocol::Packet& value);
		bool RecvTargetUpdatePacket(const protocol::Packet& value);
		bool RecvTimePacket(const protocol::Packet& value);
		bool RecvViewEquipPacket(const protocol::Packet& value);
		bool RecvWalkModePacket(const protocol::Packet& value);
		bool RecvWarpPacket(const protocol::Packet& value);
};
