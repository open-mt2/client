#pragma once

#include <Client/Utils/AbstractSingleton.h>

class IAbstractChat : public TAbstractSingleton<IAbstractChat>
{
	public:
		IAbstractChat() {}
		virtual ~IAbstractChat() {}

		virtual void AppendChat(int iType, const char* c_szChat) = 0;
};