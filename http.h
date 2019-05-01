#ifndef _HTTP_H
#define _HTTP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ws2tcpip.h>

namespace Web
{
    class Http
    {
        int errcode;
        SOCKET connection;
        unsigned int bytes_read, bytes_sent;
        char buffer[2048], request_string[2048];
        struct addrinfo addr, *ai;
        const char *host;

        public:
            Http(const char *host, const char *port);
            ~Http();
            char *Get(const char *target, const char *params);
            char *Post(const char *target, const char *params);
    };
}

#include "http.cpp"

#endif
