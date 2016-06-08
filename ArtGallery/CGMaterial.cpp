//
//  CGMaterial.cpp
//  GLUT3DApp
//
//  Created by Ben Johnson on 9/05/2016.
//  Copyright © 2016 Benjamin Johnson. All rights reserved.
//

#include "CGMaterial.hpp"

#ifdef __APPLE__
#include <GLUT/GLUT.h> //GLUT Library, will make you life easier
#include <OpenGL/OpenGL.h> //OpenGL Library
#elif defined _WIN32 || defined _WIN64
#    include <glut.h>
#endif


void CGMaterial::loadTexture(CGTexture *texture) {
    // Check if existing buffer'
    
    
    if(textureID > 0) {
        glBindTexture(GL_TEXTURE_2D, textureID);       // bind texture type to ID
    } else {
        glGenTextures(1, &textureID);                  // generate texture names
        glBindTexture(GL_TEXTURE_2D, textureID);       // bind texture type to ID
        
        // generate texture mipmaps
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture->size.width, texture->size.height, GL_RGB, GL_UNSIGNED_BYTE, texture->pixelData);
    }
  
    

    
}

void CGMaterial::loadMaterialProperty(CGMaterialProperty *property, GLenum type) {
    
    GLenum faceType = doubleSided ? GL_FRONT_AND_BACK : GL_FRONT;

    if (property->content != NULL) {
        loadTexture(property->content);
        property->loadContent();
    } else {
    }
    
    if (!property->color.isEmpty()){
        CGColor diffuseColor = property->color;
        float diffuseValues[] = {diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a};
        glMaterialfv(faceType, type, diffuseValues);
    }
    
    
}

void CGMaterial::loadMaterial() {
    
    glBindTexture(GL_TEXTURE_2D, 0);       // bind texture type to ID
    
    GLenum faceType = doubleSided ? GL_FRONT_AND_BACK : GL_FRONT;

    loadMaterialProperty(diffuse, GL_DIFFUSE);
    
    loadMaterialProperty(ambient, GL_AMBIENT);
    
    loadMaterialProperty(specular, GL_SPECULAR);
    
    loadMaterialProperty(emission, GL_EMISSION);
    
    glMaterialf(faceType, GL_SHININESS, shininess);
    
    
 }