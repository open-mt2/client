#include <Client/UI/StdAfx.h>
#include <Client/UI/PythonTextTail.h>
#include <Client/UI/PythonCharacterManager.h>

ScriptObject* textTailClear(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonTextTail::Instance().Clear();
	return Script_BuildNone();
}

ScriptObject* textTailUpdateAllTextTail(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonTextTail::Instance().UpdateAllTextTail();
	return Script_BuildNone();
}

ScriptObject* textTailUpdateShowingTextTail(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonTextTail::Instance().UpdateShowingTextTail();
	return Script_BuildNone();
}

ScriptObject* textTailRender(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonTextTail::Instance().Render();
	return Script_BuildNone();
}

ScriptObject* textTailRegisterCharacterTextTail(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iGuildID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iGuildID))
		return Script_BuildException();
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iVirtualID))
		return Script_BuildException();

	CPythonTextTail::Instance().RegisterCharacterTextTail(iGuildID, iVirtualID, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	return Script_BuildNone();
}

ScriptObject* textTailGetPosition(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int VirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &VirtualID))
		return Script_BuildException();

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	bool isData = CPythonTextTail::Instance().GetTextTailPosition(VirtualID, &x, &y, &z);
	if (!isData)
	{
		CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
		CInstanceBase* pkInstMain = rkChrMgr.GetMainInstancePtr();
		if (pkInstMain)
		{
			const D3DXVECTOR3& c_rv3Position = pkInstMain->GetGraphicThingInstanceRef().GetPosition();
			CPythonGraphic::Instance().ProjectPositionUI(c_rv3Position.x, c_rv3Position.y, c_rv3Position.z, &x, &y);
		}
	}

	return Script_BuildValue("fff", x, y, z);
}

ScriptObject* textTailIsChat(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int VirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &VirtualID))
		return Script_BuildException();

	return Script_BuildValue("i", CPythonTextTail::Instance().IsChatTextTail(VirtualID));
}

ScriptObject* textTailRegisterChatTail(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int VirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &VirtualID))
		return Script_BuildException();

	char* szText;
	if (!ScriptTuple_GetString(poArgs, 1, &szText))
		return Script_BuildException();

	CPythonTextTail::Instance().RegisterChatTail(VirtualID, szText);

	return Script_BuildNone();
}

ScriptObject* textTailRegisterInfoTail(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int VirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &VirtualID))
		return Script_BuildException();

	char* szText;
	if (!ScriptTuple_GetString(poArgs, 1, &szText))
		return Script_BuildException();

	CPythonTextTail::Instance().RegisterInfoTail(VirtualID, szText);

	return Script_BuildNone();
}

ScriptObject* textTailAttachTitle(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Script_BuildException();
	char* szName;
	if (!ScriptTuple_GetString(poArgs, 1, &szName))
		return Script_BuildException();
	float fr;
	if (!ScriptTuple_GetFloat(poArgs, 2, &fr))
		return Script_BuildException();
	float fg;
	if (!ScriptTuple_GetFloat(poArgs, 3, &fg))
		return Script_BuildException();
	float fb;
	if (!ScriptTuple_GetFloat(poArgs, 4, &fb))
		return Script_BuildException();

	CPythonTextTail::Instance().AttachTitle(iVirtualID, szName, D3DXCOLOR(fr, fg, fb, 1.0f));

	return Script_BuildNone();
}

ScriptObject* textTailShowCharacterTextTail(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int VirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &VirtualID))
		return Script_BuildException();

	CPythonTextTail::Instance().ShowCharacterTextTail(VirtualID);
	return Script_BuildNone();
}

ScriptObject* textTailShowItemTextTail(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int VirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &VirtualID))
		return Script_BuildException();

	CPythonTextTail::Instance().ShowItemTextTail(VirtualID);
	return Script_BuildNone();
}

ScriptObject* textTailArrangeTextTail(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonTextTail::Instance().ArrangeTextTail();
	return Script_BuildNone();
}

ScriptObject* textTailHideAllTextTail(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonTextTail::Instance().HideAllTextTail();
	return Script_BuildNone();
}

ScriptObject* textTailShowAllTextTail(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonTextTail::Instance().ShowAllTextTail();
	return Script_BuildNone();
}

ScriptObject* textTailPick(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int ix;
	if (!ScriptTuple_GetInteger(poArgs, 0, &ix))
		return Script_BuildException();
	int iy;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iy))
		return Script_BuildException();

	int iValue = CPythonTextTail::Instance().Pick(ix, iy);
	return Script_BuildValue("i", iValue);
}

ScriptObject* textTailSelectItemName(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iVirtualID;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Script_BuildException();

	CPythonTextTail::Instance().SelectItemName(iVirtualID);
	return Script_BuildNone();
}

ScriptObject* textTailEnablePKTitle(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int iFlag;
	if (!ScriptTuple_GetInteger(poArgs, 0, &iFlag))
		return Script_BuildException();

	CPythonTextTail::Instance().EnablePKTitle(iFlag);
	return Script_BuildNone();
}

void initTextTail()
{
	static ScriptMethodDef s_methods[] =
		{
			{"Clear", textTailClear, SCRIPT_VARARGS, NULL},

			{"UpdateAllTextTail", textTailUpdateAllTextTail, SCRIPT_VARARGS, NULL},
			{"UpdateShowingTextTail", textTailUpdateShowingTextTail, SCRIPT_VARARGS, NULL},
			{"Render", textTailRender, SCRIPT_VARARGS, NULL},

			{"ShowCharacterTextTail", textTailShowCharacterTextTail, SCRIPT_VARARGS, NULL},
			{"ShowItemTextTail", textTailShowItemTextTail, SCRIPT_VARARGS, NULL},

			{"GetPosition", textTailGetPosition, SCRIPT_VARARGS, NULL},
			{"IsChat", textTailIsChat, SCRIPT_VARARGS, NULL},

			{"ArrangeTextTail", textTailArrangeTextTail, SCRIPT_VARARGS, NULL},
			{"HideAllTextTail", textTailHideAllTextTail, SCRIPT_VARARGS, NULL},
			{"ShowAllTextTail", textTailShowAllTextTail, SCRIPT_VARARGS, NULL},

			{"Pick", textTailPick, SCRIPT_VARARGS, NULL},
			{"SelectItemName", textTailSelectItemName, SCRIPT_VARARGS, NULL},

			{"EnablePKTitle", textTailEnablePKTitle, SCRIPT_VARARGS, NULL},

			// For Test
			{"RegisterCharacterTextTail", textTailRegisterCharacterTextTail, SCRIPT_VARARGS, NULL},
			{"RegisterChatTail", textTailRegisterChatTail, SCRIPT_VARARGS, NULL},
			{"RegisterInfoTail", textTailRegisterInfoTail, SCRIPT_VARARGS, NULL},
			{"AttachTitle", textTailAttachTitle, SCRIPT_VARARGS, NULL},

			{NULL, NULL, 0, NULL},
		};

	ScriptModule_Create("textTail", s_methods);
}