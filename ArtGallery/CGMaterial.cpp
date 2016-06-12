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
#include <iostream>

void CGMaterial::loadTexture(CGTexture *texture) {
    // Check if existing buffer'
    
    GLuint textureID = texture->textureID;
    if(textureID > 0) {
        glBindTexture(GL_TEXTURE_2D, textureID);       // bind texture type to ID
    } else if(true) {
        // OLD TEXTURE LOADING CODE GLUBUILD2DMIPMAPS CAUSING ISSUES
        
        glGenTextures(1, &textureID);                  // generate texture names
        glBindTexture(GL_TEXTURE_2D, textureID);       // bind texture type to ID
        std::cout << "Generated Texture Buffer with ID: " << textureID << std::endl;

        // generate texture mipmaps
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture->size.width, texture->size.height, GL_RGB, GL_UNSIGNED_BYTE, texture->pixelData);
    } else {
        std::cout << "Failed to load texture" << std::endl;
        //exit(0);
    }
  
    

    
}

void CGMaterial::loadMaterialProperty(CGMaterialProperty *property, GLenum type) {
    
    GLenum faceType = doubleSided ? GL_FRONT_AND_BACK : GL_FRONT;

    if (property->content != NULL && CGMaterial::texturesEnabled) {
        
        loadTexture(property->content);
        property->loadContent();
        CGColor diffuseColor = CGColorWhite();

        float diffuseValues[] = {diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a};
        glMaterialfv(faceType, type, diffuseValues);
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