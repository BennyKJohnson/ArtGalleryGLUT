//
//  main.cpp
//  GLUTDrawingApp
//
//  Created by Benjamin Johnson on 12/04/2016.
//  Copyright © 2016 Benjamin Johnson. All rights reserved.
//

#include <iostream>
#include <string>

#ifdef __APPLE__
#include <GLUT/GLUT.h> //GLUT Library, will make you life easier
#include <OpenGL/OpenGL.h> //OpenGL Library
#elif defined _WIN32 || defined _WIN64
#include <glut.h>
#endif

#include "CG.hpp"
#include "CGScene.hpp"
#include "CGParametricGeometry.hpp"
#include "CGPresetMaterials.hpp"
#include <math.h>
#include <sys/stat.h>
#include <ctime>

#include "monkey.h"
#include "frame.h"
#include "fan.h"
#include "banana.h"
#include "jitter.h"
#include <dirent.h>

#define MY_PI 		(3.14159265359)    //declare PI value
#define DEG2RAD(a) 	(MY_PI/180*(a))    //convert degrees into radians


int mainWindow;

CGScene *scene;

CGNode *pointOfView;

enum ContextMenu {
    Clear,
    Exit
};

enum CGDisplayMode {
    CGDisplayModeDefault,
    CGDisplayModeColored,
    CGDisplayModeShadedOnly,
    CGDisplayModeTexturedWithoutLighting,
};

enum CGModifyMode {
    CGModifyModeNone,
    CGModifyModeShading,
    CGModifyModeLighting,
    CGModifyModeWire
};

GLfloat aspect;

GLfloat movementSpeed = 0.2;

GLfloat cameraYaw = 0.0;

GLfloat cameraPitch = 0.0;

GLfloat moveForward = 1.0;

GLfloat rotationSpeed = 2;

GLenum shadeModel = GL_SMOOTH;

GLenum cullFaceMode = GL_BACK;

GLfloat lightAttenuation = 0.0;

bool yellowAmbientEnabled = false;

bool translucentSurfaces = true;

bool antiAliasing = false;

bool stencilBuffering  = true;

bool motionBlur = false;

void resetCamera();

int accumulationSize = 8;

// Global Nodes

CGNode *floorNode;

CGLight *light;

CGNode *leftLightNode;

CGNode *rightLightNode;

CGNode *spotLightNode;

CGNode *fanNode;

CGNode *cameraNode;

CGNode *monkeyNode;

std::vector<CGNode*> lightSources;


float fps;

GLfloat gAngle = 0.0f;  //cube rotation angle

GLfloat monkeyAngle = 0.0f; // Monkey rotation angle

int frame=0, elapsedTime ,timebase=0;

GLfloat lastX = 400;

GLfloat lastY = 300;


CGModifyMode modifyMode = CGModifyModeNone;

bool hasReceivedInitalMouseMovement = false;

std::string *helpString;

std::string dayModeStatus;

std::string displayModeStatus;

std::string modifyingStatus;

bool showInstructions;

CGVector3 cameraFront = CGVector3(0, 0 ,-1);

CGVector3 cameraUp = CGVector3(0,1,0);

CGMaterialProperty *screenshotMaterialProperty;

// Static variable init
bool CGMaterial::texturesEnabled = true;

void setWireFrameMode(CGColor backgroundColor, CGColor wireColor);

bool nightMode = false;

void setOGLProjection(int width, int height) {
    glViewport(0, 0, width, height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glOrtho(0, width, height, 0, -1, 1);
    
    //Back to the modelview so we can draw stuff
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
}

void windowShouldRedraw() {
    glutSetWindow(mainWindow);
    glutPostRedisplay();
}

void setDisplayMode(CGDisplayMode displayMode) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //fill mode
    
    switch (displayMode) {
        case CGDisplayModeDefault:
            displayModeStatus = "Shading with Lighting and Texture";
            glEnable(GL_LIGHTING);
            glEnable(GL_TEXTURE_2D);	// enable 2D texturing
            break;
        case CGDisplayModeColored:
            displayModeStatus = "Shaded without Lighting and Texture";
            glDisable(GL_LIGHTING);
            glDisable(GL_TEXTURE_2D);
            break;
        case CGDisplayModeShadedOnly:
            displayModeStatus = "Shaded without Textures";
            glEnable(GL_LIGHTING);
            glDisable(GL_TEXTURE_2D);
            break;
        case CGDisplayModeTexturedWithoutLighting:
            displayModeStatus = "Shaded without lighting";

            glEnable(GL_TEXTURE_2D);
            glDisable(GL_LIGHTING);
            break;
    }
    
    glutPostRedisplay();
}

void setNightMode(bool night) {
    nightMode = night;
    if(nightMode) {
        for (int i = 0; i < lightSources.size(); i++) {
            lightSources[i]->hidden = true;
        }
        
        leftLightNode->hidden = false;


        dayModeStatus = "Night";
        
    } else {
        for (int i = 0; i < lightSources.size(); i++) {
            lightSources[i]->hidden = false;
        }
        
        dayModeStatus = "Day";

    }
    
    
}

void toggleOpenGLSetting(GLenum property) {
    if (glIsEnabled(property)) {
        glDisable(property);
    } else {
        glEnable(property);
    }
}


