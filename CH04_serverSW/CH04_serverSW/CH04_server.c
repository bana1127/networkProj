#include <stdio.h>
#include <WinSock2.h>

int main() {
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa); //DLL ��������
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
	svrAddr.sin_port = htons(9000); // host(B/L) or --> big 16��Ʈ�� hton �ڿ� s����
	svrAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");// 32bit ����

	bind(hSocket, (SOCKADDR*)&svrAddr, sizeof(svrAddr));

	//3. listen : TCP�� listen ���·� ����, �����û ����/ó�� ����
	int backlog = 3;
	listen(hSocket,backlog);
	printf("Server> ���� ��� �� \n");

	//4. accept: TCP ���� ��û ����. �̾����� ������ �ۼ��� �غ�. �ּҴ� client�ּ�
	SOCKADDR_IN clientAddr;
	SOCKET clientSock;
	int size = sizeof(clientAddr); //accept�� ������ clientAddr�� ũ�⸦ ��� ������ �ּ�(������)�� �������
	clientSock = accept(hSocket, (SOCKADDR *)&clientAddr, &size);
	printf("Server> client(IP : %s, port : %d) is connected.\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port)); //net order=Big endian -> host order ��ȯ �ʿ�
	
	//5. ��� client ���α׷��� ������ �ۼ���
	char buf[1000];
	int ret;
	ret = recv(clientSock, buf, sizeof(buf),0);
	buf[ret] = 0;

	printf("Server> recved data length = %d data = %s\n", ret, buf);

	ret = send(clientSock, buf, ret, 0); // ������ ����
	printf("Server> return %d bytes \n", ret);

	closesocket(clientSock);

	WSACleanup();

	return 0;
}