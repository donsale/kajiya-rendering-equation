#include "camera.h"
#include "loader.h"
#include "rectangle.h"
#include "renderer.h"
#include "util.h"
#include <algorithm>
#include <atomic>
#include <chrono>
#include <iostream>
#include <vector>

float bias			  = 0.0001;
int max_depth		  = 2;
int rays_per_pixel	  = 10;
const unsigned width  = 300;
const unsigned height = 300;

kajiya::Material LIGHT = kajiya::Material::get_light();
float LIGHT_AREA	   = 13184;

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

kajiya::Spectrum Lr(kajiya::Hittable *object, kajiya::Ray &r,
					std::vector<kajiya::Hittable *> objects, int depth);

kajiya::Spectrum Li(kajiya::Ray &ray, std::vector<kajiya::Hittable *> objects,
					int depth) {
	kajiya::Hittable *closest;
	std::optional<kajiya::Vec3> intersection_point;

	closest			   = trace_ray(ray, objects);
	intersection_point = closest->intersect(ray);

	if (intersection_point.has_value()) {
		auto surface_normal = closest->normal(intersection_point.value());

		kajiya::Vec3 new_origin =
			(kajiya::Vec3::dot(-ray.direction, surface_normal) < 0
				 ? intersection_point.value() - surface_normal * bias
				 : intersection_point.value() + surface_normal * bias);

		kajiya::Ray new_ray		 = kajiya::Ray(new_origin, -ray.direction);
		new_ray.refractive_index = ray.refractive_index;

		kajiya::Spectrum spectrum = closest->material().emittance;

		if (depth > 0)
			spectrum = spectrum + Lr(closest, new_ray, objects, depth - 1);

		return spectrum;
	}

	return kajiya::Spectrum();
}

kajiya::Spectrum Lr(kajiya::Hittable *object, kajiya::Ray &r,
					std::vector<kajiya::Hittable *> objects, int depth) {

	kajiya::Vec3 new_direction;
	kajiya::Spectrum relevant_object_spectrum;
	kajiya::Spectrum direct_light_contribution;
	float brdf_over_probability = 1;
	float brdf					= 1; // Move this to material.
	float probability			= 1;
	float preservation			= 0.8f;
	if (depth == max_depth - 1)
		preservation = 1;

	// Direct light sampling (just one light for now).
	if (object->material().type != kajiya::Material::light) {
		float rand_light_z = 228 + (331 - 228) * rand_float();
		float rand_light_x = 214 + (342 - 214) * rand_float();
		kajiya::Vec3 light_dist_vec =
			kajiya::Vec3(rand_light_x, 548.8, rand_light_z) - r.origin;
		kajiya::Ray light_ray(r.origin, light_dist_vec.unit());

		auto closest_temp			 = trace_ray(light_ray, objects);
		auto intersection_point_temp = closest_temp->intersect(light_ray);

		if (intersection_point_temp.has_value()) {
			if (closest_temp->material().type == kajiya::Material::light) {
				float light_dist_squared =
					(intersection_point_temp.value() - r.origin).norm_squared();
				float light_cos = kajiya::Vec3::dot(-light_ray.direction,
													kajiya::Vec3(0, -1, 0));
				float light_direction_probability =
					LIGHT_AREA * light_cos / light_dist_squared;

				float light_dot = kajiya::Vec3::dot(
					object->normal(r.origin),
					(intersection_point_temp.value() - r.origin).unit());

				direct_light_contribution =
					LIGHT.emittance * light_direction_probability * light_dot;
			}
		}
	}

	if (object->material().type == kajiya::Material::metal) {
		probability					= 1;
		brdf						= 1;
		kajiya::Vec3 surface_normal = object->normal(r.origin);
		new_direction			 = (r.direction).reflect_around(surface_normal);
		relevant_object_spectrum = object->material().reflectance;
	} else if (object->material().type == kajiya::Material::glass) {
		kajiya::Vec3 surface_normal = object->normal(r.origin);
		float refractive_index_1	= r.refractive_index;
		float refractive_index_2	= object->material().refractive_index;

		float fresnel_reflection_coef =
			fresnel(-r.direction, surface_normal, refractive_index_1,
					refractive_index_2);

		if (rand_float() < fresnel_reflection_coef) {
			brdf_over_probability = 1.f / fresnel_reflection_coef;
			new_direction = (r.direction).reflect_around(surface_normal);
			relevant_object_spectrum = object->material().reflectance;
		} else {
			probability = 1 - fresnel_reflection_coef;
			brdf		= 1.f;
			brdf *= 0.007; // Experimentation.

			// Handle case when ray travels within object.
			bool ray_is_internal_to_object =
				kajiya::Vec3::dot(-r.direction, surface_normal) > 0;
			if (ray_is_internal_to_object) {
				surface_normal = -surface_normal;
			}

			float refractive_index = refractive_index_1 / refractive_index_2;
			float C1 = kajiya::Vec3::dot(r.direction, surface_normal) /
					   (r.direction.norm());
			float total_internal_reflection_indicator =
				1 - refractive_index * refractive_index * (1 - C1 * C1);
			float C2 = std::sqrt(total_internal_reflection_indicator);

			// Form refraction ray.
			new_direction = (-r.direction * refractive_index +
							 surface_normal * (refractive_index * C1 - C2))
								.unit();

			r.direction = -r.direction;
			std::optional<kajiya::Vec3> intersection_point_opt =
				object->intersect(r);
			kajiya::Vec3 intersection_point;
			if (intersection_point_opt.has_value()) {
				intersection_point = intersection_point_opt.value();
			}

			r.origin = (kajiya::Vec3::dot(new_direction, surface_normal) < 0
							? intersection_point - surface_normal * bias
							: intersection_point + surface_normal * bias);

			// If there is no total internal reflection.
			r.refractive_index = refractive_index_2;

			relevant_object_spectrum = object->material().transmittance;
		}
	} else {
		new_direction =
			rand_unit_vector_on_hemisphere(object->normal(r.origin));
		probability				 = 1.f / (2 * pi);
		brdf					 = 1.f / pi;
		relevant_object_spectrum = object->material().reflectance;
	}

	kajiya::Ray new_ray(r.origin, new_direction);
	new_ray.refractive_index = r.refractive_index;

	float Li_dot = kajiya::Vec3::dot(object->normal(new_ray.origin).unit(),
									 new_ray.direction.unit());

	kajiya::Spectrum incoming;
	auto closest			= trace_ray(new_ray, objects);
	auto intersection_point = closest->intersect(new_ray);
	if (intersection_point.has_value()) {
		if (closest->material().type != kajiya::Material::light) {
			incoming = Li(new_ray, objects, depth) * brdf *
					   relevant_object_spectrum * preservation * Li_dot /
					   probability;
		}
	}

	return incoming + direct_light_contribution * brdf * preservation *
						  relevant_object_spectrum;
}

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
