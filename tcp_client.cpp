#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <iostream>

constexpr uint16_t BUFFER_SIZE = 1024;

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        printf("Не указан порт\n");
        return -1;
    }

    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Невозможно создать сокет\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = std::stoi(argv[1]);

    // Преобразование адресов IPv4 и IPv6 из текста в двоичную форму
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("Неверный адрес\n");
        return -1;
    }

    // Попытка соединения к сокету
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("Не удалось соединиться\n");
        return -1;
    }

    while (true)
    {
        int valread = 0;
        std::string message;

        // Ввод сообщения
        std::cin >> message;

        // Отправка сообщения
        if (send(sock, message.c_str(), message.length(), 0) < 0)
        {
            printf("Не удалось отправить сообщение\n");
            break;
        }

        printf("Сообщение отправлено\n\n");

        // Чтение ответа
        valread = read(sock, buffer, BUFFER_SIZE);
        if (valread < 0)
        {
            printf("Не удалось прочитать ответ\n");
            break;
        }

        printf("Полученный ответ:\n%s\n\n", buffer);
        memset(buffer, '\0', BUFFER_SIZE);
    }

    return 0;
}
