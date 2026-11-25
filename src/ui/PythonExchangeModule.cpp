#include <Client/UI/StdAfx.h>
#include <Client/UI/PythonExchange.h>

ScriptObject* exchangeInitTrading(ScriptObject* poSelf, ScriptObject* poArgs)
{
	CPythonExchange::Instance().End();
	return Script_BuildNone();
}

ScriptObject* exchangeisTrading(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonExchange::Instance().isTrading());
}

ScriptObject* exchangeGetElkFromSelf(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonExchange::Instance().GetElkFromSelf());
}

ScriptObject* exchangeGetElkFromTarget(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonExchange::Instance().GetElkFromTarget());
}

ScriptObject* exchangeGetAcceptFromSelf(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonExchange::Instance().GetAcceptFromSelf());
}

ScriptObject* exchangeGetAcceptFromTarget(ScriptObject* poSelf, ScriptObject* poArgs)
{
	return Script_BuildValue("i", CPythonExchange::Instance().GetAcceptFromTarget());
}

ScriptObject* exchangeGetItemVnumFromSelf(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int pos;

	if (!ScriptTuple_GetInteger(poArgs, 0, &pos))
		return Script_BuildException();

	return Script_BuildValue("i", CPythonExchange::Instance().GetItemVnumFromSelf((char)pos));
}

ScriptObject* exchangeGetItemVnumFromTarget(ScriptObject* poTarget, ScriptObject* poArgs)
{
	int pos;

	if (!ScriptTuple_GetInteger(poArgs, 0, &pos))
		return Script_BuildException();

	return Script_BuildValue("i", CPythonExchange::Instance().GetItemVnumFromTarget((char)pos));
}

ScriptObject* exchangeGetItemCountFromSelf(ScriptObject* poSelf, ScriptObject* poArgs)
{
	int pos;

	if (!ScriptTuple_GetInteger(poArgs, 0, &pos))
		return Script_BuildException();

	return Script_BuildValue("i", CPythonExchange::Instance().GetItemCountFromSelf((char)pos));
}

ScriptObject* exchangeGetItemCountFromTarget(ScriptObject* poTarget, ScriptObject* poArgs)
{
	int pos;

	if (!ScriptTuple_GetInteger(poArgs, 0, &pos))
		return Script_BuildException();

	return Script_BuildValue("i", CPythonExchange::Instance().GetItemCountFromTarget((char)pos));
}

ScriptObject* exchangeGetNameFromSelf(ScriptObject* poTarget, ScriptObject* poArgs)
{
	return Script_BuildValue("s", CPythonExchange::Instance().GetNameFromSelf());
}

ScriptObject* exchangeGetNameFromTarget(ScriptObject* poTarget, ScriptObject* poArgs)
{
	return Script_BuildValue("s", CPythonExchange::Instance().GetNameFromTarget());
}

ScriptObject* exchangeGetItemMetinSocketFromTarget(ScriptObject* poTarget, ScriptObject* poArgs)
{
	int pos;
	if (!ScriptTuple_GetInteger(poArgs, 0, &pos))
		return Script_BuildException();
	int iMetinSocketPos;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iMetinSocketPos))
		return Script_BuildException();
	return Script_BuildValue("i", CPythonExchange::Instance().GetItemMetinSocketFromTarget(pos, iMetinSocketPos));
}

ScriptObject* exchangeGetItemMetinSocketFromSelf(ScriptObject* poTarget, ScriptObject* poArgs)
{
	int pos;
	if (!ScriptTuple_GetInteger(poArgs, 0, &pos))
		return Script_BuildException();
	int iMetinSocketPos;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iMetinSocketPos))
		return Script_BuildException();
	return Script_BuildValue("i", CPythonExchange::Instance().GetItemMetinSocketFromSelf(pos, iMetinSocketPos));
}

