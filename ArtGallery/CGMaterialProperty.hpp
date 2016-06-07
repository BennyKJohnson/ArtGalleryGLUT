//
//  CGMaterialProperty.hpp
//  ArtGallery
//
//  Created by Benjamin Johnson on 28/05/2016.
//  Copyright © 2016 Benjamin Johnson. All rights reserved.
//

#ifndef CGMaterialProperty_hpp
#define CGMaterialProperty_hpp

#include <stdio.h>
#include "CG.hpp"
#include "CGTexture.hpp"

#ifdef __APPLE__
#include <GLUT/GLUT.h> //GLUT Library, will make you life easier
#include <OpenGL/OpenGL.h> //OpenGL Library
#elif defined _WIN32 || defined _WIN64
#    include <glut.h>
#endif

typedef enum CGWrapMode {
    CGWrapModeClamp,
    CGWrapModeRepeat,
    CGWrapModeClampToBorder,
    CGWrapModeMirror
} CGWrapMode;

typedef enum CGFilterMode {
    CGFilterModeNearest,
    CGFilterModeLinear
} CGFilterMode;


class CGMaterialProperty {
    
    
public:
    
    CGWrapMode wrapS;
    
    CGWrapMode wrapT;
    
    CGFilterMode magnificationFilter;
    
    CGFilterMode minificationFilter;
    
    CGTexture *content;
    
    CGColor color;
    
    CGColor borderColor;

    
    CGMaterialProperty(CGTexture* content) {
        
        wrapS = CGWrapModeClamp;
        wrapT = CGWrapModeClamp;
        magnificationFilter = CGFilterModeNearest;
        minificationFilter = CGFilterModeNearest;
        borderColor = CGColor();
        
        this->content = content;
        
    }
    
    CGMaterialProperty(CGColor color) {
        this->color = color;
        content = NULL;
        
    }
    
    
    
    void loadFilter(GLenum filterType, CGFilterMode filter) {
        
        GLenum openGLFilter = GL_NEAREST;
        switch (openGLFilter) {
            case CGFilterModeNearest:
                openGLFilter = GL_NEAREST;
                break;
            case CGFilterModeLinear:
                openGLFilter = GL_LINEAR;
                break;
        }
        
        glTexParameteri(GL_TEXTURE_2D, filterType, openGLFilter);	// Set the preferences

        
    }
    
    void loadWrap(GLenum coordinate, CGWrapMode wrapMode) {
        
        GLenum openGLWrapMode = GL_CLAMP;
        
        switch (wrapMode) {
            case CGWrapModeClamp:
                openGLWrapMode = GL_CLAMP;
                break;
            case CGWrapModeMirror:
                openGLWrapMode = GL_MIRRORED_REPEAT;
                break;
            case CGWrapModeRepeat:
                openGLWrapMode = GL_REPEAT;
                break;
            case CGWrapModeClampToBorder:
                openGLWrapMode = GL_CLAMP_TO_BORDER;
                break;
        }
        
        glTexParameteri(GL_TEXTURE_2D, coordinate, openGLWrapMode);	// Set the preferences

    }
    
    void loadContent() {
            // Load Texture
            
            // Setup Wraps
            loadWrap(GL_TEXTURE_WRAP_S, wrapS);
            loadWrap(GL_TEXTURE_WRAP_T, wrapT);
            
            // Setup Filters
            loadFilter(GL_TEXTURE_MAG_FILTER, magnificationFilter);
            loadFilter(GL_TEXTURE_MIN_FILTER, minificationFilter);
            
            // Load Border Color
            if (!borderColor.isEmpty()) {
                float color[] = {borderColor.r, borderColor.g, borderColor.b, borderColor.a};
                glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
            }
   
        
    }
};

#endif /* CGMaterialProperty_hpp */
