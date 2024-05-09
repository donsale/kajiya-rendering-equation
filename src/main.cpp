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
int max_depth	   = 3;
int rays_per_pixel = 10;

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

std::vector<std::vector<kajiya::Vec3>> light_mesh;

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

// Hardcoded for testing.
kajiya::Material LIGHT; 
float LIGHT_AREA = 13184;
kajiya::Spectrum Lr(kajiya::Hittable *object, kajiya::Ray &r,
					std::vector<kajiya::Hittable *> objects, int depth) {

	kajiya::Vec3 new_direction;
	kajiya::Spectrum relevant_object_spectrum;
	kajiya::Spectrum direct_light_contribution;
	float brdf_over_probability		   = 1;
	float brdf = 1; // Move this to material.
	float probability = 1;
	float preservation		   = 0.8f;
	if (depth == max_depth - 1)
		preservation = 1;

	// Direct light sampling (just one light for now).
	if (object->material().type != kajiya::Material::light) {
#if 1 // Probabilistic light sampling
		float rand_light_z = 228 + (331 - 228) * rand_float();
		float rand_light_x = 214 + (342 - 214) * rand_float();
		kajiya::Vec3 light_dist_vec = kajiya::Vec3(rand_light_x, 548.8, rand_light_z) - r.origin;
		kajiya::Ray light_ray(r.origin, light_dist_vec.unit());
		
		auto closest_temp = trace_ray(light_ray, objects);
		auto intersection_point_temp = closest_temp->intersect(light_ray);
		
		if(intersection_point_temp.has_value()) {
			if(closest_temp->material().type == kajiya::Material::light) {
				float light_dist_squared = light_dist_vec.norm_squared();
				float light_cos = kajiya::Vec3::dot(-light_ray.direction, kajiya::Vec3(0, -1, 0));
				//float light_direction_probability = LIGHT_AREA * light_cos / (2 * pi);
				//float light_direction_probability = 1.f / (LIGHT_AREA * light_cos);
				
				// Looks better with this probability that with other ones (why???).
				float light_direction_probability = 1;
				
				direct_light_contribution = LIGHT.emittance * LIGHT_AREA * light_cos / light_dist_squared / light_direction_probability;
			}
		}
#else // Deterministic light sampling (slower)
		int hits		   = 0;
		float light_dist_squared = 0;
		float light_cos	   = 0;

		for (int i = 0; i < light_mesh.size(); ++i) {
			for (int j = 0; j < light_mesh[0].size(); ++j) {
				kajiya::Vec3 light_dist_vec = light_mesh[i][j] - r.origin;
				kajiya::Ray light_ray(r.origin, light_dist_vec.unit());

				light_dist_squared += light_dist_vec.norm_squared();
				light_cos +=
					kajiya::Vec3::dot(-light_ray.direction, kajiya::Vec3(0, -1, 0));

				auto closest = trace_ray(light_ray, objects);

				if (closest->material().type == kajiya::Material::light) {
					hits++;
					new_direction = light_mesh[i][j] - r.origin;
				}
			}
		}

		light_dist_squared /= ((light_mesh.size() + 1) * (light_mesh[0].size() + 1));
		light_cos /= ((light_mesh.size() + 1) * (light_mesh[0].size() + 1));

		float direct_light_scaling =
			static_cast<float>(hits) /
			((light_mesh.size() + 1) * (light_mesh[0].size() + 1));

		direct_light_contribution = LIGHT.emittance * LIGHT_AREA * light_cos / light_dist_squared * direct_light_scaling;
#endif
	}

	if (object->material().type == kajiya::Material::metal) {
		probability = 1;
		brdf = 1;
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
			brdf_over_probability  = 1.f / fresnel_reflection_coef;
			new_direction = (r.direction).reflect_around(surface_normal);
			relevant_object_spectrum = object->material().reflectance;
		} else {
			probability = 1 - fresnel_reflection_coef;
			brdf = 1.f;
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
		probability = 1.f / (2 * pi);
		brdf = 1.f / pi;
		relevant_object_spectrum = object->material().reflectance;
	}

	kajiya::Ray new_ray(r.origin, new_direction);
	new_ray.refractive_index = r.refractive_index;

	float Li_dot = kajiya::Vec3::dot(object->normal(new_ray.origin).unit(),
					  new_ray.direction.unit());

	// Weird dots come from Li (can be checked by multiplying by probability instead of dividing). We can this to also see what image looks like without Li, but with direct lighting.
	return relevant_object_spectrum * brdf * preservation *
		(direct_light_contribution + Li(new_ray, objects, depth) * Li_dot / probability);
}