void didPressNumKey(int index) {
    switch (modifyMode) {
        case CGModifyModeWire:
            switch (index) {
                case 1:
                    setWireFrameMode(CGColorBlack(), CGColorWhite());
                    displayModeStatus = "Wire Black and White";
                    break;
                case 2:
                    setWireFrameMode(CGColorWhite(), CGColorBlack());
                    displayModeStatus = "Wire White and Black";

                    break;
                case 3:
                    setWireFrameMode(CGColorBlue(), CGColorSimpleYellow());
                    displayModeStatus = "Wire Blue and Yellow";

                default:
                    return;
            }
            break;
        case CGModifyModeShading: {
            
            int enumIndex = index - 1;
            if (enumIndex < 4) {
                CGDisplayMode shadingMode = (CGDisplayMode)enumIndex;
                setDisplayMode(shadingMode);
            }
            break;
            
            
        }
        case CGModifyModeLighting: {
            int lightIndex = index - 1;
            if(lightIndex >= 0 && lightIndex < lightSources.size()) {
                CGNode *light = lightSources[lightIndex];
                light->hidden = !light->hidden;
            }
            
            
        }
        default:
            break;
    }
}

void setModifyMode(CGModifyMode mode) {
    if(mode == modifyMode) {
        // Set to none
        modifyMode = CGModifyModeNone;
    } else {
        modifyMode = mode;
    }
    
    
    switch (modifyMode) {
        case CGModifyModeWire:
            std:: cout << "Setting WireMode" << std::endl;
            modifyingStatus = "Setting Wire mode use 1-3";
            break;
        case CGModifyModeShading:
            std:: cout << "Setting Shading Mode" << std::endl;
            modifyingStatus = "Setting Wire mode use 1-4";
            break;
        case CGModifyModeLighting:
             modifyingStatus = "Setting Lighting use 1-4";
           break;
        case CGModifyModeNone:
            modifyingStatus = "";
               break;
  
    }
    
    
    
}


void mouseDidMove( int xPos, int yPos) {
    
    if (!hasReceivedInitalMouseMovement) {
        lastX = xPos;
        lastY = yPos;
        
        hasReceivedInitalMouseMovement = true;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos; // Reversed since y-coordinates range from bottom to top
    
    // Update previous positions
    lastX = xPos;
    lastY = yPos;
    
    GLfloat sensitivity = 0.3;
    xOffset *= sensitivity;
    yOffset *= sensitivity;
    
    cameraYaw += xOffset;
    cameraPitch += yOffset;
    
    if (cameraPitch > 89.0) {
        cameraPitch = 89.0;
    } else if(cameraPitch < -89.0) {
        cameraPitch = -89;
    }
    
    CGVector3 front;
    front.x = cosf(degreesToRadians(cameraYaw)) * cosf(degreesToRadians(cameraPitch));
    front.y = sinf(degreesToRadians(cameraPitch));
    front.z = sinf(degreesToRadians(cameraYaw)) * cosf(degreesToRadians(cameraPitch));
    
    cameraFront = front.normalize();
    
    glutPostRedisplay();
}

void toggleShadeModel() {
    if (shadeModel == GL_SMOOTH) {
        shadeModel = GL_FLAT;
        
    } else {
        shadeModel = GL_SMOOTH;
    }
    
    glShadeModel(shadeModel);
    glutPostRedisplay();
}

void toggleTranslucentSurfaces() {
    if(translucentSurfaces) {
        // Disable
        glDisable(GL_BLEND);       // enable blending

        translucentSurfaces = false;
    }else  {
        glEnable(GL_BLEND);       // enable blending

        translucentSurfaces = true;
    }
}



void setPolygonMode(CGPolygonMode mode) {
    switch (mode) {
        case CGPolygonModeWire:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);    //wireframe mode
        case CGPolygonModeSolid:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //fill mode
    }
    
    windowShouldRedraw();
}

void setCamera(CGVector3 camPosition, CGVector3 lookAt, CGVector3 camUp) {
    
   //  glLoadIdentity();
    gluLookAt(camPosition.x, camPosition.y, camPosition.z, lookAt.x, lookAt.y, lookAt.z, camUp.x, camUp.y,camUp.z);
}

void updateCamera() {
    
    if (cameraYaw > 360.0) {
        cameraYaw -= 360.0;
    } else if(cameraYaw < 0.0)
        cameraYaw += 360.0;
  
    glLoadIdentity();
    
    // Enable perspective projection with fovy, aspect, zNear and zFar
    CGCamera *camera = pointOfView->camera;
    gluPerspective(camera->yFov, aspect, camera->zNear, camera->zFar);

   // std::cout << "Camera Position: " << pointOfView->position << std::endl;
   //  std::cout << "Camera Front: " << cameraFront << std::endl;

    CGVector3 lookAt = cameraFront + pointOfView->position;
   // std::cout << "Looking At: " << lookAt.x << ", " << lookAt.y << ", " << lookAt.z << std::endl;
    
    setCamera(pointOfView->position,  lookAt, cameraUp);
    
    glutPostRedisplay();
   // windowShouldRedraw();
}


void toggleAmbientLighting() {
    if (!scene->autoenablesDefaultLighting) {
        return;
    }

    if (yellowAmbientEnabled) {
        scene->globalAmbientColor = CGColor(0.2, 0.2, 0.2, 1.0);
        yellowAmbientEnabled = false;
    } else {
        scene->globalAmbientColor = CGColorSimpleYellow();
        yellowAmbientEnabled = true;
    }
    
    float values[] {scene->globalAmbientColor.r, scene->globalAmbientColor.b, scene->globalAmbientColor.g, scene->globalAmbientColor.a};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, values);
}

void toggleCullFace() {
    if (cullFaceMode == GL_BACK) {
        cullFaceMode = GL_FRONT;
    } else {
        cullFaceMode = GL_BACK;
    }
    
    glCullFace(cullFaceMode);
    
}



void setWireFrameMode(CGColor backgroundColor, CGColor wireColor)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);    //wireframe mode
    scene->backgroundColor = backgroundColor;
    scene->color = wireColor;
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glutPostRedisplay();
}

