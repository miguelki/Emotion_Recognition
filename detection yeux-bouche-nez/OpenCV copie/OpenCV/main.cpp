//
//  main.cpp
//  OpenCV
//
//  Created by julia vigo on 24/11/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#define MAC

#ifdef MAC
#include <OpenCV/OpenCV.h>
#include <OpenCV/cvcompat.h>
#include <cassert>
#include <iostream>
#else
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>
#endif

// Create memory for calculations
static CvMemStorage* storage = 0;

// Create a new Haar classifier
static CvHaarClassifierCascade* cascade = 0;

// Function prototype for detecting and drawing an object from an image
bool detect_and_draw( IplImage* image ,CvHaarClassifierCascade* cascade);

// Create a string that contains the cascade name
const char *cascade_name[4]={"/Users/juliavigo/ING5/Semestre 9/Platforms and methods for multi modal system architectures/Mini-Project/OpenCV copie/OpenCV/haar/face.xml",
    "/Users/juliavigo/ING5/Semestre 9/Platforms and methods for multi modal system architectures/Mini-Project/OpenCV copie/OpenCV/haar/eye.xml",
    "/Users/juliavigo/ING5/Semestre 9/Platforms and methods for multi modal system architectures/Mini-Project/OpenCV copie/OpenCV/haar/nose.xml",
    "/Users/juliavigo/ING5/Semestre 9/Platforms and methods for multi modal system architectures/Mini-Project/OpenCV copie/OpenCV/haar/mouth.xml"
};

//Point d'entrée du programme
int main (int argc, char * const argv[]) {
    // Structure for getting video from camera or avi
    CvCapture* capture = 0;
    
    // Images to capture the frame from video or camera or from file
    IplImage *frame, *frame_copy = 0;
    
    // Used for calculations
    int optlen = strlen("--cascade=");
    
    // Input file name for avi or image file.
    const char* input_name;
    
    
    // Load the HaarClassifierCascade
    
    
    // Allocate the memory storage
    storage = cvCreateMemStorage(0);
    
    
    capture = cvCaptureFromCAM( 0);
    // Create a new named window with title: result
    cvNamedWindow( "result", 1 );
    
    
    // If loaded succesfully, then:
    if( capture )
    {
        // Capture from the camera.
        for(;;)
        {
            // Capture the frame and load it in IplImage
            if( !cvGrabFrame( capture ))
                break;
            frame = cvRetrieveFrame( capture );
            
            // If the frame does not exist, quit the loop
            if( !frame )
                break;
            
            // Allocate framecopy as the same size of the frame
            if( !frame_copy )
                frame_copy = cvCreateImage( cvSize(frame->width,frame->height),
                                           IPL_DEPTH_8U, frame->nChannels );
            
            // Check the origin of image. If top left, copy the image frame to frame_copy. 
            if( frame->origin == IPL_ORIGIN_TL )
                cvCopy( frame, frame_copy, 0 );
            // Else flip and copy the image
            else
                cvFlip( frame, frame_copy, 0 );
            
            
            for(int i=0;i<4;i++)
            {
                
                cascade = (CvHaarClassifierCascade*)cvLoad( cascade_name[i], 0, 0, 0 );
                
                // Check whether the cascade has loaded successfully. Else report and error and quit
                if( !cascade )
                {
                    fprintf( stderr, "ERROR: Could not load classifier cascade\n" );
                    return -1;
                }
                // Call the function to detect and draw the face
            	if(detect_and_draw(frame_copy,cascade))
                {
                    
                    std::cout<< i <<std::endl;
                    switch(i)
                    {
                        case 0:	
                            std::cout<<"face detetected"<<std::endl;
                            break;
                        case 1:	
                            std::cout<<"eye detetected"<<std::endl;
                            break;
                        case 2:	
                            std::cout<<"nose detetected"<<std::endl;
                            break;
                        case 3:	
                            std::cout<<"mouth detetected"<<std::endl;
                            break;
                            
                    }	
                    
                }						
                
            }
            // Wait for a while before proceeding to the next frame
            if( cvWaitKey( 1 ) >= 0 )
                break;
            
            //              cvWaitKey (10);
            
            
            
        }
        
        // Release the images, and capture memory
        cvReleaseHaarClassifierCascade(&cascade);
        
        cvReleaseImage( &frame_copy );
        cvReleaseCapture( &capture );
        cvReleaseMemStorage(&storage);
        
        
    }
    
    // If the capture is not loaded succesfully, then:
    return 0;
    
}

// Function to detect and draw any faces that is present in an image
bool detect_and_draw( IplImage* img,CvHaarClassifierCascade* cascade )
{
    int scale = 1;
    
    // Create a new image based on the input image
    IplImage* temp = cvCreateImage( cvSize(img->width/scale,img->height/scale), 8, 3 );
    
    // Create two points to represent the face locations
    CvPoint pt1, pt2;
    int i;
    
    // Clear the memory storage which was used before
    cvClearMemStorage( storage );
    
    // Find whether the cascade is loaded, to find the faces. If yes, then:
    if( cascade )
    {
        
        // There can be more than one face in an image. So create a growable sequence of faces.
        // Detect the objects and store them in the sequence
        CvSeq* faces = cvHaarDetectObjects( img, cascade, storage,
                                           1.1, 30, CV_HAAR_DO_CANNY_PRUNING,
                                           cvSize(40, 40) );
        
        // Loop the number of faces found.
        for( i = 0; i < (faces ? faces->total : 0); i++ )
        {
            // Create a new rectangle for drawing the face
            CvRect* r = (CvRect*)cvGetSeqElem( faces, i );
            
            // Find the dimensions of the face,and scale it if necessary
            pt1.x = r->x*scale;
            pt2.x = (r->x+r->width)*scale;
            pt1.y = r->y*scale;
            pt2.y = (r->y+r->height)*scale;
            
            // Draw the rectangle in the input image
            cvRectangle( img, pt1, pt2, CV_RGB(255,0,0), 3, 8, 0 );
        }
    }
    
	
    // Show the image in the window named "result"
    cvShowImage( "result", img );
    if(i  > 0)
		return 1;
	else
		return 0;
    
    
    // Release the temp image created.
    
    
    cvReleaseImage( &temp );
    
}





