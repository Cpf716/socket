//
//  main.cpp
//  socket
//
//  Created by Corey Ferguson on 9/3/25.
//

#include "socket.h"
#include <ctime>

using namespace mysocket;
using namespace std;

tcp_server* server = NULL;
tcp_client* client = NULL;

void onsignal(int signum) {
    // Perform garbage collection
    client->close();
    server->close();
}

int main(int argc, const char* argv[]) {
    signal(SIGINT, onsignal);
    signal(SIGTERM, onsignal);

    // Initialize server
    server = new tcp_server(8080);

    thread([]() {
        // Connect to server
        client = new tcp_client("127.0.0.1", 8080);

        while (true)
            cout << client->recv() << endl;
    }).detach();

    // Wait for connection
    vector<tcp_server::connection*> connections;

    while (true) {
        connections = server->connections();

        if (connections.size())
            break;
    }

    // Send date and time to client on the second
    while (true) {
        time_t now = time(0);
        char*  dt = ctime(&now);

        connections[0]->send(string(dt));

        this_thread::sleep_for(chrono::milliseconds(1000));
    }
}
