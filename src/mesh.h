#pragma once

#include "rectangle.h"
#include "hittable.h"
#include "tracer.h"
#include <vector>

namespace kajiya {
class Mesh : public Hittable {
public:
	// vertices
	// number of faces
	// face index (array holding info about polygons degrees)
	// indices
	// bounding box

	std::vector<Triangle> triangles;
	//std::vector<Triangle*> buffer;

	virtual std::optional<IntersectionData> intersect(Ray &r) const {
		Triangle t = trace_mesh_triangles(r);
		std::optional<IntersectionData> intersection_data = t.intersect(r);

		if(intersection_data.has_value()) {
			return intersection_data.value();
		}
		
		return std::nullopt;
	}

	virtual Vec3 normal(IntersectionData &intersection_data) const {
		return Vec3(0, 0, 0);
	}

	virtual Material material(IntersectionData &intersection_data) const {
		return Material::get_white();
	}

	Triangle trace_mesh_triangles(Ray &ray) const {
		return *std::min_element(
		triangles.begin(), triangles.end(),
		[&ray](const Triangle t1, const Triangle t2) {
			auto result1 = t1.intersect(ray);
			auto result2 = t2.intersect(ray);

			if (result1.has_value() && result2.has_value()) {
				return result1.value().point.norm() < result2.value().point.norm();
			} else if (result1.has_value()) {
				return true;
			} else if (result2.has_value()) {
				return false;
			} else {
				return true;
			}
		});
	}
};
}
