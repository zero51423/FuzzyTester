#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include <sys/select.h>
#include <string>

using namespace std;

class Network {

    public:

        // Public constructor
	Network(string ipAddress);

        // Public destructor
        ~Network();

        // Public methods
        bool get_query(string page);        // Perform a GET request
        bool post_query(string page);       // Perform a POST request

    private:

        // Private data members
	string myIpAddress;                 // Host IP address
        struct sockaddr_in *remote;
        struct timeval tv;
        int sock;
        int tmpres;
        fd_set readfds;

        // Private methods
        int create_tcp_socket();            // Establish master socket
};

#endif // NETWORK_H_INCLUDED
