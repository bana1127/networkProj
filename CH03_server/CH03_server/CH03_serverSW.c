#include <stdio.h>
#include <WinSock2.h>

int main() {
	WSADATA wsa;
	WSAStartup(MAKEWORD(2,2), &wsa); //DLL 동적으로
	//1. 소켓 생성.
	SOCKET hSocket;
	hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //소켓 생성 & TCP/UDP 연결
	if (hSocket == INVALID_SOCKET) {
		printf("<ERROR> socket() fail. \n");
		WSACleanup();
		return 1;
	}
	//2. server case:
	// 서버의 주소 (IP, Port) 설정
	SOCKADDR_IN svrAddr;//IPv4

	memset(&svrAddr, 0, sizeof(svrAddr));
	svrAddr.sin_family = AF_INET; //ipv4의미
	svrAddr.sin_port = htons(0x0809); // host(B/L) or --> big 16비트는 hton 뒤에 s붙임
	svrAddr.sin_addr.S_un.S_addr = inet_addr("1.2.3.4");// 32bit 숫자

#if 0
	//inet_addr : 주소 string "1.2.3.4" --> 주소 (big en)
	//inet_ntoa : 주소 ---> 주소 string "1.2.3.4"

	char *tempIPstr = inet_ntoa(svrAddr.sin_addr);
	printf("inet_ntoa> str IP addr = %s \n", tempIPstr);
#endif

	bind(hSocket, (SOCKADDR *)&svrAddr, sizeof(svrAddr));

	//WSAStringtoAddress
	char tmpStrIP[50] = "1.2.3.4:9000";
	int size;
	size = sizeof(svrAddr);
	WSAStringToAddressA(
		tmpStrIP, //주소 스트링의 시작 주소
		AF_INET,//ipv4
		NULL, //프로토콜 프로바이더
		(SOCKADDR*)&svrAddr, // IPv4 주소 SOCKADDR_IN 사용.. ip, port 쉽게 설정/주소구조체
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
	short net_port = svrAddr.sin_port; // port 16bits, Big or로 저장
	short host_port = 0x0809; // CPU or(host order)로 저장

	printf("Order> net port(Big or): 0x%x\n", net_port);
	printf("Order> host port(B/L): 0x%x\n", host_port);
#endif
	WSACleanup();

	return 0;
}