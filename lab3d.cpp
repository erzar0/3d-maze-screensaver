#include "pch.h"

typedef sf::Event sfe;
typedef sf::Keyboard sfk;

struct Spherical
{
    float distance, theta, phi;
    Spherical(float gdistance, float gtheta, float gphi) : distance(gdistance), theta(gtheta), phi(gphi) { }
    float getX() { return distance * cos(phi) * cos(theta); }
    float getY() { return distance * sin(phi); }
    float getZ() { return distance * cos(phi) * sin(theta); }
};

Spherical camera(3.0f, 1.0f, 0.2f), light_position(4.0f, 0.2f, 1.2f);
sf::Vector3f pos(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f), rot(0.0f, 0.0f, 0.0f);
bool perspective_projection = true;
float fov = 45.0f;
float timer = 0.0;

float *pos_offsets[3] = { &pos.x, &pos.y, &pos.z };
float* scale_offsets[3] = { &scale.x, &scale.y, &scale.z };
float* rot_offsets[3] = { &rot.x, &rot.y, &rot.z };

GLubyte* data;
std::atomic<bool> write_thread_is_up(false);

sf::Texture TEXid;

void initOpenGL(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat light_ambient_global[4] = { 0.5,0.5,0.5,1 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient_global);

    TEXid.loadFromFile("water.jpg");
    TEXid.generateMipmap();
}

