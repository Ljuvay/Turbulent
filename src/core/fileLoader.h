#ifndef FILELOADER_H
#define FILELOADER_H

#include <glm/glm.hpp>

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

		std::unordered_map<int, unsigned int> positionIndexMap;

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

				std::vector<unsigned int> verts, uvIdx, normIdx;

				while (ss >> token) {
					unsigned int vIdx = 0, vtIdx = 0, vnIdx = 0;
					if (token.find('/') != std::string::npos) {
						std::replace(line.begin(), line.end(), '/', ' ');
						std::istringstream ts(token);
						ts >> vIdx;
						if (!normals.empty()) { ts >> vtIdx >> vnIdx; }
					}
					else { vIdx = std::stoi(token); }


					verts.push_back(vIdx);
					if (vtIdx > 0) { uvIdx.push_back(vtIdx); }
					if (vnIdx > 0) { normIdx.push_back(vnIdx); }
				}
				
				for (int k = 0; k < 3; k++) {
					int positionID = verts[k] - 1;

					if (positionIndexMap.count(positionID)) {
						outMesh.indices.push_back(positionIndexMap[positionID]);
					}
					else {
						Vertex v;
						v.position = positions[positionID];
						if (!normals.empty() && normIdx.size() >= 3) {
							v.normal = normals[normIdx[k] - 1];
						}
						if (!uvs.empty() && uvIdx.size() >= 3) {
							v.uv = uvs[uvIdx[k] - 1];
						}

						unsigned int newID = outMesh.vertices.size();
						outMesh.vertices.push_back(v);
						outMesh.indices.push_back(newID);
						positionIndexMap[positionID] = newID;
					}

				}
			}
			
		}
		outMesh.originalVertices = outMesh.vertices;
		return outMesh;
	}
};

#endif // !FILELOADER_H