#include <iostream>
#include <string>
#include <sstream>		// Stringstream library used to conveniently handle the API requests
#include <fstream>		// We will be sending all the files requested based on the URI
#include <winsock2.h>	// Win Sock (Windows Socket) is the library that will help us do our HTTP request handling
#include <exception>	// useful exception classes
#include <windows.h>	// API for windows features
#include <thread>		// we use multithreading for certain functionality like timing out send requests
#include <chrono>		// used for timing functions
#include <algorithm>	// useful algos like trim

#pragma once
#pragma comment(lib, "ws2_32.lib")

namespace HTTP{ 

	// MARK: HTTP Classes/Structs

	struct CurrentUser {
		std::string Name;
		std::string Pass;
		std::string Email;
		std::string Age;
		std::string Address;
		std::string Pets;
	};
	CurrentUser currUser;

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
	std::string UserData[] = {
		"/username",
		"/email",
		"/age",
		"/address",
		"/pets"
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

	class UnauthorisedError : public HTTPException
	{
		public:
			UnauthorisedError(std::string Message) : 
				HTTPException(Message, 401) {};
	};

	class InfiniteLoopException : public HTTPException
	{
		public:
			InfiniteLoopException(std::string Message) : 
				HTTPException(Message, 508) {};
	};

	// MARK: HTTP User Defined Functions

	// Function to remove whitespace characters from a string
	std::string trim_text(const std::string& str) {
		std::cout << "(DEBUG): Trimming string" << str << std::endl;
		std::string OUTPUT = "";
		int i = 0;

		for (i; i < str.size(); i++) {
			if (isspace(str[i])) {
				continue;
			}
			OUTPUT += str[i];
		}

		return OUTPUT;
	}
	/**
	 * @brief This function initialises the HTTP Server
	 * @return SOCKET: The listening socket for the server
	*/
	SOCKET InitialiseHTTPServer()
	{
		currUser.Name		= "NOBODY";
		currUser.Pass		= "NA";
		currUser.Email		= "NA";
		currUser.Address	= "NA";
		currUser.Age		= "NA";

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

	std::string getUserData(int place) {
		/*
			std::string UserData[] = {
				"/username",
				"/email",
				"/age",
				"/address",
				"/pets"
			};
		*/

		switch (place) {
			case 0:
				return currUser.Name;
			case 1:
				return currUser.Email;
			case 2:
				return currUser.Age;
			case 3:
				return currUser.Address;
			case 4:
				return currUser.Pets;
			default:
				return "NA";
		}
	}

	/**
	*	@brief This function sends a file to the client with appropriate headers
	*	@param clientSocket: The socket to send the file to
	*	@param filename: The name of the file to send
	*/
	void SendFileWithHeaders(SOCKET clientSocket, const std::string& filename) {

		// just send user data if thats whats requested
		for (int i = 0; i < UserData->size(); i++) {
			if (filename == UserData[i]) {
				std::cout << "(Debug) Datatype Match!" << std::endl;
				std::string headers = "HTTP/1.1 200 OK\r\n";
				std::string content = getUserData(i);
				headers += "Content-Type: \r\n";
				headers += "Content-Length: " + std::to_string(content.size()) + "\r\n\r\n";
				send(clientSocket, headers.c_str(), headers.size(), 0);
				send(clientSocket, content.c_str(), content.size(), 0);
				
				return;
			}
		}


		std::string current_dir = getCurrDir();
		std::cout << "(Debug) Current Directory: " << current_dir << std::endl; 
		std::string newFilename; // filename is a constant reference, so we need to create a new string to modify it
		newFilename = filename; // All our files are in the HTTP directory
		int i = 0;

		// Replace all forward slashes with backslashes
		// This is for better compatibility with Windows file system
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

		// Use a std::thread to send the file with a timeout
		std::atomic<bool> timedOut(false);
		auto sendThread = std::thread([&clientSocket, buffer, &file, &fileSize, &timedOut] {
			auto start = std::chrono::steady_clock::now();
			while (file.read(buffer, sizeof(char) * fileSize)) {
				if (timedOut) {
					break;
				}
				int bytesSent = send(clientSocket, buffer, file.gcount(), 0);
				if (bytesSent == SOCKET_ERROR) {
					break;
				}
				auto end = std::chrono::steady_clock::now();
				std::chrono::duration<double, std::milli> elapsed = end - start;
				// You can adjust the timeout value here in milliseconds
				if (elapsed.count() > 1000) { // Timeout after 1 second
					timedOut.store(true);
					break;
				}
			}
			});

		// Wait for the thread to finish or timeout
		sendThread.join();

		// Close the file
		file.close();

		// Free the buffer
		delete[] buffer;

		// Handle timeout scenario
		if (timedOut.load()) {
			throw GatewayTimeoutException("INTERNAL SERVER ERROR: TIMEOUT, file took too long to load, so it was interrupted");
		}
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

	void CheckPasswordAndRedirect(SOCKET clientSocket, std::string request) {
		int index = request.find("username=");
		std::string username;
		std::string password;
		index += 9;
		for (index; index < request.size(); index++) {
			if (request[index] == '&') {
				break;
			}
			username += request[index];
		}

		index = request.find("password=");
		index += 9;
		for (index; index < request.size(); index++) {
			if (request[index] == '\n') {
				break;
			}
			password += request[index];
		}

		std::cout << "(DEBUG): Username: " << username << std::endl;
		std::cout << "(DEBUG): Password: " << password << std::endl;

		std::ifstream userdata;

		userdata.open("HTTP\\PROTECTED\\" + username + "\\UserData.XML", std::ios::in);

		if(userdata.fail()) {
			std::string headers = "HTTP/1.1 303 See Other\r\n";
			headers += "Content-Type: \r\n";
			headers += "Location: /pages/access_denied.html\r\n";
			headers += "Content-Length: 0\r\n\r\n";

			send(clientSocket, headers.c_str(), headers.size(), 0);
			throw UnauthorisedError("User doesn't exist");
		}
		std::string completeData;

		std::string temp;
		while(std::getline(userdata, temp)) {
			completeData += temp;
		}

		std::string correctPassword;
		int index_start, index_end;

		index_start = completeData.find("<password>") + 10; // not the most elegant solution, i know xD
		index_end	= completeData.find("</password>"); 

		correctPassword = completeData.substr(index_start, index_end - index_start);
		correctPassword = trim_text(correctPassword);
		std::cout << "(DEBUG): Correct Password: \"" << correctPassword << "\"" << std::endl; // we add quotations so we can see any spaces at the edges


		if(password != correctPassword) {
			std::string headers = "HTTP/1.1 303 See Other\r\n";
			headers += "Content-Type: \r\n";
			headers += "Location: /pages/access_denied.html\r\n";
			headers += "Content-Length: 0\r\n\r\n";

			send(clientSocket, headers.c_str(), headers.size(), 0);
			throw UnauthorisedError("Password is incorrect");
		}

		index_start = completeData.find("<email>") + 7; 
		index_end = completeData.find("</email>");
		std::string email = completeData.substr(index_start, index_end - index_start);
		email = trim_text(email);

		index_start = completeData.find("<address>") + 9; 
		index_end = completeData.find("</address>");
		std::string address = completeData.substr(index_start, index_end - index_start);
		address = trim_text(address);

		index_start = completeData.find("<age>") + 5; 
		index_end	= completeData.find("</age>");
		std::string age = completeData.substr(index_start, index_end - index_start);
		age = trim_text(age);

		index_start = completeData.find("<pets>") + 6; 
		index_end = completeData.find("</pets>");
		std::string pets = completeData.substr(index_start, index_end - index_start);
		pets = trim_text(pets);

		currUser.Name		= username;
		currUser.Pass		= password;
		currUser.Email		= email;
		currUser.Address	= address;
		currUser.Age		= age;
		currUser.Pets		= pets;

		std::string headers = "HTTP/1.1 303 See Other\r\n";
		headers += "Content-Type: \r\n";
		headers += "Location: /pages/home.html\r\n";
		headers += "Content-Length: 0\r\n\r\n";

		send(clientSocket, headers.c_str(), headers.size(), 0);

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
						CheckPasswordAndRedirect(clientSocket, request);
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

				catch (const UnauthorisedError& E) {
					std::cout << E.HTTPDetailedWhat() << std::endl;
				}

				catch (const InfiniteLoopException& E) {
					std::cout << E.HTTPDetailedWhat() << std::endl;
				}
				
				// Disable the catch all for testing so we can see the exact errors when they come
				catch (...) {
					std::cout << "Unable to process request (Usually because too much data was prompted at once)";
					std::string headers = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";
					send(clientSocket, headers.c_str(), headers.size(), 0);
				}

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