#include "http.h"

int main(int argc, char *argv[])
{
    Web::Http http1("127.0.0.1", "80");
    
    char *result = http1.Post("/website/public/test.php", "submit=Submit&user[email]=&user[password]=");

    if(result)
    {
        printf("%s", result);
    }
    else
    {
        puts("Error!");
    }
    
    return 0;
}
