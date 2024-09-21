#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <bitset>

using namespace std;
using namespace cv;

// Функция для встраивания сообщения в изображение с использованием LSB
void embedMessage(Mat& image, const string& message, int packingDegree) {

    // Сообщение в виде бинарной строки
    string binaryMessage;

    // Преобразуем сообщение в последовательность бит
    for (char c : message) {
        binaryMessage += bitset<8>(c).to_string();
    }

    // Проверяем, хватает ли места в изображении
    int totalBits = image.rows * image.cols * image.channels() * packingDegree;
    if (binaryMessage.size() > totalBits) {
        cerr << "Ошибка: размер сообщения слишком велик для данного изображения при степени упаковки " << packingDegree << endl;
        exit(1);
    }

    // Вставляем сообщение в изображение
    int messageIndex = 0;
    for (int row = 0; row < image.rows; row++) {
        for (int col = 0; col < image.cols; col++) {
            Vec3b& pixel = image.at<Vec3b>(row, col); // Получаем пиксель
            for (int channel = 0; channel < 3; channel++) { // Для каждого канала RGB
                if (messageIndex < binaryMessage.size()) {
                    // Изменяем младшие биты согласно степени упаковки
                    bitset<8> color(pixel[channel]);
                    for (int bitPos = 0; bitPos < packingDegree && messageIndex < binaryMessage.size(); bitPos++) {
                        color[bitPos] = binaryMessage[messageIndex++] - '0';
                    }
                    pixel[channel] = static_cast<uchar>(color.to_ulong()); // Сохраняем обновленный цвет канала
                }
            }
        }
    }
}

// Чтение текстового файла
string readTextFile(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Ошибка: не удалось открыть файл " << filePath << endl;
        exit(1);
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char** argv) {
    setlocale(LC_ALL, "ru");

    // Проверка количества аргументов
    if (argc != 4) {
        cout << "Использование: " << argv[0] << " <C:\\practika_leto\\steganografia\\image.jpg> <C:\\practika_leto\\steganografia\\text.txt> <1>" << endl;
        return -1;
    }
    cout << "Проверка аргументов завершена." << endl;
    string imagePath = argv[1];
    string secretFilePath = argv[2];
    int packingDegree;

    // Вывод путей
    cout << "Путь к изображению: " << imagePath << endl;
    cout << "Путь к текстовому файлу: " << secretFilePath << endl;

    try {
        packingDegree = stoi(argv[3]);
    }
    catch (const exception& e) {
        cerr << "Ошибка: неверный формат степени упаковки." << endl;
        return -1;
    }

    // Проверка допустимости степени упаковки
    if (packingDegree < 1 || packingDegree > 2) {
        cerr << "Ошибка: допустимая степень упаковки 1 или 2 бита на байт." << endl;
        return -1;
    }

    // Загрузка изображения
    cout << "Загрузка изображения: " << imagePath << endl;
    Mat image = imread(imagePath);
    if (image.empty()) {
        cerr << "Ошибка: не удалось загрузить изображение!" << endl;
        return -1;
    }
    cout << "Изображение успешно загружено. Размер: " << image.cols << "x" << image.rows << endl;

    // Чтение секретного сообщения
    string secretMessage = readTextFile(secretFilePath);
    if (secretMessage.empty()) {
        cerr << "Ошибка: файл с сообщением пуст." << endl;
        return -1;
    }
    cout << "Размер сообщения: " << secretMessage.size() << " символов." << endl;

    // Добавляем информацию о типе файла и его размере
    secretMessage = "txt" + to_string(secretMessage.size()) + "|" + secretMessage;

    // Встраиваем сообщение в изображение
    embedMessage(image, secretMessage, packingDegree);

    // Сохраняем изображение с вкрапленным сообщением
    imwrite("C:\\practika_leto\\steganografia\\stegano_image.png", image);

    cout << "Сообщение успешно встроено в изображение." << endl;
    return 0;
}

