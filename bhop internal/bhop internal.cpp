#include <Windows.h>
#include <iostream>

struct offsets
{
	DWORD m_fFlags = 0x100;
	DWORD dwForceJump = 0x50E13BC;
	DWORD dwLocalPlayer = 0xC618AC;
	DWORD m_vecVelocity = 0x110;
}	offset;

struct variables
{
	DWORD localPlayer;
	DWORD gameModule;
	BYTE flag;
}	val;

struct vector
{
	float x, y, z;
};

bool isPlayerMoving()
{
	vector playerVel = *(vector*)(val.localPlayer + offset.m_vecVelocity);
	int vel = playerVel.x + playerVel.y + playerVel.z;
	if (vel != 0)
		return true;
	else
		return false;
}

void main()
{
	val.gameModule = (DWORD)GetModuleHandle("client_panorama.dll");
	val.localPlayer = *(DWORD*)(val.gameModule + offset.dwLocalPlayer);

	if (val.localPlayer == NULL)
		while (val.localPlayer == NULL)
			val.localPlayer = *(DWORD*)(val.gameModule + offset.dwLocalPlayer);

	while (true)
	{
		val.flag = *(BYTE*)(val.localPlayer + offset.m_fFlags);

		if (isPlayerMoving())
			if (GetAsyncKeyState(VK_SPACE) && val.flag & (1 << 0))
				*(DWORD*)(val.gameModule + offset.dwForceJump) = 6;
	}
}


BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(hModule);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)main, NULL, NULL, NULL);
	}
	return TRUE;
}

