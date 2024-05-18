#pragma once

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "hittable.h"
#include "material.h"
#include "rectangle.h"
#include "mesh.h"
#include "vec3.h"
#include <vector>
#include <string>

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace kajiya {
class Loader {
public:
	static std::vector<Triangle*> load_obj(std::string inputfile) {
		tinyobj::ObjReaderConfig reader_config;
		reader_config.mtl_search_path = "./assets";

		tinyobj::ObjReader reader;

		if(!reader.ParseFromFile(inputfile, reader_config)) {
			if(!reader.Error().empty()) {
				std::cerr << "TinyObjReader: " << reader.Error();
			}
		}

		if(!reader.Warning().empty()) {
			std::cout << "TinyObjReader: " << reader.Warning();
		}

		auto& attrib = reader.GetAttrib();
		auto& shapes = reader.GetShapes();
		auto& materials = reader.GetMaterials();

		std::vector<Triangle*> triangles;
		Triangle temp;
		temp.m = Material::get_white();

		// Loop over shapes
		for(size_t shape_index = 0; shape_index < shapes.size(); shape_index++) {
			size_t index_offset = 0;

			// Loop over faces
			for(size_t face_index = 0; face_index < shapes[shape_index].mesh.num_face_vertices.size(); face_index++) {
				size_t current_face_vertices_num = size_t(shapes[shape_index].mesh.num_face_vertices[face_index]);

				// NOTE(stekap): Not storage efficient.
				for(int i = 1; i < current_face_vertices_num - 1; ++i) {
					tinyobj::index_t idx_1 = shapes[shape_index].mesh.indices[index_offset + 0];
					tinyobj::index_t idx_2 = shapes[shape_index].mesh.indices[index_offset + i];
					tinyobj::index_t idx_3 = shapes[shape_index].mesh.indices[index_offset + i + 1];
					
					temp.p1 = Vec3(attrib.vertices[3*size_t(idx_1.vertex_index) + 0],
								   attrib.vertices[3*size_t(idx_1.vertex_index) + 1],
								   attrib.vertices[3*size_t(idx_1.vertex_index) + 2]);
					temp.p2 = Vec3(attrib.vertices[3*size_t(idx_2.vertex_index) + 0],
								   attrib.vertices[3*size_t(idx_2.vertex_index) + 1],
								   attrib.vertices[3*size_t(idx_2.vertex_index) + 2]);
					temp.p3 = Vec3(attrib.vertices[3*size_t(idx_3.vertex_index) + 0],
								   attrib.vertices[3*size_t(idx_3.vertex_index) + 1],
								   attrib.vertices[3*size_t(idx_3.vertex_index) + 2]);

					// NOTE(stekap): For now, we take normal at one point and set it as triangle normal.
					if(idx_1.normal_index >= 0) {
						temp.n = Vec3(attrib.normals[3*size_t(idx_1.normal_index) + 0],
									  attrib.normals[3*size_t(idx_1.normal_index) + 1],
									  attrib.normals[3*size_t(idx_1.normal_index) + 2]);
					}

					// Check if `texcoord_index` is zero or positive. negative = no texcoord data
					// if (idx.texcoord_index >= 0) {
					// 	tinyobj::real_t tx = attrib.texcoords[2*size_t(idx.texcoord_index)+0];
					// 	tinyobj::real_t ty = attrib.texcoords[2*size_t(idx.texcoord_index)+1];
					// }

					// Optional: vertex colors
					// tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
					// tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
					// tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];

					triangles.push_back(new Triangle(temp.p1, temp.p2, temp.p3, temp.n, temp.m));
				}

				// TODO(stekap): More efficient way to represent meshes. (currently it is just array of triangles)
				// TODO(stekap): Incorporate per vertex normals.
				// TODO(stekap): Handle material and color.

				index_offset += current_face_vertices_num;
			}
		}

		return triangles;
	}
	
	static std::vector<Hittable*> load_cornell_box() {
		std::vector<kajiya::Hittable *> objects;

		glm::mat4 obj_translation = glm::translate(glm::mat4(1.0f),
												   glm::vec3(150, 350, 350));
		glm::mat4 obj_transform = glm::scale(obj_translation, glm::vec3(100, 100, 100));

		
		std::vector<Triangle*> triangles = load_obj("assets/icosahedron.obj");
		
		for(Triangle*& t : triangles) {
			t->p1 = Vec3::from_glm_vec4(obj_transform * t->p1.to_glm_vec4());
			t->p2 = Vec3::from_glm_vec4(obj_transform * t->p2.to_glm_vec4());
			t->p3 = Vec3::from_glm_vec4(obj_transform * t->p3.to_glm_vec4());
			objects.push_back(t);
		}
		
		// light
		objects.push_back(new kajiya::Rectangle(
			kajiya::Vec3(343.0, 548.8, 227.0),
			kajiya::Vec3(343.0, 548.8, 332.0),
			kajiya::Vec3(213.0, 548.8, 332.0),
			kajiya::Vec3(213.0, 548.8, 227.0), kajiya::Material::get_light()));
		// floor
		objects.push_back(new kajiya::Rectangle(
			kajiya::Vec3(552.8, 0.0, 0.0), kajiya::Vec3(0.0, 0.0, 0.0),
			kajiya::Vec3(0.0, 0.0, 559.2), kajiya::Vec3(549.6, 0.0, 559.2),
			kajiya::Material::get_white()));
		// ceiling
		objects.push_back(new kajiya::Rectangle(
			kajiya::Vec3(556.0, 548.8, 0.0), kajiya::Vec3(556.0, 548.8, 559.2),
			kajiya::Vec3(0.0, 548.8, 559.2), kajiya::Vec3(0.0, 548.8, 0.0),
			kajiya::Material::get_white()));
		// back wall
		objects.push_back(new kajiya::Rectangle(
			kajiya::Vec3(549.6, 0.0, 559.2), kajiya::Vec3(0.0, 0.0, 559.2),
			kajiya::Vec3(0.0, 548.8, 559.2), kajiya::Vec3(556.0, 548.8, 559.2),
			kajiya::Material::get_white()));
		// right wall
		objects.push_back(new kajiya::Rectangle(
			kajiya::Vec3(0.0, 0.0, 559.2), kajiya::Vec3(0.0, 0.0, 0.0),
			kajiya::Vec3(0.0, 548.8, 0.0), kajiya::Vec3(0.0, 548.8, 559.2),
			kajiya::Material::get_green()));
		// left wall
		objects.push_back(new kajiya::Rectangle(
			kajiya::Vec3(552.8, 0.0, 0.0), kajiya::Vec3(549.6, 0.0, 559.2),
			kajiya::Vec3(556.0, 548.8, 559.2), kajiya::Vec3(556.0, 548.8, 0.0),
			kajiya::Material::get_red()));
		// short block
		objects.push_back(new kajiya::Rectangle(
			kajiya::Vec3(130.0, 165.0, 65.0), kajiya::Vec3(82.0, 165.0, 225.0),
			kajiya::Vec3(240.0, 165.0, 272.0),
			kajiya::Vec3(290.0, 165.0, 114.0), kajiya::Material::get_white()));
		objects.push_back(new kajiya::Rectangle(
			kajiya::Vec3(290.0, 0.0, 114.0), kajiya::Vec3(290.0, 165.0, 114.0),
			kajiya::Vec3(240.0, 165.0, 272.0), kajiya::Vec3(240.0, 0.0, 272.0),
			kajiya::Material::get_white()));
		objects.push_back(new kajiya::Rectangle(
			kajiya::Vec3(130.0, 0.0, 65.0), kajiya::Vec3(130.0, 165.0, 65.0),
			kajiya::Vec3(290.0, 165.0, 114.0), kajiya::Vec3(290.0, 0.0, 114.0),
			kajiya::Material::get_white()));
		objects.push_back(new kajiya::Rectangle(
			kajiya::Vec3(82.0, 0.0, 225.0), kajiya::Vec3(82.0, 165.0, 225.0),
			kajiya::Vec3(130.0, 165.0, 65.0), kajiya::Vec3(130.0, 0.0, 65.0),
			kajiya::Material::get_white()));
		objects.push_back(new kajiya::Rectangle(
			kajiya::Vec3(240.0, 0.0, 272.0), kajiya::Vec3(240.0, 165.0, 272.0),
			kajiya::Vec3(82.0, 165.0, 225.0), kajiya::Vec3(82.0, 0.0, 225.0),
			kajiya::Material::get_white()));
		// tall block
		objects.push_back(new kajiya::Rectangle(
			kajiya::Vec3(423.0, 330.0, 247.0),
			kajiya::Vec3(265.0, 330.0, 296.0),
			kajiya::Vec3(314.0, 330.0, 456.0),
			kajiya::Vec3(472.0, 330.0, 406.0), kajiya::Material::get_white()));
		objects.push_back(new kajiya::Rectangle(
			kajiya::Vec3(423.0, 0.0, 247.0), kajiya::Vec3(423.0, 330.0, 247.0),
			kajiya::Vec3(472.0, 330.0, 406.0), kajiya::Vec3(472.0, 0.0, 406.0),
			kajiya::Material::get_white()));
		objects.push_back(new kajiya::Rectangle(
			kajiya::Vec3(472.0, 0.0, 406.0), kajiya::Vec3(472.0, 330.0, 406.0),
			kajiya::Vec3(314.0, 330.0, 456.0), kajiya::Vec3(314.0, 0.0, 456.0),
			kajiya::Material::get_white()));
		objects.push_back(new kajiya::Rectangle(
			kajiya::Vec3(314.0, 0.0, 456.0), kajiya::Vec3(314.0, 330.0, 456.0),
			kajiya::Vec3(265.0, 330.0, 296.0), kajiya::Vec3(265.0, 0.0, 296.0),
			kajiya::Material::get_white()));
		objects.push_back(new kajiya::Rectangle(
			kajiya::Vec3(265.0, 0.0, 296.0), kajiya::Vec3(265.0, 330.0, 296.0),
			kajiya::Vec3(423.0, 330.0, 247.0), kajiya::Vec3(423.0, 0.0, 247.0),
			kajiya::Material::get_white()));

		return objects;
	}
};
} // namespace kajiya
