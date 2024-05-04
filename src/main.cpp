#include "rectangle.h"
#include "util.h"
#include <algorithm>
#include <atomic>
#include <chrono>
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

float pi		   = 3.1415926535897932;
float bias		   = 0.0001;
int max_depth	   = 1;
int rays_per_pixel = 1;

kajiya::Spectrum Lr(kajiya::Hittable *object, kajiya::Ray &r,
					std::vector<kajiya::Hittable *> objects, int depth);

kajiya::Spectrum Li(kajiya::Ray &ray, std::vector<kajiya::Hittable *> objects,
					int depth) {
	kajiya::Hittable *closest;
	std::optional<kajiya::Vec3> intersection_point;

	// This part gives all white image (but code portion for this case hasn't
	// been written below under if(intersection_point.has_value()) condition.

	// if (depth == max_depth) {
	// 	closest = *std::find_if(
	// 		objects.begin(), objects.end(), [&](auto const &object) {
	// 			return object->material().type == kajiya::Material::light;
	// 		});
	// 	// Hardcoded picking of first point of light for testing.
	// 	intersection_point = std::optional<kajiya::Vec3>(
	// 		dynamic_cast<kajiya::Rectangle *>(closest)->p1);

	// } else {
	// 	closest			   = trace_ray(ray, objects);
	// 	intersection_point = closest->intersect(ray);
	// }

	closest			   = trace_ray(ray, objects);
	intersection_point = closest->intersect(ray);

	if (intersection_point.has_value()) {
		auto surface_normal = closest->normal(intersection_point.value());

		kajiya::Vec3 new_origin =
			(kajiya::Vec3::dot(-ray.direction, surface_normal) < 0
				 ? intersection_point.value() - surface_normal * bias
				 : intersection_point.value() + surface_normal * bias);

		kajiya::Ray new_ray = kajiya::Ray(new_origin, -ray.direction);

		kajiya::Spectrum spectrum = closest->material().emittance;

		if (depth > 0)
			spectrum = spectrum + Lr(closest, new_ray, objects, depth - 1);

		return spectrum;
	}

	return kajiya::Spectrum();
}

kajiya::Vec3 visible_light_corner(kajiya::Vec3 point,
								  std::vector<kajiya::Hittable *> objects) {
	kajiya::Vec3 p1(342.0, 548.8, 228.0);
	kajiya::Vec3 p2(342.0, 548.8, 331.0);
	kajiya::Vec3 p3(214.0, 548.8, 331.0);
	kajiya::Vec3 p4(214.0, 548.8, 228.0);

	std::vector<kajiya::Vec3> points = {p1, p2, p3, p4};

	for (auto &p : points) {
		kajiya::Ray ray(point, (p - point).unit());
		auto closest = trace_ray(ray, objects);
		if (!closest->intersect(ray).has_value())
			return p;
	}

	return p1;
}

kajiya::Spectrum Lr(kajiya::Hittable *object, kajiya::Ray &r,
					std::vector<kajiya::Hittable *> objects, int depth) {

	kajiya::Vec3 new_direction;
	if (depth == max_depth - 1) {
		new_direction = visible_light_corner(r.origin, objects) - r.origin;
	} else {
		new_direction =
			rand_unit_vector_on_hemisphere(object->normal(r.origin));
	}

	kajiya::Ray new_ray(r.origin, new_direction);
	return object->material().reflectance * Li(new_ray, objects, depth) * 2 *
		   pi *
		   kajiya::Vec3::dot(object->normal(new_ray.origin), new_ray.direction);
}

