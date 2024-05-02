#pragma once

#include <array>
#include <fstream>

#include "hittable.h"
#include "material.h"
#include "rectangle.h"
#include "vec3.h"
#include <map>
#include <vector>

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

float fresnel(kajiya::Vec3 I, kajiya::Vec3 N, float ior1, float ior2) {
	float cosi = clamp(-1, 1, kajiya::Vec3::dot(I, N));
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

void load_scene(std::vector<kajiya::Hittable *> &objects) {

	kajiya::Material white;
	white.brdf.reflectance_spectrum[500] = 0.747;
	white.brdf.reflectance_spectrum[600] = 0.740;
	white.brdf.reflectance_spectrum[700] = 0.737;
	white.type							 = kajiya::Material::diffuse;

	kajiya::Material red;
	red.brdf.reflectance_spectrum[500] = 0.058;
	red.brdf.reflectance_spectrum[600] = 0.287;
	red.brdf.reflectance_spectrum[700] = 0.642;
	red.type						   = kajiya::Material::diffuse;

	kajiya::Material green;
	green.brdf.reflectance_spectrum[500] = 0.285;
	green.brdf.reflectance_spectrum[600] = 0.160;
	green.brdf.reflectance_spectrum[700] = 0.159;
	green.type							 = kajiya::Material::diffuse;

	kajiya::Material light;
	light.brdf.reflectance_spectrum[500] = 8.0;
	light.brdf.reflectance_spectrum[600] = 15.6;
	light.brdf.reflectance_spectrum[700] = 18.4;
	light.type							 = kajiya::Material::light;

	// floor, white
	kajiya::Rectangle floor(
		kajiya::Vec3(552.8, 0.0, 0.0), kajiya::Vec3(0.0, 0.0, 0.0),
		kajiya::Vec3(0.0, 0.0, 559.2), kajiya::Vec3(549.6, 0.0, 559.2), white);

	// light
	kajiya::Rectangle light(kajiya::Vec3(343.0, 548.8, 227.0),
							kajiya::Vec3(343.0, 548.8, 332.0),
							kajiya::Vec3(213.0, 548.8, 332.0),
							kajiya::Vec3(213.0, 548.8, 227.0), light);

	// ceiling, white
	kajiya::Rectangle ceiling(
		kajiya::Vec3(556.0, 548.8, 0.0), kajiya::Vec3(556.0, 548.8, 559.2),
		kajiya::Vec3(0.0, 548.8, 559.2), kajiya::Vec3(0.0, 548.8, 0.0), white);

	// back wall, white
	kajiya::Rectangle back_wall(kajiya::Vec3(549.6, 0.0, 559.2),
								kajiya::Vec3(0.0, 0.0, 559.2),
								kajiya::Vec3(0.0, 548.8, 559.2),
								kajiya::Vec3(556.0, 548.8, 559.2), white);

	// right wall, green
	kajiya::Rectangle right_wall(
		kajiya::Vec3(0.0, 0.0, 559.2), kajiya::Vec3(0.0, 0.0, 0.0),
		kajiya::Vec3(0.0, 548.8, 0.0), kajiya::Vec3(0.0, 548.8, 559.2), green);

	// left wall, red
	kajiya::Rectangle left_wall(kajiya::Vec3(552.8, 0.0, 0.0),
								kajiya::Vec3(549.6, 0.0, 559.2),
								kajiya::Vec3(556.0, 548.8, 559.2),
								kajiya::Vec3(556.0, 548.8, 0.0), red);

	// short block, white
	kajiya::Rectangle short_block_top(kajiya::Vec3(130.0, 165.0, 65.0),
									  kajiya::Vec3(82.0, 165.0, 225.0),
									  kajiya::Vec3(240.0, 165.0, 272.0),
									  kajiya::Vec3(290.0, 165.0, 114.0), white);

	kajiya::Rectangle short_block_left(kajiya::Vec3(290.0, 0.0, 114.0),
									   kajiya::Vec3(290.0, 165.0, 114.0),
									   kajiya::Vec3(240.0, 165.0, 272.0),
									   kajiya::Vec3(240.0, 0.0, 272.0), white);

	kajiya::Rectangle short_block_front(kajiya::Vec3(130.0, 0.0, 65.0),
										kajiya::Vec3(130.0, 165.0, 65.0),
										kajiya::Vec3(290.0, 165.0, 114.0),
										kajiya::Vec3(290.0, 0.0, 114.0), white);
	kajiya::Rectangle short_block_right(kajiya::Vec3(82.0, 0.0, 225.0),
										kajiya::Vec3(82.0, 165.0, 225.0),
										kajiya::Vec3(130.0, 165.0, 65.0),
										kajiya::Vec3(130.0, 0.0, 65.0), white);
	kajiya::Rectangle short_block_back(kajiya::Vec3(240.0, 0.0, 272.0),
									   kajiya::Vec3(240.0, 165.0, 272.0),
									   kajiya::Vec3(82.0, 165.0, 225.0),
									   kajiya::Vec3(82.0, 0.0, 225.0), white);

	// tall block, white
	kajiya::Rectangle tall_block_top(kajiya::Vec3(423.0, 330.0, 247.0),
									 kajiya::Vec3(265.0, 330.0, 296.0),
									 kajiya::Vec3(314.0, 330.0, 456.0),
									 kajiya::Vec3(472.0, 330.0, 406.0), white);
	kajiya::Rectangle tall_block_left(kajiya::Vec3(423.0, 0.0, 247.0),
									  kajiya::Vec3(423.0, 330.0, 247.0),
									  kajiya::Vec3(472.0, 330.0, 406.0),
									  kajiya::Vec3(472.0, 0.0, 406.0), white);
	kajiya::Rectangle tall_block_back(kajiya::Vec3(472.0, 0.0, 406.0),
									  kajiya::Vec3(472.0, 330.0, 406.0),
									  kajiya::Vec3(314.0, 330.0, 456.0),
									  kajiya::Vec3(314.0, 0.0, 456.0), white);
	kajiya::Rectangle tall_block_right(kajiya::Vec3(314.0, 0.0, 456.0),
									   kajiya::Vec3(314.0, 330.0, 456.0),
									   kajiya::Vec3(265.0, 330.0, 296.0),
									   kajiya::Vec3(265.0, 0.0, 296.0), white);
	kajiya::Rectangle tall_block_front(kajiya::Vec3(265.0, 0.0, 296.0),
									   kajiya::Vec3(265.0, 330.0, 296.0),
									   kajiya::Vec3(423.0, 330.0, 247.0),
									   kajiya::Vec3(423.0, 0.0, 247.0), white);
}
