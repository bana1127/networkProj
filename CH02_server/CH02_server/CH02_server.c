#include <stdio.h>
#include <WinSock2.h> //���� �Լ�
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

	//������ �ۼ��� �غ�
	//1.���� ����.
	SOCKET handle;
	handle = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);



	WSACleanup();
	return 0;
}
