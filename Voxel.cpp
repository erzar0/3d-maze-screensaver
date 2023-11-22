#include "pch.h"
#include "Voxel.h"
#include "Cell.h"
#include "materials.h"

Voxel::Voxel(float x, float y, float z, float size, VoxelType type, sf::Texture* texture) : m_x(x), m_y(y), m_z(z), m_size(size), m_type(type), m_texture(texture) {}

void Voxel::draw() const {
    float ds = m_size / 2.0f;

    bool debug = false;

    sf::Texture::bind(m_texture);
    //glDisable(GL_COLOR_MATERIAL);
    glPushMatrix();
    glPolygonMode(GL_BACK, GL_FILL);
    //glMaterialfv(GL_FRONT, GL_AMBIENT, PolishedGoldAmbient);
    //glMaterialfv(GL_FRONT, GL_DIFFUSE, PolishedGoldDiffuse);
    //glMaterialfv(GL_FRONT, GL_SPECULAR, PolishedGoldSpecular);
    //glMaterialf(GL_FRONT, GL_SHININESS, PolishedGoldShininess);
    //if (m_type == VoxelType::WALL)
    //{
    //    glColor3f(1.0f, 0.0f, 0.0f); 
    //}
    //else if (m_type == VoxelType::PATH)
    //{
    //    glColor3f(0.8f, 0.8f, 0.8f);
    //}
    glTranslatef(m_x, m_y, m_z);
    glBegin(GL_QUADS);
    {
        // NORTH
        if (debug) { glColor3f(0, 0, 1); }
        glTexCoord2f(0, 1);
        glVertex3f(-ds, -ds, ds);
        glTexCoord2f(0, 0);
        glVertex3f(-ds, ds, ds);
        glTexCoord2f(1, 0);
        glVertex3f(ds, ds, ds);
        glTexCoord2f(1, 1);
        glVertex3f(ds, -ds, ds);

        // SOUTH
        if (debug) { glColor3f(0, 0, 0.5); }
        glTexCoord2f(0, 1);
        glVertex3f(-ds, -ds, -ds);
        glTexCoord2f(0, 0);
        glVertex3f(-ds, ds, -ds);
        glTexCoord2f(1, 0);
        glVertex3f(ds, ds, -ds);
        glTexCoord2f(1, 1);
        glVertex3f(ds, -ds, -ds);

        // EAST 
        if (debug) { glColor3f(1, 0, 0); }
        glTexCoord2f(0, 0);
        glVertex3f(ds, -ds, -ds);
        glTexCoord2f(1, 0);
        glVertex3f(ds, -ds, ds);
        glTexCoord2f(1, 1);
        glVertex3f(ds, ds, ds);
        glTexCoord2f(0, 1);
        glVertex3f(ds, ds, -ds);

        // WEST
        if (debug) { glColor3f(0.5, 0, 0); }
        glTexCoord2f(0, 0);
        glVertex3f(-ds, -ds, -ds);
        glTexCoord2f(1, 0);
        glVertex3f(-ds, -ds, ds);
        glTexCoord2f(1, 1);
        glVertex3f(-ds, ds, ds);
        glTexCoord2f(0, 1);
        glVertex3f(-ds, ds, -ds);

        // TOP
        if (debug) { glColor3f(0, 0, 1); }
        glTexCoord2f(0, 0);
        glVertex3f(-ds, ds, -ds);
        glTexCoord2f(1, 0);
        glVertex3f(-ds, ds, ds);
        glTexCoord2f(1, 1);
        glVertex3f(ds, ds, ds);
        glTexCoord2f(0, 1);
        glVertex3f(ds, ds, -ds);

        // BOTTOM
        if (debug) { glColor3f(0, 0, 0.5); }
        glTexCoord2f(0, 0);
        glVertex3f(-ds, -ds, -ds);
        glTexCoord2f(1, 0);
        glVertex3f(-ds, -ds, ds);
        glTexCoord2f(1, 1);
        glVertex3f(ds, -ds, ds);
        glTexCoord2f(0, 1);
        glVertex3f(ds, -ds, -ds);

    }
    glEnd();
    glPopMatrix();
}

VoxelType Voxel::getType() const {
    return m_type;
}

sf::Texture* Voxel::getTexture() const {
    return  m_texture;
}


float Voxel::getX() const
{
    return m_x;
}
float Voxel::getY() const 
{
    return m_y;
}
float Voxel::getZ() const 
{
    return m_z;
}


