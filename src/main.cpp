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

float pi   = 3.1415926535897932;
float bias = 0.00001;

kajiya::Vec3 rand_unit_vector_on_hemisphere() {
	return kajiya::Vec3(rand_float(), rand_float(), rand_float()).unit();
}

kajiya::Vec3 Lr(kajiya::Hittable *object, kajiya::Ray &r,
				std::vector<kajiya::Hittable *> objects);

kajiya::Vec3 Li(kajiya::Ray &ray, std::vector<kajiya::Hittable *> objects) {
	auto closest			= trace_ray(ray, objects);
	auto intersection_point = closest->intersect(ray);

	if (intersection_point.has_value()) {
		auto surface_normal = closest->normal(intersection_point.value());

		kajiya::Vec3 new_origin =
			(kajiya::Vec3::dot(-ray.direction, surface_normal) < 0
				 ? intersection_point.value() - surface_normal * bias
				 : intersection_point.value() + surface_normal * bias);

		kajiya::Ray new_ray = kajiya::Ray(new_origin, -ray.direction);
		return closest->material().emittance + Lr(closest, new_ray, objects);
	}

	return kajiya::Vec3(0, 0, 0);
}

kajiya::Vec3 Lr(kajiya::Hittable *object, kajiya::Ray &r,
				std::vector<kajiya::Hittable *> objects) {
	kajiya::Ray new_ray(r.origin, rand_unit_vector_on_hemisphere());
	return object->material().brdf * Li(new_ray, objects) * 2 * pi *
		   kajiya::Vec3::dot(object->normal(new_ray.origin), new_ray.direction);
}

int main() {
	srand(time(0));

	std::vector<kajiya::Hittable *> objects;
	load_scene(objects);

	return 0;
}
