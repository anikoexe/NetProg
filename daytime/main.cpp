#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
    const char* server_ip = "172.16.40.1";
    int server_port = 13;

    if (argc >= 2) server_ip = argv[1];
    if (argc >= 3) server_port = std::stoi(argv[2]);

    std::cout << "Подключение к Daytime-серверу (UDP) " << server_ip << ":" << server_port << "...\n";

    //создание сокета (UDP)
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s == -1) {
        std::cerr << "Ошибка: не удалось создать сокет!\n";
        return 1;
    }

    //устанавливаем таймаут ожидания ответа
    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    //подготовка адресной структуры сервера
    sockaddr_in srv_addr{};
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(server_port); //преобразование порядка байт в сетевой
    srv_addr.sin_addr.s_addr = inet_addr(server_ip);

    //отправка пустого запроса для инициации ответа
    char dummy = '\n';
    ssize_t sent = sendto(s, &dummy, sizeof(dummy), 0,
                          (const sockaddr*)&srv_addr, sizeof(srv_addr));
    if (sent == -1) {
        std::cerr << "Ошибка при отправке запроса (sendto)!\n";
        close(s);
        return 1;
    }

    //прием строки с датой и временем
    char buf[512];
    sockaddr_in from_addr{};
    socklen_t from_len = sizeof(from_addr);

    ssize_t received = recvfrom(s, buf, sizeof(buf) - 1, 0,
                                (sockaddr*)&from_addr, &from_len);

    if (received == -1) {
        std::cerr << "Таймаут: ответ от сервера " << server_ip << " не получен (возможно, сервер недоступен).\n";
        close(s);
        return 1;
    }

    buf[received] = '\0'; //нуль-терминированная строка
    std::cout << "Ответ от сервера:\n" << buf << std::endl;

    //закрытие сокета
    close(s);
    return 0;
}
