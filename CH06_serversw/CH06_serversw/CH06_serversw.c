#include <stdio.h>
#include <WinSock2.h>
#define MAX_BUF_SIZE 100
#define DEBUG_LEV03_MSG
#define STATE_GO 0 
#define STATE_STOP 1
int calculate(int opndCnt, int opnds[], char op);

int main() {
	int flag;

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa); //DLL 동적으로
	//1. 소켓 생성.
	SOCKET hSocket;
	hSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP); //소켓 생성 & TCP/UDP 연결
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
#if 0
	//3. listen : TCP를 listen 상태로 변경, 연결요청 수신/처리 가능  // UDP는 필요X
	int backlog = 3; //대기큐 개수
	listen(hSocket, backlog);
#endif

	//4. accept: TCP 연결 요청 수락. 이어지는 데이터 송수신 준비. 주소는 client주소 //UDP는accept 필요X
	SOCKADDR_IN clientAddr;
	SOCKET clientSock;
	int size = sizeof(clientAddr); //accept는 변수에 clientAddr의 크기를 담고 변수의 주소(포인터)를 적어야함

	for (int i = 0; i < 5; i++) { // 5개 클라이언트 서비스 제공
		printf("Server> client 계산 요청 대기 중 \n");
#if 0
		clientSock = accept(hSocket, (SOCKADDR*)&clientAddr, &size);
		printf("Server> client(IP : %s, port : %d) is connected.\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port)); //net order=Big endian -> host order 변환 필요
#endif
#if 0
		//응용 1
		//5. 상대 client 프로그램과 데이터 송수신
		char buf[1000];
		int ret;
		ret = recv(clientSock, buf, sizeof(buf), 0);
		buf[ret] = 0;

		printf("Server> recved data length = %d data = %s\n", ret, buf);

		ret = send(clientSock, buf, ret, 0); // 성공을 가정
		printf("Server> return %d bytes \n", ret);
#endif
		flag = STATE_GO;
		while (1) {
			//응용 2: 계산기 서버
			int ret, rcvSum, rcvTarget, dbgloop = 0;
			char buf[MAX_BUF_SIZE];
			char opndCnt;
			//1. 클라이언트의 계산 요청 멧지 수신 ->char buf[] 저장.
			ret = recvfrom(hSocket, buf, MAX_BUF_SIZE, 0,(SOCKADDR*)&clientAddr, &size); 


#if 0
			ret = recv(clientSock, &opndCnt, 1, 0); //피연산자 갯수. opndCnt에 저장 // 첫번째 바이트 읽음
			if (ret == 0 || ret == SOCKET_ERROR) {
				break;
			}
			// 추가적으로 더 읽어야 하는 바이트 수 계산:
			// len = opndCnt * sizeof(int) + 1 // 1은 연산자
			rcvSum = 0; // 현재까지 읽은 누적치
			rcvTarget = opndCnt * sizeof(int) + 1;//목표치
			while (rcvSum < rcvTarget) {
				ret = recv(clientSock, &buf[rcvSum], rcvTarget - rcvSum, 0);
				if (ret == 0 || ret == SOCKET_ERROR) {
					flag = STATE_STOP;
					break;
				}
				rcvSum += ret; //rcvSum 읽은 누적치에 ret을 누적
#ifdef DEBUG_LEV03_MSG
				printf("Server> recv %d (rcvSum : %d, rcvTarget : %d\n", ++dbgloop, rcvSum, rcvTarget); //디버깅 코드
#endif
			}
#endif
			if (flag == STATE_GO) {
				//2. buf[] 내용 해석, 계산 -> int result
				// - opndCnt : 피연산자 수
				// - char buf[] : 피연산자 배열, 연산자(1 bytes)

				int result;
				result = calculate((int)buf[0], (int*)&buf[1], buf[(int)buf[0] * sizeof(int) + 1]);

				//3. result를 다시 send to Client
				ret = sendto(hSocket, (char*)&result, sizeof(result), 0, (SOCKADDR*)&clientAddr,sizeof(clientAddr));
				printf("Server> 계산 결과 client로 전송 완료. (result = %d)\n", result);
			}
		}
		//closesocket(clientSock);
	}
	closesocket(hSocket);
	WSACleanup();

	return 0;
}

int calculate(int opndCnt, int opnds[], char op)
{
	int result = opnds[0];
	switch (op) {
	case '+':
		for (int i = 1; i < opndCnt; i++) {
			result += opnds[i]; //누적 합
		}
		break;
	case '-':
		for (int i = 1; i < opndCnt; i++) {
			result -= opnds[i]; //누적 차
		}
		break;
	case '*':
		for (int i = 1; i < opndCnt; i++) {
			result = result * opnds[i]; //누적 곱
		}
		break;
	}


	return result;
}