void toggleFlashlight() {
    if(cameraNode->light == NULL) {
        
        CGLight *flashlight =  new CGLight();
        flashlight->color = CGColorSimpleYellow();
        flashlight->constantAttenuation = 5.0;
        flashlight->spotOuterAngle = 30;
        flashlight->type = CGLightTypeSpot;
        
        cameraNode->light = flashlight;
    } else {
        CGLight *flashlight = cameraNode->light;
        delete flashlight;
        cameraNode->light = NULL;
    }
    
    glutPostRedisplay();
    
}

char* getLastScreenshot() {
    
    DIR           *directory;
    struct dirent *dir;
    directory = opendir("screenshots");
    
    char *filename = NULL;
    if (directory) {
        while ((dir = readdir(directory)) != NULL)
        {
            if ( strstr( dir->d_name, ".bmp" )) {
                if(filename == NULL) {
                    filename = new char[256];
                }
                unsigned long length = strlen(dir->d_name);
                strcpy(filename, dir->d_name);
                filename[length] = '\0';
            }
            
        }
        closedir(directory);
        
    }
    
    printf("LAST SCREENSHOT %s\n", filename);

    return filename;
}

CGTexture* loadScreenshotTexture() {
    
    char screenshotURL[512] = "screenshots/";
    char *screenshotFilename = getLastScreenshot();
    if(screenshotFilename != NULL) {
        strcat(screenshotURL, screenshotFilename);
        CGTexture *texture = new CGTexture(screenshotURL);
        return texture;
    } else {
        return NULL;
    }
}

void takeScreenshot() {

    int exists = mkdir("screenshots", S_IRWXU);
    
    time_t date = time(0);
    char filename[256];
    sprintf(filename, "screenshots/%ld.bmp", date);
    
    int save_result = SOIL_save_screenshot
    (
     filename,
     SOIL_SAVE_TYPE_BMP,
     0, 0, 800, 600
     );
    
    if(save_result > 0) {
        std::cout << "Successfully saved screenshot" << std::endl;
    } else {
        std::cout << "Failed to save screenshot" << std::endl;
    }
    
    getLastScreenshot();
    
    
    
    // Load Texture
    CGTexture *screenshotTexture = loadScreenshotTexture();
    if(screenshotTexture) {
        screenshotMaterialProperty->content = screenshotTexture;
        std::cout << "FrameMaterial Address: " << screenshotMaterialProperty << std::endl;

    }
    
}


void keyboardHandler(unsigned char key, int x, int y)
{
    switch(key)
    {
            //if ESC pressed, quit program
        case 27: exit(1);     //quit
            break;
        case 'a':
            antiAliasing = !antiAliasing;
            glutPostRedisplay();
            std::cout << "antiAliasing " << antiAliasing << std::endl;
            break;
        
        case 'q':
            exit(0);
            break;
        case 'w':
            setModifyMode(CGModifyModeWire);
           // setWireFrameMode(CGColorWhite(), CGColorBlack());
            break;
        case 'e':
            setWireFrameMode(CGColorBlue(), CGColorSimpleYellow());
            break;
        case 'z':
            setDisplayMode(CGDisplayModeDefault);
            break;
        case 's':
            setModifyMode(CGModifyModeShading);
            break;
            /*
        case 'z':
            toggleShadeModel();
            break;
        */
        case 'd':
            toggleOpenGLSetting(GL_DEPTH_TEST);
            break;
        case 'c':
            takeScreenshot();
           // toggleOpenGLSetting(GL_CULL_FACE);
            break;
        case 'f':
            toggleCullFace();
            break;
        case 'm':
            toggleOpenGLSetting(GL_COLOR_MATERIAL);
            break;
        case 'n':
            setNightMode(!nightMode);
            glutPostRedisplay();
            break;
        case 'l':
            setModifyMode(CGModifyModeLighting);
            break;
        case 'r':
            resetCamera();
            break;
        case 'o':
            light->attenuationFalloffExponent -= 0.01;
            glutPostRedisplay();
            break;
        case 'p':
            light->attenuationFalloffExponent += 0.01;
            glutPostRedisplay();
            break;
        case 'h':
            showInstructions = !showInstructions;
            glutPostRedisplay();
            break;
        case 't':
            toggleTranslucentSurfaces();
            glutPostRedisplay();
            break;
        case 'v':
            toggleFlashlight();
            break;
        case '1':
            didPressNumKey(1);
            // Toggle Light 1
            //leftLightNode->hidden = !leftLightNode->hidden;
            break;
        case '2':
            didPressNumKey(2);
            // Toggle Light 2
          //  rightLightNode->hidden = !rightLightNode->hidden;
            break;
        case '3':
            didPressNumKey(3);
            //spotLightNode->hidden = !spotLightNode->hidden;
            break;
        case '4':
            didPressNumKey(4);
           // setDisplayMode(CGDisplayModeShadedOnly);
            break;
            
        default:
            break;
    };
}

std::string fpsString;



//called when no event in queue
void myIdleFunc()
{
    // Calculate Frames Per Second
    frame++;
    elapsedTime=glutGet(GLUT_ELAPSED_TIME);
    
    if (elapsedTime - timebase > 1000) {
        fps = frame*1000.0/(elapsedTime-timebase);
        timebase = elapsedTime;
        frame = 0;
        char buff[100];
        sprintf(buff,"FPS: %4.2f",fps);
        fpsString = buff;
        
       // std::cout << "FPS: " << fps << std::endl;
    }
    
    
    gAngle += 3;    //increment rotation angle
    monkeyAngle += 0.5;
    if(gAngle >= 360) //wrap around if rotation angle greater than 360
    {
        gAngle -= 360;
    }
    
    if(monkeyAngle >= 360) {
        monkeyAngle -= 360;
    }
    monkeyNode->eulerAngles = CGVector3(0,monkeyAngle,0);
    fanNode->eulerAngles = CGVector3(0,gAngle,0);
    
    
    glutPostRedisplay();  //refresh display
}



