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
	WSAStartup(MAKEWORD(2, 2), &wsa); //DLL ��������
	//1. ���� ����.
	SOCKET hSocket;
	hSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP); //���� ���� & TCP/UDP ����
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
	svrAddr.sin_port = htons(9000); // host(B/L) or --> big 16��Ʈ�� hton �ڿ� s����
	svrAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");// 32bit ����

	bind(hSocket, (SOCKADDR*)&svrAddr, sizeof(svrAddr));
#if 0
	//3. listen : TCP�� listen ���·� ����, �����û ����/ó�� ����  // UDP�� �ʿ�X
	int backlog = 3; //���ť ����
	listen(hSocket, backlog);
#endif

	//4. accept: TCP ���� ��û ����. �̾����� ������ �ۼ��� �غ�. �ּҴ� client�ּ� //UDP��accept �ʿ�X
	SOCKADDR_IN clientAddr;
	SOCKET clientSock;
	int size = sizeof(clientAddr); //accept�� ������ clientAddr�� ũ�⸦ ��� ������ �ּ�(������)�� �������

	for (int i = 0; i < 5; i++) { // 5�� Ŭ���̾�Ʈ ���� ����
		printf("Server> client ��� ��û ��� �� \n");
#if 0
		clientSock = accept(hSocket, (SOCKADDR*)&clientAddr, &size);
		printf("Server> client(IP : %s, port : %d) is connected.\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port)); //net order=Big endian -> host order ��ȯ �ʿ�
#endif
#if 0
		//���� 1
		//5. ��� client ���α׷��� ������ �ۼ���
		char buf[1000];
		int ret;
		ret = recv(clientSock, buf, sizeof(buf), 0);
		buf[ret] = 0;

		printf("Server> recved data length = %d data = %s\n", ret, buf);

		ret = send(clientSock, buf, ret, 0); // ������ ����
		printf("Server> return %d bytes \n", ret);
#endif
		flag = STATE_GO;
		while (1) {
			//���� 2: ���� ����
			int ret, rcvSum, rcvTarget, dbgloop = 0;
			char buf[MAX_BUF_SIZE];
			char opndCnt;
			//1. Ŭ���̾�Ʈ�� ��� ��û ���� ���� ->char buf[] ����.
			ret = recvfrom(hSocket, buf, MAX_BUF_SIZE, 0,(SOCKADDR*)&clientAddr, &size); 


#if 0
			ret = recv(clientSock, &opndCnt, 1, 0); //�ǿ����� ����. opndCnt�� ���� // ù��° ����Ʈ ����
			if (ret == 0 || ret == SOCKET_ERROR) {
				break;
			}
			// �߰������� �� �о�� �ϴ� ����Ʈ �� ���:
			// len = opndCnt * sizeof(int) + 1 // 1�� ������
			rcvSum = 0; // ������� ���� ����ġ
			rcvTarget = opndCnt * sizeof(int) + 1;//��ǥġ
			while (rcvSum < rcvTarget) {
				ret = recv(clientSock, &buf[rcvSum], rcvTarget - rcvSum, 0);
				if (ret == 0 || ret == SOCKET_ERROR) {
					flag = STATE_STOP;
					break;
				}
				rcvSum += ret; //rcvSum ���� ����ġ�� ret�� ����
#ifdef DEBUG_LEV03_MSG
				printf("Server> recv %d (rcvSum : %d, rcvTarget : %d\n", ++dbgloop, rcvSum, rcvTarget); //����� �ڵ�
#endif
			}
#endif
			if (flag == STATE_GO) {
				//2. buf[] ���� �ؼ�, ��� -> int result
				// - opndCnt : �ǿ����� ��
				// - char buf[] : �ǿ����� �迭, ������(1 bytes)

				int result;
				result = calculate((int)buf[0], (int*)&buf[1], buf[(int)buf[0] * sizeof(int) + 1]);

				//3. result�� �ٽ� send to Client
				ret = sendto(hSocket, (char*)&result, sizeof(result), 0, (SOCKADDR*)&clientAddr,sizeof(clientAddr));
				printf("Server> ��� ��� client�� ���� �Ϸ�. (result = %d)\n", result);
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
			result += opnds[i]; //���� ��
		}
		break;
	case '-':
		for (int i = 1; i < opndCnt; i++) {
			result -= opnds[i]; //���� ��
		}
		break;
	case '*':
		for (int i = 1; i < opndCnt; i++) {
			result = result * opnds[i]; //���� ��
		}
		break;
	}


	return result;
}