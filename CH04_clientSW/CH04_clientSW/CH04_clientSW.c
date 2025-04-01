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
		ret = recv(cSock, &buf[rcvSum], rcvTarget-rcvSum, 0);//x ����Ʈ ����
		rcvSum += ret;
	}
	buf[rcvTarget] = 0;
	printf("Client> rev echo back data %d bytes, data = %s\n", ret, buf);
	closesocket(cSock);
	
	WSACleanup();
	return 0;
}