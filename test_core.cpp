
#include <iostream>         // For std::cout, std::cerr
#include <sys/socket.h>     // For socket(), bind(), listen(), accept()
#include <netinet/in.h>     // For sockaddr_in structure
#include <unistd.h>         // For close(), read(), write()
#include <sys/epoll.h>      // For epoll_create1(), epoll_ctl(), epoll_wait()
#include <fcntl.h>          // For fcntl() to set non-blocking mode
#include <string.h>         // For memset()
#include <errno.h>          // For errno

// --- Constants ---
#define PORT 8080           // The port we will listen on
#define MAX_EVENTS 10       // Max events to retrieve from epoll_wait() at once
#define BUFFER_SIZE 1024    // Size of the buffer for reading client data

// --- Error Handling Function ---
// A simple utility to print an error message and exit
void error(const char *msg) {
    perror(msg); // perror prints the error message plus the system error description
    exit(EXIT_FAILURE);
}

int main() {
    // ====================================================================
    // STEP 1: SET UP THE LISTENING SOCKET
    // This is the special socket that just listens for new connection requests.
    // ====================================================================

    // 1a. Create the socket
    int listener_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listener_fd < 0) {
        error("Failed to create socket");
    }
    std::cout << "1. Listening socket created with fd: " << listener_fd << std::endl;

    // 1b. Make the listening socket non-blocking
    // This is crucial for event-driven models.
    if (fcntl(listener_fd, F_SETFL, O_NONBLOCK) < 0) {
        error("Failed to set socket to non-blocking");
    }

    // 1c. Define the server address structure
    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr)); // Clear the struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // 1d. Bind the socket to our specified IP and port
    if (bind(listener_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        error("Failed to bind socket");
    }
    std::cout << "2. Socket bound to port " << PORT << std::endl;

    // 1e. Start listening for incoming connections
    if (listen(listener_fd, SOMAXCONN) < 0) {
        error("Failed to listen on socket");
    }
    std::cout << "3. Server is listening for connections..." << std::endl;

    // ====================================================================
    // STEP 2: SET UP THE EPOLL INSTANCE
    // This is the kernel mechanism that will tell us when a socket has an event.
    // ====================================================================

    // 2a. Create the epoll instance
    int epoll_fd = epoll_create1(0);
    if (epoll_fd < 0) {
        error("Failed to create epoll instance");
    }
    std::cout << "4. Epoll instance created with fd: " << epoll_fd << std::endl;

    // 2b. Register the listening socket with epoll
    // We want to be notified when a new connection is ready to be accepted.
    struct epoll_event event;
    event.events = EPOLLIN; // We are interested in "read" events (i.e., incoming data/connections)
    event.data.fd = listener_fd;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listener_fd, &event) < 0) {
        error("Failed to add listening socket to epoll");
    }
    std::cout << "5. Listening socket added to epoll interest list." << std::endl;

    // ====================================================================
    // STEP 3: THE EVENT LOOP
    // This is the heart of the server. It waits for events and dispatches them.
    // ====================================================================

    struct epoll_event events[MAX_EVENTS];
    char buffer[BUFFER_SIZE];

    std::cout << "\nEntering the event loop. Waiting for events..." << std::endl;

    while (true) {
        // 3a. Wait for events
        // This is the ONLY blocking call in the entire program.
        int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (num_events < 0) {
            error("epoll_wait failed");
        }

        // 3b. Loop through all the detected events
        for (int i = 0; i < num_events; ++i) {
            // --- CASE 1: Event on the listening socket ---
            // This means a new client is trying to connect.
            if (events[i].data.fd == listener_fd) {
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                int client_fd = accept(listener_fd, (struct sockaddr *)&client_addr, &client_len);
                
                if (client_fd < 0) {
                    perror("accept failed");
                    continue; // Don't let a failed accept crash the server
                }

                // Make the new client socket non-blocking
                if (fcntl(client_fd, F_SETFL, O_NONBLOCK) < 0) {
                    perror("Failed to set client socket to non-blocking");
                    close(client_fd);
                    continue;
                }

                // Add the new client socket to our epoll interest list
                event.events = EPOLLIN; // We want to know when the client sends data
                event.data.fd = client_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) < 0) {
                    perror("Failed to add client socket to epoll");
                    close(client_fd);
                    continue;
                }
                std::cout << "  [Event] New connection accepted with fd: " << client_fd << std::endl;

            }
            // --- CASE 2: Event on a client socket ---
            // This means an existing client sent us data.
            else {
                int client_fd = events[i].data.fd;
                ssize_t bytes_read = read(client_fd, buffer, BUFFER_SIZE);

                // If read returns 0, the client has closed the connection.
                if (bytes_read == 0) {
                    std::cout << "  [Event] Client disconnected with fd: " << client_fd << std::endl;
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL); // Remove from interest list
                    close(client_fd); // Close the socket
                } 
                // If read returns > 0, we have data.
                else if (bytes_read > 0) {
                    std::cout << "  [Event] Received " << bytes_read << " bytes from fd: " << client_fd << std::endl;
                    // Echo the data back to the client
                    write(client_fd, buffer, bytes_read);
                }
                // If read returns < 0, an error occurred.
                else {
                    // For non-blocking sockets, EAGAIN and EWOULDBLOCK are not "real" errors.
                    // They just mean "no data right now, try again later". We can ignore them
                    // because epoll will tell us when to try again.
                    if (errno != EAGAIN && errno != EWOULDBLOCK) {
                        perror("read error");
                        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
                        close(client_fd);
                    }
                }
            }
        }
    }

    // Cleanup (though the loop is infinite, this is good practice)
    close(listener_fd);
    close(epoll_fd);

    return 0;
}

