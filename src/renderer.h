#pragma once

#include "camera.h"
#include "hittable.h"
#include "ray.h"
#include "spectrum.h"
#include "vec3.h"
#include <atomic>
#include <chrono>
#include <iostream>
#include <vector>

// Temporary solution
extern kajiya::Spectrum Li(kajiya::Ray &, std::vector<kajiya::Hittable *>, int);

namespace kajiya {
class Renderer {
public:
	Camera camera;
	std::vector<Hittable *> objects;

	Renderer(Camera c, std::vector<Hittable *> objs)
		: camera(c), objects(objs) {
	}

	std::vector<unsigned> render(const unsigned width, const unsigned height,
								 int max_depth, int rays_per_pixel) {
		std::vector<unsigned> pixels(width * height, 0);

		float half_width  = width / 2.f;
		float half_height = height / 2.f;

		auto start_time = std::chrono::high_resolution_clock::now();

		int two_percent_progress		  = width * height / 50.f;
		std::atomic<int> processed_pixels = {};

		std::cout << "__________________________________________________"
				  << std::endl;

		// TODO(stekap): Make this automatic without guessing the number.
		float aalias_factor = 10;
		
#pragma omp parallel for collapse(2) shared(processed_pixels)
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				float normalized_x = (1 - x / half_width);
				float normalized_y = (1 - y / half_height);

				Vec3 normalized_image_plane_pixel =
					camera.position +
					camera.x_axis * normalized_x * camera.screen_width / 2.f +
					camera.y_axis * normalized_y * camera.screen_height / 2.f;

				Vec3 focal_point = camera.focus();

				Ray ray(focal_point,
						(normalized_image_plane_pixel - focal_point).unit());

				ray.refractive_index = 1.0f;

				Spectrum spectrum;
				for (int i = 0; i < rays_per_pixel; ++i) {
					kajiya::Ray temp_ray = ray;
					temp_ray.direction = temp_ray.direction + rand_unit_vector_on_hemisphere(temp_ray.direction)* (rand_float() * camera.screen_width / (float)width * aalias_factor);
					spectrum = spectrum + Li(temp_ray, objects, max_depth);
				}
				spectrum = spectrum / rays_per_pixel;

				spectrum = spectrum * 0.2;

				pixels[y * width + x] =
					spectrum_to_color(spectrum).clamp().to_hex();

				++processed_pixels;
				if (processed_pixels % two_percent_progress == 0) {
					std::cout << "#" << std::flush;
				}
			}
		}

		auto end_time = std::chrono::high_resolution_clock::now();
		auto render_time =
			std::chrono::duration_cast<std::chrono::milliseconds>(end_time -
																  start_time);
		std::cout << "\nRender Time: " << render_time.count() / 1000.f << "s"
				  << std::endl;

		return std::move(pixels);
	}
};
} // namespace kajiya