int main() {
	srand(time(0));

	std::vector<kajiya::Hittable *> objects;

	// floor, white
	kajiya::Rectangle floor(
		kajiya::Vec3(552.8, 0.0, 0.0), kajiya::Vec3(0.0, 0.0, 0.0),
		kajiya::Vec3(0.0, 0.0, 559.2), kajiya::Vec3(549.6, 0.0, 559.2),
		kajiya::Material::get_white());

	objects.push_back(&floor);
	// light
	kajiya::Rectangle light(
		kajiya::Vec3(343.0, 548.8, 227.0), kajiya::Vec3(343.0, 548.8, 332.0),
		kajiya::Vec3(213.0, 548.8, 332.0), kajiya::Vec3(213.0, 548.8, 227.0),
		kajiya::Material::get_light());
	objects.push_back(&light);
	// ceiling, white
	kajiya::Rectangle ceiling(
		kajiya::Vec3(556.0, 548.8, 0.0), kajiya::Vec3(556.0, 548.8, 559.2),
		kajiya::Vec3(0.0, 548.8, 559.2), kajiya::Vec3(0.0, 548.8, 0.0),
		kajiya::Material::get_white());
	objects.push_back(&ceiling);
	// back wall, white
	kajiya::Rectangle back_wall(
		kajiya::Vec3(549.6, 0.0, 559.2), kajiya::Vec3(0.0, 0.0, 559.2),
		kajiya::Vec3(0.0, 548.8, 559.2), kajiya::Vec3(556.0, 548.8, 559.2),
		kajiya::Material::get_white());
	objects.push_back(&back_wall);
	// right wall, green
	kajiya::Rectangle right_wall(
		kajiya::Vec3(0.0, 0.0, 559.2), kajiya::Vec3(0.0, 0.0, 0.0),
		kajiya::Vec3(0.0, 548.8, 0.0), kajiya::Vec3(0.0, 548.8, 559.2),
		kajiya::Material::get_green());
	objects.push_back(&right_wall);
	// left wall, red
	kajiya::Rectangle left_wall(
		kajiya::Vec3(552.8, 0.0, 0.0), kajiya::Vec3(549.6, 0.0, 559.2),
		kajiya::Vec3(556.0, 548.8, 559.2), kajiya::Vec3(556.0, 548.8, 0.0),
		kajiya::Material::get_red());
	objects.push_back(&left_wall);
	// short block, white
	kajiya::Rectangle short_block_top(
		kajiya::Vec3(130.0, 165.0, 65.0), kajiya::Vec3(82.0, 165.0, 225.0),
		kajiya::Vec3(240.0, 165.0, 272.0), kajiya::Vec3(290.0, 165.0, 114.0),
		kajiya::Material::get_white());
	objects.push_back(&short_block_top);
	kajiya::Rectangle short_block_left(
		kajiya::Vec3(290.0, 0.0, 114.0), kajiya::Vec3(290.0, 165.0, 114.0),
		kajiya::Vec3(240.0, 165.0, 272.0), kajiya::Vec3(240.0, 0.0, 272.0),
		kajiya::Material::get_white());
	objects.push_back(&short_block_left);
	kajiya::Rectangle short_block_front(
		kajiya::Vec3(130.0, 0.0, 65.0), kajiya::Vec3(130.0, 165.0, 65.0),
		kajiya::Vec3(290.0, 165.0, 114.0), kajiya::Vec3(290.0, 0.0, 114.0),
		kajiya::Material::get_white());

	objects.push_back(&short_block_front);
	kajiya::Rectangle short_block_right(
		kajiya::Vec3(82.0, 0.0, 225.0), kajiya::Vec3(82.0, 165.0, 225.0),
		kajiya::Vec3(130.0, 165.0, 65.0), kajiya::Vec3(130.0, 0.0, 65.0),
		kajiya::Material::get_white());

	objects.push_back(&short_block_right);
	kajiya::Rectangle short_block_back(
		kajiya::Vec3(240.0, 0.0, 272.0), kajiya::Vec3(240.0, 165.0, 272.0),
		kajiya::Vec3(82.0, 165.0, 225.0), kajiya::Vec3(82.0, 0.0, 225.0),
		kajiya::Material::get_white());
	objects.push_back(&short_block_back);

	// tall block, white
	kajiya::Rectangle tall_block_top(
		kajiya::Vec3(423.0, 330.0, 247.0), kajiya::Vec3(265.0, 330.0, 296.0),
		kajiya::Vec3(314.0, 330.0, 456.0), kajiya::Vec3(472.0, 330.0, 406.0),
		kajiya::Material::get_white());
	objects.push_back(&tall_block_top);
	kajiya::Rectangle tall_block_left(
		kajiya::Vec3(423.0, 0.0, 247.0), kajiya::Vec3(423.0, 330.0, 247.0),
		kajiya::Vec3(472.0, 330.0, 406.0), kajiya::Vec3(472.0, 0.0, 406.0),
		kajiya::Material::get_white());
	objects.push_back(&tall_block_left);
	kajiya::Rectangle tall_block_back(
		kajiya::Vec3(472.0, 0.0, 406.0), kajiya::Vec3(472.0, 330.0, 406.0),
		kajiya::Vec3(314.0, 330.0, 456.0), kajiya::Vec3(314.0, 0.0, 456.0),
		kajiya::Material::get_white());
	objects.push_back(&tall_block_back);
	kajiya::Rectangle tall_block_right(
		kajiya::Vec3(314.0, 0.0, 456.0), kajiya::Vec3(314.0, 330.0, 456.0),
		kajiya::Vec3(265.0, 330.0, 296.0), kajiya::Vec3(265.0, 0.0, 296.0),
		kajiya::Material::get_white());
	objects.push_back(&tall_block_right);
	kajiya::Rectangle tall_block_front(
		kajiya::Vec3(265.0, 0.0, 296.0), kajiya::Vec3(265.0, 330.0, 296.0),
		kajiya::Vec3(423.0, 330.0, 247.0), kajiya::Vec3(423.0, 0.0, 247.0),
		kajiya::Material::get_white());
	objects.push_back(&tall_block_front);

	const unsigned width  = 800;
	const unsigned height = 800;
	float half_width	  = width / 2.f;
	float half_height	  = height / 2.f;

	kajiya::Vec3 camera_focal_length(0, 0, 0.035);
	float camera_width	= 0.025;
	float camera_height = 0.025;
	kajiya::Vec3 camera_position(278, 273, -800);

	std::array<std::array<unsigned, width>, height> pixels;

	auto start_time = std::chrono::high_resolution_clock::now();

	int two_percent_progress		  = width * height / 50.f;
	std::atomic<int> processed_pixels = {};

	std::cout << "__________________________________________________"
			  << std::endl;

#pragma omp parallel for collapse(2) shared(processed_pixels)
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			kajiya::Vec3 normalized_image_plane_pixel(
				(1 - x / half_width) * camera_width / 2.f + camera_position.x,
				(1 - y / half_height) * camera_height / 2.f + camera_position.y,
				camera_position.z);

			kajiya::Vec3 focal_point = camera_position - camera_focal_length;
			kajiya::Ray ray(
				focal_point,
				(normalized_image_plane_pixel - focal_point).unit());

			ray.refractive_index = 1.0f;

			kajiya::Spectrum spectrum;
			for (int i = 0; i < rays_per_pixel; ++i) {
				// ovdje mozemo varirati ray kasnije (ali svakako ima random
				// odbijanje)
				spectrum = spectrum + Li(ray, objects, max_depth);
			}
			spectrum = spectrum / rays_per_pixel;

			pixels[y][x] = spectrum_to_color(spectrum).clamp().to_hex();

			++processed_pixels;
			if (processed_pixels % two_percent_progress == 0) {
				std::cout << "#";
			}
		}
	}

	auto end_time	 = std::chrono::high_resolution_clock::now();
	auto render_time = std::chrono::duration_cast<std::chrono::milliseconds>(
		end_time - start_time);
	std::cout << "\nRender Time: " << render_time.count() / 1000.f << "s"
			  << std::endl;

	save<width, height>("image.ppm", pixels);

	return 0;
}
