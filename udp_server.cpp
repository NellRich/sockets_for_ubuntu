// Серверная реализация модели клиент-сервер UDP
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string>
#include <cstring>

constexpr uint16_t BUFFER_SIZE = 1024;

// Код драйвера

int main(int argc, char const *argv[])
{
    if (argc != 2 && argc != 3)
    {
        printf("Не указан порт\n");
        return -1;
    }

    int sockfd = 0;
    char buffer[BUFFER_SIZE] = {0};

    std::string message = argc == 3 ? argv[2] : "Данные получены сервером";

    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Создание дескриптора файла сокета
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("Не удалось создать сокет\n");
        return -1;
    }

    // Заполнение информации о сервере
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = std::stoi(argv[1]);

    // Привязываем сокет с адресом сервера
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("Не удалось привязать сокет к порту\n");
        return -1;
    }

    // Цикл приёма сообщений
    while (true)
    {
        int len = sizeof(socklen_t *);
        int valread = 0;

        valread = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&cliaddr, (socklen_t *)&len);

        if (valread < 0)
        {
            printf("Не удалось прочитать данные\n");
            break;
        }

        printf("Полученные данные: %s\n", buffer);

        if (sendto(sockfd, (const char *)message.c_str(), message.length(), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len) < 0)
        {
            printf("Не удалось отправить данные\n");
            break;
        }

        printf("Сообщение отправлено\n\n");
        memset(buffer, '\0', BUFFER_SIZE);
    }

    return 0;
}
