#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include <cstdlib>
#include <direct.h>
#include "Campfire.h"
#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "directionLight.h"
#include "sphere.h"
#include "sphereWithTexture.h"
#include "spotLight.h"
#include "cube.h"
#include "stb_image.h"
#include "cylinder.h"
#include "Model.h"
#include "Sphere2.h"
#include "Bush.h"
#include "Barrack.h"
//#include "JeepCar.h"
#include "jeep.h"
#include "truck.h"
#include "TubeLight.h"
#include "WatchTower.h"
#include "Garageplatform.h"
#include "boundary.h"
#include "road.h"
#include "RadarOffice.h"
#include "build.h"
#include "furniture.h"
#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);
unsigned int createProceduralTexture(int width, int height, unsigned char r, unsigned char g, unsigned char b);
// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

// Global scene sizes (change GROUND_SIZE to make the ground larger)
const float GROUND_SIZE = 200.0f; // full width/length of the ground plane
const float GROUND_HEIGHT = 0.1f; // thickness of the ground

void drawFractalTree(Shader& shader, const glm::mat4& model, int depth, Cube& CubeGreen, const glm::mat4& a_translate_mat, Bush& Leaf_Obj, const glm::mat4& model2, unsigned int cubeVAO) {
    if (depth == 0) return;

    // Draw the current cube
    CubeGreen.drawCubeWithTexture(shader, a_translate_mat * model);
    //CubeGreen.drawTexture(shader, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, a_translate_mat * model);
    if (depth == 1)
    {
        Leaf_Obj.drawWithTexture(shader, a_translate_mat * model2);
    }

    // Scaling and translation for the branches
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.75f, 0.5f));

    float deg = 0;
    glm::mat4 rotateY = rotate(glm::mat4(1.0f), glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
    // Left branch 1
    glm::mat4 translateLeft = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.05f));
    glm::mat4 rotateLeft = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 leftBranchModel = translateLeft * rotateY * rotateLeft * scaleMatrix * model;
    glm::mat4 leftBranchforLeaf = translateLeft * rotateY * rotateLeft * model2;
    drawFractalTree(shader, leftBranchModel, depth - 1, CubeGreen, a_translate_mat, Leaf_Obj, leftBranchforLeaf, cubeVAO);
    // Right branch 1
    glm::mat4 translateRight = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.05f));
    glm::mat4 rotateRight = glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 rightBranchModel = translateRight * rotateY * rotateRight * scaleMatrix * model;
    glm::mat4 rightBranchforLeaf = translateRight * rotateY * rotateRight * model2;
    drawFractalTree(shader, rightBranchModel, depth - 1, CubeGreen, a_translate_mat, Leaf_Obj, rightBranchforLeaf, cubeVAO);

    deg = 90;
    rotateY = rotate(glm::mat4(1.0f), glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
    // Left branch 2
    translateLeft = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.15f));
    glm::mat4 leftBranchModel2 = translateLeft * rotateY * rotateLeft * scaleMatrix * model;
    glm::mat4 leftBranchforLeaf2 = translateLeft * rotateY * rotateLeft * model2;
    drawFractalTree(shader, leftBranchModel2, depth - 1, CubeGreen, a_translate_mat, Leaf_Obj, leftBranchforLeaf2, cubeVAO);
    translateRight = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.15f));
    glm::mat4 rightBranchModel2 = translateRight * rotateY * rotateRight * scaleMatrix * model;
    glm::mat4 rightBranchforLeaf2 = translateRight * rotateY * rotateRight * model2;
    drawFractalTree(shader, rightBranchModel2, depth - 1, CubeGreen, a_translate_mat, Leaf_Obj, rightBranchforLeaf2, cubeVAO);

}

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;



float RadarRotateAngle = 0.0f;
bool RotateRadar = false;
float cx = 0.0f;
float cy = 5.2f;
float cz = 15.0f;
//initial values

bool tpv = true;

// camera
float camera_z = 0.0f;
//Camera camera(glm::vec3(0.0f, 5.2f, 15.0f));
Camera camera(glm::vec3(cx, cy, cz));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 10.0, eyeZ = 1.0;
float lookAtX = 0.0, lookAtY = 10.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);
SpotLight spotlight(
    0.0f, 1.0f, -3.0f,
    0.0f, -1.0f, 0.0f,
    .0f, .0f, .0f,
    .0f, .0f, .0f,
    0.0f, 0.0f, 0.0f,
    30.0f,
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f //k_q
);

// Sun position and direction for directional light
glm::vec3 sunPosition = glm::vec3(80.0f, 90.0f, -80.0f);
glm::vec3 sunDirection = glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - sunPosition);

DirectionLight directlight(
    sunDirection.x, sunDirection.y, sunDirection.z,
    0.35f, 0.35f, 0.32f,   // ambient (warm)
    0.95f, 0.90f, 0.75f,   // diffuse (warm golden)
    1.0f, 0.95f, 0.85f     // specular
);

// Interior point lights for barrack - simulates tube light emission from ceiling
// Barrack is at (-45, 0.1, -40) and scaled 0.55x
// Tube lights inside are at local coords: left(-5, 2.75, -9.2), center(0, 2.75, -9.2), right(5, 2.75, -9.2)
// After scaling 0.55x: left(-2.75, 1.5, -5.06), center(0, 1.5, -5.06), right(2.75, 1.5, -5.06)
// Then translate to world: +barrack offset (-45, 0.1, -40)
TubeLight tubeLightLeft(
    -47.75f, 1.6f, -45.06f,  // left tube light position (world coordinates, adjusted for barrack position & scale)
    1.0f, 1.0f, 0.95f,       // ambient (very bright)
    1.2f, 1.2f, 1.1f,        // diffuse (bright)
    1.0f, 1.0f, 0.95f,       // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    21      // light number
);

TubeLight tubeLightRight(
    -42.25f, 1.6f, -45.06f,  // right tube light position (world coordinates, adjusted for barrack position & scale)
    1.0f, 1.0f, 0.95f,
    1.2f, 1.2f, 1.1f,
    1.0f, 1.0f, 0.95f,
    1.0f,
    0.09f,
    0.032f,
    22      // light number
);

TubeLight tubeLightCenter(
    -45.0f, 1.6f, -45.06f,   // center tube light position (world coordinates, adjusted for barrack position & scale)
    0.6f, 0.6f, 0.55f,       // further reduced ambient
    0.8f, 0.8f, 0.75f,       // further reduced diffuse (was 1.0f, 1.0f, 0.95f)
    0.6f, 0.6f, 0.55f,       // further reduced specular
    0.8f,   // further reduced k_c (was 1.0f)
    0.09f,  // adjusted k_l
    0.032f, // adjusted k_q
    23      // light number
);

// positions of the point lights - covering entire base camp including 4 corners
glm::vec3 pointLightPositions[] = {
    // Northeast corner area (60, -60)
    glm::vec3(50.0f,  20.0f,  -50.0f),
    glm::vec3(60.0f,  20.0f,  -60.0f),
    glm::vec3(70.0f,  20.0f,  -50.0f),
    // Northwest corner area (-60, -60)
    glm::vec3(-50.0f,  20.0f,  -50.0f),
    glm::vec3(-60.0f,  20.0f,  -60.0f),
    glm::vec3(-70.0f,  20.0f,  -50.0f),
    // Southwest corner area (-60, 60)
    glm::vec3(-50.0f,  20.0f,  50.0f),
    glm::vec3(-60.0f,  20.0f,  60.0f),
    glm::vec3(-70.0f,  20.0f,  50.0f),
    // Southeast corner area (60, 60)
    glm::vec3(50.0f,  20.0f,  50.0f),
    glm::vec3(60.0f,  20.0f,  60.0f),
    glm::vec3(70.0f,  20.0f,  50.0f),
    // Center area lights
    glm::vec3(0.0f,  20.0f,  0.0f),
    glm::vec3(20.0f,  20.0f,  0.0f),
    glm::vec3(-20.0f,  20.0f,  0.0f),
    glm::vec3(0.0f,  20.0f,  20.0f),
    glm::vec3(0.0f,  20.0f,  -20.0f),
    glm::vec3(20.0f,  20.0f,  20.0f),
    glm::vec3(-20.0f,  20.0f,  20.0f),
    glm::vec3(20.0f,  20.0f,  -20.0f),
    glm::vec3(-20.0f,  20.0f,  -20.0f),

};
PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);
PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);
PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    4       // light number
);

PointLight pointlight5(

    pointLightPositions[4].x, pointLightPositions[4].y, pointLightPositions[4].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    5       // light number
);
PointLight pointlight6(

    pointLightPositions[5].x, pointLightPositions[5].y, pointLightPositions[5].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    6       // light number
);

PointLight pointlight7(

    pointLightPositions[6].x, pointLightPositions[6].y, pointLightPositions[6].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    7       // light number
);

PointLight pointlight8(

    pointLightPositions[7].x, pointLightPositions[7].y, pointLightPositions[7].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    8      // light number
);

PointLight pointlight9(

    pointLightPositions[8].x, pointLightPositions[8].y, pointLightPositions[8].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    9      // light number
);
PointLight pointlight10(

    pointLightPositions[9].x, pointLightPositions[9].y, pointLightPositions[9].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    10      // light number
);
PointLight pointlight11(

    pointLightPositions[10].x, pointLightPositions[10].y, pointLightPositions[10].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    11      // light number
);
PointLight pointlight12(

    pointLightPositions[11].x, pointLightPositions[11].y, pointLightPositions[11].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    12      // light number
);

PointLight pointlight13(

    pointLightPositions[12].x, pointLightPositions[12].y, pointLightPositions[12].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    13      // light number
);
PointLight pointlight14(

    pointLightPositions[13].x, pointLightPositions[13].y, pointLightPositions[13].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    14      // light number
);
PointLight pointlight15(

    pointLightPositions[14].x, pointLightPositions[14].y, pointLightPositions[14].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    15     // light number
);
PointLight pointlight16(

    pointLightPositions[15].x, pointLightPositions[15].y, pointLightPositions[15].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    16      // light number
);
PointLight pointlight17(

    pointLightPositions[16].x, pointLightPositions[16].y, pointLightPositions[16].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    17      // light number
);
PointLight pointlight18(

    pointLightPositions[17].x, pointLightPositions[17].y, pointLightPositions[17].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    18     // light number
);

PointLight pointlight19(

    pointLightPositions[18].x, pointLightPositions[18].y, pointLightPositions[18].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    19      // light number
);

PointLight pointlight20(

    pointLightPositions[19].x, pointLightPositions[19].y, pointLightPositions[19].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    20      // light number
);


// light settings
bool pointLightOn = true;
bool directionLighton = true;
bool spotlighton = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;
bool sunlightToggle = true;  // NEW: Toggle for sun/directional light
bool garageLampsOn = true;   // Toggle for garage lamps
bool kGarageLampsPressed = false;

// Ground texture settings
int groundRenderMode = 1;  // 0=color, 1=texture, 2=blend
float groundTextureTiling = 4.0f;  // Scale factor for texture repetition (4x smaller tiles)
glm::vec3 groundColor = glm::vec3(0.8f, 0.8f, 0.7f);  // Sandy color
bool kGroundPressed = false;

