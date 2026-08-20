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

struct TextureTarget {
	std::string_view name;
	std::string_view path;
};

inline constexpr ShaderTarget SHADER_TARGETS[] = {
	{ "debugGrid", "resources/shaders/debugGridTest_vs.glsl", "resources/shaders/debugGridTest_fs.glsl" },
	{ "defaultMesh", "resources/shaders/defaultMesh_vs.glsl", "resources/shaders/defaultMesh_fs.glsl" },
	{ "defaultTerrain", "resources/shaders/terrain_vs.glsl", "resources/shaders/terrain_fs.glsl"},
	{ "blinnPhong", "resources/shaders/blinnPhong_vs.glsl", "resources/shaders/blinnPhong_fs.glsl"},
	{ "billboardParticle", "resources/shaders/billboardParticle_vs.glsl", "resources/shaders/billboardParticle_fs.glsl"},
	{ "depthShadow", "resources/shaders/depthShader_vs.glsl", "resources/shaders/depthShader_fs.glsl"}
};

inline constexpr MeshTarget MESH_TARGETS[] = {
	{ "cube", "resources/models/cube.obj"},
	{ "gem", "resources/models/gem.obj"},
	{ "sphere", "resources/models/sphere.obj"},
	{ "stanfordBunny", "resources/models/stanford-bunny.obj"},
	{ "stanfordBunnyHighRes", "resources/models/stanford-bunny-highres.obj"},
	{ "monkey", "resources/models/monkey.obj"},
	{ "cat", "resources/models/concrete_cat_statue_4k.obj"},
	{ "billboard", "resources/models/billboard.obj"}
};

inline constexpr TextureTarget TEXTURE_TARGETS[] = {
	{"wood", "resources/textures/oriented_strand_board_diff_4k.png"},
	{"cat", "resources/textures/concrete_cat_statue_diff_4k.png" },
	{"grass", "resources/textures/rocky_terrain_02_diff_4k.png"},
	{"rock", "resources/textures/rock_01_diff_4k.png"},
	{"snow", "resources/textures/snow_02_diff_4k.png"},
	{"sand", "resources/textures/sandy_gravel_02_diff_4k.png"}
};

#endif // !RESOURCETARGETS_H
