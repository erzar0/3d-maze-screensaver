#include "pch.h"
#include "Camera.h"
#include "Voxel.h"
#include "Cell.h"
#include "CellType.h"
#include "Maze.h"


typedef sf::Event sfe;
typedef sf::Keyboard sfk;

int SCREEN_WIDTH = 1280;
int SCREEN_HEIGHT = 720;
Camera camera(3.0f, 3.0f, 3.0f, 0.0f, 0.0f, 0.005f, 0.05f, SCREEN_WIDTH, SCREEN_HEIGHT);

GLubyte* data;
std::atomic<bool> write_thread_is_up(false);

sf::Texture bricks_texture;
sf::Texture gold_texture;
sf::Texture iron_texture;

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

    bricks_texture.loadFromFile("bricks.png");
    bricks_texture.generateMipmap();
    gold_texture.loadFromFile("gold_block.png");
    gold_texture.generateMipmap();
    iron_texture.loadFromFile("iron_block.png");
    iron_texture.generateMipmap();
}

void reshapeScreen(sf::Vector2u size)
{
    glViewport(0, 0, (GLsizei)size.x, (GLsizei)size.y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (camera.isPerspectiveProjection())
    {
        gluPerspective(camera.getFov(), (GLdouble)size.x / (GLdouble)size.y, 0.1, 100.0);
    }
    else
    {
        glOrtho(-1.245 * ((GLdouble)size.x / (GLdouble)size.y), 1.245 * ((GLdouble)size.x / (GLdouble)size.y), -1.245, 1.245, -3.0, 12.0);
    }
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

void drawScene(const std::vector<std::reference_wrapper<std::vector<Voxel>>>& world_voxels)
{
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(camera.getX(), camera.getY(), camera.getZ(),camera.getSightX(), camera.getSightY(), camera.getSightZ(), 0.0, 1.0, 0.0);

    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0); glVertex3f(0, 0, 0); glVertex3f(100000.0, 0, 0);
    glColor3f(0.0, 1.0, 0.0); glVertex3f(0, 0, 0); glVertex3f(0, 100000.0, 0);
    glColor3f(0.0, 0.0, 1.0); glVertex3f(0, 0, 0); glVertex3f(0, 0, 100000.0);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    for (const auto& voxel_vec_ref_wrapper : world_voxels)
    {
        const auto& voxel_vec = voxel_vec_ref_wrapper.get();
        for (const auto& voxel : voxel_vec)
        {
            voxel.draw();
        }
    }
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
    sf::ContextSettings context(24, 0, 0, 4, 5);
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "grafika-3d-projekt", 7U, context);
    sf::Clock deltaClock;

    ImGui::SFML::Init(window);

    window.setVerticalSyncEnabled(true);

    reshapeScreen(window.getSize());
    initOpenGL();

    Maze maze(10, 10);
    maze.generate();
    maze.draw();

    std::vector<std::vector<Voxel>> floor_voxels = std::vector<std::vector<Voxel>>(maze.getSizeX());
    std::vector<std::vector<Voxel>> ceil_voxels = std::vector<std::vector<Voxel>>(maze.getSizeX());
    std::vector<std::reference_wrapper<std::vector<Voxel>>> world_voxels;

    for (int i = 0; i < maze.getSizeX(); i++)
    {
        for (int j = 0; j < maze.getSizeY(); j++)
        {
            const Cell& cell = maze.getCellAt(i, j);
            if (cell.type == CellType::WALL)
            {
                floor_voxels[i].push_back(Voxel((float)i, -0.5f, (float)j, 1.0f, VoxelType::WALL, &bricks_texture));

            }
            else if (cell.type == CellType::PATH)
            {
                floor_voxels[i].push_back(Voxel((float)i, -0.5f, (float)j, 1.0f, VoxelType::PATH, &iron_texture));
            }
            ceil_voxels[i].push_back(Voxel((float)i, 1.5f, (float)j, 1.0f, VoxelType::CEIL, &gold_texture));
        }
        world_voxels.emplace_back(std::ref(floor_voxels[i]));
        world_voxels.emplace_back(std::ref(ceil_voxels[i]));
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

        drawScene(world_voxels);

        ImGui::SFML::Update(window, df);
        ImGui::SFML::Render(window);

        window.display();
    }
    ImGui::SFML::Shutdown();
    return 0;
}