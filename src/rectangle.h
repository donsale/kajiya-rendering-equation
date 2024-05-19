#pragma once

#include "color.h"
#include "hittable.h"
#include "material.h"
#include "ray.h"
#include "vec3.h"
#include <iostream>
#include <optional>

namespace kajiya {
struct BarycentricCoords {
	//    p1 p2 p3
	float u, v, w;

	BarycentricCoords(float u, float v, float w) : u(u), v(v), w(w) {}
};
	
class Triangle : public Hittable {
public:
	// NOTE(stekap): Maybe pack vertex attributes.
	Vec3 p1, p2, p3;
	Vec3 n1, n2, n3; // Not used until barycentric interpolation is introduced.
	
	Material m;

	Triangle() {
		p1 = Vec3();
		p2 = Vec3();
		p3 = Vec3();
		n1 = Vec3();
		n2 = Vec3();
		n3 = Vec3();
		m  = Material::get_white();
	}

	Triangle(Vec3 p1, Vec3 p2, Vec3 p3, Vec3 n1, Vec3 n2, Vec3 n3, Material m)
		: p1(p1), p2(p2), p3(p3), n1(n1), n2(n2), n3(n3), m(m) {
	}

	std::optional<IntersectionData> intersect(Ray &r) const {
		Vec3 plane_normal = Vec3::cross(p2 - p1, p3 - p1).unit();

		float ray_parallel_with_plane_indicator = Vec3::dot(r.direction, plane_normal);

		// Ray is parallel
		if (ray_parallel_with_plane_indicator < 0.0005 &&
			ray_parallel_with_plane_indicator > -0.0005) {

			return std::nullopt;
		}

		float t =
			Vec3::dot((p1 - r.origin), plane_normal) / ray_parallel_with_plane_indicator;

		if (t > 0) {
			Vec3 intersection_point = r.value(t);

			Vec3 c1 = Vec3::cross(intersection_point - p1, p2 - p1);
			Vec3 c2 = Vec3::cross(intersection_point - p2, p3 - p2);
			Vec3 c3 = Vec3::cross(intersection_point - p3, p1 - p3);

			float d1 = Vec3::dot(c1, plane_normal);
			float d2 = Vec3::dot(c2, plane_normal);
			float d3 = Vec3::dot(c3, plane_normal);

			// Equality for when the point is on the edge of the triangle.
			if ((d1 <= 0 && d2 <= 0 && d3 <= 0) ||
				(d1 >= 0 && d2 >= 0 && d3 >= 0))
				return (IntersectionData){intersection_point, -1};
			else
				return std::nullopt;
		}

		return std::nullopt;
	}

	Vec3 normal(IntersectionData &intersection_data) const {
		BarycentricCoords bc = barycentric_coords_for(intersection_data.point);
		return n1 * bc.u + n2 * bc.v + n3 * bc.w;
	}

	Material material(IntersectionData &intersection_data) const {
		return m;
	}

	BarycentricCoords barycentric_coords_for(Vec3 intersection_point) const {
		Vec3 c1 = Vec3::cross(intersection_point - p1, p2 - p1);
		Vec3 c2 = Vec3::cross(intersection_point - p2, p3 - p2);
		Vec3 c3 = Vec3::cross(intersection_point - p3, p1 - p3);

		float triangle_area = Vec3::cross(p2 - p1, p3 - p1).norm();
		
		return BarycentricCoords(c2.norm() / triangle_area,
								 c3.norm() / triangle_area,
								 c1.norm() / triangle_area);
	}
};

class Rectangle : public Hittable {
public:
	Vec3 p1, p2, p3, p4, n;
	Triangle t1, t2;
	Material m;

	Rectangle(Vec3 p1, Vec3 p2, Vec3 p3, Vec3 p4, Material m)
		: p1(p1), p2(p2), p3(p3), p4(p4), m(m),
		  n(Vec3::cross(p2 - p1, p4 - p1).unit()),
		  t1(Triangle(p1, p2, p3, n, n, n, m)),
		  // p3, p4, p1 gives black diagonal on left wall (tested on depth 2,
		  // ray 50)
		  t2(Triangle(p1, p3, p4, n, n, n, m)) {
	}

	std::optional<IntersectionData> intersect(Ray &r) const {
		auto t1_result = t1.intersect(r);

		if (!t1_result.has_value()) {
			return t2.intersect(r);
		}

		return t1_result;
	}

	Vec3 normal(IntersectionData &intersection_data) const {
		return n;
	}

	Material material(IntersectionData &intersection_data) const {
		return m;
	}
};
} // namespace kajiya
