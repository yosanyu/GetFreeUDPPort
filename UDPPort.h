#pragma once

#ifdef UE4Dll_EXPORTS
#define UE4Dll_API __declspec(dllexport)
#else
#define UE4Dll_API __declspec(dllimport)
#endif

extern "C" UE4Dll_API void SetFreeSocialUDPPorts();
extern "C" UE4Dll_API int GetFreeSocialUDPPort();
extern "C" UE4Dll_API void SetFreeBattleUDPPorts();
extern "C" UE4Dll_API int GetFreeBattleUDPPort();