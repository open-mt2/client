#include <Client/UI/StdAfx.h>
#include <Client/UI/PythonSafeBox.h>

void CPythonSafeBox::OpenSafeBox(int iSize)
{
	m_dwMoney = 0;
	m_ItemInstanceVector.clear();
	m_ItemInstanceVector.resize(SAFEBOX_SLOT_X_COUNT * iSize);

	for (DWORD i = 0; i < m_ItemInstanceVector.size(); ++i)
	{
		TItemData& rInstance = m_ItemInstanceVector[i];
		ZeroMemory(&rInstance, sizeof(rInstance));
	}
}

void CPythonSafeBox::SetItemData(DWORD dwSlotIndex, const TItemData& rItemInstance)
{
	if (dwSlotIndex >= m_ItemInstanceVector.size())
	{
		TraceError("CPythonSafeBox::SetItemData(dwSlotIndex=%d) - Strange slot index", dwSlotIndex);
		return;
	}

	m_ItemInstanceVector[dwSlotIndex] = rItemInstance;
}

void CPythonSafeBox::DelItemData(DWORD dwSlotIndex)
{
	if (dwSlotIndex >= m_ItemInstanceVector.size())
	{
		TraceError("CPythonSafeBox::DelItemData(dwSlotIndex=%d) - Strange slot index", dwSlotIndex);
		return;
	}

	TItemData& rInstance = m_ItemInstanceVector[dwSlotIndex];
	ZeroMemory(&rInstance, sizeof(rInstance));
}

void CPythonSafeBox::SetMoney(DWORD dwMoney)
{
	m_dwMoney = dwMoney;
}

DWORD CPythonSafeBox::GetMoney()
{
	return m_dwMoney;
}

int CPythonSafeBox::GetCurrentSafeBoxSize()
{
	return m_ItemInstanceVector.size();
}

BOOL CPythonSafeBox::GetSlotItemID(DWORD dwSlotIndex, DWORD* pdwItemID)
{
	if (dwSlotIndex >= m_ItemInstanceVector.size())
	{
		TraceError("CPythonSafeBox::GetSlotItemID(dwSlotIndex=%d) - Strange slot index", dwSlotIndex);
		return FALSE;
	}

	*pdwItemID = m_ItemInstanceVector[dwSlotIndex].vnum;

	return TRUE;
}

BOOL CPythonSafeBox::GetItemDataPtr(DWORD dwSlotIndex, TItemData** ppInstance)
{
	if (dwSlotIndex >= m_ItemInstanceVector.size())
	{
		TraceError("CPythonSafeBox::GetItemData(dwSlotIndex=%d) - Strange slot index", dwSlotIndex);
		return FALSE;
	}

	*ppInstance = &m_ItemInstanceVector[dwSlotIndex];

	return TRUE;
}

void CPythonSafeBox::OpenMall(int iSize)
{
	m_MallItemInstanceVector.clear();
	m_MallItemInstanceVector.resize(SAFEBOX_SLOT_X_COUNT * iSize);

	for (DWORD i = 0; i < m_MallItemInstanceVector.size(); ++i)
	{
		TItemData& rInstance = m_MallItemInstanceVector[i];
		ZeroMemory(&rInstance, sizeof(rInstance));
	}
}

void CPythonSafeBox::SetMallItemData(DWORD dwSlotIndex, const TItemData& rItemData)
{
	if (dwSlotIndex >= m_MallItemInstanceVector.size())
	{
		TraceError("CPythonSafeBox::SetMallItemData(dwSlotIndex=%d) - Strange slot index", dwSlotIndex);
		return;
	}

	m_MallItemInstanceVector[dwSlotIndex] = rItemData;
}

void CPythonSafeBox::DelMallItemData(DWORD dwSlotIndex)
{
	if (dwSlotIndex >= m_MallItemInstanceVector.size())
	{
		TraceError("CPythonSafeBox::DelMallItemData(dwSlotIndex=%d) - Strange slot index", dwSlotIndex);
		return;
	}

	TItemData& rInstance = m_MallItemInstanceVector[dwSlotIndex];
	ZeroMemory(&rInstance, sizeof(rInstance));
}

