#include "Client.h"
#include <fstream>
#include <stdexcept>

// Пути к файлам запросов и ответов
const std::string requestFilePath = "request_queue.txt";
const std::string responseFilePath = "response_queue.txt";

// Отправка запроса на сервер
void Client::sendRequest(const std::string& request) {
    std::ofstream requestFile(requestFilePath, std::ios::app); // Открытие в режиме добавления
    if (!requestFile.is_open()) {
        throw std::runtime_error("Ошибка: Не удалось открыть файл запросов.");
    }

    requestFile << request << "\n"; // Запись запроса в файл
    requestFile.close();
}

// Получение ответа от сервера
std::string Client::receiveResponse() {
    std::ifstream responseFile(responseFilePath);
    if (!responseFile.is_open()) {
        throw std::runtime_error("Ошибка: Не удалось открыть файл ответов.");
    }

    std::string response;
    if (std::getline(responseFile, response)) {
        // Удаляем прочитанный ответ
        std::ofstream tempFile(responseFilePath, std::ios::trunc); // Очистка файла
        tempFile.close();
        return response;
    }
    else {
        return "Нет ответа от сервера.";
    }
}
