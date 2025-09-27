//
//  main.cpp
//  socket
//
//  Created by Corey Ferguson on 9/3/25.
//

#include "socket.h"
#include <fstream>

using namespace mysocket;
using namespace std;

const std::string PATH = "";

int main(int argc, const char* argv[]) {
    // Initialize server
    auto server = new udp_server(8080);

    atomic<bool> alive = true;

    thread([&alive] {
        // Connect to server
        auto client = new udp_client("127.0.0.1", 8080);

        // Request file from server
        client->sendto("");

        // Receive file
        cout << client->recvfrom() << endl;

        // Disconnect and perform garbage collection
        client->close();

        alive.store(false);
    }).detach();

    // Wait for request from client
    server->recvfrom();

    ifstream      file(PATH);
    ostringstream oss;

    oss << file.rdbuf();

    file.close();

    // Send file
    server->sendto(oss.str());

    // Wait for client to receive message
    while (alive.load())
        continue;

    // Shut down server and perform garbage collection
    server->close();
}
