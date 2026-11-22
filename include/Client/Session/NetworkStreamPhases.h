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

		virtual ENetworkStreamPhaseResult Handle(uint8_t header, const OMT2Client::Protocol::Packet& value) = 0;

	protected:
		CPythonNetworkStream& m_rkStream;
		friend class CPythonNetworkStream;
};

class CNetworkStreamOfflinePhase : public CNetworkStreamPhase
{
	public:
		explicit CNetworkStreamOfflinePhase(CPythonNetworkStream& rkStream);
		ENetworkStreamPhaseResult Handle(uint8_t header, const OMT2Client::Protocol::Packet& value) override;

		void OffLinePhase();
};

class CNetworkStreamHandShakePhase : public CNetworkStreamPhase
{
	public:
		explicit CNetworkStreamHandShakePhase(CPythonNetworkStream& rkStream);
		ENetworkStreamPhaseResult Handle(uint8_t header, const OMT2Client::Protocol::Packet& value) override;

		bool RecvHandshakeOKPacket(const OMT2Client::Protocol::Packet& value);
};

class CNetworkStreamSelectPhase : public CNetworkStreamPhase
{
	public:
		explicit CNetworkStreamSelectPhase(CPythonNetworkStream& rkStream);
		ENetworkStreamPhaseResult Handle(uint8_t header, const OMT2Client::Protocol::Packet& value) override;

		bool __RecvChangeName(const OMT2Client::Protocol::Packet& value);
		bool __RecvPlayerCreateFailurePacket(const OMT2Client::Protocol::Packet& value);
		bool __RecvPlayerCreateSuccessPacket(const OMT2Client::Protocol::Packet& value);
		bool __RecvPlayerDestroyFailurePacket(const OMT2Client::Protocol::Packet& value);
		bool __RecvPlayerDestroySuccessPacket(const OMT2Client::Protocol::Packet& value);
};

class CNetworkStreamLoginPhase : public CNetworkStreamPhase
{
	public:
		explicit CNetworkStreamLoginPhase(CPythonNetworkStream& rkStream);
		ENetworkStreamPhaseResult Handle(uint8_t header, const OMT2Client::Protocol::Packet& value) override;

		bool __RecvChinaMatrixCardPacket(const OMT2Client::Protocol::Packet& value);
		bool __RecvEmpirePacket(const OMT2Client::Protocol::Packet& value);
		bool __RecvLoginFailurePacket(const OMT2Client::Protocol::Packet& value);
		bool __RecvLoginKeyPacket(const OMT2Client::Protocol::Packet& value);
		bool __RecvLoginSuccessPacket3(const OMT2Client::Protocol::Packet& value);
		bool __RecvLoginSuccessPacket4(const OMT2Client::Protocol::Packet& value);
		bool __RecvNEWCIBNPasspodFailurePacket(const OMT2Client::Protocol::Packet& value);
		bool __RecvNEWCIBNPasspodRequestPacket(const OMT2Client::Protocol::Packet& value);
		bool __RecvRunupMatrixQuizPacket(const OMT2Client::Protocol::Packet& value);
};

class CNetworkStreamLoadingPhase : public CNetworkStreamPhase
{
	public:
		explicit CNetworkStreamLoadingPhase(CPythonNetworkStream& rkStream);
		ENetworkStreamPhaseResult Handle(uint8_t header, const OMT2Client::Protocol::Packet& value) override;

		bool RecvMainCharacter(const OMT2Client::Protocol::Packet& value);
		bool RecvMainCharacter2_EMPIRE(const OMT2Client::Protocol::Packet& value);
		bool RecvMainCharacter3_BGM(const OMT2Client::Protocol::Packet& value);
		bool RecvMainCharacter4_BGM_VOL(const OMT2Client::Protocol::Packet& value);
		bool __RecvPlayerPoints(const OMT2Client::Protocol::Packet& value);
};

class CNetworkStreamGameActorPhase : public CNetworkStreamPhase
{
	public:
		explicit CNetworkStreamGameActorPhase(CPythonNetworkStream& rkStream);
		ENetworkStreamPhaseResult Handle(uint8_t header, const OMT2Client::Protocol::Packet& value) override;

		bool RecvCharacterAdditionalInfo(const OMT2Client::Protocol::Packet& value);
		bool RecvCharacterAppendPacket(const OMT2Client::Protocol::Packet& value);
		bool RecvCharacterAppendPacketNew(const OMT2Client::Protocol::Packet& value);
		bool RecvCharacterDeletePacket(const OMT2Client::Protocol::Packet& value);
		bool RecvCharacterMovePacket(const OMT2Client::Protocol::Packet& value);
		bool RecvCharacterUpdatePacket(const OMT2Client::Protocol::Packet& value);
		bool RecvCharacterUpdatePacketNew(const OMT2Client::Protocol::Packet& value);
		bool RecvOwnerShipPacket(const OMT2Client::Protocol::Packet& value);
};

class CNetworkStreamGameItemPhase : public CNetworkStreamPhase
{
	public:
		explicit CNetworkStreamGameItemPhase(CPythonNetworkStream& rkStream);
		ENetworkStreamPhaseResult Handle(uint8_t header, const OMT2Client::Protocol::Packet& value) override;