//called when non-ASCII key pressed
void specialKeyHandler(int key, int x, int y)
{
    
    float cameraSpeed = 0.2f;
    switch(key)
    {
            //if home pressed
        case GLUT_KEY_HOME:
            //  gCameraPosition[1] += gMovementSensitivity;       //increment camera height
            //  gCameraLookAt[1] += gMovementSensitivity;
            break;
            //if end pressed
        case GLUT_KEY_END:
            //gCameraPosition[1] -= gMovementSensitivity;       //decrement camera height
            //     gCameraLookAt[1] -= gMovementSensitivity;
            break;
            //if arrow up pressed
        case GLUT_KEY_UP: {
            CGVector3 newPosition = CGVector3(cameraFront.x * cameraSpeed, cameraFront.y * cameraSpeed, cameraFront.z * cameraSpeed);
            std::cout << "New Position: " << newPosition << std::endl;
            CGVector3 oldPos = pointOfView->position;
            std::cout << "Previous Camera Position: " << oldPos << std::endl;
            CGVector3 newCameraPosition = CGVector3(oldPos.x + newPosition.x, oldPos.y + newPosition.y, oldPos.z + newPosition.z);
            pointOfView->position = newCameraPosition;
            
            break;

        }
          //  teapotNode->position.x += 1;
           // pointOfView->position.x += movementSpeed;
      
            //pointOfView->position += cameraSpeed * cameraFront;
            
            //gMoveForward += gMovementSensitivity;             //increment forward movement
            //if arrow down pressed
        case GLUT_KEY_DOWN: {
            CGVector3 oldPos = pointOfView->position;

            CGVector3 newPosition = CGVector3(cameraFront.x * cameraSpeed, cameraFront.y * cameraSpeed, cameraFront.z * cameraSpeed);
            CGVector3 newCameraPosition = CGVector3(oldPos.x - newPosition.x, oldPos.y - newPosition.y, oldPos.z - newPosition.z);

            pointOfView->position = newCameraPosition;
            
            break;
        }

            //if left pressed
        case GLUT_KEY_LEFT: {
            CGVector3 previousPos = pointOfView->position;
            CGVector3 newPos = cameraFront.crossProduct(cameraUp).normalize() * cameraSpeed;
            
            pointOfView->position = CGVector3(previousPos.x - newPos.x, previousPos.y - newPos.y, previousPos.z - newPos.z);

            break;
        }

            
            //if right pressed
        case GLUT_KEY_RIGHT: {
            
            CGVector3 previousPos = pointOfView->position;
            CGVector3 newPos = cameraFront.crossProduct(cameraUp).normalize() * cameraSpeed;
            
            pointOfView->position = CGVector3(previousPos.x + newPos.x, previousPos.y + newPos.y, previousPos.z + newPos.z);
            break;

        }
      

        default: break;
    };
    updateCamera();
    
    //   updateCamera();       //call function to update camera and refresh the scene
}





void resize(int width, int height) {
    
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    aspect = (GLfloat)width / (GLfloat)height;
    
    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);
    
    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   // glLoadIdentity();             // Reset
    updateCamera();
    
    
  }


CGRect getWindowRect() {
    int x = glutGet(GLUT_WINDOW_X);
    int y = glutGet(GLUT_WINDOW_Y);
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    
    return CGRectMake((float)x, (float)y, (float)width, (float)height);
}

void renderHUD() {
    glMatrixMode(GL_PROJECTION);
    
    glPushMatrix();
    
    glLoadIdentity();
    //glViewport(0, 0, 800, 600);
    
    gluOrtho2D(0.0, 800, 0.0, 600);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    if (showInstructions) {
        setContextColor(CGColorWhite());
        renderBitmapString(0, 70, helpString, GLUT_BITMAP_HELVETICA_12);

        renderBitmapString(0, 55, &modifyingStatus, GLUT_BITMAP_HELVETICA_12);
        renderBitmapString(0,40, &dayModeStatus, GLUT_BITMAP_HELVETICA_12);
        renderBitmapString(0,25, &displayModeStatus, GLUT_BITMAP_HELVETICA_12);
        renderBitmapString(0, 10, &fpsString, GLUT_BITMAP_HELVETICA_12);
    }
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

void renderStencilBuffer() {
    // Setup Stencil Buffer
    if (true) {
        
        glDisable(GL_DEPTH_TEST);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xffffffff);
        
        glPushMatrix();
        glScalef(1.0f, -1.0f, 1.0f); //flip the reflection vertically
        glTranslatef(0,2,0); //translate the reflection onto the
        glRotatef(40,0,1,0); //rotate the reflection

        scene->renderNode(floorNode);
        
        glPopMatrix();
        
        /* Re-enable update of color and depth. */
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST); //disable the stencil testing
        
        /* Now, only render where stencil is set to 1. */
        glStencilFunc(GL_EQUAL, 1, 0xffffffff);  /* draw if ==1 */
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        
    }
}

void standardRender() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //      glClearStencil(0);
    
    glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
    
    glLoadIdentity();
    CGVector3 lookAt = cameraFront + pointOfView->position;
    setCamera(pointOfView->position,  lookAt, cameraUp);
    
    
    scene->render();

}

