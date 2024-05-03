#include "rectangle.h"
#include "util.h"
#include <algorithm>
#include <iostream>
#include <vector>

kajiya::Hittable *trace_ray(kajiya::Ray &ray,
							std::vector<kajiya::Hittable *> objects) {
	return *std::min_element(
		objects.begin(), objects.end(),
		[&ray](const kajiya::Hittable *obj1, const kajiya::Hittable *obj2) {
			auto result1 = obj1->intersect(ray);
			auto result2 = obj2->intersect(ray);

			if (result1.has_value() && result2.has_value()) {
				return result1.value().norm() < result2.value().norm();
			} else if (result1.has_value()) {
				return true;
			} else if (result2.has_value()) {
				return false;
			} else {
				return true;
			}
		});
}

float rand_float() {
	return rand() / static_cast<float>(RAND_MAX);
}

float pi		   = 3.1415926535897932;
float bias		   = 0.00001;
int max_depth	   = 0;
int rays_per_pixel = 1;

kajiya::Vec3 rand_unit_vector_on_hemisphere() {
	return kajiya::Vec3(rand_float(), rand_float(), rand_float()).unit();
}

kajiya::Vec3 Lr(kajiya::Hittable *object, kajiya::Ray &r,
				std::vector<kajiya::Hittable *> objects, int depth);

kajiya::Vec3 Li(kajiya::Ray &ray, std::vector<kajiya::Hittable *> objects,
				int depth) {
	auto closest			= trace_ray(ray, objects);
	auto intersection_point = closest->intersect(ray);

	if (intersection_point.has_value()) {
		auto surface_normal = closest->normal(intersection_point.value());

		kajiya::Vec3 new_origin =
			(kajiya::Vec3::dot(-ray.direction, surface_normal) < 0
				 ? intersection_point.value() - surface_normal * bias
				 : intersection_point.value() + surface_normal * bias);

		kajiya::Ray new_ray = kajiya::Ray(new_origin, -ray.direction);

		kajiya::Vec3 spectrum = closest->material().emittance;

		if (depth > 0)
			spectrum = spectrum + Lr(closest, new_ray, objects, depth - 1);

		return spectrum;
	}

	return kajiya::Vec3(0, 0, 0);
}

