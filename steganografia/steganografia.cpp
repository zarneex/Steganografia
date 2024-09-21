#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    setlocale(LC_ALL, "RU");
    Mat image = imread("C:\\practika_leto\\steganografia\\image.jpg");
    if (image.empty()) {
        cout << "Ошибка: не удалось загрузить изображение!" << endl;
        return -1;
    }
    imshow("Отображение изображения", image);
    waitKey(0);
    return 0;
}