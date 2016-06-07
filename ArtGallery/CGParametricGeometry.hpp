//
//  CGBox.hpp
//  GLUT3DApp
//
//  Created by Ben Johnson on 8/05/2016.
//  Copyright © 2016 Benjamin Johnson. All rights reserved.
//

#ifndef CGBox_hpp
#define CGBox_hpp

#include <stdio.h>
#include "CGGeometry.hpp"
#ifdef __APPLE__
#include <GLUT/GLUT.h> //GLUT Library, will make you life easier
#include <OpenGL/OpenGL.h> //OpenGL Library
#elif defined _WIN32 || defined _WIN64
#include <glut.h>
#endif

class CGBox: public CGGeometry {
    
    
public:
    
    float width;
    
    float height;
    
    float length;
    
    CGBox(float width, float height, float length) {
        
        this->width = width;
        
        this->height = height;
        
        this->length = length;
        
    }
    
    void presentationGeometry();
};

class CGPlane: public CGGeometry {
    
public:
    
    float width;
    
    float height;
    
    int widthSegmentCount;
    
    int heightSegmentCount;
    
    CGPlane(float width, float height) {
        
        this->width = width;
        
        this->height = height;
        
        widthSegmentCount = 1;
        
        heightSegmentCount = 1;
      //  glutsolid
    }
    
    void presentationGeometry();
};

class CGPyramid: public CGGeometry {
    
public:
    
    float width;
    
    float height;
    
    float length;
    
    CGPyramid(float width, float height, float length) {
        
        
    }
    
    void presentationGeometry();

};

class CGSphere: public CGGeometry {
    
    float radius;
    
    int segmentCount;
    
public:
    
    CGSphere(float radius) {
        this->radius = radius;
        segmentCount = 48;
        
    }
    
    void presentationGeometry();
    
};

class CGTeapot: public CGGeometry {
    
    float size;
    
public:
    
    CGTeapot(float size) {
        
        this->size = size;
        
    }
    
    void presentationGeometry();
};

class CGCone: public CGGeometry {
    
public:
    
    float bottomRadius;
    
    float height;
    
    int heightSegmentCont;
    
    int radialSegmentCount;
    
    CGCone(float bottomRadius, float height) {
        
        this->bottomRadius = bottomRadius;
        
        this->height = height;
        
        heightSegmentCont = 1;
        
        radialSegmentCount = 48;
        
    }
    
    void presentationGeometry() {
    
        glutSolidCone(bottomRadius, height, radialSegmentCount, heightSegmentCont);
    }
};

class CGDodecahedron: public CGGeometry {
    
public:
    
    CGDodecahedron() {}
    
    void presentationGeometry() {
        
        glutSolidDodecahedron();
        
    }
};

class CGOctahedron: public CGGeometry {
    
public:
    
    CGOctahedron() {}
    
    void presentationGeometry() {
        
        glutSolidOctahedron();
        
    }
};

class CGTetrahedron: public CGGeometry {
    
public:
    
    CGTetrahedron() {}
    
    void presentationGeometry() {
        
        glutSolidTetrahedron();
        
    }
};

class CGIcosahedron: public CGGeometry {
    
public:
    
    CGIcosahedron() {}
    
    void presentationGeometry() {
        
        glutSolidIcosahedron();
        
    }
};


class CGTorus: public CGGeometry {
    
    
public:
    
    float ringRadius;
    
    float pipeRadius;
    
    int ringSegmentCount;
    
    int pipeSegmentCount;
    
    CGTorus(float ringRadius, float pipeRadius) {
        
        this->ringRadius = ringRadius;
        
        this->pipeRadius = pipeRadius;
        
        ringSegmentCount = 48;
        
        pipeSegmentCount = 24;
        
    }
    
    void presentationGeometry() {
        glutSolidTorus(pipeRadius, ringRadius, pipeSegmentCount, ringSegmentCount);
    }
};


#endif /* CGBox_hpp */