void render(void) {
    
    GLint viewport[4];
    glGetIntegerv (GL_VIEWPORT, viewport);
    
    // Setup Scene background color
    CGColor backgroundColor = scene->backgroundColor;
    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    int jitterSize = accumulationSize;
    if((!antiAliasing) || motionBlur) {

        jitterSize = 1;
    } else {

        std::cout << "No Aliasing";
    }
    
    if(!motionBlur) {
        glClearAccum(0.0, 0.0, 0.0, 0.0);

        glClear(GL_ACCUM_BUFFER_BIT);

    }


    for (int jitter = 0; jitter < jitterSize; jitter++) {
        //  Clear screen and Z-buffer
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  //      glClearStencil(0);

        if (motionBlur) {
            glAccum(GL_RETURN, 0.95f);
            glClear(GL_ACCUM_BUFFER_BIT);

        }
        
        glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
        
        glLoadIdentity();
        CGVector3 lookAt = cameraFront + pointOfView->position;
        setCamera(pointOfView->position,  lookAt, cameraUp);
        
        // Setup Jittering
        glPushMatrix();
        if (antiAliasing) {
            
            glTranslatef (j8[jitter].x*4.5/viewport[2],
                          j8[jitter].y*4.5/viewport[3], 0.0);
             
        }
        
        if (stencilBuffering) {
            //renderStencilBuffer();
        }
        
    
        scene->render();
        glPopMatrix();
        if (!motionBlur) {
            glAccum(GL_ACCUM, 1.0/jitterSize);
        }
    }
    
    if (!motionBlur) {
        glAccum(GL_RETURN, 1.0);
    }

 
   renderHUD();
    
    glutSwapBuffers();
}

void setupLights() {
    
    CGSphere *lightSphere = new CGSphere(0.2);
    CGMaterial *emissionMaterial = new CGMaterial();
    emissionMaterial->emission = new CGMaterialProperty(CGColorWhite());
    lightSphere->setMaterial(emissionMaterial);
    
    
    light = new CGLight();
    rightLightNode = new CGNode();
    rightLightNode->position = CGVector3(5, 5, 0);
    rightLightNode->light = light;
    rightLightNode->geometry = lightSphere;
    light->color = CGColorWhite();
    lightSources.push_back(rightLightNode);
    
    CGLight *leftLight = new CGLight();
    leftLight->color = CGColorBlue();
    leftLight->constantAttenuation = 5.0;
    leftLightNode = new CGNode();
    leftLightNode->position = CGVector3(-5, 5, 0);
    leftLightNode->light = leftLight;
    leftLightNode->geometry = lightSphere;
    lightSources.push_back(leftLightNode);

    CGLight *fanLight = new CGLight();
    fanLight->color = CGColorWhite();
    CGNode *fanLightNode = new CGNode();
    fanLightNode->position =  CGVector3(0.7,3.8, 0);
    fanLightNode->light = fanLight;
    //fanLightNode->geometry = lightSphere;
    lightSources.push_back(fanLightNode);

    
    // Spot Light
    spotLightNode = new CGNode();
    spotLightNode->position = CGVector3(0,5,0);
    CGLight *spotlight = new CGLight();
    spotlight->type = CGLightTypeSpot;
    spotLightNode->light = spotlight;
    lightSources.push_back(spotLightNode);

    
//    spotLightNode->geometry = new CGBox(1,1,1);
    
    
    scene->rootNode->addChildNode(rightLightNode);
    scene->rootNode->addChildNode(leftLightNode);
    scene->rootNode->addChildNode(spotLightNode);
    scene->rootNode->addChildNode(fanLightNode);
    
}

void setupCamera() {
    // Setup Camera
    CGCamera *sceneCamera = new CGCamera();
    cameraNode = new CGNode();
    // x, y ,z modifiying in z- is back
    cameraNode->position = CGVector3(0,1,3);
    //cameraNode->position = CGVector3(0,10,15);
    cameraNode->camera = sceneCamera;

    pointOfView = cameraNode;
    
    scene->rootNode->addChildNode(cameraNode);
    
}

void resetCamera() {
    pointOfView->position = CGVector3(0,1,5);
    moveForward = 0;
    cameraYaw = 0;
    
    glutPostRedisplay();
}

CGNode* createChair() {
    
    float zPosition = 0;
    
    // Table
    float tableLeftPosition = -0.25;
    float tableRightPosition = 0.25;
    float tableVerticalPosition = 0;
    
    CGNode *tableNode = new CGNode();
    tableNode->position = CGVector3(0, 0, zPosition);
    //cubeNode->rotation = CGVector4(1, 1, 0, 45);
    
    CGBox *tableComponent = new CGBox(1,1,1);
    tableComponent->setMaterial(CGPresentMaterial::brownMaterial());
    
    
    CGNode *tableTopNode = new CGNode(tableComponent);
    tableTopNode->scale = CGVector3(0.5,0.03,0.5);
    tableTopNode->position = CGVector3(0,0.8,0);
    
    
    CGNode *leftBottomLegNode = new CGNode(tableComponent);
    leftBottomLegNode->scale = CGVector3(0.06,0.8,0.06);
    leftBottomLegNode->position = CGVector3(tableLeftPosition, tableVerticalPosition, 0.25);
    
    CGNode *rightBottomLegNode = new CGNode(*leftBottomLegNode);
    rightBottomLegNode->position = CGVector3(tableRightPosition,tableVerticalPosition,0.25);
    
    CGNode *leftTopLegNode = new CGNode(*leftBottomLegNode);
    leftTopLegNode->position = CGVector3(tableLeftPosition,tableVerticalPosition,-0.25);
    
    CGNode *rightTopLegNode = new CGNode(*leftBottomLegNode);
    rightTopLegNode->position = CGVector3(tableRightPosition,tableVerticalPosition,-0.25);
    
    
    tableNode->addChildNode(tableTopNode);
    tableNode->addChildNode(leftBottomLegNode);
    tableNode->addChildNode(rightBottomLegNode);
    tableNode->addChildNode(leftTopLegNode);
    tableNode->addChildNode(rightTopLegNode);
    
    
    return tableNode;
}

