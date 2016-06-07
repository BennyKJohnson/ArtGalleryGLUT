//
//  CGGeometry.hpp
//  GLUTGraphApplication
//  Helper structs and methods for 2D drawing in OpenGL
//  Created by Benjamin Johnson on 19/03/2016.
//  Copyright © 2016 Benjamin Johnson. All rights reserved.
//

#ifndef CG_hpp
#define CG_hpp

#include <stdio.h>
#include <string>
#include <vector>

typedef enum CGPolygonMode {
    CGPolygonModeSolid,
    CGPolygonModeWire
} CGPolygonMode;


typedef struct CGVector3 {
    
    float x,y,z;
    
    CGVector3() {
        x = 0;
        y = 0;
        z = 0;
    }
    
    CGVector3(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    
    bool operator==(const CGVector3& rhs) const
    {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }
    
} CGVector3;

typedef struct CGVector4 {
    float x,y,z,w;
    
    CGVector4() {
        x = 0;
        y = 0;
        z = 0;
        w = 0;
    }
    
    CGVector4(float x, float y, float z, float w) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }
    
} CGVector4;

CGVector3 CGVector3Make(float x, float y, float z);

// Simple struct for a rectangle area represented by x,y and width and heigh
// struct to represent a coordinate
typedef struct CGPoint {
    float x;
    float y;
    bool operator<(const CGPoint& rhs) const
    {
        return y <= rhs.y;
    }
    
} CGPoint;

typedef struct CGSize {
    float width;
    float height;
} CGSize;

typedef struct CGRect {
    CGPoint origin;
    CGSize size;
} CGRect;


// Returns a rectangle with the coordinate and size
CGRect CGRectMake(float x, float y, float width, float height);
void translate(CGVector3 vector);

CGPoint CGPointMake(float x, float y);

// Simple struct for color rgba values
typedef struct CGColor {
    float r;
    float g;
    float b;
    float a;
    
    CGColor(float r, float g, float b, float a) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }
    
    CGColor() {
        this->r = 0;
        this->g = 0;
        this->b = 0;
        this->a = 0;
    }
    
    bool isEmpty() {
        if(r == 0 && g == 0 && b == 0 && a == 0) {
            return true;
        }
        return false;
    }
    
    bool operator==(const CGColor& rhs) const
    {
        return r == rhs.r && g == rhs.g && b == rhs.b;
    }
    bool operator!=(const CGColor& rhs) const
    {
        return !operator==(rhs);
    }
    
} CGColor;

// Convenience Constructor with rgb
CGColor CGColorMakeWithRGB(int r,int g,int b);
CGColor CGColorMakeWithRGB(char r,char g, char b);

// Sets OpenGL current color to CGColor values
void setContextColor(CGColor color);
void checkForGLError();
// Convenience Init for colors
// Basic colors
CGColor CGColorRed();
CGColor CGColorBlue();
CGColor CGColorGreen();
CGColor CGColorBlack();
CGColor CGColorWhite();
CGColor CGColorBrown();
CGColor CGColorClearColor();

CGColor statusBarColor();
// Colors for main color scheme
CGColor CGColorSimpleCyan();
CGColor CGColorSimpleYellow();
CGColor CGColorSimpleOrange();
CGColor CGColorSimpleGreen();
CGColor CGColorSimpleRed();
CGColor CGColorSimpleBlue();

std::vector<CGColor> simpleColorScheme();

// Converts degrees to radians
float degreesToRadians(float degree);

CGPoint getCenter(CGRect rect);

// Moves rect to center on its original origin
CGRect offsetRectToCenterOnOrigin(CGRect rect);
bool hitTestWithRect(CGRect rect, CGPoint point);

// Draw Helper Methods
void drawPoint(CGPoint point, CGColor color);

void drawLine(CGPoint fromPoint, CGPoint toPoint, float lineWidth);
// Draws Circle within rect with color
void drawCircle(CGPoint center, float radius, CGColor color);
void drawCircle(CGRect rect, CGColor color);
// Draws rectangle of color within the coordinate and size
void drawRect(CGRect rect, CGColor color);
// Draws string at x,y with font
void renderBitmapString(int x, int y, std::string *string, void *font);

CGRect centerAtPoint(CGPoint center, CGRect rect);

CGColor colorOfPixelAtPoint(CGPoint point);


#endif /* CGGeometry_hpp */
