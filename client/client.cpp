#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment( lib, "ws2_32.lib" )

// ポート番号
const unsigned short SERVERPORT = 8888;
// 送受信するメッセージの最大値
const unsigned int MESSAGELENGTH = 1024;

// WinSockの初期化
int InitWinSock();

int main()
{
	std::cout << "Chat Client" << std::endl;

	WSADATA wsaData;
	int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (ret != 0)
	{
		std::cout << "Error: InitWinSock ( ErrorCode:" << ret << " )" << std::endl;
		return 1;	// おーわり！
	}
	std::cout << "Success: InitWinSock" << std::endl;


	// UDPソケットの作成
	int sock;
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		std::cout << "Error: socket ( ErrorCode:" << WSAGetLastError() << " )" << std::endl;
		return 1;	// おーわり！
	}
	std::cout << "Success: socket" << std::endl;


	// 宛先となるサーバのソケットアドレス情報の割り当て
	std::string serverIpv4Address;
	std::cout << "Input Server IPv4 Address : ";
	std::cin >> serverIpv4Address;
	struct sockaddr_in toAddr;
	memset(&toAddr, 0, sizeof(toAddr));
	toAddr.sin_family = AF_INET;
	toAddr.sin_port = htons(SERVERPORT);
	inet_pton(AF_INET, serverIpv4Address.c_str(), &toAddr.sin_addr.s_addr);


	while (true)
	{
		char buff[MESSAGELENGTH];			// 送受信メッセージの格納領域
		struct sockaddr_in fromAddr;		// 送信元アドレスの格納領域
		int fromlen = sizeof(fromAddr);		// fromAddrのサイズ


		// 送信用メッセージの入力
		std::cout << "Input message : ";
		std::cin >> buff;

		// 送信！
		ret = sendto(sock, buff, strlen(buff), 0, (struct sockaddr*)&toAddr, sizeof(toAddr));
		if (ret != strlen(buff))
		{
			std::cout << "Error: sendto ( ErrorCode:" << WSAGetLastError() << " )" << std::endl;
			return 1;
		}

		// 受信待ち
		std::cout << "wait..." << std::endl;

		// 受信	\0は送ってこないバージョン
		ret = recvfrom(sock, buff, sizeof(buff) - 1, 0, (struct sockaddr*)&fromAddr, &fromlen);
		if (ret < 0)
		{
			std::cout << "Error: recvfrom ( ErrorCode:" << WSAGetLastError() << " )" << std::endl;
			return 1;
		}
		buff[ret] = '\0';	// 終端記号追加
		std::cout << "Receive message : " << buff << std::endl;
	}

	return 0;
}

// WinSockの初期化
int InitWinSock()
{
	WSADATA wsaData;
	return WSAStartup(MAKEWORD(2, 2), &wsaData);
}