		bool RecvDragonSoulRefine(const OMT2Client::Protocol::Packet& value);
		bool RecvItemGroundAddPacket(const OMT2Client::Protocol::Packet& value);
		bool RecvItemGroundDelPacket(const OMT2Client::Protocol::Packet& value);
		bool RecvItemOwnership(const OMT2Client::Protocol::Packet& value);
		bool RecvItemSetPacket(const OMT2Client::Protocol::Packet& value);	  // Alarm to python
		bool RecvItemSetPacket2(const OMT2Client::Protocol::Packet& value);	  // Alarm to python
		bool RecvItemUpdatePacket(const OMT2Client::Protocol::Packet& value); // Alarm to python
		bool RecvItemUsePacket(const OMT2Client::Protocol::Packet& value);	  // Alarm to python
		bool RecvMallItemDelPacket(const OMT2Client::Protocol::Packet& value);
		bool RecvMallItemSetPacket(const OMT2Client::Protocol::Packet& value);
		bool RecvMallOpenPacket(const OMT2Client::Protocol::Packet& value);
		bool RecvQuickSlotAddPacket(const OMT2Client::Protocol::Packet& value);	 // Alarm to python
		bool RecvQuickSlotDelPacket(const OMT2Client::Protocol::Packet& value);	 // Alarm to python
		bool RecvQuickSlotMovePacket(const OMT2Client::Protocol::Packet& value); // Alarm to python
		bool RecvSafeBoxDelPacket(const OMT2Client::Protocol::Packet& value);
		bool RecvSafeBoxMoneyChangePacket(const OMT2Client::Protocol::Packet& value);
		bool RecvSafeBoxSetPacket(const OMT2Client::Protocol::Packet& value);
		bool RecvSafeBoxSizePacket(const OMT2Client::Protocol::Packet& value);
		bool RecvSafeBoxWrongPasswordPacket(const OMT2Client::Protocol::Packet& value);
		bool RecvSpecialEffect(const OMT2Client::Protocol::Packet& value);
		bool RecvSpecificEffect(const OMT2Client::Protocol::Packet& value);
};

class CNetworkStreamGamePhase : public CNetworkStreamPhase
{
	public:
		explicit CNetworkStreamGamePhase(CPythonNetworkStream& rkStream);
		ENetworkStreamPhaseResult Handle(uint8_t header, const OMT2Client::Protocol::Packet& value) override;

		bool RecvAddFlyTargetingPacket(const OMT2Client::Protocol::Packet& value);
		bool RecvAffectAddPacket(const OMT2Client::Protocol::Packet& value);
		bool RecvAffectRemovePacket(const OMT2Client::Protocol::Packet& value);
		bool RecvChangeSkillGroupPacket(const OMT2Client::Protocol::Packet& value);
		bool RecvChangeSpeedPacket(const OMT2Client::Protocol::Packet& value);
		bool RecvChannelPacket(const OMT2Client::Protocol::Packet& value);
		bool RecvCharacterPositionPacket(const OMT2Client::Protocol::Packet& value);
		bool RecvCreateFlyPacket(const OMT2Client::Protocol::Packet& value);
		bool RecvDamageInfoPacket(const OMT2Client::Protocol::Packet& value);
		bool RecvDeadPacket(const OMT2Client::Protocol::Packet& value);
		bool RecvDigMotionPacket(const OMT2Client::Protocol::Packet& value);
		bool RecvExchangePacket(const OMT2Client::Protocol::Packet& value);
		bool RecvFishing(const OMT2Client::Protocol::Packet& value);
		bool RecvFlyTargetingPacket(const OMT2Client::Protocol::Packet& value);
		bool RecvLovePointUpdatePacket(const OMT2Client::Protocol::Packet& value);
		bool RecvLoverInfoPacket(const OMT2Client::Protocol::Packet& value);
		bool RecvMotionPacket(const OMT2Client::Protocol::Packet& value);
		bool RecvMountPacket(const OMT2Client::Protocol::Packet& value);
		bool RecvPVPPacket(const OMT2Client::Protocol::Packet& value);
		bool RecvPartyAdd(const OMT2Client::Protocol::Packet& value);
		bool RecvPartyInvite(const OMT2Client::Protocol::Packet& value);
		bool RecvPartyLink(const OMT2Client::Protocol::Packet& value);
		bool RecvPartyParameter(const OMT2Client::Protocol::Packet& value);
		bool RecvPartyRemove(const OMT2Client::Protocol::Packet& value);
		bool RecvPartyUnlink(const OMT2Client::Protocol::Packet& value);
		bool RecvPartyUpdate(const OMT2Client::Protocol::Packet& value);
		bool RecvPointChange(const OMT2Client::Protocol::Packet& value); // Alarm to python
		bool RecvQuestConfirmPacket(const OMT2Client::Protocol::Packet& value);
		bool RecvRefineInformationPacket(const OMT2Client::Protocol::Packet& value);
		bool RecvRefineInformationPacketNew(const OMT2Client::Protocol::Packet& value);
		bool RecvRequestMakeGuild(const OMT2Client::Protocol::Packet& value);
		bool RecvShopSignPacket(const OMT2Client::Protocol::Packet& value);
		bool RecvSkillLevel(const OMT2Client::Protocol::Packet& value);
		bool RecvSkillLevelNew(const OMT2Client::Protocol::Packet& value);
		bool RecvStunPacket(const OMT2Client::Protocol::Packet& value);
		bool RecvTargetCreatePacketNew(const OMT2Client::Protocol::Packet& value);
		bool RecvTargetDeletePacket(const OMT2Client::Protocol::Packet& value);
		bool RecvTargetPacket(const OMT2Client::Protocol::Packet& value);
		bool RecvTargetUpdatePacket(const OMT2Client::Protocol::Packet& value);
		bool RecvTimePacket(const OMT2Client::Protocol::Packet& value);
		bool RecvViewEquipPacket(const OMT2Client::Protocol::Packet& value);
		bool RecvWalkModePacket(const OMT2Client::Protocol::Packet& value);
		bool RecvWarpPacket(const OMT2Client::Protocol::Packet& value);
};