bool onTrack = true;

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;
float speed = 0.003;



float r_z = -5.0;

float car_angle = 0.0;

float keyLiftTime = 0.0;


float car_t;

bool gameOver = false;

float score_bx = 0.0f;
float score_bz = -5.8f;


bool cameraShake = false;
float shakeTime = 0.0f;

int r_type = 1;

int viewMode = true;
bool goTank = false;
bool kache = true;
bool dure = false;

// ===== BIRD'S EYE VIEW =====
int birdEyeMode = 0;  // 0=off, 1=wide view, 2=close shot, 3=very close shot
bool kBirdEyePressed = false;

// ===== JEEP CONTROL SYSTEM =====
struct JeepState {
    glm::vec3 pos;
    float yawDeg = 0.0f;

    bool doorTargetOpen = false;
    bool windowTargetOpen = false;

    float doorAngleDeg = 0.0f;
    float windowDrop = 0.0f;
    float wheelSpinDeg = 0.0f;

    bool headlightsOn = false;
    int camMode = 0; // 0=back, 1=front, 2=left, 3=right
};

JeepState jeepA{ glm::vec3(-24.6f, 1.3f, 9.0f), 0.0f };
JeepState jeepB{ glm::vec3(-19.4f, 1.3f, 9.0f), 0.0f };

// Garage Lamp Point Lights - 2 lamps per garage (4 total)
// These light downward from the lamp fixtures
PointLight garageLamp1(
    0.0f, 0.0f, 0.0f,      // position (updated from garage platform)
    0.2f, 0.2f, 0.15f,     // ambient (warm yellowish)
    0.9f, 0.85f, 0.5f,     // diffuse (warm golden yellow)
    0.8f, 0.8f, 0.6f,      // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    24      // light number (after other 20 base + 3 tube lights)
);

PointLight garageLamp2(
    0.0f, 0.0f, 0.0f,      // position (updated from garage platform)
    0.2f, 0.2f, 0.15f,     // ambient
    0.9f, 0.85f, 0.5f,     // diffuse
    0.8f, 0.8f, 0.6f,      // specular
    1.0f,
    0.09f,
    0.032f,
    25      // light number
);

PointLight garageLamp3(
    0.0f, 0.0f, 0.0f,      // position (updated from garage platform)
    0.2f, 0.2f, 0.15f,     // ambient
    0.9f, 0.85f, 0.5f,     // diffuse
    0.8f, 0.8f, 0.6f,      // specular
    1.0f,
    0.09f,
    0.032f,
    26      // light number
);

PointLight garageLamp4(
    0.0f, 0.0f, 0.0f,      // position (updated from garage platform)
    0.2f, 0.2f, 0.15f,     // ambient
    0.9f, 0.85f, 0.5f,     // diffuse
    0.8f, 0.8f, 0.6f,      // specular
    1.0f,
    0.09f,
    0.032f,
    27      // light number
);

// Headlight spotlights for jeeps
SpotLight jeepAHeadlightL(
    0.0f, 0.0f, 0.0f,  // position (updated each frame)
    0.0f, 0.0f, 0.0f,  // direction (updated each frame)
    0.0f, 0.0f, 0.0f,  // ambient
    0.0f, 0.0f, 0.0f,  // diffuse (will be enabled when headlights on)
    0.0f, 0.0f, 0.0f,  // specular
    20.0f,             // cutoff angle
    1.0f,              // k_c
    0.09f,             // k_l
    0.032f             // k_q
);

SpotLight jeepAHeadlightR(
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    20.0f,
    1.0f,
    0.09f,
    0.032f
);

SpotLight jeepBHeadlightL(
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    20.0f,
    1.0f,
    0.09f,
    0.032f
);

SpotLight jeepBHeadlightR(
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    20.0f,
    1.0f,
    0.09f,
    0.032f
);

int selectedJeep = -1;
bool followSelectedJeep = true;

// ===== TRUCK CONTROL SYSTEM =====
struct TruckState {
    glm::vec3 pos;
    float yawDeg = 0.0f;

    float wheelSpinDeg = 0.0f;
    int camMode = 0; // 0=back, 1=front, 2=left, 3=right
};

TruckState truckState{ glm::vec3(-50.0f, 1.3f, 10.0f), 0.0f };

int selectedTruck = -1;

// Truck render mode toggle
Truck* gTruck = nullptr;
bool kTruckPressed = false;

// Boundary gate control
MilitaryBoundary* gBoundary = nullptr;

// Building control
Build* gBuild = nullptr;

static float clampf(float v, float lo, float hi) {
    return (v < lo) ? lo : (v > hi) ? hi : v;
}

glm::vec3 jeepForward(float yawDeg) {
    float r = glm::radians(yawDeg);
    return glm::normalize(glm::vec3(sinf(r), 0.0f, -cosf(r)));
}

void updateJeepAnimation(JeepState& j, float dt) {
    float doorTarget = j.doorTargetOpen ? 70.0f : 0.0f;
    float windowTarget = j.windowTargetOpen ? 0.35f : 0.0f;

    float doorSpeed = 120.0f;
    float winSpeed  = 1.2f;

    if (j.doorAngleDeg < doorTarget) j.doorAngleDeg = std::min(j.doorAngleDeg + doorSpeed * dt, doorTarget);
    if (j.doorAngleDeg > doorTarget) j.doorAngleDeg = std::max(j.doorAngleDeg - doorSpeed * dt, doorTarget);

    if (j.windowDrop < windowTarget) j.windowDrop = std::min(j.windowDrop + winSpeed * dt, windowTarget);
    if (j.windowDrop > windowTarget) j.windowDrop = std::max(j.windowDrop - winSpeed * dt, windowTarget);
}

void updateJeepHeadlights(JeepState& jeep, SpotLight& headlightL, SpotLight& headlightR) {
// Forward direction of the jeep
glm::vec3 forward = jeepForward(jeep.yawDeg);
glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
    
// Headlight positions (on the front of the jeep)
float headlightY = jeep.pos.y + 0.47f;  // height of headlights
float headlightForwardOffset = 1.2f;    // full distance in front (at the front bumper)
float headlightForwardOffsetMin = 0.12f; // 10% forward position (0.1 * 1.2)
    
// Gradually move headlight forward from 10% to full position
float blendFactor = (lastFrame * 0.5f > 1.0f) ? 1.0f : (lastFrame * 0.5f);
float currentOffset = headlightForwardOffsetMin + (headlightForwardOffset - headlightForwardOffsetMin) * blendFactor;
    
glm::vec3 headlightBasePosL = jeep.pos + forward * currentOffset + right * (-0.6f) + glm::vec3(0, headlightY - jeep.pos.y, 0);
glm::vec3 headlightBasePosR = jeep.pos + forward * currentOffset + right * (0.6f) + glm::vec3(0, headlightY - jeep.pos.y, 0);
    
headlightL.position = headlightBasePosL;
headlightL.direction = forward;
headlightR.position = headlightBasePosR;
headlightR.direction = forward;
    
    // Set light colors based on headlights on/off
    if (jeep.headlightsOn) {
        // Bright yellow-white light when on
        headlightL.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
        headlightL.diffuse = glm::vec3(1.0f, 0.95f, 0.8f);
        headlightL.specular = glm::vec3(1.0f, 1.0f, 1.0f);
        
        headlightR.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
        headlightR.diffuse = glm::vec3(1.0f, 0.95f, 0.8f);
        headlightR.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    } else {
        // Dark/off when lights are off
        headlightL.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
        headlightL.diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
        headlightL.specular = glm::vec3(0.0f, 0.0f, 0.0f);
        
        headlightR.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
        headlightR.diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
        headlightR.specular = glm::vec3(0.0f, 0.0f, 0.0f);
    }
}

JeepState* currentSelectedJeep() {
    if (selectedJeep == 0) return &jeepA;
    if (selectedJeep == 1) return &jeepB;
    return nullptr;
}

TruckState* currentSelectedTruck() {
    if (selectedTruck == 0) return &truckState;
    return nullptr;
}

struct AABB2D {
    float minX, maxX;
    float minZ, maxZ;
};

bool intersects2D(const AABB2D& a, const AABB2D& b)
{
    return !(a.maxX < b.minX || a.minX > b.maxX ||
             a.maxZ < b.minZ || a.minZ > b.maxZ);
}

AABB2D makeJeepBox(const glm::vec3& pos)
{
    const float halfW = 1.10f;
    const float halfL = 2.00f;

    return {
        pos.x - halfW, pos.x + halfW,
        pos.z - halfL, pos.z + halfL
    };
}

AABB2D makeTruckBox(const glm::vec3& pos)
{
    const float halfW = 1.42f;  // Truck is wider than jeep
    const float halfL = 2.65f;  // Truck is longer than jeep

    return {
        pos.x - halfW, pos.x + halfW,
        pos.z - halfL, pos.z + halfL
    };
}

std::vector<AABB2D> buildObstacleBoxes()
{
    std::vector<AABB2D> obs;

    // ===== GARAGE PLATFORM 1 (JEEP) =====
    // Position: -22, 10; dimensions: width 14, depth 18
    // Jeeps start at z=9, so we only block near the rear entrance (z < 1) and rear wall
    // left wall (only blocking rear section)
    obs.push_back({ -29.f, -28.f, -5.f, 1.f });
    // right wall (only blocking rear section)
    obs.push_back({ -15.f, -14.f, -5.f, 1.f });
    // back wall
    obs.push_back({ -29.f, -14.f, -5.f, -4.f });

    // ===== GARAGE PLATFORM 2 (TRUCK) =====
    // Position: -50, 10; dimensions: width 16, depth 20
    // left wall (only blocking rear section)
    obs.push_back({ -58.f, -57.f, -2.f, 2.f });
    // right wall (only blocking rear section)
    obs.push_back({ -42.f, -41.f, -2.f, 2.f });
    // back wall
    obs.push_back({ -58.f, -42.f, -2.f, -1.f });

    // ===== BARRACK 1 =====
    // Position: X = 40, Z = -40; scaled 0.66 * 1.3 = 0.858
    // Original size ~8x8, scaled = ~6.9x6.9
    obs.push_back({ 36.0f, 44.0f, -43.0f, -37.0f });

    // ===== BARRACK 2 =====
    // Position: X = 40, Z = -10; scaled 0.66 * 1.3 = 0.858
    obs.push_back({ 36.0f, 44.0f, -13.0f, -7.0f });

    // ===== BUILD OBJECT =====
    // Position: (-30, GROUND_HEIGHT, -30); width 14, depth 18
    obs.push_back({ -37.0f, -23.0f, -39.0f, -21.0f });

    // ===== MILITARY BOUNDARY WALLS =====
    // Boundary center at (0, 0); boundaryW=160, boundaryL=150
    // Outer walls with thickness ~0.85
    // Left wall: x = -80
    obs.push_back({ -81.0f, -79.0f, -75.0f, 75.0f });
    // Right wall: x = 80
    obs.push_back({ 79.0f, 81.0f, -75.0f, 75.0f });
    // Front wall: z = -75
    obs.push_back({ -80.0f, 80.0f, -76.0f, -74.0f });
    // Back wall: z = 75
    obs.push_back({ -80.0f, 80.0f, 74.0f, 76.0f });

    // ===== WATCHTOWERS =====
    // Northeast corner
    obs.push_back({ 57.5f, 62.5f, -62.5f, -57.5f });
    // Northwest corner
    obs.push_back({ -62.5f, -57.5f, -62.5f, -57.5f });
    // Southwest corner
    obs.push_back({ -62.5f, -57.5f, 57.5f, 62.5f });
    // Southeast corner
    obs.push_back({ 57.5f, 62.5f, 57.5f, 62.5f });

    // ===== TANK (moving) =====
    obs.push_back({ 18.0f, 28.0f, -8.0f, 8.0f });

    // ===== RADAR OFFICE =====
    // Radar base at (40, 0, 22) with scale ~9x9
    obs.push_back({ 35.5f, 44.5f, 17.5f, 26.5f });

    // ===== CAMPFIRE =====
    // Position: (30, GROUND_HEIGHT, -25); scaled 2.5x2.5x2.5
    // Original size ~1 unit, scaled = ~2.5 units
    obs.push_back({ 28.0f, 32.0f, -27.0f, -23.0f });

    // ===== TREE BLOCK 1 =====
    // Position: (-15, 0, 3); scaled 1.2*1.3 = 1.56
    obs.push_back({ -17.0f, -13.0f, 1.0f, 5.0f });

    // ===== TREE BLOCKS (original) =====
    obs.push_back({ -12.0f, -8.0f, 20.0f, 26.0f });
    obs.push_back({ 8.0f, 12.0f, 26.0f, 32.0f });
    obs.push_back({ -26.0f, -22.0f, 12.0f, 18.0f });

    // ===== MAIN TENT =====
    // Position: (30, 0, 10); scaled 0.015x0.015x0.015 (very small)
    // Original ~20x20, scaled = ~0.3x0.3 (minimal collision)
    obs.push_back({ 29.85f, 30.15f, 9.85f, 10.15f });

    // NOTE: ROAD is NOT blocked - vehicles can drive on it!
    // Road position: (0, GROUND_HEIGHT + 0.01, 35)
    // Road dimensions: width 16, length 260
    // Vehicles are FREE to drive on the road

    return obs;
}

