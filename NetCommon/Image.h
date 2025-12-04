#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

namespace smmo
{
	namespace image
	{
		struct Pixel
		{
			// uint8_t is an alias for unsigned char, so using 32bit ints to store r,g,b
			// avoiding the cast when we save.
			uint32_t red;
			uint32_t green;
			uint32_t blue;
		};

		class Image
		{
		public:
			int32_t height, width;
			std::vector<std::vector<Pixel>> pixels;

			Image(int32_t height, int32_t width) : height(height), width(width) 
			{
				pixels = std::vector<std::vector<Pixel>>(height, std::vector<Pixel>(width));

				for (int i = 0; i < height; i++)
				{
					for (int j = 0; j < height; j++)
					{
						pixels[i][j] = {255, 255, 255};
					}
				}
			}

			// Set pixel
			void SetXYToV(int32_t x, int32_t y, Pixel& v)
			{
				pixels[y][x] = v;
			}

			// P3 for ASCII text, 8 bit color depth, rgb.
			bool Save(const std::string& path)
			{
				try
				{
					std::ofstream ofs(path, std::ofstream::out);
					ofs.exceptions(std::ofstream::failbit | std::ofstream::badbit);

					// PPM header, width, height, and max colour value.
					// P3 for ASCII text, 8 bit color depth, rgb.
					ofs << "P3\n" << width << " " << height << "\n255\n";
					
					for (auto& row : pixels)
					{
						for (auto& pxl : row)
						{
							ofs << pxl.red << " " << pxl.green << " " << pxl.blue << "\n";
						}
					}
					return true;
				}
				
				catch (const std::ios_base::failure& e)
				{
					std::cerr << "Error saving file: " << path << ": " << e.what() << std::endl;
					return false;
				}

				catch (const std::exception& e)
				{
					std::cerr << "Error saving file: " << path << ": " << e.what() << std::endl;
					return false;
				}

				catch (...)
				{
					std::cerr << "Unknown error saving file: " << path << ": " << std::endl;
					return false;
				}
			}

			// Draw square of N length (pixels) at the XY offset.
			void DrawNSquareAtXY(int32_t x, int32_t y, int32_t N)
			{

				// Hardcoding color to black but this should be an argument.
				for (int i = y; i <= (y + N); i++)
				{
					for (int j = x; j <= (x + N); j++)
					{
						if (i == y || i == y+N || j == x || j == x + N)
						pixels[i][j] = { 0, 0, 0 };
					}
				}
			}

			// Draw filled square of N length (pixels) at the XY offset.
			void DrawNFilledSquareAtXY(int32_t x, int32_t y, int32_t N)
			{

				// Hardcoding color to black but this should be an argument.
				for (int i = y; i <= (y + N); i++)
				{
					for (int j = x; j <= (x + N); j++)
					{
							pixels[i][j] = { 0, 0, 0 };
					}
				}
			}


		private:
			
		};
	}
}