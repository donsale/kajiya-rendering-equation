#pragma once

#include <array>
#include <fstream>

#include "vec3.h"

template <unsigned int width, int height>
void save(std::string filename,
		  std::array<std::array<unsigned int, width>, height> pixels) {
	std::ofstream out_ppm;
	out_ppm.open(filename, std::ios_base::binary);
	out_ppm << "P6\n" << width << " " << height << "\n255\n";

	for (const auto &innerArray : pixels) {
		for (int val : innerArray) {
			out_ppm << static_cast<char>((val >> 24) & 0xFF);
			out_ppm << static_cast<char>((val >> 16) & 0xFF);
			out_ppm << static_cast<char>((val >> 8) & 0xFF);
		}
	}
}

float clamp(float low, float high, float value) {
	return std::max(low, std::min(high, value));
}

float fresnel(kajiya::vec3 I, kajiya::vec3 N, float ior1, float ior2) {
	float cosi = clamp(-1, 1, kajiya::vec3::dot(I, N));
	float etai = ior1, etat = ior2;
	if (cosi > 0) {
		std::swap(etai, etat);
	}
	// Compute sini using Snell's law
	float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
	// Total internal reflection
	if (sint >= 1) {
		return 1;
	}
	float cost = sqrtf(std::max(0.f, 1 - sint * sint));
	cosi	   = fabsf(cosi);
	float Rs =
		((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
	float Rp =
		((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
	return (Rs * Rs + Rp * Rp) / 2;

	// As a consequence of the conservation of energy, transmittance is given
	// by: kt = 1 - kr;
}