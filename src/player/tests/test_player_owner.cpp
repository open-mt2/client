#include <cassert>
#include <cstdio>

#include <Client/UI/StdAfx.h>
#include <Client/Feature/Character/PythonCharacterManager.h>
#include <Client/Player/PythonPlayer.h>

int main()
{
	CPythonSkill skill;
	CPythonCharacterManager characterManager;
	CPythonPlayer player;

	player.SetMainCharacterIndex(1234);
	assert(player.GetMainCharacterIndex() == 1234);
	assert(player.IsMainCharacterIndex(1234));
	assert(!player.IsMainCharacterIndex(1235));

	player.SetMainCharacterIndex(5678);
	assert(player.GetMainCharacterIndex() == 5678);
	assert(player.IsMainCharacterIndex(5678));
	assert(!player.IsMainCharacterIndex(1234));

	std::printf("player owner tests passed\n");
	return 0;
}