BOOL CPythonSafeBox::GetMallItemDataPtr(DWORD dwSlotIndex, TItemData** ppInstance)
{
	if (dwSlotIndex >= m_MallItemInstanceVector.size())
	{
		TraceError("CPythonSafeBox::GetMallSlotItemID(dwSlotIndex=%d) - Strange slot index", dwSlotIndex);
		return FALSE;
	}

	*ppInstance = &m_MallItemInstanceVector[dwSlotIndex];

	return TRUE;
}

BOOL CPythonSafeBox::GetSlotMallItemID(DWORD dwSlotIndex, DWORD* pdwItemID)
{
	if (dwSlotIndex >= m_MallItemInstanceVector.size())
	{
		TraceError("CPythonSafeBox::GetMallSlotItemID(dwSlotIndex=%d) - Strange slot index", dwSlotIndex);
		return FALSE;
	}

	*pdwItemID = m_MallItemInstanceVector[dwSlotIndex].vnum;

	return TRUE;
}

DWORD CPythonSafeBox::GetMallSize()
{
	return m_MallItemInstanceVector.size();
}

CPythonSafeBox::CPythonSafeBox()
{
	m_dwMoney = 0;
}

CPythonSafeBox::~CPythonSafeBox()
{
}

ScriptObject* safeboxGetCurrentSafeboxSize(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonSafeBox::Instance().GetCurrentSafeBoxSize());
}

ScriptObject* safeboxGetItemID(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int ipos;
	if (!ScriptTuple_GetInteger(poArgs, 0, &ipos))
		return Script_BadArgument();

	TItemData* pInstance;
	if (!CPythonSafeBox::Instance().GetItemDataPtr(ipos, &pInstance))
		return Script_BuildException();

	return Script_BuildValue("i", pInstance->vnum);
}

ScriptObject* safeboxGetItemCount(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int ipos;
	if (!ScriptTuple_GetInteger(poArgs, 0, &ipos))
		return Script_BadArgument();

	TItemData* pInstance;
	if (!CPythonSafeBox::Instance().GetItemDataPtr(ipos, &pInstance))
		return Script_BuildException();

	return Script_BuildValue("i", pInstance->count);
}

ScriptObject* safeboxGetItemFlags(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int ipos;
	if (!ScriptTuple_GetInteger(poArgs, 0, &ipos))
		return Script_BadArgument();

	TItemData* pInstance;
	if (!CPythonSafeBox::Instance().GetItemDataPtr(ipos, &pInstance))
		return Script_BuildException();

	return Script_BuildValue("i", pInstance->flags);
}

ScriptObject* safeboxGetItemMetinSocket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Script_BadArgument();
	int iSocketIndex;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iSocketIndex))
		return Script_BadArgument();

	if (iSocketIndex >= ITEM_SOCKET_SLOT_MAX_NUM)
		return Script_BuildException();

	TItemData* pItemData;
	if (!CPythonSafeBox::Instance().GetItemDataPtr(iSlotIndex, &pItemData))
		return Script_BuildException();

	return Script_BuildValue("i", pItemData->alSockets[iSocketIndex]);
}

ScriptObject* safeboxGetItemAttribute(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Script_BuildException();
	int iAttrSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iAttrSlotIndex))
		return Script_BuildException();

	if (iAttrSlotIndex >= 0 && iAttrSlotIndex < ITEM_ATTRIBUTE_SLOT_MAX_NUM)
	{
		TItemData* pItemData;
		if (CPythonSafeBox::Instance().GetItemDataPtr(iSlotIndex, &pItemData))
			return Script_BuildValue("ii", pItemData->aAttr[iAttrSlotIndex].bType, pItemData->aAttr[iAttrSlotIndex].sValue);
	}

	return Script_BuildValue("ii", 0, 0);
}

ScriptObject* safeboxGetMoney(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonSafeBox::Instance().GetMoney());
}

