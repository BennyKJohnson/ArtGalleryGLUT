//
//  CGLight.hpp
//  GLUT3DApp
//
//  Created by Ben Johnson on 8/05/2016.
//  Copyright © 2016 Benjamin Johnson. All rights reserved.
//

#ifndef CGLight_hpp
#define CGLight_hpp

#include <stdio.h>
#include "CG.hpp"
#ifdef __APPLE__
#include <GLUT/GLUT.h> //GLUT Library, will make you life easier
#include <OpenGL/OpenGL.h> //OpenGL Library
#elif defined _WIN32 || defined _WIN64
#include <glut.h>
#endif


typedef enum CGLightType {
    CGLightTypeAmbient,
    CGLightTypeOmni,
    CGLightTypeDirectional,
    CGLightTypeSpot
} CGLightType;

class CGLight {
    
    
public:
    
    CGColor color;
    
    CGLightType type;
    
    void render();
    
    float spotOuterAngle;
    
    float attenuationFalloffExponent;
    
    float constantAttenuation;
    
    float linearAttenuation;
    
    CGLight() {
    
        color = CGColorSimpleYellow();
        
        type = CGLightTypeAmbient;
        
        spotOuterAngle = 45.0;
        
        attenuationFalloffExponent = 0.0;
        
        constantAttenuation = 1.5; // Smaller the brighter
        
        linearAttenuation = 0.0; // Smaller the brighter
    }
    
    static GLenum lightIDForIndex(int index) {
        switch (index) {
            case 0:
                return GL_LIGHT0;
            case 1:
                return GL_LIGHT1;
            case 2:
                return GL_LIGHT2;
            case 3:
                return GL_LIGHT3;
            case 4:
                return GL_LIGHT4;
            case 5:
                return GL_LIGHT5;
            case 6:
                return GL_LIGHT6;
            case 7:
                return GL_LIGHT7;
            default:
                return GL_LIGHT7;
        }
    }
    
};

#endif /* CGLight_hpp */