// Separate collision check for truck vs jeep (optional, currently disabled)
bool truckCollidesWith(const glm::vec3& truckNextPos, const JeepState& jeep)
{
    // DISABLED: Trucks and jeeps can share space
    // Uncomment below to enable truck-jeep collision
    // AABB2D truckBox = makeTruckBox(truckNextPos);
    // AABB2D jeepBox = makeJeepBox(jeep.pos);
    // return intersects2D(truckBox, jeepBox);
    return false;
}

bool collidesAtPosition(const glm::vec3& nextPos, const JeepState* self)
{
    AABB2D jeepBox = makeJeepBox(nextPos);

    static std::vector<AABB2D> obstacles = buildObstacleBoxes();
    for (const auto& o : obstacles) {
        if (intersects2D(jeepBox, o)) return true;
    }

    // JEEP-TO-JEEP COLLISION: Enable collision between jeeps
    const JeepState* other = nullptr;
    if (self == &jeepA) other = &jeepB;
    else if (self == &jeepB) other = &jeepA;

    if (other) {
        if (intersects2D(jeepBox, makeJeepBox(other->pos))) return true;
    }

    // JEEP-TO-TRUCK COLLISION: DISABLED - Jeeps and trucks can share space
    // Uncomment below to prevent jeeps from driving through trucks
    // AABB2D truckBox = makeTruckBox(truckState.pos);
    // if (intersects2D(jeepBox, truckBox)) return true;

    return false;
}

bool truckCollidesAtPosition(const glm::vec3& nextPos)
{
    AABB2D truckBox = makeTruckBox(nextPos);

    static std::vector<AABB2D> obstacles = buildObstacleBoxes();
    for (const auto& o : obstacles) {
        if (intersects2D(truckBox, o)) return true;
    }

    // TRUCK-TO-JEEP COLLISION: DISABLED - Trucks and jeeps can share space
    // Uncomment below to prevent trucks from driving through jeeps
    // AABB2D jeepABox = makeJeepBox(jeepA.pos);
    // AABB2D jeepBBox = makeJeepBox(jeepB.pos);
    // if (intersects2D(truckBox, jeepABox)) return true;
    // if (intersects2D(truckBox, jeepBBox)) return true;

    return false;
}

float getSurfaceYAt(float x, float z)
{
    // default ground level
    float y = GROUND_HEIGHT;

    // ===== GARAGE PLATFORM 1 HEIGHT SYSTEM =====
    const float garageCx = -22.0f;
    const float garageCz = 10.0f;
    const float garageHalfW = 7.0f;
    const float garageHalfD = 9.0f;
    const float platformTopY = GROUND_HEIGHT + 1.2f;

    // X range of garage platform
    bool insideGarageX = (x >= garageCx - garageHalfW && x <= garageCx + garageHalfW);

    // flat platform area
    bool onPlatformFlat =
        insideGarageX &&
        z >= (garageCz - garageHalfD) &&
        z <= (garageCz + garageHalfD - 2.5f);

    if (onPlatformFlat) {
        return platformTopY;
    }

    // front ramp area: smoothly connect platform to ground
    float rampStartZ = garageCz + garageHalfD - 2.5f;
    float rampEndZ   = garageCz + garageHalfD + 2.0f;

    bool onRamp =
        insideGarageX &&
        z > rampStartZ &&
        z < rampEndZ;

    if (onRamp) {
        float t = (z - rampStartZ) / (rampEndZ - rampStartZ);
        return platformTopY * (1.0f - t) + GROUND_HEIGHT * t;
    }

    // ===== GARAGE PLATFORM 2 HEIGHT SYSTEM =====
    const float truckGarageCx = -50.0f;
    const float truckGarageCz = 10.0f;
    const float truckGarageHalfW = 8.0f;
    const float truckGarageHalfD = 10.0f;

    bool insideTruckGarageX = (x >= truckGarageCx - truckGarageHalfW && x <= truckGarageCx + truckGarageHalfW);

    bool onTruckPlatformFlat =
        insideTruckGarageX &&
        z >= (truckGarageCz - truckGarageHalfD) &&
        z <= (truckGarageCz + truckGarageHalfD - 2.5f);

    if (onTruckPlatformFlat) {
        return platformTopY;
    }

    float truckRampStartZ = truckGarageCz + truckGarageHalfD - 2.5f;
    float truckRampEndZ   = truckGarageCz + truckGarageHalfD + 2.0f;

    bool onTruckRamp =
        insideTruckGarageX &&
        z > truckRampStartZ &&
        z < truckRampEndZ;

    if (onTruckRamp) {
        float t = (z - truckRampStartZ) / (truckRampEndZ - truckRampStartZ);
        return platformTopY * (1.0f - t) + GROUND_HEIGHT * t;
    }

    return y;
}




void drawStair(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float baseR)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix;

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.5f, 0.0f, baseR + 3.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25f, 10.5f, 0.25f));
    glm::mat4 modelForStairs = model * translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, modelForStairs, 0.251f, 0.251f, 0.251f);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.5f, 0.0f, baseR + 3.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25f, 10.5f, 0.25f));
    modelForStairs = model * translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, modelForStairs, 0.251f, 0.251f, 0.251f);

    float stairH = 0.45f;
    for (int i = 0;i <= 7;i++)
    {
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.75f, stairH, baseR + 3.5f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.75f, 0.5, 0.25f));
        modelForStairs = model * translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, modelForStairs, 0.439f, 0.439f, 0.439f);

        stairH += 1.5f;
    }

}

void drawradarbase(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, Shape& radarbase)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(40.0f, 4.0f, 26.375f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.75f, 1.0f, 1.0f));
    glm::mat4 modelForRadarbase = model * translateMatrix * scaleMatrix;
    radarbase.drawTexture(lightingShader, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, modelForRadarbase);

}



