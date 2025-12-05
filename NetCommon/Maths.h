#pragma once

#include "Image.h"

namespace smmo
{
	namespace maths
	{
		smmo::image::Image BoxBlur(smmo::image::Image& img)
		{
			// 3x3 Kernel, odd so we have a center.
			smmo::image::Kernel krnl(3);

			// Bounds checking? Ie krnl < img.
			for (auto& row : img.pixels)
			{
				for (auto& col : row)
				{
					/* 
					  Slide kernel across the image (convolve f * g).
					  Let M x N be image size, m x n be kernel size,
					  for now we have O(M * N * m * n). But since
					  box blur is separable we can decompose kernel into
					  two 1d vectors reducing complexity to: O(M * N (m+n)).
					 */
				}
			}
		}
	}
}