#pragma once

#include "hittable.h"
#include "material.h"
#include "rectangle.h"
#include "vec3.h"
#include <vector>

namespace kajiya {
class Loader {
public:
	static std::vector<Hittable *> load_cornell_box() {
		std::vector<kajiya::Hittable *> objects;
		// floor
		objects.push_back(new kajiya::Rectangle(
			kajiya::Vec3(552.8, 0.0, 0.0), kajiya::Vec3(0.0, 0.0, 0.0),
			kajiya::Vec3(0.0, 0.0, 559.2), kajiya::Vec3(549.6, 0.0, 559.2),
			kajiya::Material::get_white()));
		// light
		objects.push_back(new kajiya::Rectangle(
			kajiya::Vec3(343.0, 548.8, 227.0),
			kajiya::Vec3(343.0, 548.8, 332.0),
			kajiya::Vec3(213.0, 548.8, 332.0),
			kajiya::Vec3(213.0, 548.8, 227.0), kajiya::Material::get_light()));
		// ceiling
		objects.push_back(new kajiya::Rectangle(
			kajiya::Vec3(556.0, 548.8, 0.0), kajiya::Vec3(556.0, 548.8, 559.2),
			kajiya::Vec3(0.0, 548.8, 559.2), kajiya::Vec3(0.0, 548.8, 0.0),
			kajiya::Material::get_white()));
		// back wall
		objects.push_back(new kajiya::Rectangle(
			kajiya::Vec3(549.6, 0.0, 559.2), kajiya::Vec3(0.0, 0.0, 559.2),
			kajiya::Vec3(0.0, 548.8, 559.2), kajiya::Vec3(556.0, 548.8, 559.2),
			kajiya::Material::get_white()));
		// right wall
		objects.push_back(new kajiya::Rectangle(
			kajiya::Vec3(0.0, 0.0, 559.2), kajiya::Vec3(0.0, 0.0, 0.0),
			kajiya::Vec3(0.0, 548.8, 0.0), kajiya::Vec3(0.0, 548.8, 559.2),
			kajiya::Material::get_green()));
		// left wall
		objects.push_back(new kajiya::Rectangle(
			kajiya::Vec3(552.8, 0.0, 0.0), kajiya::Vec3(549.6, 0.0, 559.2),
			kajiya::Vec3(556.0, 548.8, 559.2), kajiya::Vec3(556.0, 548.8, 0.0),
			kajiya::Material::get_red()));
		// short block
		objects.push_back(new kajiya::Rectangle(
			kajiya::Vec3(130.0, 165.0, 65.0), kajiya::Vec3(82.0, 165.0, 225.0),
			kajiya::Vec3(240.0, 165.0, 272.0),
			kajiya::Vec3(290.0, 165.0, 114.0), kajiya::Material::get_white()));
		objects.push_back(new kajiya::Rectangle(
			kajiya::Vec3(290.0, 0.0, 114.0), kajiya::Vec3(290.0, 165.0, 114.0),
			kajiya::Vec3(240.0, 165.0, 272.0), kajiya::Vec3(240.0, 0.0, 272.0),
			kajiya::Material::get_white()));
		objects.push_back(new kajiya::Rectangle(
			kajiya::Vec3(130.0, 0.0, 65.0), kajiya::Vec3(130.0, 165.0, 65.0),
			kajiya::Vec3(290.0, 165.0, 114.0), kajiya::Vec3(290.0, 0.0, 114.0),
			kajiya::Material::get_white()));
		objects.push_back(new kajiya::Rectangle(
			kajiya::Vec3(82.0, 0.0, 225.0), kajiya::Vec3(82.0, 165.0, 225.0),
			kajiya::Vec3(130.0, 165.0, 65.0), kajiya::Vec3(130.0, 0.0, 65.0),
			kajiya::Material::get_white()));
		objects.push_back(new kajiya::Rectangle(
			kajiya::Vec3(240.0, 0.0, 272.0), kajiya::Vec3(240.0, 165.0, 272.0),
			kajiya::Vec3(82.0, 165.0, 225.0), kajiya::Vec3(82.0, 0.0, 225.0),
			kajiya::Material::get_white()));
		// tall block
		objects.push_back(new kajiya::Rectangle(
			kajiya::Vec3(423.0, 330.0, 247.0),
			kajiya::Vec3(265.0, 330.0, 296.0),
			kajiya::Vec3(314.0, 330.0, 456.0),
			kajiya::Vec3(472.0, 330.0, 406.0), kajiya::Material::get_white()));

		objects.push_back(new kajiya::Rectangle(
			kajiya::Vec3(423.0, 0.0, 247.0), kajiya::Vec3(423.0, 330.0, 247.0),
			kajiya::Vec3(472.0, 330.0, 406.0), kajiya::Vec3(472.0, 0.0, 406.0),
			kajiya::Material::get_white()));
		objects.push_back(new kajiya::Rectangle(
			kajiya::Vec3(472.0, 0.0, 406.0), kajiya::Vec3(472.0, 330.0, 406.0),
			kajiya::Vec3(314.0, 330.0, 456.0), kajiya::Vec3(314.0, 0.0, 456.0),
			kajiya::Material::get_white()));
		objects.push_back(new kajiya::Rectangle(
			kajiya::Vec3(314.0, 0.0, 456.0), kajiya::Vec3(314.0, 330.0, 456.0),
			kajiya::Vec3(265.0, 330.0, 296.0), kajiya::Vec3(265.0, 0.0, 296.0),
			kajiya::Material::get_white()));
		objects.push_back(new kajiya::Rectangle(
			kajiya::Vec3(265.0, 0.0, 296.0), kajiya::Vec3(265.0, 330.0, 296.0),
			kajiya::Vec3(423.0, 330.0, 247.0), kajiya::Vec3(423.0, 0.0, 247.0),
			kajiya::Material::get_white()));

		return objects;
	}
};
} // namespace kajiya
