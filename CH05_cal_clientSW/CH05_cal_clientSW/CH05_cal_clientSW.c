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

#if 0
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
		ret = recv(cSock, &buf[rcvSum], rcvTarget - rcvSum, 0);//x 바이트 수신
		rcvSum += ret;
	}
	buf[rcvTarget] = 0;
	printf("Client> rev echo back data %d bytes, data = %s\n", ret, buf);
#endif
	int opndCnt;
	char buf[MAX_BUF_SIZE];
	while (1) {
		//2. 응용 2 : 계산기

		//1. 사용자로부터 계산에 필요한 정보 입력 받기.
		//- 피연산자 수(n) 입력, 이 값이 0 일 때 종료.
		printf("> Operand count : ");
		scanf("%d", &opndCnt);
		buf[0] = (char)opndCnt;
		//- n개 피연산자 입력
		for (int i = 0; i < opndCnt; i++) {
			printf("Operand %d : ", i + 1);
			scanf("%d", &buf[i*sizeof(int)+1]);
		}

		getchar(); //앞 피연산자 입력한 엔터를 삭제 / 숫자 입력하고 엔터를 눌렀을때 scanf(%c)가 엔터를 가져감 따라서 operator를 입력하기 위해서 제거해줘야함
		//- 연산자 입력
		// char buf[] 저장 : 미리 정한 메시지(패킷) 구조에 따라서
		printf("> Operator : ");
		scanf("%c", &buf[opndCnt * sizeof(int) + 1]);
		
		//2. 서버로 전송 : send
		send(cSock, buf, opndCnt * sizeof(int) + 2, 0);

		//3. 서버로 부터 결과 수신 : result(int) 저장
		 //외우자 
		int rcvSum = 0;// 수신 누적치
		int rcvTarget = sizeof(int); //4bytes
		int ret;
		while (rcvSum < rcvTarget) {
			ret = recv(cSock, &buf[rcvSum], rcvTarget - rcvSum, 0);
			rcvSum += ret;
		}
		//서버로부터 result 수신 완료.
		printf("Client> result from 서버 = %d \n", *((int *)buf)); //buf가 char타입이라서 형변환 해줘야함
	}

	closesocket(cSock);

	WSACleanup();
	return 0;
}