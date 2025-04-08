#include <stdio.h>
#include <WinSock2.h>
#define MAX_BUF_SIZE 100

int main() {
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	//1.socket ����
	SOCKET cSock;
	cSock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP); //�ּҰ���(ü��), Ÿ��, ��ü���� �������� / PF_INET => �������� INET, AF_INET => address INET ���� ����

	//2.connect(socket, �ּ�(ip)) -> �����ؾ��ϴ� ������ �ּ�
	SOCKADDR_IN svrAddr;
	memset(&svrAddr, 0, sizeof(svrAddr));
	svrAddr.sin_family = AF_INET;
	svrAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//inet_addr�� ����ϸ� ��Ʈ���� 32��Ʈ �򿣵������ �ٲ㼭 ��������
	//svrAddr.sin_addr.s_addr �ε� 32��Ʈ ��밡��
	svrAddr.sin_port = htons(9000); // big endian
	

#if 0
	connect(cSock, (SOCKADDR*)&svrAddr, sizeof(svrAddr)); //TCP ���� ��û to TCP 
	printf("Client> ������ ���� ���� �Ϸ�. \n");
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
	char serverIP[50];
	int serverPort;
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
			scanf("%d", &buf[i * sizeof(int) + 1]);
		}

		getchar(); //�� �ǿ����� �Է��� ���͸� ���� / ���� �Է��ϰ� ���͸� �������� scanf(%c)�� ���͸� ������ ���� operator�� �Է��ϱ� ���ؼ� �����������
		//- ������ �Է�
		// char buf[] ���� : �̸� ���� �޽���(��Ŷ) ������ ����
		printf("> Operator : ");
		scanf("%c", &buf[opndCnt * sizeof(int) + 1]);

		//����� ���� �ּ� �Է¹ޱ�
		printf("���� IP : ");
		scanf("%s", serverIP);
		printf("���� port : ");
		scanf("%d", &serverPort);
		memset(&svrAddr, 0, sizeof(svrAddr));
		svrAddr.sin_family = AF_INET;
		svrAddr.sin_addr.S_un.S_addr = inet_addr(serverIP);//inet_addr�� ����ϸ� ��Ʈ���� 32��Ʈ �򿣵������ �ٲ㼭 ��������
		//svrAddr.sin_addr.s_addr �ε� 32��Ʈ ��밡��
		svrAddr.sin_port = htons((short)serverPort); // big endian

		//2. ������ ���� : sendto(udp)
		sendto(cSock, buf, opndCnt * sizeof(int) + 2, 0, (SOCKADDR*)&svrAddr, sizeof(svrAddr));

		//3. ������ ���� ��� ���� : result(int) ����
		 //�ܿ��� 
		int rcvSum = 0;// ���� ����ġ
		int rcvTarget = sizeof(int); //4bytes
		int ret;
		int result, size;
		SOCKADDR_IN fromAddr;
		size = sizeof(fromAddr);
		ret = recvfrom(cSock, &result, sizeof(result), 0, (SOCKADDR*)&fromAddr, &size); //UDP�� �޽��� ������ �ɰ��� ������ �˾Ƽ� �ٿ��� ������ ���� �ʿ� X

#if 0
		while (rcvSum < rcvTarget) {
			ret = recv(cSock, &buf[rcvSum], rcvTarget - rcvSum, 0);
			rcvSum += ret;
		}
#endif
		//�����κ��� result ���� �Ϸ�.
		printf("Client> result from ���� = %d \n", result); //buf�� charŸ���̶� ����ȯ �������
	}

	closesocket(cSock);

	WSACleanup();
	return 0;
}