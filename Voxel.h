#pragma once
#include "VoxelType.h"

class Voxel {
public:
    Voxel(float x, float y, float z, float size, VoxelType type);

    void draw() const;
    VoxelType getType() const;

private:
    float m_x;
    float m_y;
    float m_z;
    float m_size;
    VoxelType m_type;
};
