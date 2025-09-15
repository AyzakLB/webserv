#pragma once

// HEADERS 
#include <map>
#include <string>
#include <vector>
#include <cstring>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <cerrno>

// MACROS
#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define MAX_EVENTS 10
#define MAX_READ_SIZE 5000