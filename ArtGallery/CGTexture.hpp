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

class CGTexture {
    
    unsigned char* loadBMPFile(const char *filename);

    
public:
    
    CGSize size;
    unsigned char* pixelData;
    
    
    CGTexture(std::string *filename) {
        
        int channels = 0;
        int width;
        int height;
        
        unsigned char *data = SOIL_load_image
        (
         filename->c_str(),
         &width, &height, &channels,
         SOIL_LOAD_AUTO
         );
        
        pixelData = data;
        
        size.width = (float)width;
        size.height = (float)height;
        
        //loadBMPFile(filename->c_str());
    }
    
    ~CGTexture() {
        SOIL_free_image_data( pixelData);
    }
    
};


#endif /* CGTexture_hpp */
