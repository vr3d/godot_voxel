#include "voxel_generator_heightmap.h"
#include "../util/array_slice.h"
#include "../util/fixed_array.h"

VoxelGeneratorHeightmap::VoxelGeneratorHeightmap() {
}

void VoxelGeneratorHeightmap::set_height_start(float start) {
	_range.start = start;
}

float VoxelGeneratorHeightmap::get_height_start() const {
	return _range.start;
}

void VoxelGeneratorHeightmap::set_height_range(float range) {
	_range.height = range;
}

float VoxelGeneratorHeightmap::get_height_range() const {
	return _range.height;
}

void VoxelGeneratorHeightmap::set_iso_scale(float iso_scale) {
	_iso_scale = iso_scale;
}

float VoxelGeneratorHeightmap::get_iso_scale() const {
	return _iso_scale;
}

void VoxelGeneratorHeightmap::_bind_methods() {

	ClassDB::bind_method(D_METHOD("set_height_start", "start"), &VoxelGeneratorHeightmap::set_height_start);
	ClassDB::bind_method(D_METHOD("get_height_start"), &VoxelGeneratorHeightmap::get_height_start);

	ClassDB::bind_method(D_METHOD("set_height_range", "range"), &VoxelGeneratorHeightmap::set_height_range);
	ClassDB::bind_method(D_METHOD("get_height_range"), &VoxelGeneratorHeightmap::get_height_range);

	ClassDB::bind_method(D_METHOD("set_iso_scale", "scale"), &VoxelGeneratorHeightmap::set_iso_scale);
	ClassDB::bind_method(D_METHOD("get_iso_scale"), &VoxelGeneratorHeightmap::get_iso_scale);

	ADD_PROPERTY(PropertyInfo(Variant::REAL, "height_start"), "set_height_start", "get_height_start");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "height_range"), "set_height_range", "get_height_range");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "iso_scale"), "set_iso_scale", "get_iso_scale");
}
