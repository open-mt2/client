#pragma once

#include <Neoeter/Model/Grn/ThingInstance.h>
#include <Client/Map/MapType.h>

#include <cstdint>

class CActorInstance;

class CActorInstanceCollisionDetection
{
	public:
		explicit CActorInstanceCollisionDetection(CActorInstance& rkOwner);

		int32_t AttackingProcess(CActorInstance& rVictim);
		bool AvoidObject(const CGraphicObjectInstance& c_rkBGObj);
		void BlockMovement();
		bool CanSkipCollision();
		bool CheckCollisionDetection(const CDynamicSphereInstanceVector* c_pAttackingSphereVector, D3DXVECTOR3* pv3Position);
		void DisableSkipCollision();
		void EnableSkipCollision();
		bool IsBlockObject(const CGraphicObjectInstance& c_rkBGObj);
		int32_t TestActorCollision(CActorInstance& rVictim);
		int32_t TestPhysicsBlendingCollision(CActorInstance& rVictim);
		void UpdateAdvancingPointInstance();
		void UpdatePointInstance();
		void __InitializeCollisionData();
		int32_t __NormalAttackProcess(CActorInstance& rVictim);
		int32_t __SplashAttackProcess(CActorInstance& rVictim);
		int32_t __TestObjectCollision(const CGraphicObjectInstance* c_pObjectInstance);

	private:
		CActorInstance& m_rkOwner;
};
