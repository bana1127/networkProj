#include <stdio.h>
#include <WinSock2.h> //家南 窃荐
#define SHOW_DEBUG_MSG

int main() {
	WSADATA ws;
	int ret;
	ret = WSAStartup(MAKEWORD(2, 2), &ws);
#ifdef SHOW_DEBUG_MSG
	if(ret != 0){
		printf("<ERROR> WSAStartup() error\n error code=%d\n", WSAGetLastError);
		return 1;
	}
#endif

	//单捞磐 价荐脚 霖厚
	//1.家南 积己.
	SOCKET handle;
	handle = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);



	WSACleanup();
	return 0;
}
