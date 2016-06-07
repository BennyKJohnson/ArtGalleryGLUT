//
//  CGMaterial.hpp
//  GLUT3DApp
//
//  Created by Ben Johnson on 9/05/2016.
//  Copyright © 2016 Benjamin Johnson. All rights reserved.
//

#ifndef CGMaterial_hpp
#define CGMaterial_hpp

#include <stdio.h>
#include "CG.hpp"
#include <string.h>
#include "CGMaterialProperty.hpp"


class CGMaterial {
    
    void loadTexture(CGTexture *texture);
    
    void loadMaterialProperty(CGMaterialProperty *property, GLenum type);

public:
    
    std::string *name;
    
    bool locksAmbientWithDiffuse;
    
    CGMaterialProperty *diffuse;
    
    CGMaterialProperty *ambient;
    
    CGMaterialProperty *specular;
    
    CGMaterialProperty *emission;
    
    float shininess;
    
    bool doubleSided;
    
    GLuint textureID;           //texture id

    
    CGMaterial() {
        
        locksAmbientWithDiffuse = true;
        
        ambient = new CGMaterialProperty(CGColor(0.2, 0.2, 0.2, 1));
        
        diffuse = new CGMaterialProperty(CGColor(1.0, 1.0, 1.0, 1.0));
        
        specular = new CGMaterialProperty(CGColor(0, 0, 0, 1));
        
        emission = new CGMaterialProperty(CGColor(0, 0 , 0, 1));
        
        shininess = 0;
        
        doubleSided = false;
        
    }
    
    void loadMaterial();
};

#endif /* CGMaterial_hpp */