void generate_light_mesh(std::vector<std::vector<kajiya::Vec3>> &points, int n1,
						 int n2) {
	kajiya::Vec3 p1(342.0, 548.8, 228.0);
	kajiya::Vec3 p2(342.0, 548.8, 331.0);
	kajiya::Vec3 p3(214.0, 548.8, 331.0);
	kajiya::Vec3 p4(214.0, 548.8, 228.0);
	

	kajiya::Vec3 d1 = (p2 - p1);
	kajiya::Vec3 d2 = (p4 - p1);

	float d1s = d1.norm() / n1;
	float d2s = d2.norm() / n2;

	d1 = d1.unit() * d1s;
	d2 = d2.unit() * d2s;

	for (int i = 0; i <= n1; ++i) {
		points.push_back(std::vector<kajiya::Vec3>());
		for (int j = 0; j <= n2; ++j) {
			points[i].push_back(p1 + (d1 * i) + (d2 * j));
		}
	}
}

int main() {
	srand(time(0));

	std::vector<kajiya::Hittable *> objects;

	generate_light_mesh(light_mesh, 2, 2);
	LIGHT = kajiya::Material::get_light();

	// floor
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
	// ceiling
	kajiya::Rectangle ceiling(
		kajiya::Vec3(556.0, 548.8, 0.0), kajiya::Vec3(556.0, 548.8, 559.2),
		kajiya::Vec3(0.0, 548.8, 559.2), kajiya::Vec3(0.0, 548.8, 0.0),
		kajiya::Material::get_white());
	objects.push_back(&ceiling);
	// back wall
	kajiya::Rectangle back_wall(
		kajiya::Vec3(549.6, 0.0, 559.2), kajiya::Vec3(0.0, 0.0, 559.2),
		kajiya::Vec3(0.0, 548.8, 559.2), kajiya::Vec3(556.0, 548.8, 559.2),
		kajiya::Material::get_white());
	objects.push_back(&back_wall);
	// right wall
	kajiya::Rectangle right_wall(
		kajiya::Vec3(0.0, 0.0, 559.2), kajiya::Vec3(0.0, 0.0, 0.0),
		kajiya::Vec3(0.0, 548.8, 0.0), kajiya::Vec3(0.0, 548.8, 559.2),
		kajiya::Material::get_green());
	objects.push_back(&right_wall);
	// left wall
	kajiya::Rectangle left_wall(
		kajiya::Vec3(552.8, 0.0, 0.0), kajiya::Vec3(549.6, 0.0, 559.2),
		kajiya::Vec3(556.0, 548.8, 559.2), kajiya::Vec3(556.0, 548.8, 0.0),
		kajiya::Material::get_red());
	objects.push_back(&left_wall);
	// short block
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

	// tall block
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

	const unsigned width  = 300;
	const unsigned height = 300;
	float half_width	  = width / 2.f;
	float half_height	  = height / 2.f;

	kajiya::Vec3 camera_focal_length(0, 0, 0.035);
	float camera_width	= 0.025;
	float camera_height = 0.025;
	kajiya::Vec3 camera_position(278, 273, -800);

	std::vector<unsigned> pixels(width * height, 0);

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

			spectrum = spectrum * 0.1;

			pixels[y * width + x] =
				spectrum_to_color(spectrum).clamp().to_hex();

			// pixels[y * width + x] = (spectrum_to_color(spectrum) *
			// spectrum_luminance_integrated(spectrum)).clamp().to_hex();

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
