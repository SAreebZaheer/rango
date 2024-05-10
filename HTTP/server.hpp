#include <iostream>
#include <string>
#include <sstream>		// Stringstream library used to conveniently handle the API requests
#include <fstream>		// We will be sending all the files requested based on the URI
#include <winsock2.h>	// Win Sock (Windows Socket) is the library that will help us do our HTTP request handling
#include <exception>	// useful exception classes
#include <windows.h>	// API for windows features

#pragma once
#pragma comment(lib, "ws2_32.lib")

namespace HTTP{ 

	// MARK: HTTP Classes/Structs

	enum RequestType {
		HTTP_GET,
		HTTP_POST,
		HTTP_PUT,
		HTTP_DELETE,
		HTTP_HEAD,
		HTTP_OPTIONS,
		HTTP_TRACE,
		HTTP_CONNECT,
		HTTP_UNKNOWN
	};
	std::string RequestTypeNames[] = {
		"GET",
		"POST",
		"PUT",
		"DELETE",
		"HEAD",
		"OPTIONS",
		"TRACE",
		"CONNECT"
	};

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

	class NotImplemented : public HTTPException
	{
		public:
			NotImplemented(std::string Message) : 
				HTTPException(Message, 501) {};
	};

	class GatewayTimeoutException : public HTTPException
	{
		public:
			GatewayTimeoutException(std::string Message) : 
				HTTPException(Message, 504) {};
	};

	// The most famous error xD
	class ResourceNotFoundException : public HTTPException
	{
		public:
			ResourceNotFoundException(std::string Message) : 
				HTTPException(Message, 404) {};
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

	std::string getCurrDir() {
		DWORD buffer_size = GetCurrentDirectoryA(0, NULL);
		char* buffer = new char[buffer_size + 1];
		GetCurrentDirectoryA(buffer_size, buffer);
		std::string current_dir(buffer);
		delete[] buffer;
		return current_dir;
	
	}

	/**
	*	@brief This function sends a file to the client with appropriate headers
	*	@param clientSocket: The socket to send the file to
	*	@param filename: The name of the file to send
	*/
	void SendFileWithHeaders(SOCKET clientSocket, const std::string& filename) {
		std::string current_dir = getCurrDir();
		std::cout << "(Debug) Current Directory: " << current_dir << std::endl; 
		std::string newFilename; // filename is a constant reference, so we need to create a new string to modify it
		newFilename = filename; // All our files are in the HTTP directory
		int i = 0;

		// Replace all forward slashes with backslashes
		while (!newFilename[i] == '\0') {
			if (newFilename[i] == '/') {
				newFilename[i] = '\\';
			}
			i++;
		}
		std::string oldFilename = newFilename;
		newFilename = "";

		// Remove the query part for the file loading
		for (int i = 0; i < oldFilename.size(); i++) {
			if (oldFilename[i] == '?') {
				break;
			}
			newFilename += oldFilename[i];
		}

		newFilename = current_dir + "\\HTTP" + newFilename;

		// Get the root file
		if (newFilename == current_dir + "\\HTTP\\") {
			newFilename = current_dir + "\\HTTP\\index.html";
		}

		// Open the file in binary mode
		std::ifstream file(newFilename, std::ios::binary);

		if (!file.is_open()) {
			throw ResourceNotFoundException("INTERNAL SERVER ERROR: Unable to open the requested file: " + newFilename);
		}

		// Get file size
		file.seekg(0, std::ios::end);
		int fileSize = file.tellg();
		file.seekg(0, std::ios::beg); // reset pointer to start

		// Get file extension (assuming filename includes extension)
		std::string extension;
		for (i = newFilename.size() -1; i > 0; i--) {
			if (newFilename[i] == '.') {
				break;
			}
			extension += newFilename[i];
		}

		for (i = 0; i < extension.size() / 2; i++) {
			char temp = extension[i];
			extension[i] = extension[extension.size() - i - 1];
			extension[extension.size() - i - 1] = temp;
		}

		std::cout << "(DEBUG) file location: " << newFilename << "\n";
		std::cout << "(DEBUG) extension: " << extension << "\n";
		
		// Create headers with Content-Type
		std::string headers = "HTTP/1.1 200 OK\r\n";
		headers += "Content-Type: \r\n";
		headers += "Content-Length: " + std::to_string(fileSize) + "\r\n\r\n";

		// Send headers
		send(clientSocket, headers.c_str(), headers.size(), 0);

		// Allocate buffer to hold file content (optional for large files)
		char* buffer = new char[fileSize]; // Adjust buffer size as needed

		// Read and send the file content in chunks
		while (file.read(buffer, sizeof(char)*fileSize)) {
			if (!buffer) {
				break;
			}
			send(clientSocket, buffer, file.gcount(), 0);
		}

		// Close the file
		file.close();
	}

