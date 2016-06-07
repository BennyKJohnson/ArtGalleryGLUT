//
//  CGCamera.hpp
//  GLUT3DApp
//
//  Created by Ben Johnson on 8/05/2016.
//  Copyright © 2016 Benjamin Johnson. All rights reserved.
//

#ifndef CGCamera_hpp
#define CGCamera_hpp

#include <stdio.h>

class CGCamera {
    
    
public:
    
    double zNear;
    
    double zFar;
    
    double yFov;
    
    double xFov;
    
    bool usesOrthographicProjection;
    
    CGCamera() {
        
        this->zNear = 1.0;
        
        this->zFar = 100.0;
        
        this->yFov = 45.0;
        
        usesOrthographicProjection = false;
        
    }
    
};

#endif /* CGCamera_hpp */
