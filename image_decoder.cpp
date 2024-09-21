#include <opencv2/opencv.hpp>
#include <iostream>
#include <bitset>
#include <string>

using namespace std;
using namespace cv;

// Функция для извлечения сообщения из изображения
string extractMessage(const Mat& image, int packingDegree) {
    string binaryMessage;
    int messageBits = 0;

    // Извлечение битов из пикселей
    for (int row = 0; row < image.rows; row++) {
        for (int col = 0; col < image.cols; col++) {
            Vec3b pixel = image.at<Vec3b>(row, col);
            for (int channel = 0; channel < 3; channel++) {
                // Извлекаем младшие биты в зависимости от степени упаковки
                bitset<8> color(pixel[channel]);
                for (int bitPos = 0; bitPos < packingDegree; bitPos++) {
                    binaryMessage += color[bitPos] ? '1' : '0';
                }
            }
        }
    }

    // Преобразование бинарной строки в символы
    string extractedMessage;
    for (size_t i = 0; i < binaryMessage.size(); i += 8) {
        if (binaryMessage[i] == '0' && binaryMessage[i + 1] == '0' && binaryMessage[i + 2] == '0' && binaryMessage[i + 3] == '0') {
            break; // Завершение сообщения (можно изменить условие)
        }
        bitset<8> bits(binaryMessage.substr(i, 8));
        extractedMessage += static_cast<char>(bits.to_ulong());
    }

    return extractedMessage;
}

int main(int argc, char** argv) {

    setlocale(LC_ALL, "ru");
    // Проверка количества аргументов
    if (argc != 3) {
        cout << "Использование: " << argv[0] << " <C:\\practika_leto\\steganografia\\stegano_image.png> <1>" << endl;
        return -1;
    }

    string imagePath = argv[1];
    int packingDegree;

    // Получение степени упаковки
    try {
        packingDegree = stoi(argv[2]);
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
    Mat image = imread(imagePath);
    if (image.empty()) {
        cerr << "Ошибка: не удалось загрузить изображение!" << endl;
        return -1;
    }

    // Извлечение сообщения
    string secretMessage = extractMessage(image, packingDegree);
    cout << "Извлеченное сообщение: " << secretMessage << endl;

    return 0;
}