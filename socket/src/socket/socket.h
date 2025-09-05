//
//  socket.h
//  socket
//
//  Created by Corey Ferguson on 9/3/25.
//

#ifndef socket_h
#define socket_h

#include "util.h"
#include <arpa/inet.h>          // inet_ptons
#include <csignal>              // signal
#include <mutex>
#include <netinet/in.h>         // sockaddr_in
#include <sys/socket.h>         // socket
#include <thread>
#include <unistd.h>             // close, read

namespace mysocket {
    // Typedef
    
    struct error: public std::exception {
        // Constructors

        error(const int errnum);

        error(const std::string what);

        // Member Fields

        int         errnum() const;

        const char* what() const throw();
    private:
        // Member Fields

        int         _errnum = 0;
        std::string _what;
    };

    struct tcp_client {
        // Constructors

        tcp_client(const std::string host, const int port);

        // Member Functions

        void        close();

        std::string recv();

        int         send(const std::string message);
    private:
        // Member Fields

        struct sockaddr_in* _address;
        int                 _file_descriptor;
    };

    struct tcp_server {
        // Typedef

        struct connection {
            // Constructors

            connection(const int file_descriptor);

            // Member Functions
            
            int         file_descriptor() const;

            std::string recv();

            int         send(const std::string message);
        private:
            // Member Fields

            int         _file_descriptor;
        };

        // Constructors

        tcp_server(const int port, const int backlog = 1024);

        tcp_server(const int port, const std::function<void(connection*)> handler, const int backlog = 1024);

        // Member Functions

        void connections(std::vector<connection*>& connections);

        void close();

        void close(struct connection* connection);
    private:
        // Member Fields

        struct sockaddr_in               _address;
        int                              _address_length;
        std::vector<connection*>         _connections;
        std::atomic<bool>                _done = false;
        int                              _file_descriptor;
        std::function<void(connection*)> _handler = [](const struct connection* connection){};
        std::thread                      _thread;
        std::mutex                       _mutex;

        // Member Functions

        int _find_connection(const struct connection* connection);

        int _find_connection(const struct connection* connection, const size_t start, const size_t end);
    };

    struct udp_socket {
        // Member Functions

        void        close();

        std::string recvfrom();

        int         sendto(const std::string message);
    protected:
        // Member Fields

        struct sockaddr_in* _address = NULL;
        int                 _file_descriptor;
    };

    struct udp_client: public udp_socket {
        // Constructors

        udp_client(const std::string host, const int port);
    };

    struct udp_server: public udp_socket {
        // Constructors

        udp_server(const int port);
    };
}

#endif /* socket_h */
