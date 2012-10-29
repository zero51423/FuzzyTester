#include <stdio.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

#include "Network.h"

#define PAGE "/"
#define PORT 80
#define BUFFER_SIZE 40960
#define USERAGENT "HTMLGET 1.1"
#define CONTENT_TYPE "application/x-www-form-urlencoded"
#define FILE_LOC "file.xml"
#define TIMEOUT 5

// Pre: Any IPv4 address
// Post: Establishes a TCP connection to the host IPv4 address specififed
Network::Network(string address) {
    myIpAddress = address;

    // Create master socket
    sock = create_tcp_socket();
    remote = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in *));
    remote->sin_family = AF_INET;
    // Convert the IPv4 address into a network address structure
    tmpres = inet_pton(AF_INET, address.c_str(), (void *)(&(remote->sin_addr.s_addr)));
    if (tmpres < 0) {
        //error("Can't set remote->sin_addr.s_addr);
        exit(1);
    } else if (tmpres == 0) {
        //error("__ is not a valid IP address");
        exit(1);
    }
    remote->sin_port = htons(PORT);
    // Establish a connection to the host IPv4 address
    if (connect(sock, (struct sockaddr *)remote, sizeof(struct sockaddr)) < 0) {
        //error("Could not connect");
        exit(1);
    }

    // Set timeout
    tv.tv_sec = TIMEOUT;
    tv.tv_usec = 0;
    FD_SET(sock, &readfds);
}

// Pre: Network object instantiated
// Post: Deallocates memory associated with object and closes
//  TCP socket connection
Network::~Network() {
    FD_ZERO(&readfds);
    free(remote);
    close(sock);
}

// Pre: None
// Post: Returns a valid socket descriptor denoting state of the connection, otherwise it will return -1.
//  This will create a socket in which the method of connection is a byte stream.
int Network::create_tcp_socket() {
    int sockLoc;
    if ((sockLoc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        //error("Can't createTCP socket);
        exit(1);
    }

    return sockLoc;
}

// Pre: Specific page associated with a URL (i.e. "/webservices/automation/data/panel.xml"
//  of "http://128.163.146.74/webservices/automation/data/panel.xml")
// Post: Returns true if the GET request was successful, false otherwise. If valid,
//  also generates a file for the response
bool Network::get_query(string page) {
    string getPage = page;

    if (getPage.at(0) == '/') {
        getPage = getPage.substr(1, getPage.length() - 1);
    }

    // Build GET request
    string query = "GET " + getPage + " HTTP/1.1\r\nHost: " + myIpAddress + "\r\nUser-Agent: " + USERAGENT + "\r\n\r\n";
    // Send the GET request to the host
    send(sock, query.c_str(), query.length(), 0);
    char buffer[BUFFER_SIZE + 1];

    tmpres = select(sock + 1, &readfds, NULL, NULL, &tv);

    if (tmpres == -1) {
        //error("Error occured in select()");
        return false;
    } else if (tmpres == 0) {
        //error("Timeout occured");
        return false;
    } else {
        recv(sock, buffer, BUFFER_SIZE, 0);
    }

    // Write response to file
    ofstream myfile;
    myfile.open(FILE_LOC);
    myfile << buffer;
    myfile.close();

    return true;
}

// Pre: Specific page & parameters associated with a URL (i.e. "/webservices/automation/request/touch?x=100&y=50"
//  of "http://128.163.146.74/webservices/automation/request/touch?x=100&y=50")
// Post: Returns true if the POST request was successful, false otherwise. If valid,
//  also generates a file for the response
bool Network::post_query(string page) {
    // Extract parameters for POST
    int paramLoc = page.find_first_of('?');
    if (paramLoc == string::npos) {
        //error
        exit(1);
    }
    string params = page.substr(paramLoc + 1, page.length() - paramLoc);

    string getPage;
    if (page.at(0) == '/') {
        getPage = page.substr(1, paramLoc - 1);
    } else {
        getPage = page.substr(0, paramLoc - 1);
    }

    // Build POST request
    string query = "POST " + getPage + " HTTP/1.1\r\nHost: " + myIpAddress + "\r\nContent-type: " + CONTENT_TYPE + "\r\n\r\n" + params + "\r\n\r\n";
    // Send the POST request to the host
    send(sock, query.c_str(), query.length(), 0);
    char buffer[BUFFER_SIZE + 1];
    tmpres = select(sock + 1, &readfds, NULL, NULL, &tv);

    if (tmpres == -1) {
        //error("Error occured in select()");
        return false;
    } else if (tmpres == 0) {
        //error("Timeout occured");
        return false;
    } else {
        recv(sock, buffer, BUFFER_SIZE, 0);
    }

    // Write response to file
    ofstream myfile;
    myfile.open(FILE_LOC);
    myfile << buffer;
    myfile.close ();

    return true;
}
