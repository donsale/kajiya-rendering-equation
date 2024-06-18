#pragma once

#include "material.h"
#include "ray.h"
#include "vec3.h"
#include <optional>

namespace kajiya {

struct IntersectionData {
	Vec3 point;
	int primitive_index;

	IntersectionData(Vec3 p, int pi) : point(p), primitive_index(pi) {
	}
};

class Hittable {
public:
	virtual std::optional<IntersectionData> intersect(Ray &r) const		 = 0;
	virtual Vec3 normal(IntersectionData &intersection_data) const		 = 0;
	virtual Material material(IntersectionData &intersection_data) const = 0;
};

} // namespace kajiya
