#include "pch.h"
#include "Voxel.h"
#include "Cell.h"
#include "materials.h"

Voxel::Voxel(float x, float y, float z, float size, VoxelType type) : m_x(x), m_y(y), m_z(z), m_size(size), m_type(type) {}

void Voxel::draw() const {
    float ds = m_size / 2.0f;

    glDisable(GL_COLOR_MATERIAL);
    glPushMatrix();
    glPolygonMode(GL_BACK, GL_FILL);
    glMaterialfv(GL_FRONT, GL_AMBIENT, PolishedGoldAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, PolishedGoldDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, PolishedGoldSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, PolishedGoldShininess);
    if (m_type == VoxelType::WALL)
    {
        glColor3f(1.0f, 0.0f, 0.0f); 
    }
    else if (m_type == VoxelType::PATH)
    {
        glColor3f(0.8f, 0.8f, 0.8f);
    }
    glTranslatef(m_x, m_y, m_z);
    glBegin(GL_QUAD_STRIP);
    {
        glVertex3f(-ds, -ds, -ds);
        glVertex3f(-ds, -ds, ds);
        glVertex3f(ds, -ds, -ds);
        glVertex3f(ds, -ds, ds);
        glVertex3f(ds, ds, -ds);
        glVertex3f(ds, ds, ds);
        glVertex3f(-ds, ds, -ds);
        glVertex3f(-ds, ds, ds);
        glVertex3f(-ds, -ds, -ds);
        glVertex3f(-ds, -ds, ds);
    }
    glEnd();
    glBegin(GL_QUADS);
    {
        glVertex3f(-ds, -ds, -ds);
        glVertex3f(ds, -ds, -ds);
        glVertex3f(ds, ds, -ds);
        glVertex3f(-ds, ds, -ds);
        glVertex3f(-ds, -ds, ds);
        glVertex3f(ds, -ds, ds);
        glVertex3f(ds, ds, ds);
        glVertex3f(-ds, ds, ds);
    }
    glEnd();
    glPopMatrix();
    glEnable(GL_COLOR_MATERIAL);
}

VoxelType Voxel::getType() const {
    return m_type;
}
