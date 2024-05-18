#include "camera.h"
#include "loader.h"
#include "rectangle.h"
#include "renderer.h"
#include "tracer.h"
#include "util.h"
#include <algorithm>
#include <atomic>
#include <chrono>
#include <iostream>
#include <vector>

int main() {
	srand(time(0));

	std::vector<kajiya::Hittable *> objects =
		kajiya::Loader::load_cornell_box();

	kajiya::Camera camera(kajiya::Vec3(278, 273, -800), 0.025, 0.025, 0.035,
						  kajiya::Vec3(0, 0, 1), kajiya::Vec3(0, 1, 0));

	kajiya::Renderer renderer(camera, objects);
	std::vector<unsigned> pixels =
		renderer.render(width, height, max_depth, rays_per_pixel);
	save<width, height>("image.ppm", pixels);

	return 0;
}
