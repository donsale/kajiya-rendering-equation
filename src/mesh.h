#pragma once

#include "hittable.h"
#include "rectangle.h"
#include "tracer.h"
#include <limits>
#include <vector>

namespace kajiya {
class Mesh : public Hittable {
public:
	// bounding box

	// TODO(stekap): Black points appear on rendered mesh. Maybe bias somewhere???

	std::vector<Triangle> triangles;

	virtual std::optional<IntersectionData> intersect(Ray &r) const {
		// TODO: Use bounding box here.

		int primitive_index = trace_mesh_triangles(r);

		if (primitive_index == -1)
			return std::nullopt;

		std::optional<IntersectionData> intersection_data_opt =
			triangles[primitive_index].intersect(r);

		if (intersection_data_opt.has_value()) {
			intersection_data_opt.value().primitive_index = primitive_index;
			return intersection_data_opt;
		}

		return std::nullopt;
	}

	virtual Vec3 normal(IntersectionData &intersection_data) const {
		return triangles[intersection_data.primitive_index].normal(
			intersection_data);
	}

	virtual Material material(IntersectionData &intersection_data) const {
		return Material::get_white();
	}

	int trace_mesh_triangles(Ray &ray) const {
		int primitive_index	   = -1;
		float closest_distance = std::numeric_limits<float>::max();
		for (int i = 0; i < triangles.size(); ++i) {
			std::optional<IntersectionData> intersection_data_opt =
				triangles[i].intersect(ray);

			if (intersection_data_opt.has_value() &&
				intersection_data_opt.value().point.norm() < closest_distance) {
				closest_distance = intersection_data_opt.value().point.norm();
				primitive_index	 = i;
			}
		}

		return primitive_index;
	}
};
} // namespace kajiya
