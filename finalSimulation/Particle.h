#ifndef Particle_Filter_Particle_h
#define Particle_Filter_Particle_h

#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/legacy/legacy.hpp>

#include <cstdlib>

#include <opencv/highgui.h>
#include <opencv/cv.h>
#include "Util.h"
#include <math.h>

using namespace std;

double randomDouble()
{
    //cout << "RAND_MAX = " << double(RAND_MAX) << endl;
    return double(rand()) / (double(RAND_MAX) + 1.0);
}

double randomDoubleFromNormal(const double s)
{
    double sum = 0;
    for(int i=0; i<12; i++){
        sum += randomDouble()*2*s - s;
    }
    return sum/2;
}

class Particle {
    
private:
    cv::Point particlePt;
    double weight;
    
public:
    Particle(cv::Point start = cv::Point(0,0), double w = 0.0) {
        particlePt = start;
        weight = w;
    }
    ~Particle(){}
    
    void moveParticle(double angle, double distance, double variance) {
        double realAngle   = angle + 3*randomDoubleFromNormal(variance); //I use scale 3 to make the angle error bigger
        //double realAngle   = angle;
        double realDistance = distance + randomDoubleFromNormal(variance);
        
        particlePt.x = round(double(particlePt.x) + realDistance* cos(realAngle * M_PI/180.0));
        
        particlePt.y = round(double(particlePt.y) + realDistance* sin(realAngle * M_PI/180.0));
        
        //cout << "particlePt = " << particlePt << endl;
    }
    
    
    
    cv::Point getPosition(){return particlePt;}
    // Get Particle's probability
    double getWeight(){return weight;}
    // Modifies the Particle's probability
    void setWeight(double w){weight = w;}
    
    
};


#endif
