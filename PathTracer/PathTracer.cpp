#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    int image_height = 256;
    int image_width = 256;

    // Creating PPM file.
    ofstream ofs("my_image.ppm");
    cout.rdbuf(ofs.rdbuf());

    cout << "P3\n" << image_height << " " << image_width << "\n255";

    for (int i = 0; i < image_height; i++)
    {
        for (int j = 0; j < image_width; j++)
        {
            auto r = double(i) / (image_width - 1);
            auto g = double(j) / (image_width - 1);
            auto b = 0.0;

            int ir = (int)255.999 * r;
            int ig = (int)255.999 * g; // 255.999 * 1.0 = 255.
            int ib = (int)255.999 * b;

            cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}