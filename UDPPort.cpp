#include "pch.h"
#include "UDPPort.h"
#include <Windows.h>
#include <WinSock.h>
#include <tcpmib.h>
#include <IPHlpApi.h>
#include <queue>
#include <set>
#include <memory>

#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "IPHlpApi.lib")

#define Social_Port_Min 5001
#define Social_Port_Max 9999
#define Battle_Port_Min 10000
#define Battle_Port_Max 11000

using std::queue;
using std::set;
using std::unique_ptr;

set<int> usedSocialUDPPorts;
queue<int> freeSocialUDPPorts;
set<int> usedBattleUDPPorts;
queue<int> freeBattleUDPPorts;

void SetFreeSocialUDPPorts() {
	usedSocialUDPPorts.clear();
	freeSocialUDPPorts = queue<int>();
	ULONG size = 0;
	GetUdpTable(NULL, &size, TRUE);
	unique_ptr<char[]>  buffer(new char[size]);
	PMIB_UDPTABLE udpTable = reinterpret_cast<PMIB_UDPTABLE>(buffer.get());
	if (GetUdpTable(udpTable, &size, FALSE) == NO_ERROR) {
		for (size_t i = 0; i < udpTable->dwNumEntries; i++) {
			int port = (int)ntohs(udpTable->table[i].dwLocalPort);
			usedSocialUDPPorts.insert(port);
		}
		for (int i = Social_Port_Min; i <= Social_Port_Max; i++) {
			if (!usedSocialUDPPorts.count(i))
				freeSocialUDPPorts.push(i);
		}
	}
}

int GetFreeSocialUDPPort() {
	if (freeSocialUDPPorts.empty())
		SetFreeSocialUDPPorts();
	if (freeSocialUDPPorts.empty())
		return -1;
	int port = freeSocialUDPPorts.front();
	freeSocialUDPPorts.pop();
	return port;
}

void SetFreeBattleUDPPorts() {
	usedBattleUDPPorts.clear();
	freeSocialUDPPorts = queue<int>();
	ULONG size = 0;
	GetUdpTable(NULL, &size, TRUE);
	unique_ptr<char[]>  buffer(new char[size]);
	PMIB_UDPTABLE udpTable = reinterpret_cast<PMIB_UDPTABLE>(buffer.get());
	if (GetUdpTable(udpTable, &size, FALSE) == NO_ERROR) {
		for (size_t i = 0; i < udpTable->dwNumEntries; i++) {
			int port = (int)ntohs(udpTable->table[i].dwLocalPort);
			usedBattleUDPPorts.insert(port);
		}
		for (int i = Battle_Port_Min; i <= Battle_Port_Max; i++) {
			if (!usedBattleUDPPorts.count(i))
				freeBattleUDPPorts.push(i);
		}
	}
}

int GetFreeBattleUDPPort() {
	if (freeBattleUDPPorts.empty())
		SetFreeBattleUDPPorts();
	if (freeBattleUDPPorts.empty())
		return -1;
	int port = freeBattleUDPPorts.front();
	freeBattleUDPPorts.pop();
	return port;
}

