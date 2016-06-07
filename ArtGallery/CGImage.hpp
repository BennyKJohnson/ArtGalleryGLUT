//
//  CGImage.hpp
//  ArtGallery
//
//  Created by Benjamin Johnson on 28/05/2016.
//  Copyright © 2016 Benjamin Johnson. All rights reserved.
//

#ifndef CGImage_hpp
#define CGImage_hpp

#include <iostream>
#include <string>

class CGImage {
    
    void loadBitmapFromFile(const char* filename);

public:
    
    int width;
    
    int height;
    
    int size;
    
    unsigned char** imageData;
    
    
    CGImage(std::string *filename) {
        
        width = -1;
        
        height = -1;
        
        size = -1;
        
        loadBitmapFromFile(filename->c_str());
    
    }
    
    
};

#endif /* CGImage_hpp */
