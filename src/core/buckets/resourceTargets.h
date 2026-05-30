#ifndef RESOURCETARGETS_H
#define RESOURCETARGETS_H

#include <string_view>

struct MeshTarget {
	std::string_view name;
	std::string_view path;
};

struct ShaderTarget {
	std::string_view name;
	std::string_view vs;
	std::string_view fs;
};

inline constexpr ShaderTarget SHADER_TARGETS[] = {
	{ "debugGrid", "resources/shaders/debugGridTest_vs.glsl", "resources/shaders/debugGridTest_fs.glsl" },
	{ "defaultMesh", "resources/shaders/defaultMesh_vs.glsl", "resources/shaders/defaultMesh_fs.glsl" },
	{ "defaultTerrain", "resources/shaders/terrain_vs.glsl", "resources/shaders/terrain_fs.glsl"},
	{ "blinnPhong", "resources/shaders/blinnPhong_vs.glsl", "resources/shaders/blinnPhong_fs.glsl"}
};

inline constexpr MeshTarget MESH_TARGETS[] = {
	{ "cube", "resources/models/cube.obj"},
	{ "bunny_lo", "resources/models/bunny_lo.obj"},
	{ "monkey", "resources/models/monkey.obj"}
};

#endif // !RESOURCETARGETS_H
