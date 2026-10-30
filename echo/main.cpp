#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
    const char* server_ip = "172.16.40.1";
    int server_port = 7;

    if (argc >= 2) server_ip = argv[1];
    if (argc >= 3) server_port = std::stoi(argv[2]);

    std::cout << "Подключение к Echo-серверу (TCP) " << server_ip << ":" << server_port << "...\n";

    //создание сокета (TCP)
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1) {
        std::cerr << "Ошибка создания сокета!\n";
        return 1;
    }

    sockaddr_in srv_addr{};
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(server_port);
    srv_addr.sin_addr.s_addr = inet_addr(server_ip);

    //установка TCP соединения
    if (connect(s, (const sockaddr*)&srv_addr, sizeof(srv_addr)) == -1) {
        std::cerr << "Ошибка подключения к серверу " << server_ip << ":" << server_port << "!\n";
        close(s);
        return 1;
    }
    std::cout << "Соединение установлено! Введите строку для отправки: ";

    std::string msg;
    std::getline(std::cin, msg);
    msg += "\n";

    //отправка данных серверу
    ssize_t sent = send(s, msg.c_str(), msg.length(), 0);
    if (sent == -1) {
        std::cerr << "Ошибка отправки данных!\n";
        close(s);
        return 1;
    }

    //прием эхо-ответа
    char buf[1024];
    ssize_t received = recv(s, buf, sizeof(buf) - 1, 0);
    if (received == -1) {
        std::cerr << "Ошибка приема ответа!\n";
        close(s);
        return 1;
    }

    buf[received] = '\0';
    std::cout << "Эхо-ответ от сервера: " << buf;

    //закрытие сокета
    close(s);
    return 0;
}
