#include <iostream>
#include <string>
#include <winsock2.h> // Win Sock (Windows Socket) is the library that will help us do our HTTP request handling
#include <exception> // useful exception classes

#pragma once
#pragma comment(lib, "ws2_32.lib")

namespace HTTP{ 

	// MARK: HTTP Classes/Structs

	/**
	 * This class helps us create a base class for HTTP exceptions with typical data relevant to this, such as HTTP Error Codes.
	*/
	class HTTPException : public std::exception 
	{
		protected:
			std::string Message;
			int Code;
		public:
			HTTPException() : 
				Message("INTERNAL SERVER ERROR: Unknown Error While Processing Request"), Code(500) {};
			HTTPException(std::string Message, int Code) :
				Message(Message), Code(Code) {};\
			
			std::string HTTPDetailedWhat() const {
				return Message + "[CODE: " + std::to_string(Code) + "]";
			}

			int getCode() const {
				return Code;
			}
	};
	
	// Specific class for code 503
	class ServiceUnavailableException : public HTTPException 
	{
		public:
			ServiceUnavailableException(std::string Message) : 
				HTTPException(Message, 503) {};
	};

	// MARK: HTTP User Defined Functions

	/**
	 * @brief This function initialises the HTTP Server
	 * @return SOCKET: The listening socket for the server
	*/
	SOCKET InitialiseHTTPServer()
	{
		// First, we need to initialise Win Sock
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			throw new ServiceUnavailableException("INTERNAL SERVER ERROR: Unable to initialise the Windows Sockets API");
		}


		// Next, we create a socket for the connection
		SOCKET listeningSocket = socket(AF_INET, 		// Address Family: 		INET (IPv4)
										SOCK_STREAM,	// Socket Mode: 		Bidirectional Stream  
										IPPROTO_TCP);	// IP Address Protocol:	TCP/IP

		if (listeningSocket == INVALID_SOCKET) { throw new ServiceUnavailableException("INTERNAL SERVER ERROR: Unable to initialise the socket for listening"); }

		// Now that our server is "alive", lets attempt to bind it to an address
		sockaddr_in serverAddress;
		serverAddress.sin_family = AF_INET; 			// Address Family: 		INET (IPv4)
		serverAddress.sin_addr.s_addr = INADDR_ANY;		// Client Address:		ANY	
		serverAddress.sin_port = htons(80); 			// We will be using port 80 as it is standard for HTTP
		
		if (bind(listeningSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
			closesocket(listeningSocket);
			throw new ServiceUnavailableException("INTERNAL SERVER ERROR: Unable to bind the socket for listening");
		}

		// Finally, we can start to listen for incoming connections
		if (listen(listeningSocket, 5) == SOCKET_ERROR) {
			closesocket(listeningSocket);
			throw new ServiceUnavailableException("INTERNAL SERVER ERROR: Unable to listen for any connections");
		}

		return listeningSocket;
	}

	void ProcessRequest(SOCKET clientSocket)
	{
		char buffer[1024];
		int bytesReceived;

		// Receive data from the client
		while ((bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
			// Process the received data (parse HTTP request)
			// You'll need to parse the request method, URI, headers, etc.
			// Based on the request, you can send a response.

			// Example: Print the first line of the request (basic check)
			std::string request(buffer, bytesReceived);
			std::cout << request.substr(0, request.find("\r\n")) << std::endl;

			// Clear the buffer for next reception
			memset(buffer, 0, sizeof(buffer));
		}

		if (bytesReceived == SOCKET_ERROR) {
			std::cerr << "Error receiving data!" << std::endl;
		}
	}

	void HandleConnections(SOCKET listeningSocket)
	{
		while (true) {
			SOCKET clientSocket = accept(listeningSocket, NULL, NULL);
			if (clientSocket == INVALID_SOCKET) {
				std::cerr << "Failed to accept connection!" << std::endl;
				continue;
			}

			// Handle the connection (receive and parse HTTP request)
			// This is where the real work begins!
			ProcessRequest(clientSocket);

			// Close the client socket
			closesocket(clientSocket);
		}
	}
}