CGNode* createTable() {
    
    float zPosition = 0;

    // Table
    float tableLeftPosition = -0.9;
    float tableRightPosition = 0.9;
    float tableVerticalPosition = -0.1;
    
    CGNode *tableNode = new CGNode();
    tableNode->position = CGVector3(0, 0, zPosition);
    //cubeNode->rotation = CGVector4(1, 1, 0, 45);
    
    CGBox *tableComponent = new CGBox(1,1,1);
    tableComponent->setMaterial(CGPresentMaterial::brownMaterial());
    
    
    CGNode *tableTopNode = new CGNode(tableComponent);
    tableTopNode->scale = CGVector3(1,0.05,1);
    tableTopNode->position = CGVector3(0,0.95,0);
    
    
    CGNode *leftBottomLegNode = new CGNode(tableComponent);
    leftBottomLegNode->scale = CGVector3(0.1,1,0.11);
    leftBottomLegNode->position = CGVector3(tableLeftPosition, tableVerticalPosition, 1);
    
    CGNode *rightBottomLegNode = new CGNode(*leftBottomLegNode);
    rightBottomLegNode->position = CGVector3(tableRightPosition,tableVerticalPosition,1);
    
    CGNode *leftTopLegNode = new CGNode(*leftBottomLegNode);
    leftTopLegNode->position = CGVector3(tableLeftPosition,tableVerticalPosition,-0.8);
    
    CGNode *rightTopLegNode = new CGNode(*leftBottomLegNode);
    rightTopLegNode->position = CGVector3(tableRightPosition,tableVerticalPosition,-0.8);
    
    
    tableNode->addChildNode(tableTopNode);
    tableNode->addChildNode(leftBottomLegNode);
    tableNode->addChildNode(rightBottomLegNode);
    tableNode->addChildNode(leftTopLegNode);
    tableNode->addChildNode(rightTopLegNode);

    
    return tableNode;
}



CGGeometry* mokeyGeometry() {
    
    CGGeometry *geometry = new CGGeometry();
    CGGeometrySource *vertexSource = new CGGeometrySource(monkeyVerts, CGGeometrySourceSemanticVertex, monkeyNumVerts, 3, sizeof(CGVector3));
    geometry->geometrySources.push_back(vertexSource);
    return geometry;
}

CGGeometry* frameGeometry() {
    
    CGGeometry *geometry = new CGGeometry();
    CGGeometrySource *vertexSource = new CGGeometrySource(frameVerts, CGGeometrySourceSemanticVertex, frameNumVerts, 3, sizeof(CGVector3));
    CGGeometrySource *normalSource = new CGGeometrySource(frameNormals, CGGeometrySourceSemanticNormal, frameNumVerts, 3, sizeof(CGVector3));
    CGGeometrySource *uvSource = new CGGeometrySource(frameTexCoords, CGGeometrySourceSemanticTexcoord, frameNumVerts,2 ,sizeof(CGPoint));
    
    geometry->geometrySources.push_back(vertexSource);
    geometry->geometrySources.push_back(normalSource);
    geometry->geometrySources.push_back(uvSource);
    return geometry;
    

}

CGGeometry* fanGeometry() {
    
    CGGeometry *geometry = new CGGeometry();
    CGGeometrySource *vertexSource = new CGGeometrySource(fanVerts, CGGeometrySourceSemanticVertex, fanNumVerts, 3, sizeof(CGVector3));
    CGGeometrySource *normalSource = new CGGeometrySource(fanNormals,CGGeometrySourceSemanticNormal, fanNumVerts,3 , sizeof(CGVector3));
    CGGeometrySource *texCoordSource = new CGGeometrySource(fanTexCoords, CGGeometrySourceSemanticTexcoord, fanNumVerts,2, sizeof(CGPoint));
    
    geometry->geometrySources.push_back(vertexSource);
    geometry->geometrySources.push_back(normalSource);
    geometry->geometrySources.push_back(texCoordSource);
    
    return geometry;
}


