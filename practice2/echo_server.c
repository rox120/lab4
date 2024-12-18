#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    socklen_t addr_len = sizeof(address);
    char buffer[BUF_SIZE] = {0};
    int valread;

    // 소켓 생성
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 주소 구조체 설정
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;  // 어떤 주소에서든 접속 허용
    address.sin_port = htons(PORT);

    // 바인드
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 리슨
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Echo server is running on port %d...\n", PORT);

    // 액셉트
    if ((client_fd = accept(server_fd, (struct sockaddr *)&address, &addr_len)) < 0) {
        perror("accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 클라이언트로부터 메시지 받고 에코
    while ((valread = read(client_fd, buffer, BUF_SIZE)) > 0) {
        // 받은 데이터를 그대로 돌려줌
        write(client_fd, buffer, valread);
        memset(buffer, 0, BUF_SIZE);
    }

    close(client_fd);
    close(server_fd);

    return 0;
}

