#include <Client/Game/StdAfx.h>
#include <Client/Game/MapActorFilter.h>
#include <Client/Game/ActorInstance.h>

bool IsMapBuildingActor(CGraphicObjectInstance* object)
{
	return static_cast<CActorInstance*>(object)->IsBuilding();
}
