//
//  CGGeometrySource.hpp
//  GLUT3DApp
//
//  Created by Ben Johnson on 9/05/2016.
//  Copyright © 2016 Benjamin Johnson. All rights reserved.
//

#ifndef CGGeometrySource_hpp
#define CGGeometrySource_hpp

#include <stdio.h>
#include "CG.hpp"

typedef enum CGGeometrySourceSemantic {
    CGGeometrySourceSemanticVertex,
    CGGeometrySourceSemanticNormal,
    CGGeometrySourceSemanticColor,
    CGGeometrySourceSemanticTexcoord
} CGGeometrySourceSemantic;


class CGGeometrySource {
    
public:
    
    int vectorCount;
    
    void* data;
    
    int bytesPerComponent;
    
    int componentsPerVector;
    
    CGGeometrySourceSemantic semantic;
    
    CGGeometrySource(CGVector3 *data, int count, int componentsPerVector) {

        this->data = data;
        this->vectorCount = count;
        this->semantic = semantic;
        this->bytesPerComponent = sizeof(CGVector3);
        this->componentsPerVector = 3;
    }
    
    CGGeometrySource(void *data, CGGeometrySourceSemantic semantic, int vectorCount,int componentsPerVector, int bytesPerComponent) {
     
        this->data = data;
        this->semantic = semantic;
        this->vectorCount = vectorCount;
        this->bytesPerComponent = bytesPerComponent;
        this->componentsPerVector = componentsPerVector;
        
    }
};

#endif /* CGGeometrySource_hpp */
