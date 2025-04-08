#include <stdio.h>
#include <WinSock2.h>
#define MAX_BUF_SIZE 100

int main() {
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	//1.socket ����
	SOCKET cSock;
	cSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //�ּҰ���(ü��), Ÿ��, ��ü���� �������� / PF_INET => �������� INET, AF_INET => address INET ���� ����

	//2.connect(socket, �ּ�(ip)) -> �����ؾ��ϴ� ������ �ּ�
	SOCKADDR_IN svrAddr;
	svrAddr.sin_family = AF_INET;
	svrAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//inet_addr�� ����ϸ� ��Ʈ���� 32��Ʈ �򿣵������ �ٲ㼭 ��������
	//svrAddr.sin_addr.s_addr �ε� 32��Ʈ ��밡��
	svrAddr.sin_port = htons(9000); // big endian
	connect(cSock, (SOCKADDR*)&svrAddr, sizeof(svrAddr)); //TCP ���� ��û to TCP 
	printf("Client> ������ ���� ���� �Ϸ�. \n");

#if 0
	//���� ���α׷� �κ�
	char buf[MAX_BUF_SIZE];
	int len;
	printf("Client> ���ڿ� �Է� : ");
	fgets(buf, MAX_BUF_SIZE, stdin); //stdin -> Ű����� �Է¹޴´�.
	len = strlen(buf);

	//������ ����
	int ret;
	ret = send(cSock, buf, len + 1, 0); //������ ���Ͱ� �߰��Ǽ� abc�Է��ϸ� 4����Ʈ ������ +1 ����Ʈ �߰�
	//x ����Ʈ �۽�
	printf("Client> sent %d bytes. data = %s", ret, buf);

	int rcvTarget, rcvSum;
	rcvTarget = ret; //���� ��ǥġ.
	rcvSum = 0; //������� ������ ����ġ. 
	while (rcvSum < rcvTarget) {
		ret = recv(cSock, &buf[rcvSum], rcvTarget - rcvSum, 0);//x ����Ʈ ����
		rcvSum += ret;
	}
	buf[rcvTarget] = 0;
	printf("Client> rev echo back data %d bytes, data = %s\n", ret, buf);
#endif
	int opndCnt;
	char buf[MAX_BUF_SIZE];
	while (1) {
		//2. ���� 2 : ����

		//1. ����ڷκ��� ��꿡 �ʿ��� ���� �Է� �ޱ�.
		//- �ǿ����� ��(n) �Է�, �� ���� 0 �� �� ����.
		printf("> Operand count : ");
		scanf("%d", &opndCnt);
		buf[0] = (char)opndCnt;
		//- n�� �ǿ����� �Է�
		for (int i = 0; i < opndCnt; i++) {
			printf("Operand %d : ", i + 1);
			scanf("%d", &buf[i*sizeof(int)+1]);
		}

		getchar(); //�� �ǿ����� �Է��� ���͸� ���� / ���� �Է��ϰ� ���͸� �������� scanf(%c)�� ���͸� ������ ���� operator�� �Է��ϱ� ���ؼ� �����������
		//- ������ �Է�
		// char buf[] ���� : �̸� ���� �޽���(��Ŷ) ������ ����
		printf("> Operator : ");
		scanf("%c", &buf[opndCnt * sizeof(int) + 1]);
		
		//2. ������ ���� : send
		send(cSock, buf, opndCnt * sizeof(int) + 2, 0);

		//3. ������ ���� ��� ���� : result(int) ����
		 //�ܿ��� 
		int rcvSum = 0;// ���� ����ġ
		int rcvTarget = sizeof(int); //4bytes
		int ret;
		while (rcvSum < rcvTarget) {
			ret = recv(cSock, &buf[rcvSum], rcvTarget - rcvSum, 0);
			rcvSum += ret;
		}
		//�����κ��� result ���� �Ϸ�.
		printf("Client> result from ���� = %d \n", *((int *)buf)); //buf�� charŸ���̶� ����ȯ �������
	}

	closesocket(cSock);

	WSACleanup();
	return 0;
}