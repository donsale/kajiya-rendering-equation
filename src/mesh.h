#pragma once

#include "rectangle.h"
#include "hittable.h"
#include "tracer.h"
#include <vector>

namespace kajiya {
class Mesh : public Hittable {
public:
	std::vector<Triangle> triangles;

	virtual std::optional<Vec3> intersect(Ray &r) const {
		return std::nullopt;
	}
	
	virtual Vec3 normal(Vec3 &intersection_point) const {
		return Vec3(0, 0, 0);
	}

	virtual Material material() const {
		return Material::get_white();
	}
};
}
