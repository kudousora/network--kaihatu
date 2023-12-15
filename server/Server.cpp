#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment( lib, "ws2_32.lib" )

// �|�[�g�ԍ�
const unsigned short SERVERPORT = 8888;
// ����M���郁�b�Z�[�W�̍ő�l
const unsigned int MESSAGELENGTH = 1024;


int main()
{
	std::cout << "Chat Server" << std::endl;


	WSADATA wsaData;
	int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (ret != 0)
	{
		std::cout << "Error: InitWinSock ( ErrorCode:" << ret << " )" << std::endl;
		return 1;	// ���[���I
	}
	std::cout << "Success: InitWinSock" << std::endl;


	// UDP�\�P�b�g�̍쐬
	int sock;
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		std::cout << "Error: socket ( ErrorCode:" << WSAGetLastError() << " )" << std::endl;
		return 1;	// ���[���I
	}
	std::cout << "Success: socket" << std::endl;


	// �Œ�A�h���X�ɂ��邽�߂Ƀ\�P�b�g�A�h���X���̊��蓖��
	struct sockaddr_in bindAddr;
	memset(&bindAddr, 0, sizeof(bindAddr));
	bindAddr.sin_family = AF_INET;
	bindAddr.sin_port = htons(SERVERPORT);
	bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sock, (struct sockaddr*)&bindAddr, sizeof(bindAddr)) < 0)
	{
		std::cout << "Error: bind ( ErrorCode:" << WSAGetLastError() << " )" << std::endl;
		return 1;
	}
	std::cout << "Success: bind" << std::endl;


	while (true)
	{
		char buff[MESSAGELENGTH];			// ����M���b�Z�[�W�̊i�[�̈�
		struct sockaddr_in fromAddr;		// ���M���A�h���X�̊i�[�̈�
		int fromlen = sizeof(fromAddr);		// fromAddr�̃T�C�Y

		// ��M�҂�
		std::cout << "wait..." << std::endl;

		// ��M	\0�͑����Ă��Ȃ��o�[�W����
		ret = recvfrom(sock, buff, sizeof(buff) - 1, 0, (struct sockaddr*)&fromAddr, &fromlen);
		if (ret < 0)
		{
			std::cout << "Error: recvfrom ( ErrorCode:" << WSAGetLastError() << " )" << std::endl;
			return 1;
		}
		buff[ret] = '\0';	// �I�[�L���ǉ�
		std::cout << "Receive message : " << buff << std::endl;

		// ���M�p���b�Z�[�W�̓���
		std::cout << "Input message : ";
		std::cin >> buff;

		// ���M�I
		ret = sendto(sock, buff, strlen(buff), 0, (struct sockaddr*)&fromAddr, fromlen);
		if (ret != strlen(buff))
		{
			std::cout << "Error: sendto ( ErrorCode:" << WSAGetLastError() << " )" << std::endl;
			return 1;
		}
	}

	return 0;
}
