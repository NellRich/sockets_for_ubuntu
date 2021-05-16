#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <cstdlib>
#include <netinet/in.h>
#include <cstring>
#include <string>

constexpr uint16_t BUFFER_SIZE = 1024;

int main(int argc, char const *argv[])
{
    if (argc != 2 && argc != 3)
    {
        printf("Не указан порт\n");
        return -1;
    }

    int server_fd = 0;
    int new_socket = 0;
    int valread = 0;

    struct sockaddr_in address;
    int addrlen = sizeof(struct sockaddr_in);

    int opt = 1;
    char buffer[BUFFER_SIZE] = {0};
    std::string message = argc == 3 ? argv[2] : "Данные получены сервером";

    // Создание дескриптора файла сокета
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        printf("Не удалось создать сокет\n");
        return -1;
    }

    // Задание явных настроек
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        printf("setsockopt\n");
        return -1;
    }

    // Инициализация адреса
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = std::stoi(argv[1]);

    // Принудительное подключение сокета к порту
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        printf("Не удалось привязать сокет к порту\n");
        return -1;
    }

    // Переход в пассивный режим
    if (listen(server_fd, 3) < 0)
    {
        printf("listen\n");
        return -1;
    }

    // Ожидание получения данных
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        printf("accept\n");
        return -1;
    }

    // Цикл приёма сообщений
    while (true)
    {
        // Попытка чтения данных
        valread = read(new_socket, buffer, BUFFER_SIZE);

        if (valread < 0)
        {
            printf("Не удалось прочитать данные\n");
            break;
        }
        else if (valread == 0)
        {
            printf("Сеанс связи завершён\n");
            break;
        }

        printf("Полученные данные:\n%s\n", buffer);

        // Отправление ответа
        if (send(new_socket, message.c_str(), message.length(), 0) < 0)
        {
            printf("Не удалось прочитать данные\n");
            break;
        }

        printf("Ответ отправлен\n\n");

        memset(buffer, '\0', BUFFER_SIZE);
    }

    return 0;
}
