#include <stdio.h>
#include <WinSock2.h>

int main() {
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa); //DLL 동적으로
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
	svrAddr.sin_port = htons(9000); // host(B/L) or --> big 16비트는 hton 뒤에 s붙임
	svrAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");// 32bit 숫자

	bind(hSocket, (SOCKADDR*)&svrAddr, sizeof(svrAddr));

	//3. listen : TCP를 listen 상태로 변경, 연결요청 수신/처리 가능
	int backlog = 3;
	listen(hSocket,backlog);
	printf("Server> 연결 대기 중 \n");

	//4. accept: TCP 연결 요청 수락. 이어지는 데이터 송수신 준비. 주소는 client주소
	SOCKADDR_IN clientAddr;
	SOCKET clientSock;
	int size = sizeof(clientAddr); //accept는 변수에 clientAddr의 크기를 담고 변수의 주소(포인터)를 적어야함
	clientSock = accept(hSocket, (SOCKADDR *)&clientAddr, &size);
	printf("Server> client(IP : %s, port : %d) is connected.\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port)); //net order=Big endian -> host order 변환 필요
	
	//5. 상대 client 프로그램과 데이터 송수신
	char buf[1000];
	int ret;
	ret = recv(clientSock, buf, sizeof(buf),0);
	buf[ret] = 0;

	printf("Server> recved data length = %d data = %s\n", ret, buf);

	ret = send(clientSock, buf, ret, 0); // 성공을 가정
	printf("Server> return %d bytes \n", ret);

	closesocket(clientSock);

	WSACleanup();

	return 0;
}