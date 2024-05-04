#pragma once

#include "color.h"
#include "hittable.h"
#include "ray.h"
#include "vec3.h"
#include <iostream>
#include <optional>

namespace kajiya {
class Triangle {
public:
	Vec3 p1, p2, p3, n;

	Triangle(Vec3 p1, Vec3 p2, Vec3 p3, Vec3 n) : p1(p1), p2(p2), p3(p3), n(n) {
	}

	std::optional<Vec3> intersect(Ray &r) const {
		// G = p1
		// n = n
		// A = r.origin
		// B = r.direction
		float ray_parallel_with_plane_indicator = Vec3::dot(r.direction, n);

		// Ray is parallel
		if (ray_parallel_with_plane_indicator < 0.0005 &&
			ray_parallel_with_plane_indicator > -0.0005) {

			return std::nullopt;
		}

		float t =
			Vec3::dot((p1 - r.origin), n) / ray_parallel_with_plane_indicator;

		if (t > 0) {
			Vec3 intersection_point = r.value(t);

			Vec3 c1 = Vec3::cross(intersection_point - p1, p2 - p1);
			Vec3 c2 = Vec3::cross(intersection_point - p2, p3 - p2);
			Vec3 c3 = Vec3::cross(intersection_point - p3, p1 - p3);

			float d1 = Vec3::dot(c1, n);
			float d2 = Vec3::dot(c2, n);
			float d3 = Vec3::dot(c3, n);

			// Equality for when the point is on the edge of the triangle.
			if ((d1 <= 0 && d2 <= 0 && d3 <= 0) ||
				(d1 >= 0 && d2 >= 0 && d3 >= 0))
				return intersection_point;
			else
				return std::nullopt;
		}

		return std::nullopt;
	}
};

class Rectangle : public Hittable {
public:
	Vec3 p1, p2, p3, p4, n;
	Triangle t1, t2;
	Material m;

	Rectangle(Vec3 p1, Vec3 p2, Vec3 p3, Vec3 p4, Material m)
		: p1(p1), p2(p2), p3(p3), p4(p4), m(m),
		  n(Vec3::cross(p2 - p1, p4 - p1).unit()), t1(Triangle(p1, p2, p3, n)),
		  // p3, p4, p1 gives black diagonal on left wall (tested on depth 2,
		  // ray 50)
		  t2(Triangle(p1, p3, p4, n)) {
	}

	std::optional<Vec3> intersect(Ray &r) const {
		auto t1_result = t1.intersect(r);

		if (!t1_result.has_value()) {
			return t2.intersect(r);
		}

		return t1_result;
	}

	Material material() const {
		return m;
	}

	Vec3 normal(Vec3 &point) const {
		return n;
	}
};
} // namespace kajiya