CGGeometry *bananaGeometry() {
    CGGeometry *geometry = new CGGeometry();
    CGGeometrySource *vertexSource = new CGGeometrySource(bananaVerts, CGGeometrySourceSemanticVertex, bananaNumVerts, 3, sizeof(CGVector3));
    CGGeometrySource *normalSource = new CGGeometrySource(bananaNormals,CGGeometrySourceSemanticNormal, bananaNumVerts,3 , sizeof(CGVector3));
    CGGeometrySource *texCoordSource = new CGGeometrySource(bananaTexCoords, CGGeometrySourceSemanticTexcoord, bananaNumVerts,2, sizeof(CGPoint));
    
    geometry->geometrySources.push_back(vertexSource);
    geometry->geometrySources.push_back(normalSource);
    geometry->geometrySources.push_back(texCoordSource);
    
    return geometry;
    
    
}
void setupObjects() {
    
    float zPosition = 0;
    
    float roomHeight = 10;
    
    // Table
    CGNode *tableNode = createTable();

    // Chair
    
    CGNode *rightChairNode = createChair();
    rightChairNode->position = CGVector3(2,0,-4);
    
    CGNode *leftChairNode = createChair();
    leftChairNode->position = CGVector3(-2,0,-4);
    
    
    // Front wall
    CGPlane *floor = new CGPlane(100,100);
    CGTexture *floorTexture = new CGTexture("check.bmp");
    CGMaterial *floorMaterial = new CGMaterial();
    floorMaterial->diffuse = new CGMaterialProperty(floorTexture);
    floorMaterial->diffuse->wrapS = CGWrapModeRepeat;
    floorMaterial->diffuse->wrapT = CGWrapModeRepeat;
    floor->setMaterial(floorMaterial);
    
    
    // Floor
    floorNode  = new CGNode(floor);
    floorNode->position = CGVector3(0, 0, zPosition);
    floorNode->eulerAngles = CGVector3(180,0,0);
    
    // Roof
    CGNode *roofNode  = new CGNode(new CGPlane(100,100));
    roofNode->position = CGVector3(0, roomHeight, 0);
    //roofNode->rotation = CGVector4(1, 0, 0, -180);
    
    CGNode *frontWallNode  = new CGNode(new CGPlane(15,roomHeight));
    frontWallNode->position = CGVector3(0, 0, 7.2);
    frontWallNode->rotation = CGVector4(1, 0, 0, 90);
    
    // Back Wall
    CGNode *backWallNode  = new CGNode(new CGPlane(15,roomHeight));
    backWallNode->position = CGVector3(0, 0, -5);
    backWallNode->rotation = CGVector4(1, 0, 0, -90);
    
    // Left Wall
    CGNode *leftWallNode  = new CGNode(new CGPlane(15,roomHeight));
    leftWallNode->position = CGVector3(-7, 0, 0);
    leftWallNode->eulerAngles = CGVector3(90, 0, 90);
    
    // Left Wall
    CGNode *rightWallNode  = new CGNode(new CGPlane(15,roomHeight));
    rightWallNode->position = CGVector3(7, 0, 0);
    rightWallNode->eulerAngles = CGVector3(90, 0, -90);
    
    
    // Teapot
    CGGeometry *teapot = new CGTeapot(0.3);
    teapot->setMaterial(CGPresentMaterial::goldMaterial());
    CGNode *teapotNode = new CGNode(teapot);
    teapotNode->position = CGVector3(0 , 1.15, zPosition);
    
    // Sphere
    CGSphere *sphere = new CGSphere(0.2);
    sphere->setMaterial(CGPresentMaterial::jadeMaterial());
    CGNode *sphereNode = new CGNode(sphere);
    sphereNode->position = CGVector3(-1, 1.2, zPosition);
    
    // Cone
    CGCone *cone = new CGCone(0.15 , 0.3);
    cone->setMaterial(CGPresentMaterial::blueMaterial());
    CGNode *coneNode = new CGNode(cone);
    coneNode->position = CGVector3(1, 1.0, zPosition);
    coneNode->rotation = CGVector4(1,0,0,-90);
    
    // Torus
    CGTorus *torus = new CGTorus(0.1,0.05);
    torus->setMaterial(CGPresentMaterial::copperMaterial());
    CGNode *torusNode = new CGNode(torus);
    
    // Monkey
    CGGeometry *monkeyGeometry = mokeyGeometry();
    monkeyGeometry->setMaterial(CGPresentMaterial::copperMaterial());
    monkeyNode = new CGNode(monkeyGeometry);
    monkeyNode->position =  CGVector3(1, 1.0, zPosition);
    
    torusNode->position = CGVector3(-0.5, 1.2, zPosition);

    // Dodecahedron
    CGDodecahedron *dodecahedron = new CGDodecahedron();
    dodecahedron->setMaterial(CGPresentMaterial::redMaterial());
    CGNode *dodecahedronNode = new CGNode(dodecahedron);
    dodecahedronNode->position = CGVector3(0.7, 1.2, zPosition);
    dodecahedronNode->scale = CGVector3(0.1,0.1,0.1);
    
    // Octahedron
    CGOctahedron *octahedron = new CGOctahedron();
    octahedron->setMaterial(CGPresentMaterial::cyanMaterial());
    CGNode *octahedronNode = new CGNode(octahedron);
    octahedronNode->position = CGVector3(-0.1,1.5, zPosition - 0.5);
    octahedronNode->scale = CGVector3(0.2,0.2,0.2);
    
    // Tetrahedron
    CGTetrahedron *tetrahedron = new CGTetrahedron();
    tetrahedron->setMaterial(CGPresentMaterial::redMaterial());
    CGNode *tetrahedronNode = new CGNode(tetrahedron);
    tetrahedronNode->position = CGVector3(0.7,0.7, zPosition);
    tetrahedronNode->scale = CGVector3(0.3,0.3,0.3);
    
    // Icosahedron
    CGIcosahedron *icosahedron = new CGIcosahedron();
    icosahedron->setMaterial(CGPresentMaterial::rubyMaterial());
    CGNode *icosahedronNode = new CGNode(icosahedron);
    icosahedronNode->position = CGVector3(0.7,0.9, zPosition);
    icosahedronNode->scale = CGVector3(0.3,0.3,0.3);
    
    // Test Cube
    CGCube *cube = new CGCube();
    CGTexture *testTexture = new CGTexture("check.bmp");
    CGMaterial *testMaterial = new  CGMaterial();
    testMaterial->diffuse = new CGMaterialProperty(testTexture);
    testMaterial->diffuse->wrapS = CGWrapModeRepeat;
    testMaterial->diffuse->wrapT = CGWrapModeRepeat;

  //  testTexture->diff
    
    cube->setMaterial(testMaterial);
    
    CGNode *cubeNode = new CGNode(cube);
    cubeNode->position = CGVector3(2,1,-4);
    cubeNode->scale = CGVector3(0.5,0.5,0.5);
    
    
    CGGeometry *banana = bananaGeometry();
    CGTexture *bananaTexture = new CGTexture("banana.jpg");
    banana->setMaterial(new CGMaterial(bananaTexture));
    CGNode *bananaNode = new CGNode(banana);
    bananaNode->position = CGVector3(0.7,2, zPosition);
   
    
    // Frame
    float verticalPosition = 2.5;
    
    CGMaterial *frameMaterial = new CGMaterial();
    CGTexture *texture = loadScreenshotTexture();
    
    if(texture != NULL) {
        screenshotMaterialProperty = new CGMaterialProperty(texture);
        
        frameMaterial->diffuse = screenshotMaterialProperty;
        std::cout << "FrameMaterial Address: " << frameMaterial->diffuse << std::endl;
    }
    CGGeometry *frameGeo = frameGeometry();
    frameGeo->setMaterial(frameMaterial);
    
    CGNode *frameNode = new CGNode(frameGeo);
    frameNode->position =  CGVector3(6, verticalPosition, -2);
    frameNode->eulerAngles = CGVector3(0, 0, 180);
    frameNode->scale = CGVector3(2,2,2);
    
    
    CGGeometry *frameGeo2 = frameGeometry();
    frameGeo2->setMaterial(new CGMaterial(new CGTexture("threemusicians.jpg")));
    
    CGGeometry *frameGeo3 = frameGeometry();
    frameGeo3->setMaterial(new CGMaterial(new CGTexture("bliss.jpg")));
    
    
    CGNode *frameNode2 = new CGNode(frameGeo2);
    frameNode2->position =  CGVector3(6, verticalPosition, 1);
    frameNode2->eulerAngles = CGVector3(0, 0, 180);
    frameNode2->scale = CGVector3(2,2,2);
    
    CGNode *frameNode3 = new CGNode(frameGeo3);
    frameNode3->position =  CGVector3(6, verticalPosition, 4);
    frameNode3->eulerAngles = CGVector3(0, 0, 180);
    frameNode3->scale = CGVector3(2,2,2);
    
    
    CGGeometry *fan = fanGeometry();
    CGTexture *fanTexture = new  CGTexture(new std::string("Laminate.bmp"));
    CGMaterial *fanMaterial = new CGMaterial();
    fanMaterial->diffuse = new CGMaterialProperty(fanTexture);
    fan->setMaterial(fanMaterial);
    
    
    fanNode = new CGNode(fan);
    fanNode->position = CGVector3(0.7,4, zPosition);
    fanNode->scale = CGVector3(3,3,3);
    
    // Setup Room
    scene->rootNode->addChildNode(floorNode);
    scene->rootNode->addChildNode(roofNode);
    scene->rootNode->addChildNode(frontWallNode);
    scene->rootNode->addChildNode(backWallNode);
    scene->rootNode->addChildNode(leftWallNode);
    scene->rootNode->addChildNode(rightWallNode);
    
    scene->rootNode->addChildNode(leftChairNode);
    scene->rootNode->addChildNode(rightChairNode);
    
    scene->rootNode->addChildNode(tableNode);

    // Add ornaments
    
    scene->rootNode->addChildNode(teapotNode);
    scene->rootNode->addChildNode(sphereNode);
    scene->rootNode->addChildNode(coneNode);
    
    scene->rootNode->addChildNode(torusNode);
    scene->rootNode->addChildNode(dodecahedronNode);
    scene->rootNode->addChildNode(octahedronNode);
    scene->rootNode->addChildNode(tetrahedronNode);

    scene->rootNode->addChildNode(icosahedronNode);
    scene->rootNode->addChildNode(monkeyNode);
    
    // Add Frames
    scene->rootNode->addChildNode(frameNode);
    scene->rootNode->addChildNode(frameNode2);
    scene->rootNode->addChildNode(frameNode3);
    scene->rootNode->addChildNode(fanNode);
    scene->rootNode->addChildNode(bananaNode);
    
    
    scene->rootNode->addChildNode(cubeNode);
    
}

