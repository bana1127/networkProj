#include <stdio.h>
#include <WinSock2.h>

int main() {
	WSADATA wsa;
	WSAStartup(MAKEWORD(2,2), &wsa); //DLL ��������
	//1. ���� ����.
	SOCKET hSocket;
	hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //���� ���� & TCP/UDP ����
	if (hSocket == INVALID_SOCKET) {
		printf("<ERROR> socket() fail. \n");
		WSACleanup();
		return 1;
	}
	//2. server case:
	// ������ �ּ� (IP, Port) ����
	SOCKADDR_IN svrAddr;//IPv4

	memset(&svrAddr, 0, sizeof(svrAddr));
	svrAddr.sin_family = AF_INET; //ipv4�ǹ�
	svrAddr.sin_port = htons(0x0809); // host(B/L) or --> big 16��Ʈ�� hton �ڿ� s����
	svrAddr.sin_addr.S_un.S_addr = inet_addr("1.2.3.4");// 32bit ����

#if 0
	//inet_addr : �ּ� string "1.2.3.4" --> �ּ� (big en)
	//inet_ntoa : �ּ� ---> �ּ� string "1.2.3.4"

	char *tempIPstr = inet_ntoa(svrAddr.sin_addr);
	printf("inet_ntoa> str IP addr = %s \n", tempIPstr);
#endif

	bind(hSocket, (SOCKADDR *)&svrAddr, sizeof(svrAddr));

	//WSAStringtoAddress
	char tmpStrIP[50] = "1.2.3.4:9000";
	int size;
	size = sizeof(svrAddr);
	WSAStringToAddressA(
		tmpStrIP, //�ּ� ��Ʈ���� ���� �ּ�
		AF_INET,//ipv4
		NULL, //�������� ���ι��̴�
		(SOCKADDR*)&svrAddr, // IPv4 �ּ� SOCKADDR_IN ���.. ip, port ���� ����/�ּұ���ü
		&size
	);

	printf("WSAStrTodAddr> Port: 0x%x, IP : 0x%x\n", ntohs(svrAddr.sin_port), ntohl(svrAddr.sin_addr.S_un.S_addr));

	//WSAAdressToString
	char tmpIPstr[50];
	size = sizeof(tmpIPstr);
	WSAAddressToStringA(
		(SOCKADDR*)&svrAddr, //1.2.3.4:9000
		sizeof(svrAddr),
		NULL,
		tmpIPstr,
		&size
	);
	printf("WSAAdressToString> Addr str = %s \n", tmpIPstr);

#if 0
	short net_port = svrAddr.sin_port; // port 16bits, Big or�� ����
	short host_port = 0x0809; // CPU or(host order)�� ����

	printf("Order> net port(Big or): 0x%x\n", net_port);
	printf("Order> host port(B/L): 0x%x\n", host_port);
#endif
	WSACleanup();

	return 0;
}