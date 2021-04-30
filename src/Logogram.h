#include <vector>

#include "glm/glm.hpp"

#include "triangle.h"

float getElevation(float theta)
{
	if (theta > 300.0f)
		return ((float)rand() / RAND_MAX) * 10.0f;
	return 1.0f;
}

namespace Logogram {

	std::vector<triangle> getLogogram(float diff, int& indexCount)
	{
		std::vector<triangle>logogram;

		float theta = 0;
		float maxHeight = 0.01f;
		point a0 = { 0.5f , 0 };
		point a1 = { 0.5f + getElevation(theta) * maxHeight, 0 };
		theta += diff;
		while (theta <= 360.0f)
		{
			point b0 = { 0.5f * cos(glm::radians(theta)),0.5f * sin(glm::radians(theta)) };
			float newRadius = 0.5f + getElevation(theta) * maxHeight;
			point b1 = { newRadius * cos(glm::radians(theta)), newRadius * sin(glm::radians(theta)) };
			logogram.push_back({ a0 , a1 , b1 });
			logogram.push_back({ a0 , b1 , b0 });
			indexCount += 6;
			a0 = b0;
			a1 = b1;
			theta += diff;
		}
		if (theta > 360.0f) {
			theta = 360.0f;
			point b0 = { 0.5f * cos(glm::radians(theta)),0.5f * sin(glm::radians(theta)) };
			float newRadius = 0.5f + getElevation(theta) * maxHeight;
			point b1 = { newRadius * cos(glm::radians(theta)), newRadius * sin(glm::radians(theta)) };
			logogram.push_back({ a0 , a1 , b1 });
			logogram.push_back({ a0 , b1 , b0 });
			indexCount += 6;
		}



		return logogram;
	}
}