kajiya::Vec3 Lr(kajiya::Hittable *object, kajiya::Ray &r,
				std::vector<kajiya::Hittable *> objects, int depth) {
	kajiya::Ray new_ray(r.origin, rand_unit_vector_on_hemisphere());
	return object->material().brdf * Li(new_ray, objects, depth) * 2 * pi *
		   kajiya::Vec3::dot(object->normal(new_ray.origin), new_ray.direction);
}
int main() {
	srand(time(0));

	std::vector<kajiya::Hittable *> objects;

	kajiya::Material white;
	white.brdf = kajiya::Vec3(0.747, 0.740, 0.737);
	white.type = kajiya::Material::diffuse;

	kajiya::Material red;
	red.brdf = kajiya::Vec3(0.058, 0.287, 0.642);
	red.type = kajiya::Material::diffuse;

	kajiya::Material green;
	green.brdf = kajiya::Vec3(0.285, 0.160, 0.159);
	green.type = kajiya::Material::diffuse;

	kajiya::Material material_light;
	material_light.emittance = kajiya::Vec3(8.0, 15.6, 18.4);
	material_light.brdf		 = kajiya::Vec3(0.78, 0.78, 0.78);
	material_light.type		 = kajiya::Material::light;

	// floor, white
	kajiya::Rectangle floor(
		kajiya::Vec3(552.8, 0.0, 0.0), kajiya::Vec3(0.0, 0.0, 0.0),
		kajiya::Vec3(0.0, 0.0, 559.2), kajiya::Vec3(549.6, 0.0, 559.2), white);

	objects.push_back(&floor);
	// light
	kajiya::Rectangle light(kajiya::Vec3(343.0, 548.8, 227.0),
							kajiya::Vec3(343.0, 548.8, 332.0),
							kajiya::Vec3(213.0, 548.8, 332.0),
							kajiya::Vec3(213.0, 548.8, 227.0), material_light);
	objects.push_back(&light);
	// ceiling, white
	kajiya::Rectangle ceiling(
		kajiya::Vec3(556.0, 548.8, 0.0), kajiya::Vec3(556.0, 548.8, 559.2),
		kajiya::Vec3(0.0, 548.8, 559.2), kajiya::Vec3(0.0, 548.8, 0.0), white);
	objects.push_back(&ceiling);
	// back wall, white
	kajiya::Rectangle back_wall(kajiya::Vec3(549.6, 0.0, 559.2),
								kajiya::Vec3(0.0, 0.0, 559.2),
								kajiya::Vec3(0.0, 548.8, 559.2),
								kajiya::Vec3(556.0, 548.8, 559.2), white);
	objects.push_back(&back_wall);
	// right wall, green
	kajiya::Rectangle right_wall(
		kajiya::Vec3(0.0, 0.0, 559.2), kajiya::Vec3(0.0, 0.0, 0.0),
		kajiya::Vec3(0.0, 548.8, 0.0), kajiya::Vec3(0.0, 548.8, 559.2), green);
	objects.push_back(&right_wall);
	// left wall, red
	kajiya::Rectangle left_wall(kajiya::Vec3(552.8, 0.0, 0.0),
								kajiya::Vec3(549.6, 0.0, 559.2),
								kajiya::Vec3(556.0, 548.8, 559.2),
								kajiya::Vec3(556.0, 548.8, 0.0), red);
	objects.push_back(&left_wall);
	// short block, white
	kajiya::Rectangle short_block_top(kajiya::Vec3(130.0, 165.0, 65.0),
									  kajiya::Vec3(82.0, 165.0, 225.0),
									  kajiya::Vec3(240.0, 165.0, 272.0),
									  kajiya::Vec3(290.0, 165.0, 114.0), white);
	objects.push_back(&short_block_top);
	kajiya::Rectangle short_block_left(kajiya::Vec3(290.0, 0.0, 114.0),
									   kajiya::Vec3(290.0, 165.0, 114.0),
									   kajiya::Vec3(240.0, 165.0, 272.0),
									   kajiya::Vec3(240.0, 0.0, 272.0), white);
	objects.push_back(&short_block_left);
	kajiya::Rectangle short_block_front(kajiya::Vec3(130.0, 0.0, 65.0),
										kajiya::Vec3(130.0, 165.0, 65.0),
										kajiya::Vec3(290.0, 165.0, 114.0),
										kajiya::Vec3(290.0, 0.0, 114.0), white);

	objects.push_back(&short_block_front);
	kajiya::Rectangle short_block_right(kajiya::Vec3(82.0, 0.0, 225.0),
										kajiya::Vec3(82.0, 165.0, 225.0),
										kajiya::Vec3(130.0, 165.0, 65.0),
										kajiya::Vec3(130.0, 0.0, 65.0), white);

	objects.push_back(&short_block_right);
	kajiya::Rectangle short_block_back(kajiya::Vec3(240.0, 0.0, 272.0),
									   kajiya::Vec3(240.0, 165.0, 272.0),
									   kajiya::Vec3(82.0, 165.0, 225.0),
									   kajiya::Vec3(82.0, 0.0, 225.0), white);
	objects.push_back(&short_block_back);

	// tall block, white
	kajiya::Rectangle tall_block_top(kajiya::Vec3(423.0, 330.0, 247.0),
									 kajiya::Vec3(265.0, 330.0, 296.0),
									 kajiya::Vec3(314.0, 330.0, 456.0),
									 kajiya::Vec3(472.0, 330.0, 406.0), white);
	objects.push_back(&tall_block_top);
	kajiya::Rectangle tall_block_left(kajiya::Vec3(423.0, 0.0, 247.0),
									  kajiya::Vec3(423.0, 330.0, 247.0),
									  kajiya::Vec3(472.0, 330.0, 406.0),
									  kajiya::Vec3(472.0, 0.0, 406.0), white);
	objects.push_back(&tall_block_left);
	kajiya::Rectangle tall_block_back(kajiya::Vec3(472.0, 0.0, 406.0),
									  kajiya::Vec3(472.0, 330.0, 406.0),
									  kajiya::Vec3(314.0, 330.0, 456.0),
									  kajiya::Vec3(314.0, 0.0, 456.0), white);
	objects.push_back(&tall_block_back);
	kajiya::Rectangle tall_block_right(kajiya::Vec3(314.0, 0.0, 456.0),
									   kajiya::Vec3(314.0, 330.0, 456.0),
									   kajiya::Vec3(265.0, 330.0, 296.0),
									   kajiya::Vec3(265.0, 0.0, 296.0), white);
	objects.push_back(&tall_block_right);
	kajiya::Rectangle tall_block_front(kajiya::Vec3(265.0, 0.0, 296.0),
									   kajiya::Vec3(265.0, 330.0, 296.0),
									   kajiya::Vec3(423.0, 330.0, 247.0),
									   kajiya::Vec3(423.0, 0.0, 247.0), white);
	objects.push_back(&tall_block_front);

	const unsigned width  = 800;
	const unsigned height = 600;
	float half_width	  = width / 2.f;
	float half_height	  = height / 2.f;

	kajiya::Vec3 camera_focal_length(0, 0, 0.035);
	float camera_width	= 0.025;
	float camera_height = 0.025;
	kajiya::Vec3 camera_position(278, 273, -800);

	std::array<std::array<unsigned, width>, height> pixels;

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {

			kajiya::Vec3 normalized_image_plane_pixel(
				(x / half_width - 1) * camera_width / 2.f + camera_position.x,
				(1 - y / half_height) * camera_height / 2.f + camera_position.y,
				camera_position.z);

			kajiya::Vec3 focal_point = camera_position - camera_focal_length;
			kajiya::Ray ray(
				focal_point,
				(normalized_image_plane_pixel - focal_point).unit());

			ray.refractive_index = 1.0f;

			kajiya::Vec3 spectrum(0, 0, 0);
			for (int i = 0; i < rays_per_pixel; ++i) {
				// ovdje mozemo varirati ray kasnije (ali svakako ima random
				// odbijanje)
				spectrum = spectrum + Li(ray, objects, max_depth);
			}
			spectrum = spectrum / rays_per_pixel;

			pixels[y][x] = spectrum_to_color(spectrum).to_hex();
		}
	}

	save<width, height>("image.ppm", pixels);

	return 0;
}