ScriptObject* exchangeGetItemAttributeFromTarget(ScriptObject* poTarget, ScriptObject* poArgs)
{
	int pos;
	if (!ScriptTuple_GetInteger(poArgs, 0, &pos))
		return Script_BuildException();
	int iAttrSlotPos;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iAttrSlotPos))
		return Script_BuildException();

	BYTE byType;
	short sValue;
	CPythonExchange::Instance().GetItemAttributeFromTarget(pos, iAttrSlotPos, &byType, &sValue);

	return Script_BuildValue("ii", byType, sValue);
}

ScriptObject* exchangeGetItemAttributeFromSelf(ScriptObject* poTarget, ScriptObject* poArgs)
{
	int pos;
	if (!ScriptTuple_GetInteger(poArgs, 0, &pos))
		return Script_BuildException();
	int iAttrSlotPos;
	if (!ScriptTuple_GetInteger(poArgs, 1, &iAttrSlotPos))
		return Script_BuildException();

	BYTE byType;
	short sValue;
	CPythonExchange::Instance().GetItemAttributeFromSelf(pos, iAttrSlotPos, &byType, &sValue);

	return Script_BuildValue("ii", byType, sValue);
}

ScriptObject* exchangeGetElkMode(ScriptObject* poTarget, ScriptObject* poArgs)
{
	return Script_BuildValue("b", CPythonExchange::Instance().GetElkMode());
}

ScriptObject* exchangeSetElkMode(ScriptObject* poTarget, ScriptObject* poArgs)
{
	int elk_mode;

	if (!ScriptTuple_GetInteger(poArgs, 0, &elk_mode))
		return Script_BuildException();

	CPythonExchange::Instance().SetElkMode(elk_mode ? true : false);
	return Script_BuildNone();
}

void initTrade()
{
	static ScriptMethodDef s_methods[] =
		{
			{"InitTrading", exchangeInitTrading, SCRIPT_VARARGS, NULL},
			{"isTrading", exchangeisTrading, SCRIPT_VARARGS, NULL},

			{"GetElkFromSelf", exchangeGetElkFromSelf, SCRIPT_VARARGS, NULL},
			{"GetElkFromTarget", exchangeGetElkFromTarget, SCRIPT_VARARGS, NULL},

			{"GetItemVnumFromSelf", exchangeGetItemVnumFromSelf, SCRIPT_VARARGS, NULL},
			{"GetItemVnumFromTarget", exchangeGetItemVnumFromTarget, SCRIPT_VARARGS, NULL},

			{"GetItemCountFromSelf", exchangeGetItemCountFromSelf, SCRIPT_VARARGS, NULL},
			{"GetItemCountFromTarget", exchangeGetItemCountFromTarget, SCRIPT_VARARGS, NULL},

			{"GetAcceptFromSelf", exchangeGetAcceptFromSelf, SCRIPT_VARARGS, NULL},
			{"GetAcceptFromTarget", exchangeGetAcceptFromTarget, SCRIPT_VARARGS, NULL},

			{"GetNameFromSelf", exchangeGetNameFromSelf, SCRIPT_VARARGS, NULL},
			{"GetNameFromTarget", exchangeGetNameFromTarget, SCRIPT_VARARGS, NULL},

			{"GetItemMetinSocketFromTarget", exchangeGetItemMetinSocketFromTarget, SCRIPT_VARARGS, NULL},
			{"GetItemMetinSocketFromSelf", exchangeGetItemMetinSocketFromSelf, SCRIPT_VARARGS, NULL},

			{"GetItemAttributeFromTarget", exchangeGetItemAttributeFromTarget, SCRIPT_VARARGS, NULL},
			{"GetItemAttributeFromSelf", exchangeGetItemAttributeFromSelf, SCRIPT_VARARGS, NULL},

			{"GetElkMode", exchangeGetElkMode, SCRIPT_VARARGS, NULL},
			{"SetElkMode", exchangeSetElkMode, SCRIPT_VARARGS, NULL},

			{NULL, NULL, 0, NULL},
		};

	ScriptObject* poModule = ScriptModule_Create("exchange", s_methods);
	ScriptModule_AddIntConstant(poModule, "EXCHANGE_ITEM_MAX_NUM", CPythonExchange::EXCHANGE_ITEM_MAX_NUM);
}