Web::Http::Http(const char *host, const char *port)
{
    WSADATA wsa;
    errcode = WSAStartup(MAKEWORD(2, 2), &wsa);

    if(errcode == 0)
    {
        memset(&addr, 0, sizeof(addr));
        
        addr.ai_family = AF_INET;
        addr.ai_socktype = SOCK_STREAM;
        addr.ai_protocol = IPPROTO_TCP;

        errcode = getaddrinfo(host, port, &addr, &ai);

        if(errcode == 0)
        {
            this->host = host;
            connection = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
            
            if(connection)
            {
                errcode = connect(connection, ai->ai_addr, ai->ai_addrlen);
            }
        }
    }
}

Web::Http::~Http()
{
    WSACleanup();
}

char *Web::Http::Get(const char *target, const char *params)
{
    static char *result = 0;
    snprintf(request_string, sizeof(request_string), 
                                   "GET %s?%s HTTP/1.1\r\n"
                                   "Host: %s\r\n\r\n",
                                   target, params, host);

    bytes_sent = send(connection, request_string, strlen(request_string), 0);

    errcode = (bytes_sent == strlen(request_string)) ? 0 : -1;

    if(errcode == 0)
    {
        memset(buffer, 0, sizeof(buffer));

        while(bytes_read = recv(connection, buffer, sizeof(buffer), 0))
        {
            result = strstr(buffer, "\r\n\r\n");

            if(!result)
            {
                errcode = -1;
                return 0;
            }
            else
                result += 4;
        }
    }

    return result;
}
char *Web::Http::Post(const char *target, const char *params)
{
    static char *result = 0;
    snprintf(request_string, sizeof(request_string), 
                                   "POST %s HTTP/1.1\r\n"
                                   "Content-Type: application/x-www-form-urlencoded\r\n"
                                   "Content-Length: %d\r\n"
                                   "Host: %s\r\n\r\n%s",
                                   target, strlen(params), host, params);

    bytes_sent = send(connection, request_string, strlen(request_string), 0);

    errcode = (bytes_sent == strlen(request_string)) ? 0 : -1;

    if(errcode == 0)
    {
        memset(buffer, 0, sizeof(buffer));

        while(bytes_read = recv(connection, buffer, sizeof(buffer), 0))
        {
            result = strstr(buffer, "\r\n\r\n");

            if(!result)
            {
                errcode = -1;
                return 0;
            }
            else
                result += 4;
        }
    }

    return result;
}
