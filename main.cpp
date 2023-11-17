#include "pch.h"
#include "Maze.h"
#include "Voxel.h"
#include "Camera.h"

int WIDTH{ 1280 };
int HEIGHT{ 1024 };

void initOpenGL(void);
void reshapeScreen(sf::Vector2u size);
void drawScene(const std::vector<std::vector<Voxel>>& floorVoxels);

Camera camera(3.0f, 3.0f, 3.0f, 0.0f, 0.0f, 0.005f, 0.05f, WIDTH, HEIGHT);
bool perspective_projection{ true };
float fov{ 45.0f } ;

sf::Texture bricks_texture;
sf::Texture gold_texture;
sf::Texture iron_texture;

//sf::Vector3f cross_product(sf::Vector3f u, sf::Vector3f v)
//{
//    sf::Vector3f res;
//    res.x = u.y * v.z - u.z * v.y;
//    res.y = u.z * v.x - u.x * v.z;
//    res.z = u.x * v.y - u.y * v.x;
//    return res;
//}
//
//void glVertexsf(sf::Vector3f v)
//{
//    glVertex3f(v.x, v.y, v.z);
//}
//
//void glNormalsf(sf::Vector3f v)
//{
//    glNormal3f(v.x, v.y, v.z);
//}

int main()
{
    bool running = true;
    sf::ContextSettings context(24, 0, 0, 4, 5);
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Open GL Lab1 04", 7U, context);
    sf::Clock deltaClock;

    ImGui::SFML::Init(window);

    window.setVerticalSyncEnabled(true);

    reshapeScreen(window.getSize());
    initOpenGL();

    Maze maze(10, 10);
    maze.generate();
    maze.draw();

    std::vector<std::vector<Voxel>> floorVoxels = std::vector<std::vector<Voxel>>(maze.getSizeX());
    std::vector<std::vector<Voxel>> ceilVoxels = std::vector<std::vector<Voxel>>(maze.getSizeX());

    for (int i = 0; i < maze.getSizeX(); i++)
    {
        for (int j = 0; j < maze.getSizeY(); j++)
        {
            const Cell& cell = maze.getCellAt(i, j);
            if (cell.type == CellType::WALL)
            {
                floorVoxels[i].push_back(Voxel((float)i, 0.5f, (float)j, 1.0f, VoxelType::WALL));

            }
            else if (cell.type == CellType::PATH)
            {
                floorVoxels[i].push_back(Voxel((float)i, -0.5f, (float)j, 1.0f, VoxelType::PATH));
            }
            ceilVoxels[i].push_back(Voxel((float)i, 1.5f, (float)j, 1.0f, VoxelType::CEIL));
        }
    }

    while (running)
    {
        sf::Time df = deltaClock.restart();
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) running = false;
            if (event.type == sf::Event::Resized) reshapeScreen(window.getSize());
        }

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { camera.translation(df, sf::Keyboard::W); }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { camera.translation(df, sf::Keyboard::S); } 
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { camera.translation(df, sf::Keyboard::A); } 
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { camera.translation(df, sf::Keyboard::D); } 
        sf::Vector2i mouse = sf::Mouse::getPosition();
        camera.rotation(mouse.x, mouse.y);

        drawScene(floorVoxels);

        ImGui::SFML::Update(window, df);
        ImGui::SFML::Render(window);

        window.display();
    }
    ImGui::SFML::Shutdown();
    return 0;
}