void reshapeScreen(sf::Vector2u size)
{
    glViewport(0, 0, (GLsizei)size.x, (GLsizei)size.y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (perspective_projection) gluPerspective(fov, (GLdouble)size.x / (GLdouble)size.y, 0.1, 100.0);
    else glOrtho(-1.245 * ((GLdouble)size.x / (GLdouble)size.y), 1.245 * ((GLdouble)size.x / (GLdouble)size.y), -1.245, 1.245, -3.0, 12.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

sf::Vector3f cross_product(sf::Vector3f u, sf::Vector3f v)
{
    sf::Vector3f res;
    res.x = u.y * v.z - u.z * v.y;
    res.y = u.z * v.x - u.x * v.z;
    res.z = u.x * v.y - u.y * v.x;
    return res;
}

void glVertexsf(sf::Vector3f v)
{
    glVertex3f(v.x, v.y, v.z);
}

void glNormalsf(sf::Vector3f v)
{
    glNormal3f(v.x, v.y, v.z);
}

void drawScene()
{
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    Spherical north_of_camera(camera.distance, camera.theta, camera.phi + 0.01f);
    gluLookAt(camera.getX(), camera.getY(), camera.getZ(),
        0.0, 0.0, 0.0,
        north_of_camera.getX(), north_of_camera.getY(), north_of_camera.getZ());
    GLfloat light0_position[4] = { light_position.getX(), light_position.getY(), light_position.getZ(), 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
        glColor3f(1.0, 0.0, 0.0); glVertex3f(0, 0, 0); glVertex3f(1.0, 0, 0);
        glColor3f(0.0, 1.0, 0.0); glVertex3f(0, 0, 0); glVertex3f(0, 1.0, 0);
        glColor3f(0.0, 0.0, 1.0); glVertex3f(0, 0, 0); glVertex3f(0, 0, 1.0);
    glEnd();

    glEnable(GL_LINE_STIPPLE);
    glLineStipple(2, 0xAAAA);
    glBegin(GL_LINES);
        glColor3f(1.0, 0.0, 0.0); glVertex3f(0, 0, 0); glVertex3f(-1.0, 0, 0);
        glColor3f(0.0, 1.0, 0.0); glVertex3f(0, 0, 0); glVertex3f(0, -1.0, 0);
        glColor3f(0.0, 0.0, 1.0); glVertex3f(0, 0, 0); glVertex3f(0, 0, -1.0);
    glEnd();
    glDisable(GL_LINE_STIPPLE);

    glTranslatef(pos.x, pos.y, pos.z);
    glRotatef(rot.x, 1, 0, 0);
    glRotatef(rot.y, 0, 1, 0);
    glRotatef(rot.z, 0, 0, 1);
    glScalef(scale.x, scale.y, scale.z);

    glEnable(GL_LIGHTING);

    sf::Vector3f u, v, res[4], flag[20][20], flag_n[20][20];

    for (int x = 0; x < 20; x++)
        for (int y = 0; y < 20; y++)
        {
            flag[x][y].x = (x - 10) * 0.1f;
            flag[x][y].y = 0.2f * sin((x - 10) * 0.2f - timer) * cos((y - 10) * 0.2f + timer);
            flag[x][y].z = (y - 10) * 0.1f;
        }
    for (int x = 1; x < 19; x++)
        for (int y = 1; y < 19; y++)
        {
            u = flag[x][y + 1] - flag[x][y];
            v = flag[x + 1][y] - flag[x][y];
            res[0] = cross_product(u, v);

            u = flag[x + 1][y] - flag[x][y];
            v = flag[x][y - 1] - flag[x][y];
            res[1] = cross_product(u, v);

            u = flag[x][y - 1] - flag[x][y];
            v = flag[x - 1][y] - flag[x][y];
            res[2] = cross_product(u, v);

            u = flag[x - 1][y] - flag[x][y];
            v = flag[x][y + 1] - flag[x][y];
            res[3] = cross_product(u, v);

            flag_n[x][y] = (res[0] + res[1] + res[2] + res[3]);
        }

    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    sf::Texture::bind(&TEXid);

    glBegin(GL_QUADS);
    for (int x = 1; x < 18; x++)
        for (int y = 1; y < 18; y++)
        {
            flag[][].x -> <-1;1>
            flag[][].z -> <-1;1>
            glTexCoord2f((flag[x][y].x + 1.0f) / 2.0f, (flag[x][y].z + 1.0f) / 2.0f);
            glNormalsf(flag_n[x][y]);         glVertexsf(flag[x][y]);
            glTexCoord2f((flag[x + 1][y].x + 1.0f) / 2.0f, (flag[x + 1][y].z + 1.0f) / 2.0f);
            glNormalsf(flag_n[x + 1][y]);     glVertexsf(flag[x + 1][y]);
            glTexCoord2f((flag[x + 1][y + 1].x + 1.0f) / 2.0f, (flag[x + 1][y + 1].z + 1.0f) / 2.0f);
            glNormalsf(flag_n[x + 1][y + 1]); glVertexsf(flag[x + 1][y + 1]);
            glTexCoord2f((flag[x][y + 1].x + 1.0f) / 2.0f, (flag[x][y + 1].z + 1.0f) / 2.0f);
            glNormalsf(flag_n[x][y + 1]);     glVertexsf(flag[x][y + 1]);
        }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void write_data_to_disk(sf::Vector2u size)
{
    sf::Image image;
    image.create(size.x, size.y, data);
    image.flipVertically();
    image.saveToFile("screen.png");

    delete[]data;
    write_thread_is_up = false;
}

int main()
{
    bool running = true;
    sf::ContextSettings context(24, 0 , 0, 2, 0);
    sf::RenderWindow window(sf::VideoMode(1280, 1024), "Open GL Lab1 04", 7U, context);
    sf::Clock deltaClock;
    sf::Vector2i mouse_last_position(0, 0);

    ImGui::SFML::Init(window);

    window.setVerticalSyncEnabled(true);

    reshapeScreen(window.getSize());
    initOpenGL();

    while (running)
    {
        sfe event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sfe::Closed || (event.type == sfe::KeyPressed && event.key.code == sfk::Escape)) running = false;
            if (event.type == sfe::Resized) reshapeScreen(window.getSize());
            if (event.key.code == sfk::F1 && !write_thread_is_up)
            {
                write_thread_is_up = true;
                data = new GLubyte[window.getSize().x * window.getSize().y * 4];
                glReadPixels(0, 0, window.getSize().x, window.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, data);
                std::thread(write_data_to_disk, window.getSize()).detach();
            }
        }

        drawScene();
        timer += deltaClock.getElapsedTime().asSeconds();
    
        ImGui::SFML::Update(window, deltaClock.restart());
     
        ImGui::Begin("Camera");
            ImGui::SliderFloat("R", &camera.distance, 0.5f, 10.0f);
            ImGui::SliderAngle("theta", &camera.theta, 0.0f, 360.0f);
            ImGui::SliderAngle("phi", &camera.phi, 0.0f, 180.0f);
            if (ImGui::Checkbox("Perspective projection", &perspective_projection)) reshapeScreen(window.getSize());
            if (ImGui::SliderFloat("FoV", &fov, 10.0f, 90.0f)) reshapeScreen(window.getSize());
        ImGui::End();
        
        ImGui::Begin("Transformations");
         ImGui::SliderFloat3("Position", *pos_offsets, -3.0f, 3.0f);
         ImGui::SliderFloat3("Scale", *scale_offsets, -2.0f, 2.0f);
         ImGui::SliderFloat3("Rotation", *rot_offsets, -180.0f, 180.0f);
        ImGui::End();

        ImGui::Begin("Light position");
            ImGui::SliderAngle("theta", &light_position.theta, 0.0f, 360.0f);
            ImGui::SliderAngle("phi", &light_position.phi, 0.0f, 180.0f);
        ImGui::End();

        ImGui::SFML::Render(window);
       
        window.display();
    }
    ImGui::SFML::Shutdown();
    return 0;
}