//
//  CGTexture.hpp
//  TextureTest
//
//  Created by Benjamin Johnson on 4/06/2016.
//  Copyright © 2016 Benjamin Johnson. All rights reserved.
//

#ifndef CGTexture_hpp
#define CGTexture_hpp

#include <string>
#include "CG.hpp"

#include <stdio.h>
#include <stdlib.h>
#include "SOIL.h"
#ifdef __APPLE__
#include <GLUT/GLUT.h> //GLUT Library, will make you life easier
#include <OpenGL/OpenGL.h> //OpenGL Library
#elif defined _WIN32 || defined _WIN64
#include <glut.h>
#endif

class CGTexture {
    
    unsigned char* loadBMPFile(const char *filename);

    
public:
    
    CGSize size;
    
    unsigned char* pixelData;
    
    GLuint textureID;
    
    
    void loadFile(const char  *filename) {
        
        int channels = 0;
        int width;
        int height;
        
        /*
        unsigned char *data = SOIL_load_image
        (
         filename,
         &width, &height, &channels,
         SOIL_LOAD_AUTO
         );
        
        pixelData = data;
        
        size.width = (float)width;
        size.height = (float)height;
        
       // pixelData = loadBMPFile(filename);
         
         */
        
        textureID = SOIL_load_OGL_texture
        (
         filename,
         SOIL_LOAD_AUTO,
         SOIL_CREATE_NEW_ID,
         SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
         );
        
        /* check for an error during the load process */
        if( 0 == textureID )
        {
            printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
        }
        
    }
    
    CGTexture(const char  *filename) {
        textureID = 0;

        loadFile(filename);
        
    }
    
    CGTexture(std::string *filename) {
        textureID = 0;
        loadFile(filename->c_str());

        
        //loadBMPFile(filename->c_str());
    }
    
    ~CGTexture() {
        SOIL_free_image_data( pixelData);
    }
    
};


#endif /* CGTexture_hpp */
