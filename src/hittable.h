#pragma once

#include "material.h"
#include "ray.h"
#include "vec3.h"
#include <optional>

namespace kajiya {
class Hittable {
public:
	virtual std::optional<Vec3> intersect(Ray &r) const = 0;
	virtual Vec3 normal(Vec3 &intersection_point) const = 0;
	virtual Material material() const					= 0;
};

} // namespace kajiya