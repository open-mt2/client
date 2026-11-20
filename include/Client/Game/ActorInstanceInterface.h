#pragma once

#include <Neoeter/Model/Grn/ThingInstance.h>

class IActorInstance : public CGraphicThingInstance
{
	public:
		enum
		{
			ID = ACTOR_OBJECT
		};
		int GetType() const { return ID; }

		IActorInstance() {}
		virtual ~IActorInstance() {}
		virtual bool TestCollisionWithDynamicSphere(const CDynamicSphereInstance& dsi) = 0;
		virtual uint32_t GetVirtualID() = 0;
};