void initOpenGL(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

	bricks_texture.loadFromFile("bricks.png");
	bricks_texture.generateMipmap();
	iron_texture.loadFromFile("iron_block.png");
	iron_texture.generateMipmap();
	gold_texture.loadFromFile("gold_block.png");
	gold_texture.generateMipmap();
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

void drawScene(const std::vector<std::vector<Voxel>>& floorVoxels)
{
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    //Spherical north_of_camera(camera.distance, camera.theta, camera.phi + 0.01f);
    gluLookAt(camera.getX(), camera.getY(), camera.getZ(),
        camera.getSightX(), camera.getSightY(), camera.getSightZ(),
        0.0, 1.0, 0.0);
    //GLfloat light0_position[4] = { light_position.getX(), light_position.getY(), light_position.getZ(), 0.0f };
    //glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

    //uklad
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0); glVertex3f(0, 0, 0); glVertex3f(1.0, 0, 0);
    glColor3f(0.0, 1.0, 0.0); glVertex3f(0, 0, 0); glVertex3f(0, 1.0, 0);
    glColor3f(0.0, 0.0, 1.0); glVertex3f(0, 0, 0); glVertex3f(0, 0, 1.0);
    glEnd();

    //Linie przerywane
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(2, 0xAAAA);
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0); glVertex3f(0, 0, 0); glVertex3f(-1.0, 0, 0);
    glColor3f(0.0, 1.0, 0.0); glVertex3f(0, 0, 0); glVertex3f(0, -1.0, 0);
    glColor3f(0.0, 0.0, 1.0); glVertex3f(0, 0, 0); glVertex3f(0, 0, -1.0);
    glEnd();
    glDisable(GL_LINE_STIPPLE);

    //glEnable(GL_TEXTURE_2D);
    for (const auto& voxelVec: floorVoxels)
    {
        for (const auto& voxel: voxelVec)
        {
            //if (voxel.getType() == VoxelType::WALL) sf::Texture::bind(&bricks_texture);
            //else if (voxel.getType() == VoxelType::PATH) sf::Texture::bind(&iron_texture);
            //else if (voxel.getType() == VoxelType::CEIL) sf::Texture::bind(&gold_texture);

            voxel.draw();
        }
    }
    //glDisable(GL_TEXTURE_2D);

    //sf::Vector3f u, v, res[4], flag[20][20], flag_n[20][20];

    //for (int x = 1; x < 19; x++)
    //{
    //    for (int y = 1; y < 19; y++)
    //    {
    //        u = flag[x][y + 1] - flag[x][y];
    //        v = flag[x + 1][y] - flag[x][y];
    //        res[0] = cross_product(u, v);

    //        u = flag[x + 1][y] - flag[x][y];
    //        v = flag[x][y - 1] - flag[x][y];
    //        res[1] = cross_product(u, v);

    //        u = flag[x][y - 1] - flag[x][y];
    //        v = flag[x - 1][y] - flag[x][y];
    //        res[2] = cross_product(u, v);

    //        u = flag[x - 1][y] - flag[x][y];
    //        v = flag[x][y + 1] - flag[x][y];
    //        res[3] = cross_product(u, v);

    //        flag_n[x][y] = (res[0] + res[1] + res[2] + res[3]);

    //    }
    //}
    ////glColor3f(1.0f, 1.0f, 1.0f);
    //glEnable(GL_TEXTURE_2D);
    //sf::Texture::bind(&bricks_texture);

    ////glBegin(GL_QUADS);
    ////for (int x = 1; x < 18; x++)
    ////    for (int y = 1; y < 18; y++)
    ////    {
    ////        //flag[][].x -> <-1;1>
    ////        //flag[][].z -> <-1;1>
    ////        glTexCoord2f((flag[x][y].x + 1.0f) / 2.0f, (flag[x][y].z + 1.0f) / 2.0f);
    ////        glNormalsf(flag_n[x][y]);         glVertexsf(flag[x][y]);
    ////        glTexCoord2f((flag[x + 1][y].x + 1.0f) / 2.0f, (flag[x + 1][y].z + 1.0f) / 2.0f);
    ////        glNormalsf(flag_n[x + 1][y]);     glVertexsf(flag[x + 1][y]);
    ////        glTexCoord2f((flag[x + 1][y + 1].x + 1.0f) / 2.0f, (flag[x + 1][y + 1].z + 1.0f) / 2.0f);
    ////        glNormalsf(flag_n[x + 1][y + 1]); glVertexsf(flag[x + 1][y + 1]);
    ////        glTexCoord2f((flag[x][y + 1].x + 1.0f) / 2.0f, (flag[x][y + 1].z + 1.0f) / 2.0f);
    ////        glNormalsf(flag_n[x][y + 1]);     glVertexsf(flag[x][y + 1]);
    ////    }
    ////glEnd();
    //glDisable(GL_TEXTURE_2D);


    ////transformacje
    //glTranslatef(pos.x, pos.y, pos.z);
    //glRotatef(rot.x, 1, 0, 0);
    //glRotatef(rot.y, 0, 1, 0);
    //glRotatef(rot.z, 0, 0, 1);
    //glScalef(scale.x, scale.y, scale.z);

    ////szescian
    //glLineWidth(2.0);
    //glColor3f(0, 0, 0);
    //glBegin(GL_LINES);
    //for (unsigned char i = 0; i < 2; i++)
    //    for (unsigned char j = 0; j < 2; j++)
    //    {
    //        glVertex3f(-0.3f + 0.6f * (i ^ j), -0.3f + 0.6f * j, -0.3f); glVertex3f(-0.3f + 0.6f * (i ^ j), -0.3f + 0.6f * j, 0.3f);
    //        glVertex3f(-0.3f, -0.3f + 0.6f * (i ^ j), -0.3f + 0.6f * j); glVertex3f(0.3f, -0.3f + 0.6f * (i ^ j), -0.3f + 0.6f * j);
    //        glVertex3f(-0.3f + 0.6f * (i ^ j), -0.3f, -0.3f + 0.6f * j); glVertex3f(-0.3f + 0.6f * (i ^ j), 0.3f, -0.3f + 0.6f * j);
    //    }
    //glEnd();
    //glLineWidth(1.0);
    ////trojkat
    //glBegin(GL_TRIANGLES);
    //glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(-0.3f, 0.3f, 0.3f);
    //glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(0.3f, -0.3f, 0.3f);
    //glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(0.3f, 0.3f, -0.3f);
    //glEnd();
    //glEnable(GL_LIGHTING);

    //GLUquadricObj* qobj = gluNewQuadric();
    //gluQuadricDrawStyle(qobj, GLU_FILL);
    //gluQuadricNormals(qobj, GLU_SMOOTH);

    //glPushMatrix();
    //glColor3f(1.0f, 0.0f, 0.0f);
    //glTranslatef(-0.75, 0.0, 0.0);
    //gluSphere(qobj, 0.2, 15, 10);
    //glPopMatrix();

    //glDisable(GL_COLOR_MATERIAL);
    //glPushMatrix();
    //glMaterialfv(GL_FRONT, GL_AMBIENT, PolishedGoldAmbient);
    //glMaterialfv(GL_FRONT, GL_DIFFUSE, PolishedGoldDiffuse);
    //glMaterialfv(GL_FRONT, GL_SPECULAR, PolishedGoldSpecular);
    //glMaterialf(GL_FRONT, GL_SHININESS, PolishedGoldShininess);
    //glColor3f(0.0f, 1.0f, 0.0f);
    //glTranslatef(0.75, 0.0, 0.0);
    //glRotatef(300.0, 1.0, 0.0, 0.0);
    //gluCylinder(qobj, 0.25, 0.0, 0.5, 15, 5);
    //glPopMatrix();
    //glEnable(GL_COLOR_MATERIAL);
}
