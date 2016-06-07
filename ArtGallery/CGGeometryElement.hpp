//
//  CGGeometryElement.hpp
//  GLUT3DApp
//
//  Created by Ben Johnson on 9/05/2016.
//  Copyright © 2016 Benjamin Johnson. All rights reserved.
//

#ifndef CGGeometryElement_hpp
#define CGGeometryElement_hpp

#include <stdio.h>
#include <vector>

class CGGeometryElement {
    
    
public:
    int primitiveCount;
    
    int bytesPerIndex;
    
    void* data;
    
    CGGeometryElement(void* data, int primitiveCount, int bytesPerIndex) {
        
        this->data = data;
        
        this->primitiveCount = primitiveCount;
        
        this->bytesPerIndex = bytesPerIndex;
        
    }
    
    CGGeometryElement(std::vector<unsigned int> *data, int primitiveCount) {
        
        this->data = &data[0];
        
        this->primitiveCount = primitiveCount;
        
        this->bytesPerIndex = sizeof(unsigned int);
        
    }
    
};

#endif /* CGGeometryElement_hpp */
