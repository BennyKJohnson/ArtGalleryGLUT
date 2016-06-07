//
//  CGPresetMaterials.hpp
//  GLUT3DApp
//
//  Created by Ben Johnson on 12/05/2016.
//  Copyright © 2016 Benjamin Johnson. All rights reserved.
//

#ifndef CGPresetMaterials_hpp
#define CGPresetMaterials_hpp

#include <stdio.h>
#include "CGMaterial.hpp"
#include "CG.hpp"

class CGPresentMaterial {
    
    
public:
    
    static CGMaterial* cyanMaterial() {
        
        CGMaterial *material = new CGMaterial();
        material->diffuse = new CGMaterialProperty(CGColorSimpleCyan());
        
        return material;
    }
    
    static CGMaterial* redMaterial() {
        
        CGMaterial *material = new CGMaterial();
        material->diffuse = new CGMaterialProperty(CGColorRed());
        
        return material;
    }
    
    static CGMaterial* blueMaterial() {
        
        CGMaterial *material = new CGMaterial();
        material->diffuse = new CGMaterialProperty(CGColorBlue());
        
        return material;
        
    }
    
    static CGMaterial* brownMaterial() {
        
        CGMaterial *material = new CGMaterial();
        material->diffuse = new CGMaterialProperty(CGColorBrown());
        
        return material;
        
    }
    
    static CGMaterial* orangeMaterial() {
        
        CGMaterial *material = new CGMaterial();
        material->diffuse = new CGMaterialProperty(CGColorRed());
        
        return material;
        
    }
    
    static CGMaterial* goldMaterial() {
        
        CGMaterial *material = new CGMaterial();
        material->locksAmbientWithDiffuse = false;
        
        CGColor ambient = CGColor(0.27425, 0.1995, 0.0745, 1.0);
        CGColor diffuse = CGColor(0.75164,0.60648,0.22648,1.0);
        CGColor specular = CGColor(0.628,0.555,0.366,1.0);
        float shiness = 0.4 * 128.0;
        
        material->ambient = new CGMaterialProperty(ambient);
        material->diffuse = new CGMaterialProperty(diffuse);
        material->specular = new CGMaterialProperty(specular);
        material->shininess = shiness;
        material->name = new std::string("Gold");
        
        return material;
    }
    
    
    static CGMaterial* jadeMaterial() {
        
        CGMaterial *material = new CGMaterial();
        material->locksAmbientWithDiffuse = false;
        
        CGColor ambient = CGColor( 0.135f, 0.2225f, 0.1575f, 0.95f);
        CGColor diffuse = CGColor(0.54f, 0.89f, 0.63f, 0.95f);
        CGColor specular = CGColor(0.316228f, 0.316228f, 0.316228f, 0.95f );
        float shiness = 12.8;
        
        material->ambient = new CGMaterialProperty(ambient);
        material->diffuse = new CGMaterialProperty(diffuse);
        material->specular = new CGMaterialProperty(specular);
        material->shininess = shiness;
        material->name = new std::string("Jade");
        
        return material;
    }
    
    static CGMaterial* rubyMaterial() {
        
        CGMaterial *material = new CGMaterial();
        material->locksAmbientWithDiffuse = false;
        
        CGColor ambient = CGColor(0.1745f, 0.01175f, 0.01175f, 0.55f);
        CGColor diffuse = CGColor(0.61424f, 0.04136f, 0.04136f, 0.55f);
        CGColor specular = CGColor(0.61424f, 0.04136f, 0.04136f, 0.55f);
        float shiness = 76.8;
        
        material->ambient = new CGMaterialProperty(ambient);
        material->diffuse = new CGMaterialProperty(diffuse);
        material->specular = new CGMaterialProperty(specular);
        material->shininess = shiness;
        material->name = new std::string("Ruby");
        
        return material;
    }
    
    static CGMaterial* copperMaterial() {
        
        CGMaterial *material = new CGMaterial();
        material->locksAmbientWithDiffuse = false;
        
        CGColor ambient = CGColor(0.19125f, 0.0735f, 0.0225f, 1.0f);
        CGColor diffuse = CGColor(0.19125f, 0.0735f, 0.0225f, 1.0f);
        CGColor specular = CGColor(0.19125f, 0.0735f, 0.0225f, 1.0f);
        float shiness = 12.8;
        
        material->ambient = new CGMaterialProperty(ambient);
        material->diffuse = new CGMaterialProperty(diffuse);
        material->specular = new CGMaterialProperty(specular);
        material->shininess = shiness;
        material->name = new std::string("Copper");
        
        return material;
    }
    
};

#endif /* CGPresetMaterials_hpp */
