/*
 * main.cpp
 *
 *  Created on: Apr 5, 2016
 *      Author: Nir Leibovitch 200315232
 *      Author: Ron Naor 021615356
 *      Author: Shay Kremer 201588126
 */

#include <iostream>
#include <cmath>
#include "lodepng.h"

using namespace std;

const float robotWidth = 0.3;
const float robotHeight = 0.3;
const float mapResolution = 0.025;

const double robotSize = sqrt(pow(robotHeight/2, 2) + pow(robotWidth/2, 2));

int main() {
	std::vector<unsigned char> image; //the raw pixels
	unsigned width, height;
	unsigned error;

	//decode
	error = lodepng::decode(image, width, height, "/home/user/robotics/PcBotWorld/roboticLabMap.png");

	//if there's an error, display it
	if (error)
		std::cout << "decoder error " << error << ": "
				<< lodepng_error_text(error) << std::endl;

	std::vector<unsigned char> navImage; //the raw pixels
	navImage.resize(width * height * 4);

	for(int i = 0; i < width * height * 4; ++i) {
		navImage[i] = 255;
	}

	double blowSize = ceil((robotSize / 2) / mapResolution);
	unsigned x, y;

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			int pos = y*width*4 + x*4;
			if (image[pos + 0] || image[pos + 1] || image[pos + 2])
				continue;

			for(int i = -blowSize / 2; i < blowSize / 2; ++i) {
				for(int j = -blowSize / 2; j < blowSize / 2; ++j) {
					int blowPos = pos + i*width*4 + j*4;
					if(blowPos >= 0 && blowPos <= width * height * 4) {
						navImage[blowPos + 0] = 0;
						navImage[blowPos + 1] = 0;
						navImage[blowPos + 2] = 0;
						navImage[blowPos + 3] = 255;
					}
				}
			}
		}
	}

	//Encode the image
	error = lodepng::encode("newMap.png", navImage, width, height);

	//if there's an error, display it
	if (error)
		std::cout << "encoder error " << error << ": "
				<< lodepng_error_text(error) << std::endl;

	return 0;
}
