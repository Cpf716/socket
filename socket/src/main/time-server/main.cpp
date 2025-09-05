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

int main(int argc, const char* argv[]) {
    // Initialize server
    auto server = new tcp_server(8080);

    thread([]() {
        // Connect to server
        auto client = new tcp_client("localhost", 8080);

        // Read date and time from the server
        for (size_t i = 0; i < 10; i++)
            cout << client->recv() << endl;

        // Disconnect and perform garbage collection
        client->close();
    }).detach();

    // Wait for connection
    vector<tcp_server::connection*> connections;

    while (true) {
        server->connections(connections);

        if (connections.size())
            break;
    }

    // Send date and time to client on the second
    while (true) {
        time_t now = time(0);
        char*  dt = ctime(&now);

        try {
            connections[0]->send(string(dt));
            
            // Client disconnected
        } catch (mysocket::error& e) {
            break;
        }

        this_thread::sleep_for(chrono::milliseconds(1000));
    }

    // Shutdown server and perform garbage collection
    server->close();
}