	RequestType parseRequestType(const std::string& request) {
		RequestType requestType = HTTP_UNKNOWN;

		for (int j = 0; j < 8; j++) {
			if (request == RequestTypeNames[j]) {
				requestType = static_cast<RequestType>(j);
				break;
			}
		}

		return requestType;
	}

	void CheckPasswordAndRedirect(const std::string& request) {

	}

	void ProcessRequest(SOCKET clientSocket)
	{
		char buffer[1024];
		int bytesReceived;
		std::string URI;
		RequestType requestType;
		try {
			std::cout << "----------------- Request received ----------------- " << std::endl;
			// Receive data from the client
			while ((bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
				// Print the entire request (for debugging and display of server's working)
				std::string request(buffer, bytesReceived);
				std::cout << request << std::endl;
				try {
					std::istringstream requestStream(request); // we will use this for easy word parsing

					std::string APIRequest; requestStream >> APIRequest;

					// Understand what kind of request was submitted
					requestType = parseRequestType(APIRequest);
					switch (requestType) {
					case HTTP_GET:
						// Send the file at the URI back to the client
						// We will assume that the URI is the second word in the request
						requestStream >> URI;
						SendFileWithHeaders(clientSocket, URI);
						std::cout << "Sent file: " << URI << std::endl;
						break;

					case HTTP_POST:

						break;
					default:
						throw NotImplemented("INTERNAL SERVER ERROR: The request type is not known: " + APIRequest);
					}
				}
				catch (const NotImplemented& E) {
					std::cout << E.HTTPDetailedWhat() << std::endl;
					std::string headers = "HTTP/1.1 501 Not Implemented\r\nContent-Length: 0\r\n\r\n";
					send(clientSocket, headers.c_str(), headers.size(), 0);
				}
				catch (const ResourceNotFoundException& E) {
					std::cout << E.HTTPDetailedWhat() << std::endl;
					std::string headers = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
					send(clientSocket, headers.c_str(), headers.size(), 0);
				}
				/*
				Disable the catch all for testing so we can see the exact errors when they come
				catch (...) {
					std::cout << "Unable to process request, ignored";
				}*/

				// Clear the buffer for next reception
				memset(buffer, 0, sizeof(buffer));
			}

			if (bytesReceived == SOCKET_ERROR) {
				throw GatewayTimeoutException("INTERNAL SERVER ERROR: Gateway Timeout, perhaps the connection was terminated?");
			}
		}
		catch (const GatewayTimeoutException& E) {
			std::cout << E.HTTPDetailedWhat() << std::endl;
			return;
		}
	}


	void HandleConnections(SOCKET listeningSocket)
	{
		while (true) {
			SOCKET clientSocket = accept(listeningSocket, NULL, NULL);
			if (clientSocket == INVALID_SOCKET) {
				std::cout << "Failed to accept connection :(" << std::endl;
				continue;
			}

			// Handle the connection (receive and parse HTTP request)
			// This is where the real work begins!
			ProcessRequest(clientSocket);
		}
	}
}