ScriptObject* safeboxGetMallItemID(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int ipos;
	if (!ScriptTuple_GetInteger(poArgs, 0, &ipos))
		return Script_BadArgument();

	TItemData* pInstance;
	if (!CPythonSafeBox::Instance().GetMallItemDataPtr(ipos, &pInstance))
		return Script_BuildException();

	return Script_BuildValue("i", pInstance->vnum);
}

ScriptObject* safeboxGetMallItemCount(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int ipos;
	if (!ScriptTuple_GetInteger(poArgs, 0, &ipos))
		return Script_BadArgument();

	TItemData* pInstance;
	if (!CPythonSafeBox::Instance().GetMallItemDataPtr(ipos, &pInstance))
		return Script_BuildException();

	return Script_BuildValue("i", pInstance->count);
}

ScriptObject* safeboxGetMallItemMetinSocket(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Script_BadArgument();
	int iSocketIndex;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iSocketIndex))
		return Script_BadArgument();

	if (iSocketIndex >= ITEM_SOCKET_SLOT_MAX_NUM)
		return Script_BuildException();

	TItemData* pItemData;
	if (!CPythonSafeBox::Instance().GetMallItemDataPtr(iSlotIndex, &pItemData))
		return Script_BuildException();

	return Script_BuildValue("i", pItemData->alSockets[iSocketIndex]);
}

ScriptObject* safeboxGetMallItemAttribute(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Script_BuildException();
	int iAttrSlotIndex;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iAttrSlotIndex))
		return Script_BuildException();

	if (iAttrSlotIndex >= 0 && iAttrSlotIndex < ITEM_ATTRIBUTE_SLOT_MAX_NUM)
	{
		TItemData* pItemData;
		if (CPythonSafeBox::Instance().GetMallItemDataPtr(iSlotIndex, &pItemData))
			return Script_BuildValue("ii", pItemData->aAttr[iAttrSlotIndex].bType, pItemData->aAttr[iAttrSlotIndex].sValue);
	}

	return Script_BuildValue("ii", 0, 0);
}

ScriptObject* safeboxGetMallSize(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonSafeBox::Instance().GetMallSize());
}

void initsafebox()
{
	static ScriptMethodDef s_methods[] =
		{
			// SafeBox
			{"GetCurrentSafeboxSize", safeboxGetCurrentSafeboxSize, SCRIPT_VARARGS, NULL},
			{"GetItemID", safeboxGetItemID, SCRIPT_VARARGS, NULL},
			{"GetItemCount", safeboxGetItemCount, SCRIPT_VARARGS, NULL},
			{"GetItemFlags", safeboxGetItemFlags, SCRIPT_VARARGS, NULL},
			{"GetItemMetinSocket", safeboxGetItemMetinSocket, SCRIPT_VARARGS, NULL},
			{"GetItemAttribute", safeboxGetItemAttribute, SCRIPT_VARARGS, NULL},
			{"GetMoney", safeboxGetMoney, SCRIPT_VARARGS, NULL},

			// Mall
			{"GetMallItemID", safeboxGetMallItemID, SCRIPT_VARARGS, NULL},
			{"GetMallItemCount", safeboxGetMallItemCount, SCRIPT_VARARGS, NULL},
			{"GetMallItemMetinSocket", safeboxGetMallItemMetinSocket, SCRIPT_VARARGS, NULL},
			{"GetMallItemAttribute", safeboxGetMallItemAttribute, SCRIPT_VARARGS, NULL},
			{"GetMallSize", safeboxGetMallSize, SCRIPT_VARARGS, NULL},
			{NULL, NULL, 0, NULL},
		};

	ScriptObject* poModule = ScriptModule_Create("safebox", s_methods);
	ScriptModule_AddIntConstant(poModule, "SAFEBOX_SLOT_X_COUNT", CPythonSafeBox::SAFEBOX_SLOT_X_COUNT);
	ScriptModule_AddIntConstant(poModule, "SAFEBOX_SLOT_Y_COUNT", CPythonSafeBox::SAFEBOX_SLOT_Y_COUNT);
	ScriptModule_AddIntConstant(poModule, "SAFEBOX_PAGE_SIZE", CPythonSafeBox::SAFEBOX_PAGE_SIZE);
}
