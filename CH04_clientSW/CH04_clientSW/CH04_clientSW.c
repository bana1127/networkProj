#include <stdio.h>
#include <WinSock2.h>
#define MAX_BUF_SIZE 100

int main() {
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	//1.socket 생성
	SOCKET cSock;
	cSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //주소계층(체계), 타입, 구체적인 프로토콜 / PF_INET => 프로토콜 INET, AF_INET => address INET 값은 같음
	
	//2.connect(socket, 주소(ip)) -> 연결해야하는 서버의 주소
	SOCKADDR_IN svrAddr;
	svrAddr.sin_family = AF_INET;
	svrAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//inet_addr을 사용하면 스트링을 32비트 빅엔디언으로 바꿔서 삽입해줌
	//svrAddr.sin_addr.s_addr 로도 32비트 사용가능
	svrAddr.sin_port = htons(9000); // big endian
	connect(cSock, (SOCKADDR*)&svrAddr, sizeof(svrAddr)); //TCP 연결 요청 to TCP 
	printf("Client> 서버와 연결 설정 완료. \n");

	//응용 프로그램 부분
	char buf[MAX_BUF_SIZE];
	int len;
	printf("Client> 문자열 입력 : ");
	fgets(buf, MAX_BUF_SIZE, stdin); //stdin -> 키보드로 입력받는다.
	len = strlen(buf);

	//서버로 전송
	int ret;
	ret = send(cSock, buf, len + 1, 0); //보낼때 엔터가 추가되서 abc입력하면 4바이트 보내짐 +1 바이트 추가
	//x 바이트 송신
	printf("Client> sent %d bytes. data = %s", ret, buf);

	int rcvTarget, rcvSum;
	rcvTarget = ret; //수신 목표치.
	rcvSum = 0; //현재까지 수신한 누적치. 
	while (rcvSum < rcvTarget) {
		ret = recv(cSock, &buf[rcvSum], rcvTarget-rcvSum, 0);//x 바이트 수신
		rcvSum += ret;
	}
	buf[rcvTarget] = 0;
	printf("Client> rev echo back data %d bytes, data = %s\n", ret, buf);
	closesocket(cSock);
	
	WSACleanup();
	return 0;
}