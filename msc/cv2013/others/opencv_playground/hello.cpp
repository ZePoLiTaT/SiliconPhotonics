#include <stdlib.h>
#include <stdio.h>
#include <cv.h>
#include <math.h>
#include <highgui.h>

#define rep(i,n) for(int i=0; i<(n); i++)
#define rep2(i,n) for(int i=0; i<=(n); i++)
#define repf(i,a,b) for(int i=(a); i<=(b); i++)
#define repb(i,a,b) for (int i=(a); i>=b; i--) 

using namespace std; 

void trackbarHandler(int pos){
    printf("Trackbar position: %d\n",pos);
}

int main(int argc, char **argv) {
    IplImage* img = 0;
    int height, width, step, channels;
    uchar *data;
    int i,j,k;

    if(argc<2){
	printf("Ussage: main <image-file-name>\n\7");
	exit(0);
    }

    //load an image
    img = cvLoadImage(argv[1]);
    if(!img){
	printf("Could not load image file: %s\n",argv[1]);
	exit(0);
    }

    //get the image data
    height  = img->height;
    width   = img->width;
    step    = img->widthStep;
    channels= img-> nChannels;
    data    = (uchar *)img->imageData;
    printf("Processing a %dx%d image with %d channels\n", height, width, channels);

    //create a window
    cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE);
    cvMoveWindow("mainWin",100,100);
    
    int trackbarVal=25;
    int maxVal=100;
    cvCreateTrackbar("bar1", "mainWin", &trackbarVal, maxVal, trackbarHandler);

    //invert the image
    cvShowImage("mainWin", img);

    //wait for a key
    cvWaitKey(0);
    
    //release the image
    cvReleaseImage(&img);
    return 0;
} 


