// Клиентская реализация модели клиент-сервер UDP
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <iostream>

constexpr uint16_t BUFFER_SIZE = 1024;

// Код драйвера

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        printf("Не указан порт\n");
        return -1;
    }

    int sockfd = 0;
    char buffer[BUFFER_SIZE];

    struct sockaddr_in servaddr;

    // Создание дескриптора файла сокета
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("socket creation failed\n");
        return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Заполнение информации о сервере
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = std::stoi(argv[1]);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    while (true)
    {
        int valread = 0;
        int len = sizeof(socklen_t *);
        std::string message;

        // Ввод сообщения
        std::cin >> message;

        sendto(sockfd, message.c_str(), message.length(), MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));
        printf("Сообщение отправлено\n");

        valread = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&servaddr, (socklen_t *)&len);

        printf("Ответ сервера: %s\n", buffer);
        memset(buffer, '\0', BUFFER_SIZE);
    }

    return 0;
}