int main()
{
    // Print controls to console
    std::cout << "========================================" << std::endl;
    std::cout << "   MILITARY BASE CAMP - CONTROLS" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    std::cout << "  MOVEMENT (Free Camera Mode):" << std::endl;
    std::cout << "    W - Move Forward" << std::endl;
    std::cout << "    S - Move Backward" << std::endl;
    std::cout << "    A - Move Left" << std::endl;
    std::cout << "    D - Move Right" << std::endl;
    std::cout << std::endl;
    std::cout << "  CAMERA CONTROL (Free Camera Mode):" << std::endl;
    std::cout << "    X - Pitch Up" << std::endl;
    std::cout << "    C - Pitch Down" << std::endl;
    std::cout << "    Q - Yaw Left" << std::endl;
    std::cout << "    E - Yaw Right" << std::endl;
    std::cout << "    Mouse Movement - Look Around" << std::endl;
    std::cout << "    Scroll Wheel - Zoom In/Out" << std::endl;
    std::cout << std::endl;
    std::cout << "  VEHICLE SELECTION:" << std::endl;
    std::cout << "    J - Select/Cycle Jeeps (None -> Jeep A -> Jeep B -> None)" << std::endl;
    std::cout << "    P - Select/Deselect Truck" << std::endl;
    std::cout << std::endl;
    std::cout << "  VEHICLE MOVEMENT (When Vehicle Selected):" << std::endl;
    std::cout << "    UP ARROW - Move Forward" << std::endl;
    std::cout << "    DOWN ARROW - Move Backward" << std::endl;
    std::cout << "    LEFT ARROW - Turn Left" << std::endl;
    std::cout << "    RIGHT ARROW - Turn Right" << std::endl;
    std::cout << std::endl;
    std::cout << "  JEEP ACTIONS (When Jeep Selected):" << std::endl;
    std::cout << "    V - Cycle Vehicle Camera View (Back -> Front -> Left -> Right -> Back)" << std::endl;
    std::cout << "    D - Toggle Door Open/Close" << std::endl;
    std::cout << "    W - Toggle Window Open/Close" << std::endl;
    std::cout << "    L - Toggle Headlights On/Off" << std::endl;
    std::cout << std::endl;
    std::cout << "  TRUCK ACTIONS (When Truck Selected):" << std::endl;
    std::cout << "    V - Cycle Vehicle Camera View (Back -> Front -> Left -> Right -> Back)" << std::endl;
    std::cout << "    K - Cycle Truck Render Mode (Color -> Texture -> Blended -> Color)" << std::endl;
    std::cout << std::endl;
    std::cout << "  GROUND ACTIONS:" << std::endl;
    std::cout << "    M - Cycle Ground Render Mode (Color -> Texture -> Blended -> Color)" << std::endl;
    std::cout << std::endl;
    std::cout << "  BUILDING/STRUCTURE ACTIONS:" << std::endl;
    std::cout << "    T - Toggle Tank Movement (Starts/Stops)" << std::endl;
    std::cout << "    R - Toggle Radar Rotation On/Off" << std::endl;
    std::cout << "    B - Toggle Barrack Door Open/Close" << std::endl;
    std::cout << "    U - Toggle Boundary Gate Open/Close" << std::endl;
    std::cout << "    G - Toggle Building Door Open/Close" << std::endl;
    std::cout << "    H - Toggle Left Window of Building Open/Close" << std::endl;
    std::cout << "    I - Toggle Right Window of Building Open/Close" << std::endl;
    std::cout << std::endl;
    std::cout << "  LIGHTING CONTROL:" << std::endl;
    std::cout << "    4 - Toggle Ambient Light On/Off" << std::endl;
    std::cout << "    5 - Toggle Diffuse Light On/Off" << std::endl;
    std::cout << "    6 - Toggle Specular Light On/Off" << std::endl;
    std::cout << "    7 - Toggle Sunlight (Day/Night Mode)" << std::endl;
    std::cout << std::endl;
    std::cout << "  VIEW MODES:" << std::endl;
    std::cout << "    O - Cycle Bird's Eye View:" << std::endl;
    std::cout << "        OFF (Normal View)" << std::endl;
    std::cout << "        -> Wide View (Entire Base Camp)" << std::endl;
    std::cout << "        -> Close Shot (Medium Zoom)" << std::endl;
    std::cout << "        -> Very Close Shot (Maximum Zoom)" << std::endl;
    std::cout << "        -> Back to Normal View" << std::endl;
    std::cout << std::endl;
    std::cout << "  GENERAL:" << std::endl;
    std::cout << "    ESC - Exit Application" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

        
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    //Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float cube_vertices[] = {
        // positions      // normals
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };

    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Sphere sphere = Sphere();
    Sphere point = Sphere();
    point.setRadius(0.001f);
    float inc2 = 0.1f;
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");
    

    string diffuseMapPath = "ff.png";
    string specularMapPath = "ff.png";
    unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    Cube cube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    
    string diffuseMapPath3 = "OIP.jpeg";
    string specularMapPath3 = "OIP.jpeg";
    unsigned int diffMap3 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap3 = loadTexture(specularMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    Cube cube3 = Cube(diffMap3, specMap3, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPath11 = "metal.jpeg";
    string specularMapPath11 = "metal.jpeg";
    unsigned int diffMap11 = loadTexture(diffuseMapPath11.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap11 = loadTexture(specularMapPath11.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    Cube cube11 = Cube(diffMap11, specMap11, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    string TreediffuseMapPath3 = "Treeroot.png";
    string TreespecularMapPath3 = "Treeroot.png";
    unsigned int TreerootdiffMap3 = loadTexture(TreediffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int TreerootspecMap3 = loadTexture(TreespecularMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    Cube treeeRoot = Cube(TreerootdiffMap3, TreerootspecMap3, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string TreeleafdiffuseMapPath3 = "TreeLeaf.png";
    string TreeleafspecularMapPath3 = "TreeLeaf.png";
    unsigned int TreeleafdiffMap3 = loadTexture(TreeleafdiffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int TreeleafspecMap3 = loadTexture(TreeleafspecularMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    string barrackDiffusePath = "wood.png";
    string barrackSpecularPath = "wood.png";

    unsigned int barrackDiff = loadTexture(barrackDiffusePath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int barrackSpec = loadTexture(barrackSpecularPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    // Load pebble texture (for campfire)
    string pebblePath = "pebble.png";
    unsigned int pebbleDiff = loadTexture(pebblePath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int pebbleSpec = pebbleDiff;

    // Load jeep textures
    string jeepDiffusePath = "jeep.png";
    unsigned int jeepDiff = loadTexture(jeepDiffusePath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int jeepSpec = jeepDiff;

    // Load wheel face texture
    string wheelFacePath = "wheel.png";
    unsigned int wheelFaceDiff = loadTexture(wheelFacePath.c_str(), GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int wheelFaceSpec = wheelFaceDiff;

    // Load garage platform textures
    string wallTexPath = "wall.png";
    string floorTexPath = "floor.png";
    string gateTexPath = "gate.png";
    unsigned int wallDiff = loadTexture(wallTexPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int floorDiff = loadTexture(floorTexPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int gateDiff = loadTexture(gateTexPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    // Load build textures for Build class
    string buildWallPath = "build.png";
    string buildDoorPath = "wood.png";
    unsigned int buildWallDiff = loadTexture(buildWallPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int buildWallSpec = buildWallDiff;
    unsigned int buildDoorDiff = loadTexture(buildDoorPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int buildDoorSpec = buildDoorDiff;

    // Load box texture for medical box
    string boxPath = "box.png";
    unsigned int boxDiff = loadTexture(boxPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int boxSpec = boxDiff;

    Cube woodCube = Cube(barrackDiff, barrackSpec, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube roofCube = Cube(barrackDiff, barrackSpec, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube garagePlatWall(wallDiff, wallDiff, 32.f, 0.f, 0.f, 1.f, 1.f);
    Cube garagePlatFloor(floorDiff, floorDiff, 32.f, 0.f, 0.f, 1.f, 1.f);
    Cube boundaryWall(floorDiff, floorDiff, 32.f, 0.f, 0.f, 1.f, 1.f);
    Cube gateCube(gateDiff, gateDiff, 32.f, 0.f, 0.f, 1.f, 1.f);
    Cube buildWallCube(buildWallDiff, buildWallSpec, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube buildFloorCube(floorDiff, floorDiff, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube buildDoorCube(buildDoorDiff, buildDoorSpec, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube boxCube(boxDiff, boxSpec, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Bush Leaf_Obj = Bush(TreeleafdiffMap3, TreeleafspecMap3, 32.0f);

    // Create pure black texture for tyre cylinder
    unsigned int blackTexture = createProceduralTexture(64, 64, 0, 0, 0);  // Pure black

    // STEP 2 — Create jeep objects
    Cube jeepBodyCube(jeepDiff, jeepDiff, 32.0f, 0, 0, 1, 1);
    
    // Body/detail cylinder if you still need it elsewhere
    Shape cylinderObj = Shape("Cylinder");
    cylinderObj.initializeCylinder(1.0f, 1.0f, 1.0f, 36, 18,
                                   jeepDiff, jeepDiff, jeepDiff);

    // Separate black tyre cylinder
    Shape wheelCylinder = Shape("Cylinder");
    wheelCylinder.initializeCylinder(1.0f, 1.0f, 1.0f, 48, 24,
                                     blackTexture, blackTexture, blackTexture);

    // Separate cylinder only for the front flat wheel face
    Shape wheelFaceCylinder = Shape("Cylinder");
    wheelFaceCylinder.initializeCylinder(1.0f, 1.0f, 1.0f, 48, 24,
                                         wheelFaceDiff, wheelFaceSpec, wheelFaceDiff);

    // Create the jeep using black tyre cylinder + textured face cylinder
    Jeep myJeep(jeepBodyCube, wheelCylinder, wheelFaceCylinder);
    
    // Create second jeep for side-by-side placement
    Jeep myJeep2(jeepBodyCube, wheelCylinder, wheelFaceCylinder);

    // ===== TRUCK SETUP =====
    // Load truck body texture
    string truckBodyPath = "truck.png";
    unsigned int truckBodyDiff = loadTexture(truckBodyPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int truckBodySpec = truckBodyDiff;
    Cube truckBodyCube(truckBodyDiff, truckBodySpec, 32.0f, 0, 0, 1, 1);

    // Create truck wheel cylinder (black tyre)
    Shape truckWheelCyl = Shape("Cylinder");
    truckWheelCyl.initializeCylinder(1.0f, 1.0f, 1.0f, 48, 24,
                                     blackTexture, blackTexture, blackTexture);

    // Load cargo cylinder texture (back.png)
    string truckCargoPath = "back.png";
    unsigned int truckCargoDiff = loadTexture(truckCargoPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int truckCargoSpec = truckCargoDiff;

    Shape truckCargoCyl = Shape("Cylinder");
    truckCargoCyl.initializeCylinder(1.0f, 1.0f, 1.0f, 36, 18,
                                     truckCargoDiff, truckCargoDiff, truckCargoDiff);

    // Create truck object with body cube, wheel cylinder, and cargo cylinder
    Truck myTruck(truckBodyCube, truckWheelCyl, truckCargoCyl);

    // Set global truck pointer for keyboard toggle
    gTruck = &myTruck;

    // Create cylinder for watchtowers
    Shape cyl = Shape("Cylinder");
    cyl.initializeCylinder(0.4f, 0.4f, 2.0f, 100, 36, TreerootspecMap3, TreerootspecMap3, TreerootspecMap3);

    // Create 4 WatchTower objects at the corners of the base camp
    // Northeast corner
    WatchTower watchTower1(cyl, woodCube, roofCube, glm::vec3(60.0f, 0.0f, -60.0f), 1.0f);
    // Northwest corner
    WatchTower watchTower2(cyl, woodCube, roofCube, glm::vec3(-60.0f, 0.0f, -60.0f), 1.0f);
    // Southwest corner
    WatchTower watchTower3(cyl, woodCube, roofCube, glm::vec3(-60.0f, 0.0f, 60.0f), 1.0f);
    // Southeast corner
    WatchTower watchTower4(cyl, woodCube, roofCube, glm::vec3(60.0f, 0.0f, 60.0f), 1.0f);

    // Create the garage platform 1 - positioned at (-40, 0, 0), rotated 90 degrees (for jeeps)
    GaragePlatform garagePlatform(
        garagePlatWall,
        garagePlatFloor,
        glm::vec3(-22.f, GROUND_HEIGHT, 10.f),  // world position
        14.f,   // width  (X)
        18.f,   // depth  (Z, front-to-back)
        7.f,    // interior height
        1.2f    // floor raise above ground
    );

    // Create garage platform 2 - positioned at (-40, 0, -10), rotated 90 degrees (for truck)
    GaragePlatform truckGaragePlatform(
        garagePlatWall,
        garagePlatFloor,
        glm::vec3(-50.f, GROUND_HEIGHT, 10.f),   // world position
        16.f,   // width  (X) - slightly wider for truck
        20.f,   // depth  (Z, front-to-back) - slightly longer for truck
        7.f,    // interior height
        1.2f    // floor raise above ground
    );

    // Create military boundary with gate
    MilitaryBoundary boundary(garagePlatFloor, gateCube, nullptr);
    gBoundary = &boundary;
    
    boundary.setPosition(glm::vec3(0.0f, GROUND_HEIGHT, 0.0f));
    boundary.boundaryW = 160.0f;
    boundary.boundaryL = 150.0f;
    boundary.wallH = 14.0f;
    boundary.wallT = 0.85f;
    boundary.gateOpening = 26.0f;
    boundary.gateH = 10.5f;
    boundary.gateT = 0.40f;
    boundary.maxGateAngle = 95.0f;

    // Create military road
    MilitaryRoad road(cube);
    road.roadW = 16.0f;
    road.roadL = 260.0f;
    road.setPosition(glm::vec3(0.0f, GROUND_HEIGHT + 0.01f, 35.0f));

    // Create Build object with textured walls, floor, and wooden door
    // Position at (-30, GROUND_HEIGHT, -25)
    Build myBuild(
        buildWallCube,
        buildFloorCube,
        buildDoorCube,
        glm::vec3(-30.f, GROUND_HEIGHT, -30.0f),  // position at (-30, GROUND_HEIGHT, -25)
        14.f,   // width
        18.f,   // depth
        6.5f,   // height
        0.15f   // floor thickness
    );
    gBuild = &myBuild;

    // Radar office - using existing sphere setup below

    stbi_set_flip_vertically_on_load(true);
    Shader modelShader("1.model_loading.vs", "1.model_loading.fs");

    Model Tank("Tank_model/scene.gltf");
    
    float tank_z = 0.0f;
    float tankInc = 0.1f;

    Model Main_Tent("tent_military/scene.gltf");

    Model Jeep("jeep/scene.gltf");

    Model Truck("truck/scene.gltf");

    Campfire campfire(pebbleDiff, pebbleSpec);

   

    //ourShader.use();
    //lightingShader.use();

    /*Cylinder watchTbase = Cylinder();
    Cylinder  radarbase = Cylinder();*/


    // render loop
    // -----------

    std::random_device rd;  // Seed for the random number engine
    std::mt19937 gen(rd());  // Mersenne Twister engine
    std::uniform_real_distribution<float> dis(-2.0f,2.0f);
    float randomValue = dis(gen);
    //cout << roadF(31.416f) << endl;


    //camera.Position = glm::vec3(0.0, 0.0, 50.0);
    while (!glfwWindowShouldClose(window))
    {
        
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // Update jeep animations
        updateJeepAnimation(jeepA, deltaTime);
        updateJeepAnimation(jeepB, deltaTime);

        // Update jeep headlights
        updateJeepHeadlights(jeepA, jeepAHeadlightL, jeepAHeadlightR);
        updateJeepHeadlights(jeepB, jeepBHeadlightL, jeepBHeadlightR);

        // Update boundary gate animation
        boundary.update(deltaTime);

        // Update building (door/window animation)
        myBuild.update(deltaTime);

        // Update garage lamp positions from garage platforms
        // Garage 1 lamps (left and right at front)
        garageLamp1.position = garagePlatform.lampLightPosFrontLeft();
        garageLamp2.position = garagePlatform.lampLightPosFrontRight();
        
        // Garage 2 lamps (left and right at front)
        garageLamp3.position = truckGaragePlatform.lampLightPosFrontLeft();
        garageLamp4.position = truckGaragePlatform.lampLightPosFrontRight();

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        

        //camera.ProcessKeyboard(BACKWARD, deltaTime);
        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        if (dure) {
            kache = false;
            cx = 30.0f;
            cy = 10.0f;
            cz = 0.0f;
        }
        if (kache) {
            dure = false;
            cx = 0.0f;
            cy = 5.2f;
            cz = 15.0f;
        }
        lightingShader.setVec3("viewPos", camera.Position);

        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        // point light 4
        pointlight4.setUpPointLight(lightingShader);
        pointlight5.setUpPointLight(lightingShader);
        pointlight6.setUpPointLight(lightingShader);
        pointlight7.setUpPointLight(lightingShader);
        pointlight8.setUpPointLight(lightingShader);
        pointlight9.setUpPointLight(lightingShader);
        pointlight10.setUpPointLight(lightingShader);
        pointlight11.setUpPointLight(lightingShader);
        pointlight12.setUpPointLight(lightingShader);
        pointlight13.setUpPointLight(lightingShader);
        pointlight14.setUpPointLight(lightingShader);
        pointlight15.setUpPointLight(lightingShader);
        pointlight16.setUpPointLight(lightingShader);
        pointlight17.setUpPointLight(lightingShader);
        pointlight18.setUpPointLight(lightingShader);
        pointlight19.setUpPointLight(lightingShader);
        pointlight20.setUpPointLight(lightingShader);


        directlight.setUpDirectionLight(lightingShader);
        
        // Interior tube lights for barrack
        tubeLightLeft.setUpPointLight(lightingShader);
        tubeLightRight.setUpPointLight(lightingShader);
        tubeLightCenter.setUpPointLight(lightingShader);

        // Garage lamps (4 lamps)
        if (garageLampsOn)
        {
            garageLamp1.setUpPointLight(lightingShader);
            garageLamp2.setUpPointLight(lightingShader);
            garageLamp3.setUpPointLight(lightingShader);
            garageLamp4.setUpPointLight(lightingShader);
        }

        spotlight.setUpSpotLight(lightingShader);
        
        // Jeep headlight spotlights - SET THEM IN ARRAY
        lightingShader.use();
        lightingShader.setVec3("spotLights[0].position", jeepAHeadlightL.position);
        lightingShader.setVec3("spotLights[0].direction", jeepAHeadlightL.direction);
        lightingShader.setVec3("spotLights[0].ambient", jeepAHeadlightL.ambient);
        lightingShader.setVec3("spotLights[0].diffuse", jeepAHeadlightL.diffuse);
        lightingShader.setVec3("spotLights[0].specular", jeepAHeadlightL.specular);
        lightingShader.setFloat("spotLights[0].cos_theta", jeepAHeadlightL.cos_theta);
        lightingShader.setFloat("spotLights[0].k_c", jeepAHeadlightL.k_c);
        lightingShader.setFloat("spotLights[0].k_l", jeepAHeadlightL.k_l);
        lightingShader.setFloat("spotLights[0].k_q", jeepAHeadlightL.k_q);
        
        lightingShader.setVec3("spotLights[1].position", jeepAHeadlightR.position);
        lightingShader.setVec3("spotLights[1].direction", jeepAHeadlightR.direction);
        lightingShader.setVec3("spotLights[1].ambient", jeepAHeadlightR.ambient);
        lightingShader.setVec3("spotLights[1].diffuse", jeepAHeadlightR.diffuse);
        lightingShader.setVec3("spotLights[1].specular", jeepAHeadlightR.specular);
        lightingShader.setFloat("spotLights[1].cos_theta", jeepAHeadlightR.cos_theta);
        lightingShader.setFloat("spotLights[1].k_c", jeepAHeadlightR.k_c);
        lightingShader.setFloat("spotLights[1].k_l", jeepAHeadlightR.k_l);
        lightingShader.setFloat("spotLights[1].k_q", jeepAHeadlightR.k_q);
        
        lightingShader.setVec3("spotLights[2].position", jeepBHeadlightL.position);
        lightingShader.setVec3("spotLights[2].direction", jeepBHeadlightL.direction);
        lightingShader.setVec3("spotLights[2].ambient", jeepBHeadlightL.ambient);
        lightingShader.setVec3("spotLights[2].diffuse", jeepBHeadlightL.diffuse);
        lightingShader.setVec3("spotLights[2].specular", jeepBHeadlightL.specular);
        lightingShader.setFloat("spotLights[2].cos_theta", jeepBHeadlightL.cos_theta);
        lightingShader.setFloat("spotLights[2].k_c", jeepBHeadlightL.k_c);
        lightingShader.setFloat("spotLights[2].k_l", jeepBHeadlightL.k_l);
        lightingShader.setFloat("spotLights[2].k_q", jeepBHeadlightL.k_q);
        
        lightingShader.setVec3("spotLights[3].position", jeepBHeadlightR.position);
        lightingShader.setVec3("spotLights[3].direction", jeepBHeadlightR.direction);
        lightingShader.setVec3("spotLights[3].ambient", jeepBHeadlightR.ambient);
        lightingShader.setVec3("spotLights[3].diffuse", jeepBHeadlightR.diffuse);
        lightingShader.setVec3("spotLights[3].specular", jeepBHeadlightR.specular);
        lightingShader.setFloat("spotLights[3].cos_theta", jeepBHeadlightR.cos_theta);
        lightingShader.setFloat("spotLights[3].k_c", jeepBHeadlightR.k_c);
        lightingShader.setFloat("spotLights[3].k_l", jeepBHeadlightR.k_l);
        lightingShader.setFloat("spotLights[3].k_q", jeepBHeadlightR.k_q);

        // activate shader
        lightingShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection;
        if (birdEyeMode > 0) {
            // Bird's eye view: orthographic projection with zoom levels
            float zoomRange;
            if (birdEyeMode == 1) {
                zoomRange = 120.0f;  // Wide view
            } else if (birdEyeMode == 2) {
                zoomRange = 60.0f;   // Close shot
            } else {
                zoomRange = 30.0f;   // Very close shot
            }
            projection = glm::ortho(-zoomRange, zoomRange, -zoomRange, zoomRange, 0.1f, 300.0f);
        } else {
            // Normal perspective view
            projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        }
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view;
        if (birdEyeMode > 0) {
            // Bird's eye view: camera looking straight down from above
            float cameraHeight;
            if (birdEyeMode == 1) {
                cameraHeight = 150.0f;  // Wide view
            } else if (birdEyeMode == 2) {
                cameraHeight = 80.0f;   // Close shot
            } else {
                cameraHeight = 40.0f;   // Very close shot
            }
            glm::vec3 birdEyePos = glm::vec3(0.0f, cameraHeight, 0.0f);
            glm::vec3 birdEyeTarget = glm::vec3(0.0f, 0.0f, 0.0f);
            glm::vec3 birdEyeUp = glm::vec3(0.0f, 0.0f, -1.0f);
            view = glm::lookAt(birdEyePos, birdEyeTarget, birdEyeUp);
        } else if (selectedJeep == -1 && selectedTruck == -1) {
            view = camera.GetViewMatrix();
        }
        else if (selectedJeep != -1) {
            JeepState& j = (selectedJeep == 0) ? jeepA : jeepB;
            glm::vec3 fwd = jeepForward(j.yawDeg);
            glm::vec3 right = glm::normalize(glm::cross(fwd, glm::vec3(0, 1, 0)));
            glm::vec3 target = j.pos + glm::vec3(0.0f, 2.0f, 0.0f);
            glm::vec3 camPos;

            switch (j.camMode) {
            case 0: // back
                camPos = j.pos - fwd * 8.0f + glm::vec3(0.0f, 4.5f, 0.0f);
                break;
            case 1: // front
                camPos = j.pos + fwd * 8.0f + glm::vec3(0.0f, 3.8f, 0.0f);
                break;
            case 2: // left
                camPos = j.pos - right * 8.0f + glm::vec3(0.0f, 3.8f, 0.0f);
                break;
            default: // right
                camPos = j.pos + right * 8.0f + glm::vec3(0.0f, 3.8f, 0.0f);
                break;
            }

            view = glm::lookAt(camPos, target, glm::vec3(0, 1, 0));
        }
        else if (selectedTruck != -1) {
            TruckState& t = truckState;
            glm::vec3 fwd = jeepForward(t.yawDeg);
            glm::vec3 right = glm::normalize(glm::cross(fwd, glm::vec3(0, 1, 0)));
            glm::vec3 target = t.pos + glm::vec3(0.0f, 2.5f, 0.0f);
            glm::vec3 camPos;

            switch (t.camMode) {
            case 0: // back
                camPos = t.pos - fwd * 10.0f + glm::vec3(0.0f, 5.0f, 0.0f);
                break;
            case 1: // front
                camPos = t.pos + fwd * 10.0f + glm::vec3(0.0f, 4.5f, 0.0f);
                break;
            case 2: // left
                camPos = t.pos - right * 10.0f + glm::vec3(0.0f, 4.5f, 0.0f);
                break;
            default: // right
                camPos = t.pos + right * 10.0f + glm::vec3(0.0f, 4.5f, 0.0f);
                break;
            }

            view = glm::lookAt(camPos, target, glm::vec3(0, 1, 0));
        }
        
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShader.setMat4("view", view);


        //cout << lookAtX << lookAtY << lookAtZ << endl;
        //cout << camera.Front.x <<"   "<< camera.Front.y << "   " << camera.Front.z << endl;


        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);


        //----------------radar-----------------------//

        //radar

        string diffuseMapPath2 = "radartex.jpg";
        string specularMapPath2 = "radartex.jpg";
        unsigned int diffMap2 = loadTexture(diffuseMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
        unsigned int specMap2 = loadTexture(specularMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

        // Light gray color for radar dish (not red!)
        SphereWithTexture radar = SphereWithTexture(2.0f, 144, 72, glm::vec3(0.92f, 0.92f, 0.92f), glm::vec3(0.92f, 0.92f, 0.92f), glm::vec3(0.6f, 0.6f, 0.6f), 64.0f, diffMap2, specMap2, 0.0f, 0.0f, 1.0f, 1.0f);


        //radar base

        float pob = 4.5f;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(40.0f, 0.0f, 22.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5 * pob, 2.0f, 2 * pob));
        glm::mat4 modelForRadar2 = model * translateMatrix * scaleMatrix;
        cube11.drawCubeWithTexture(lightingShaderWithTexture, modelForRadar2);
        //drawCube(cubeVAO, lightingShader, modelForRadar2, 0.251f, 0.251f, 0.251f);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(42.25f, 2.0f, 25.25f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5 * pob, 2.0f, 0.5 * pob));
        modelForRadar2 = model * translateMatrix * scaleMatrix;
        cube11.drawCubeWithTexture(lightingShaderWithTexture, modelForRadar2);
        //drawCube(cubeVAO, lightingShader, modelForRadar2, 0.251f, 0.251f, 0.251f);

        //wall around radar
        translateMatrix = glm::translate(identityMatrix, glm::vec3(49.0f, 0.0f, 22.00f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 6.0f, 2 * pob));
        modelForRadar2 = model * translateMatrix * scaleMatrix;
        cube3.drawCubeWithTexture(lightingShaderWithTexture, modelForRadar2);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(39.0f, 0.0f, 31.00f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(10.0f, 6.0f, 0.5f));
        modelForRadar2 = model * translateMatrix * scaleMatrix;
        cube3.drawCubeWithTexture(lightingShaderWithTexture, modelForRadar2);

        //radar
        translateMatrix = glm::translate(identityMatrix, glm::vec3(43.375f, 10.0f, 23.5f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(1.0, 0.0, 0.0));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(RadarRotateAngle), glm::vec3(0.0, 1.0, 0.0));
        glm::mat4 modelForRadar = translateMatrix * rotateYMatrix * rotateXMatrix * scaleMatrix;
        radar.drawSphereWithTexture(lightingShaderWithTexture, modelForRadar);
        if (RotateRadar) {
            RadarRotateAngle += 1.0f;
        }

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 1.0f));

        glm::mat4 modelForRadar3 = translateMatrix * scaleMatrix;
        drawradarbase(cubeVAO, lightingShaderWithTexture, modelForRadar3, cyl);


        //Tree
        glm::mat4 Tree_Scaling = glm::scale(identityMatrix, glm::vec3(1.2f * 1.3f, 1.2f * 1.3f, 1.2f * 1.3f));
        glm::mat4 Tree_positoning = glm::translate(identityMatrix, glm::vec3(-15.0f, 0.0f, 3.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f * 1.3f, 4.0f * 1.3f, 0.5f * 1.3f));
        glm::mat4 rootModel = scaleMatrix;
        // Draw fractal tree
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(4.0f * 1.3f, 4.0f * 1.3f, 4.0f * 1.3f));
        glm::mat4 rootModel2 = translate(identityMatrix, glm::vec3(0.0f, -0.75f, 0.0f)) * scaleMatrix;
        drawFractalTree(lightingShaderWithTexture, rootModel, 4, treeeRoot, Tree_positoning * Tree_Scaling, Leaf_Obj, rootModel2, cubeVAO); // Change depth as needed

        
        
        updateBarrackDoor();
       /* glm::mat4 barrackModel = glm::mat4(1.0f);
        barrackModel = glm::translate(barrackModel, glm::vec3(-8.0f, 0.0f, -22.0f));
        barrackModel = glm::scale(barrackModel, glm::vec3(1.0f, 1.0f, 1.0f));*/

        glm::mat4 barrackModel = glm::mat4(1.0f);
        // First barrack - POSITIONED FOR BUILD
        // Position: X = 40, Z = -40 (on right side of boundary)
        // Rotated 90° so door faces towards the road (positive Z direction)
        barrackModel = glm::translate(barrackModel, glm::vec3(40.0f, GROUND_HEIGHT, -40.0f));
        barrackModel = glm::rotate(barrackModel, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        barrackModel = glm::scale(barrackModel, glm::vec3(0.66f * 1.3f, 0.66f * 1.3f, 0.66f * 1.3f));

        drawBarrack(cubeVAO, lightingShader, lightingShaderWithTexture, barrackModel, woodCube, roofCube);

        // Second barrack - POSITIONED FOR BUILD
        // Position: X = 40, Z = -10 (20 units forward towards road)
        // Rotated 270° (or -90°) so door also faces road, but opposite direction
        glm::mat4 barrackModel2 = glm::mat4(1.0f);
        barrackModel2 = glm::translate(barrackModel2, glm::vec3(40.0f, GROUND_HEIGHT, -10.0f));
        barrackModel2 = glm::rotate(barrackModel2, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        barrackModel2 = glm::scale(barrackModel2, glm::vec3(0.66f * 1.3f, 0.66f * 1.3f, 0.66f * 1.3f));

        drawBarrack(cubeVAO, lightingShader, lightingShaderWithTexture, barrackModel2, woodCube, roofCube);

        glm::mat4 campfireModel = glm::mat4(1.0f);
        // Campfire IN FRONT OF BARRACKS DOORS
        // Barracks doors face towards positive Z (upward in world)
        // Position: X = 40 (centered), Z = -5 (in front of doors, between door area and middle)
        // Positioned where soldiers would gather in front of the barracks entrances
        campfireModel = glm::translate(campfireModel, glm::vec3(30.0f, GROUND_HEIGHT, -25.0f));
        campfireModel = glm::scale(campfireModel, glm::vec3(2.5f, 2.5f, 2.5f));

        //-------------------------Imported Mesh Model----------------------------//

        modelShader.use();
        // view/projection transformations
        glm::mat4 Mprojection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 Mview = camera.GetViewMatrix();
        modelShader.setMat4("projection", Mprojection);
        modelShader.setMat4("view", Mview);

        


        //-------------------tanks----------------------------
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, 0.25f, tank_z));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.0264f * 1.3f, 0.0264f * 1.3f, 0.0264f * 1.3f));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0, 0.0, 0.0));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
        model = translateMatrix * rotateXMatrix * scaleMatrix;
        modelShader.setMat4("model", model);
        Tank.Draw(modelShader);

        if (goTank)
        {
            tank_z += tankInc;
            if (tank_z > 20.0f)
            {
                tankInc *= -1;
            }

            if (tank_z < 0.0f)
            {
                tankInc *= -1;
                goTank = false;
            }
        }




        // REMOVED: Static tank at (19.0f, 0.25f, -23.0f)
        // translateMatrix = glm::translate(identityMatrix, glm::vec3(19.0f, 0.25f, -23.0f));
        // scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.0264f * 1.3f, 0.0264f * 1.3f, 0.0264f * 1.3f));
        // rotateXMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0, 0.0, 0.0));
        // model = translateMatrix * rotateXMatrix * scaleMatrix;
        // modelShader.setMat4("model", model);
        // Tank.Draw(modelShader);


        // REMOVED: Static tank at (-19.0f, 0.25f, -23.0f)
        // translateMatrix = glm::translate(identityMatrix, glm::vec3(-19.0f, 0.25f, -23.0f));
        // scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.0264f * 1.3f, 0.0264f * 1.3f, 0.0264f * 1.3f));
        // rotateXMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0, 0.0, 0.0));
        // model = translateMatrix * rotateXMatrix * scaleMatrix;
        // modelShader.setMat4("model", model);
        // Tank.Draw(modelShader);


        

        
        //Main_Tent (Military)
        translateMatrix = glm::translate(identityMatrix, glm::vec3(30.0, -0.1f, 010.0f)); 
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.015f, 0.015f, 0.015f));	
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
        model = translateMatrix * rotateYMatrix * scaleMatrix;

        modelShader.setMat4("model", model);
        Main_Tent.Draw(modelShader);


        //truck (procedural truck class) - in new garage platform on RIGHT side
        lightingShader.use();
        
        // Set truck render mode uniforms BEFORE drawing
        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setInt("uTruckMode", (int)myTruck.renderMode);
        lightingShaderWithTexture.setVec3("uTruckColor", glm::vec3(0.43f, 0.46f, 0.33f));  // truck body color
        lightingShaderWithTexture.setFloat("uBlendFactor", 0.5f);  // 50% blend
        
        glm::mat4 truckModel = glm::mat4(1.0f);
        truckModel = glm::translate(truckModel, truckState.pos);
        truckModel = glm::rotate(truckModel, glm::radians(truckState.yawDeg), glm::vec3(0, 1, 0));
        truckModel = glm::scale(truckModel, glm::vec3(1.30f, 1.30f, 1.30f));
        myTruck.draw(cubeVAO, lightingShader, lightingShaderWithTexture, truckModel, truckState.wheelSpinDeg);
        
        // RESET truck render mode to default (texture only) so other objects aren't affected
        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setInt("uTruckMode", 1);

        


        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);
        lightingShaderWithTexture.setMat4("projection", projection);
        lightingShaderWithTexture.setMat4("view", view);

        lightingShaderWithTexture.use();
        // Setup directional light (SUN)
        directlight.setUpDirectionLight(lightingShaderWithTexture);
        
        // point light 1
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        // point light 2
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        // point light 3
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        // point light 4
        pointlight4.setUpPointLight(lightingShaderWithTexture);
        pointlight5.setUpPointLight(lightingShaderWithTexture);
        pointlight6.setUpPointLight(lightingShaderWithTexture);
        pointlight7.setUpPointLight(lightingShaderWithTexture);
        pointlight8.setUpPointLight(lightingShaderWithTexture);
        pointlight9.setUpPointLight(lightingShaderWithTexture);
        pointlight10.setUpPointLight(lightingShaderWithTexture);
        pointlight11.setUpPointLight(lightingShaderWithTexture);
        pointlight12.setUpPointLight(lightingShaderWithTexture);
        pointlight13.setUpPointLight(lightingShaderWithTexture);
        pointlight14.setUpPointLight(lightingShaderWithTexture);
        pointlight15.setUpPointLight(lightingShaderWithTexture);
        pointlight16.setUpPointLight(lightingShaderWithTexture);
        pointlight17.setUpPointLight(lightingShaderWithTexture);
        pointlight18.setUpPointLight(lightingShaderWithTexture);
        pointlight19.setUpPointLight(lightingShaderWithTexture);
        pointlight20.setUpPointLight(lightingShaderWithTexture);
        
        // Interior tube lights for barrack (TEXTURE SHADER)
        tubeLightLeft.setUpPointLight(lightingShaderWithTexture);
        tubeLightRight.setUpPointLight(lightingShaderWithTexture);
        tubeLightCenter.setUpPointLight(lightingShaderWithTexture);

        // Garage lamps (4 lamps) - TEXTURE SHADER
        if (garageLampsOn)
        {
            garageLamp1.setUpPointLight(lightingShaderWithTexture);
            garageLamp2.setUpPointLight(lightingShaderWithTexture);
            garageLamp3.setUpPointLight(lightingShaderWithTexture);
            garageLamp4.setUpPointLight(lightingShaderWithTexture);
        }
        
        // Jeep headlight spotlights - SET THEM IN ARRAY
        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("spotLights[0].position", jeepAHeadlightL.position);
        lightingShaderWithTexture.setVec3("spotLights[0].direction", jeepAHeadlightL.direction);
        lightingShaderWithTexture.setVec3("spotLights[0].ambient", jeepAHeadlightL.ambient);
        lightingShaderWithTexture.setVec3("spotLights[0].diffuse", jeepAHeadlightL.diffuse);
        lightingShaderWithTexture.setVec3("spotLights[0].specular", jeepAHeadlightL.specular);
        lightingShaderWithTexture.setFloat("spotLights[0].cos_theta", jeepAHeadlightL.cos_theta);
        lightingShaderWithTexture.setFloat("spotLights[0].k_c", jeepAHeadlightL.k_c);
        lightingShaderWithTexture.setFloat("spotLights[0].k_l", jeepAHeadlightL.k_l);
        lightingShaderWithTexture.setFloat("spotLights[0].k_q", jeepAHeadlightL.k_q);
        
        lightingShaderWithTexture.setVec3("spotLights[1].position", jeepAHeadlightR.position);
        lightingShaderWithTexture.setVec3("spotLights[1].direction", jeepAHeadlightR.direction);
        lightingShaderWithTexture.setVec3("spotLights[1].ambient", jeepAHeadlightR.ambient);
        lightingShaderWithTexture.setVec3("spotLights[1].diffuse", jeepAHeadlightR.diffuse);
        lightingShaderWithTexture.setVec3("spotLights[1].specular", jeepAHeadlightR.specular);
        lightingShaderWithTexture.setFloat("spotLights[1].cos_theta", jeepAHeadlightR.cos_theta);
        lightingShaderWithTexture.setFloat("spotLights[1].k_c", jeepAHeadlightR.k_c);
        lightingShaderWithTexture.setFloat("spotLights[1].k_l", jeepAHeadlightR.k_l);
        lightingShaderWithTexture.setFloat("spotLights[1].k_q", jeepAHeadlightR.k_q);
        
        lightingShaderWithTexture.setVec3("spotLights[2].position", jeepBHeadlightL.position);
        lightingShaderWithTexture.setVec3("spotLights[2].direction", jeepBHeadlightL.direction);
        lightingShaderWithTexture.setVec3("spotLights[2].ambient", jeepBHeadlightL.ambient);
        lightingShaderWithTexture.setVec3("spotLights[2].diffuse", jeepBHeadlightL.diffuse);
        lightingShaderWithTexture.setVec3("spotLights[2].specular", jeepBHeadlightL.specular);
        lightingShaderWithTexture.setFloat("spotLights[2].cos_theta", jeepBHeadlightL.cos_theta);
        lightingShaderWithTexture.setFloat("spotLights[2].k_c", jeepBHeadlightL.k_c);
        lightingShaderWithTexture.setFloat("spotLights[2].k_l", jeepBHeadlightL.k_l);
        lightingShaderWithTexture.setFloat("spotLights[2].k_q", jeepBHeadlightL.k_q);
        
        lightingShaderWithTexture.setVec3("spotLights[3].position", jeepBHeadlightR.position);
        lightingShaderWithTexture.setVec3("spotLights[3].direction", jeepBHeadlightR.direction);
        lightingShaderWithTexture.setVec3("spotLights[3].ambient", jeepBHeadlightR.ambient);
        lightingShaderWithTexture.setVec3("spotLights[3].diffuse", jeepBHeadlightR.diffuse);
        lightingShaderWithTexture.setVec3("spotLights[3].specular", jeepBHeadlightR.specular);
        lightingShaderWithTexture.setFloat("spotLights[3].cos_theta", jeepBHeadlightR.cos_theta);
        lightingShaderWithTexture.setFloat("spotLights[3].k_c", jeepBHeadlightR.k_c);
        lightingShaderWithTexture.setFloat("spotLights[3].k_l", jeepBHeadlightR.k_l);
        lightingShaderWithTexture.setFloat("spotLights[3].k_q", jeepBHeadlightR.k_q);

        //-----------------(ground)-------------------//
        float halfGround = GROUND_SIZE * 0.5f;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-halfGround, 0.0f, -halfGround));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(GROUND_SIZE, GROUND_HEIGHT, GROUND_SIZE));
        glm::mat4 modelForBase = translateMatrix * scaleMatrix;
        
        // Set ground render mode uniforms BEFORE drawing
        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setInt("uGroundMode", groundRenderMode);
        lightingShaderWithTexture.setVec3("uGroundColor", groundColor);
        lightingShaderWithTexture.setFloat("uGroundBlendFactor", 0.5f);  // 50% blend
        lightingShaderWithTexture.setFloat("uTextureTiling", groundTextureTiling);
        
        cube.drawCubeWithTexture(lightingShaderWithTexture, modelForBase);
        
        // RESET ground render mode to default so other objects aren't affected
        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setInt("uGroundMode", 1);
        lightingShaderWithTexture.setFloat("uTextureTiling", 1.0f);

        // Draw road on top of ground
        road.draw(lightingShader);

        // Draw garage platforms (jeeps and truck)
        garagePlatform.draw(cubeVAO, lightingShader, lightingShaderWithTexture);
        truckGaragePlatform.draw(cubeVAO, lightingShader, lightingShaderWithTexture);

        // Draw military boundary with animated gate
        boundary.draw(lightingShaderWithTexture);

        // Draw Build object
        glm::mat4 buildModel = glm::mat4(1.0f);
        buildModel = glm::translate(buildModel, myBuild.origin);

        myBuild.draw(cubeVAO, lightingShader, lightingShaderWithTexture, buildModel);

        // ===== Furniture inside build =====
        // Furniture in local building coordinates (centered at origin in local space)
        // Positioned on floor level
        glm::mat4 roomBase = buildModel *
                             glm::translate(glm::mat4(1.0f),
                                            glm::vec3(0.0f, myBuild.bF, 0.0f));

        // left bed - left side, back area
        drawBed(cubeVAO, lightingShader, buildWallCube,
                roomBase * glm::translate(glm::mat4(1.0f),
                                          glm::vec3(-3.5f, 0.0f, -3.5f)));

        // right bed - right side, back area
        drawBed(cubeVAO, lightingShader, buildWallCube,
                roomBase * glm::translate(glm::mat4(1.0f),
                                          glm::vec3(3.5f, 0.0f, -3.5f)));

        // table - center, front area
        drawTable(cubeVAO, lightingShader, buildWallCube,
                  roomBase * glm::translate(glm::mat4(1.0f),
                                            glm::vec3(0.0f, 0.0f, 3.5f)));

        // medical box on table
        drawMedicalBox(cubeVAO, lightingShaderWithTexture, boxCube,
                       roomBase * glm::translate(glm::mat4(1.0f),
                                                 glm::vec3(0.0f, 1.8f, 3.5f)));

        // Draw the SUN in the sky
        lightingShaderWithTexture.use();
        glm::mat4 sunTranslateMatrix = glm::translate(identityMatrix, sunPosition);
        glm::mat4 sunScaleMatrix = glm::scale(identityMatrix, glm::vec3(6.0f, 6.0f, 6.0f));
        glm::mat4 sunModel = sunTranslateMatrix * sunScaleMatrix;
        
        // Bright warm yellow-orange color for sun
        glm::vec3 sunColor = glm::vec3(1.0f, 0.95f, 0.4f);
        lightingShaderWithTexture.setVec3("material.ambient", sunColor);
        lightingShaderWithTexture.setVec3("material.diffuse", sunColor);
        lightingShaderWithTexture.setVec3("material.specular", glm::vec3(1.0f, 1.0f, 0.8f));
        lightingShaderWithTexture.setFloat("material.shininess", 64.0f);
        lightingShaderWithTexture.setMat4("model", sunModel);
        
        sphere.drawSphere(lightingShaderWithTexture, sunModel);

        // Draw multiple watch towers using WatchTower class
        watchTower1.draw(lightingShaderWithTexture);
        watchTower2.draw(lightingShaderWithTexture);
        watchTower3.draw(lightingShaderWithTexture);
        watchTower4.draw(lightingShaderWithTexture);

        // ===== DRAW CAMPFIRE (now that all lighting is properly configured) =====
        campfire.setUpPointLight(lightingShader, 21);
        campfire.setUpPointLight(lightingShaderWithTexture, 21);
        campfire.draw(lightingShader, lightingShaderWithTexture, cubeVAO, campfireModel, currentFrame);

        // Draw military jeeps ON GARAGE PLATFORM - SIDE BY SIDE
        // Garage platform is at origin (-65, 0, -28) with gF=1.2 (platform height)
        // Interior width (gW) = 14, depth (gD) = 18
        // Place two jeeps side by side on the platform floor with increased spacing
        float platformCenterX = garagePlatform.origin.x;  // -65
        float platformY = garagePlatform.platformTop() + 0.05f;  // 1.2 + small offset above platform
        float platformZ = garagePlatform.origin.z + 2.0f;  // Slightly forward from center
        
        float jeepSpacing = 5.2f;  // Increased space between two jeeps (left-right) - was 3.2f
        
        // LEFT JEEP - moved further LEFT
        glm::mat4 jm1 = glm::mat4(1.0f);
        jm1 = glm::translate(jm1, jeepA.pos);
        jm1 = glm::rotate(jm1, glm::radians(jeepA.yawDeg), glm::vec3(0, 1, 0));
        jm1 = glm::scale(jm1, glm::vec3(1.15f, 1.15f, 1.15f));
        myJeep.draw(cubeVAO, lightingShader, lightingShaderWithTexture, jm1,
                    jeepA.doorAngleDeg, jeepA.windowDrop, jeepA.wheelSpinDeg, jeepA.headlightsOn);
        
        // RIGHT JEEP - moved further RIGHT
        glm::mat4 jm2 = glm::mat4(1.0f);
        jm2 = glm::translate(jm2, jeepB.pos);
        jm2 = glm::rotate(jm2, glm::radians(jeepB.yawDeg), glm::vec3(0, 1, 0));
        jm2 = glm::scale(jm2, glm::vec3(1.15f, 1.15f, 1.15f));
        myJeep2.draw(cubeVAO, lightingShader, lightingShaderWithTexture, jm2,
                     jeepB.doorAngleDeg, jeepB.windowDrop, jeepB.wheelSpinDeg, jeepB.headlightsOn);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setFloat("material.shininess", 22.0f);

    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}


void func(unsigned int& cubeVAO, Shader& lightingShader, Cube base)
{
    float baseHeight = 0.1;
    float width = GROUND_SIZE;
    float length = GROUND_SIZE;

    //base-1
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 scale = glm::scale(model, glm::vec3(width, baseHeight, length));
    translate = glm::translate(model, glm::vec3(-width * 0.5f, 0, -length * 0.5f));
    model = translate * scale;
    base.drawCubeWithTexture(lightingShader, model);

}



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Handle ground texture mode toggle (M key) - works always
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && !kGroundPressed) {
        kGroundPressed = true;
        groundRenderMode = (groundRenderMode + 1) % 3;  // Cycle through 0, 1, 2
        std::cout << "Ground Render Mode: ";
        if (groundRenderMode == 0) std::cout << "COLOR ONLY" << std::endl;
        else if (groundRenderMode == 1) std::cout << "TEXTURE" << std::endl;
        else std::cout << "BLEND (50/50)" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_RELEASE) {
        kGroundPressed = false;
    }

    // Handle truck render mode toggle (K key) - works always
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && !kTruckPressed) {
        kTruckPressed = true;
        if (gTruck) {
            gTruck->cycleRenderMode();
        }
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_RELEASE) {
        kTruckPressed = false;
    }

    JeepState* j = currentSelectedJeep();
    TruckState* t = currentSelectedTruck();

    // Handle jeep controls
    if (j) {
        float moveSpeed = 6.0f * deltaTime;
        float turnSpeed = 90.0f * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            j->yawDeg += turnSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            j->yawDeg -= turnSpeed;
        }

        float moved = 0.0f;
        glm::vec3 fwd = jeepForward(j->yawDeg);

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            glm::vec3 nextPos = j->pos - fwd * moveSpeed;
            nextPos.y = getSurfaceYAt(nextPos.x, nextPos.z);

            if (!collidesAtPosition(nextPos, j)) {
                j->pos = nextPos;
                moved -= moveSpeed;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            glm::vec3 nextPos = j->pos + fwd * moveSpeed;
            nextPos.y = getSurfaceYAt(nextPos.x, nextPos.z);

            if (!collidesAtPosition(nextPos, j)) {
                j->pos = nextPos;
                moved += moveSpeed;
            }
        }

        if (moved != 0.0f) {
            float radius = Jeep::WR * 1.15f;
            j->wheelSpinDeg += glm::degrees(moved / radius);
        }
    }
    // Handle truck controls
    else if (t) {
        float moveSpeed = 6.0f * deltaTime;
        float turnSpeed = 90.0f * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            t->yawDeg += turnSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            t->yawDeg -= turnSpeed;
        }

        float moved = 0.0f;
        glm::vec3 fwd = jeepForward(t->yawDeg);

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            glm::vec3 nextPos = t->pos + fwd * moveSpeed;
            nextPos.y = getSurfaceYAt(nextPos.x, nextPos.z);

            if (!truckCollidesAtPosition(nextPos)) {
                t->pos = nextPos;
                moved += moveSpeed;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            glm::vec3 nextPos = t->pos - fwd * moveSpeed;
            nextPos.y = getSurfaceYAt(nextPos.x, nextPos.z);

            if (!truckCollidesAtPosition(nextPos)) {
                t->pos = nextPos;
                moved -= moveSpeed;
            }
        }

        if (moved != 0.0f) {
            t->wheelSpinDeg += glm::degrees(moved / 1.04f);
        }
    }
    else {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        camera.ProcessKeyboard(PITCH_U, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        camera.ProcessKeyboard(PITCH_D, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(YAW_L, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(YAW_R, deltaTime);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_J) {
            selectedJeep++;
            if (selectedJeep > 1) selectedJeep = -1;
            selectedTruck = -1;  // Deselect truck when selecting jeep
        }

        if (key == GLFW_KEY_P) {
            selectedTruck = (selectedTruck == -1) ? 0 : -1;
            if (selectedTruck != -1) selectedJeep = -1;  // Deselect jeeps when selecting truck
        }

        JeepState* j = currentSelectedJeep();
        TruckState* t = currentSelectedTruck();

        if (key == GLFW_KEY_D && j) {
            j->doorTargetOpen = !j->doorTargetOpen;
        }

        if (key == GLFW_KEY_W && j) {
            j->windowTargetOpen = !j->windowTargetOpen;
        }

        if (key == GLFW_KEY_L && j) {
            j->headlightsOn = !j->headlightsOn;
        }

        if (key == GLFW_KEY_V && (j || t)) {
            if (j) {
                j->camMode = (j->camMode + 1) % 4;
            } else if (t) {
                t->camMode = (t->camMode + 1) % 4;
            }
        }

        if (key == GLFW_KEY_T) {
            goTank = !goTank;
        }

        if (key == GLFW_KEY_R) {
            RotateRadar = !RotateRadar;
        }

        if (key == GLFW_KEY_U && gBoundary) {
            gBoundary->toggleGate();
        }

        if (key == GLFW_KEY_G && gBuild) {
            gBuild->doorOpen = !gBuild->doorOpen;
            gBuild->targetDoorOpenAngle = gBuild->doorOpen ? 90.0f : 0.0f;
        }

        if (key == GLFW_KEY_H && gBuild) {
            gBuild->leftWinOpen = !gBuild->leftWinOpen;
            gBuild->targetLeftWinOpen = gBuild->leftWinOpen ? 1.2f : 0.0f;
        }

        if (key == GLFW_KEY_I && gBuild) {
            gBuild->rightWinOpen = !gBuild->rightWinOpen;
            gBuild->targetRightWinOpen = gBuild->rightWinOpen ? 1.2f : 0.0f;
        }
    }

    if (key == GLFW_KEY_B && action == GLFW_PRESS)
    {
        barrackDoorOpening = !barrackDoorOpening;
    }

    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        if (ambientToggle)
        {
            pointlight1.turnAmbientOff();
            pointlight2.turnAmbientOff();
            pointlight3.turnAmbientOff();
            pointlight4.turnAmbientOff();
            directlight.turnAmbientOff();
            spotlight.turnAmbientOff();
            ambientToggle = !ambientToggle;
        }
        else
        {
            pointlight1.turnAmbientOn();
            pointlight2.turnAmbientOn();
            pointlight3.turnAmbientOn();
            pointlight4.turnAmbientOn();
            directlight.turnAmbientOn();
            spotlight.turnAmbientOn();
            ambientToggle = !ambientToggle;
        }
    }
    else if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {
        if (diffuseToggle)
        {
            pointlight1.turnDiffuseOff();
            pointlight2.turnDiffuseOff();
            pointlight3.turnDiffuseOff();
            pointlight4.turnDiffuseOff();
            directlight.turnDiffuseOff();
            spotlight.turnDiffuseOff();
            diffuseToggle = !diffuseToggle;
        }
        else
        {
            pointlight1.turnDiffuseOn();
            pointlight2.turnDiffuseOn();
            pointlight3.turnDiffuseOn();
            pointlight4.turnDiffuseOn();
            directlight.turnDiffuseOn();
            spotlight.turnDiffuseOn();
            diffuseToggle = !diffuseToggle;
        }
    }
    else if (key == GLFW_KEY_6 && action == GLFW_PRESS)
    {
        if (specularToggle)
        {
            pointlight1.turnSpecularOff();
            pointlight2.turnSpecularOff();
            pointlight3.turnSpecularOff();
            pointlight4.turnSpecularOff();
            directlight.turnSpecularOff();
            spotlight.turnSpecularOff();
            specularToggle = !specularToggle;
        }
        else
        {
            pointlight1.turnSpecularOn();
            pointlight2.turnSpecularOn();
            pointlight3.turnSpecularOn();
            pointlight4.turnSpecularOn();
            directlight.turnSpecularOn();
            spotlight.turnSpecularOn();
            specularToggle = !specularToggle;
        }
    }
    else if (key == GLFW_KEY_7 && action == GLFW_PRESS)
    {
        sunlightToggle = !sunlightToggle;
        if (sunlightToggle)
        {
            directlight.turnOn();
        }
        else
        {
            directlight.turnOff();
        }
    }
    else if (key == GLFW_KEY_O && action == GLFW_PRESS)
    {
        birdEyeMode = (birdEyeMode + 1) % 4;  // Cycle: 0 -> 1 -> 2 -> 3 -> 0
        if (birdEyeMode == 0) {
            std::cout << "Bird's Eye Mode: OFF (Normal View)" << std::endl;
        } else if (birdEyeMode == 1) {
            std::cout << "Bird's Eye Mode: ON (Wide View)" << std::endl;
        } else if (birdEyeMode == 2) {
            std::cout << "Bird's Eye Mode: ON (Close Shot)" << std::endl;
        } else {
            std::cout << "Bird's Eye Mode: ON (Very Close Shot)" << std::endl;
        }
    }
    else if (key == GLFW_KEY_8 && action == GLFW_PRESS)
    {
        garageLampsOn = !garageLampsOn;
        if (garageLampsOn)
        {
            garageLamp1.turnOn();
            garageLamp2.turnOn();
            garageLamp3.turnOn();
            garageLamp4.turnOn();
            std::cout << "Garage Lamps: ON" << std::endl;
        }
        else
        {
            garageLamp1.turnOff();
            garageLamp2.turnOff();
            garageLamp3.turnOff();
            garageLamp4.turnOff();
            std::cout << "Garage Lamps: OFF" << std::endl;
        }
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// Create a simple procedural texture when file loading fails
unsigned int createProceduralTexture(int width, int height, unsigned char r, unsigned char g, unsigned char b)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture parameters BEFORE uploading data
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Create a simple solid color texture (uniform color for black, subtle pattern for others)
    unsigned char* data = new unsigned char[width * height * 3];
    
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int idx = (y * width + x) * 3;
            
            // For pure black or near-black (tires, exhausts), use solid color (no pattern)
            // For other colors, create subtle checkerboard pattern
            unsigned char varR = r;
            unsigned char varG = g;
            unsigned char varB = b;
            
            // Only add checkerboard pattern if NOT black/near-black
            if (r > 30 || g > 30 || b > 30)
            {
                int checkSize = 16;
                bool checkPattern = ((x / checkSize) + (y / checkSize)) % 2 == 0;
                
                if (!checkPattern)
                {
                    varR = (r > 50 ? r - 20 : r + 20);
                    varG = (g > 50 ? g - 20 : g + 20);
                    varB = (b > 50 ? b - 20 : b + 20);
                }
            }
            
            data[idx] = varR;
            data[idx + 1] = varG;
            data[idx + 2] = varB;
        }
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    delete[] data;
    glBindTexture(GL_TEXTURE_2D, 0);
    
    return textureID;
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture parameters BEFORE loading data for consistency
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);
    
    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }
    else
    {
        
        // Unbind current texture and create procedural one
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, &textureID);
        
        // Determine fallback color based on filename
        unsigned char r = 128, g = 128, b = 128; // Default gray
        
        std::string pathStr(path);
        if (pathStr.find("pebble") != std::string::npos)
        {
            // Rocky/pebble texture - dark gray with slight brown
            r = 100; g = 95; b = 85;
        }
        else if (pathStr.find("jeep") != std::string::npos)
        {
            // Jeep metal - olive/army green
            r = 80; g = 90; b = 60;
        }
        
        textureID = createProceduralTexture(256, 256, r, g, b);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    return textureID;
}