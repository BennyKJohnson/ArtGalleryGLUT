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
    
};

#endif /* CGLight_hpp */