void setupScene() {
    
    scene = new CGScene();
    
    scene->backgroundColor = CGColorWhite();

    setupCamera();
    
    setupObjects();
    
    setupLights();

}

void initOpenGL() {
    
    glutDisplayFunc(render);
    glutReshapeFunc(resize);
 //   glutMouseFunc(mouseHandler);
 //   glutMouseFunc(mouseHandler);
    glutPassiveMotionFunc(mouseDidMove);
    glutIdleFunc(myIdleFunc);
    glEnable(GL_MULTISAMPLE_ARB);
    
    // Enable Texturing
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_TEXTURE_2D);	// enable 2D texturing
    
    glClearDepth(1.0f);       // Set background depth to farthest
 
    glEnable(GL_LIGHTING);    //enable lighting
    glEnable(GL_DEPTH_TEST);  // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
    
    // Setup Blending
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);   // which blending function to use
    glEnable(GL_BLEND);       // enable blending
    
    glShadeModel(shadeModel);   // Enable smooth shading
    glutKeyboardFunc(keyboardHandler);
    glutSpecialFunc(specialKeyHandler);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections

    setupScene();
    setNightMode(false);
    setDisplayMode(CGDisplayModeDefault);
    setModifyMode(CGModifyModeNone);
    
}

int main(int argc, char * argv[]) {
    
#ifdef __APPLE__
    //Init glut passing some args
    glutInit(&argc, argv);
#endif
    
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_DOUBLE | GLUT_ACCUM );      //requests properties for the window (sets up the rendering context)

    //Specify the Display Mode, this one means there is a single buffer and uses RGB to specify colors
    // glutInitDisplayMode(GLUT_DEPTH| GLUT_DOUBLE |GLUT_RGB);
    
    //Set the window size
    glutInitWindowSize(800, 600);
    
    //Where do we want to place the window initially?
    glutInitWindowPosition(100,100);
    

    
    
    //Name the window and create it
    mainWindow = glutCreateWindow("Art Gallery");
    glutSetWindow(mainWindow);

    //  Enable Z-buffer depth test
    glEnable(GL_DEPTH_TEST);
    
    helpString = new std::string("s - Shading, w - Wire Mode, l - lighting, n - night, h - Help, m - Color Tracking, c - screenshot, r - Reset Camera");
    showInstructions = true;
    initOpenGL();
    
    
    
    //Start the main loop running, nothing after this will execute for all eternity (right now)
    glutMainLoop();
    
    return 0;
}