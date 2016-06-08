//
//  CGScene.hpp
//  GLUT3DApp
//
//  Created by Ben Johnson on 8/05/2016.
//  Copyright © 2016 Benjamin Johnson. All rights reserved.
//

#ifndef CGScene_hpp
#define CGScene_hpp

#include <stdio.h>
#ifdef __APPLE__
#include <GLUT/GLUT.h> //GLUT Library, will make you life easier
#include <OpenGL/OpenGL.h> //OpenGL Library
#elif defined _WIN32 || defined _WIN64
#include <glut.h>
#endif

#include "CGNode.hpp"

class CGScene {
    
    void renderNode(CGNode *node);
    
    int lightCount;
    
    GLenum currentLightID();
    
    
    
public:
    CGColor globalAmbientColor;

    bool autoenablesDefaultLighting;
    
    
    CGNode *rootNode;
    
    void render();
    
    CGColor backgroundColor;
    
    CGColor color;
    
    CGScene();
    
    
};


#endif /* CGScene_hpp */
