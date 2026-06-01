#ifndef FILELOADER_H
#define FILELOADER_H

#include <glm/glm.hpp>

#include <array>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <algorithm>

#include "mesh.h"
#include "vertex.h"

namespace fileLoader
{
	// OBJ Mesh loader (:
	// Can only handle single object triangulated obj files
	// For now if I need a file just blender>editmode>select all>vertices>triangulate (or quads to tris)
	inline Mesh loadObj(const std::string& filePath){
		//This is the actual mesh we're outputting
		Mesh outMesh;

		std::unordered_map<uint64_t, unsigned int> positionIndexMap;

		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;

		std::string line;
		std::ifstream infile(filePath);

		if (!infile.is_open()) {
			throw std::runtime_error("ERROR::FAILED TO LOAD MESH - Bad File Path: " + filePath);
		}

		while (std::getline(infile, line)) {
			if (line.find("o ") == 0) {
				if (!outMesh.name.empty()) {
					throw std::runtime_error("ERROR::FAILED TO LOAD MESH - Multiple Objects: " + filePath);
				}
				else {
					std::istringstream ss(line);
					std::string name, identifier;
					ss >> identifier >> name;
					outMesh.name = name;
				}
			}

			if (line.find("v ") == 0) {
				std::istringstream ss(line);
				float x, y, z;
				std::string identifier;
				ss >> identifier >> x >> y >> z;
				positions.push_back({ x,y,z });
			}
			if (line.find("vn ") == 0) {
				std::istringstream ss(line);
				float x, y, z;
				std::string identifier;
				ss >> identifier >> x >> y >> z;
				normals.push_back({ x,y,z });
			}
			if (line.find("vt ") == 0) {
				std::istringstream ss(line);
				float u, v;
				std::string identifier;
				ss >> identifier >> u >> v;
				uvs.push_back({ u,v });
			}

			if (line.find("f ") == 0) {
				std::istringstream ss(line);
				std::string identifier, token;
				ss >> identifier;

				struct FaceVert { int vIdx, vtIdx, vnIdx; };
				std::vector<FaceVert> faceVerts;

				//std::string token;
				while (ss >> token) {
					FaceVert fv = { 0, 0, 0 };
					if (token.find("//") != std::string::npos) {
						// format: v//vn
						sscanf(token.c_str(), "%d//%d", &fv.vIdx, &fv.vnIdx);
						fv.vtIdx = 0;
					}
					else {
						std::replace(token.begin(), token.end(), '/', ' ');
						std::istringstream ts(token);
						ts >> fv.vIdx;
						ts >> fv.vtIdx;
						ts >> fv.vnIdx;
					}
					faceVerts.push_back(fv);
				}
				
				if (faceVerts.size() < 3) continue;
				for (size_t k = 1; k + 1 < faceVerts.size(); k++) {
					std::array<FaceVert, 3> tri = { faceVerts[0], faceVerts[k], faceVerts[k + 1] };
					for (auto& fv : tri) {
						uint64_t key = ((uint64_t)fv.vIdx << 32) |
							((uint64_t)(fv.vnIdx & 0xFFFF) << 16) |
							(fv.vtIdx & 0xFFFF);
						auto it = positionIndexMap.find(key);
						if (it != positionIndexMap.end()) {
							outMesh.indices.push_back(it->second);
						}
						else {
							Vertex v;
							if (fv.vIdx < 1 || fv.vIdx - 1 >= positions.size()) {
								std::cerr << "Bad vIdx: " << fv.vIdx << " positions size: " << positions.size() << std::endl;
								continue;
							}
							v.position = positions[fv.vIdx - 1];
							v.position = positions[fv.vIdx - 1];
							if (fv.vnIdx > 0) v.normal = normals[fv.vnIdx - 1];
							if (fv.vtIdx > 0) v.uv = uvs[fv.vtIdx - 1];
							unsigned int newID = outMesh.vertices.size();
							outMesh.vertices.push_back(v);
							outMesh.indices.push_back(newID);
							positionIndexMap[key] = newID;
						}
					}
				}
			}
			
		}
		outMesh.originalVertices = outMesh.vertices;
		return outMesh;
	}
};

#endif // !FILELOADER_H