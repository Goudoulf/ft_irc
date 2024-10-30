#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include <fcntl.h>

void setNonBlocking(int sockfd)
{
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
}
