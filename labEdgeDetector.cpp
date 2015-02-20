/**
 * Written by Austin Walters, all rights reserved 2015
 *
 * Requires OpenCV 2.4.9 install
 *
 * To Compile: g++ -I/usr/include -L/usr/lib -lopencv_core.2.4.9 -lopencv_calib3d.2.4.9 -lopencv_imgproc.2.4.9 -lopencv_highgui.2.4.9 labEdgeDetector.cpp -o edgeDetector
 *
 * To Execute: ./edgeDetector < InputFile >
 *
 * Example: ./edgeDetector skyline.jpg
 */

#include <iostream>

#include <opencv/cv.h>
#include <opencv2/opencv.hpp>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

/* version 2.4 of openCV */
using namespace cv;

/**                                                                                                 
 * Takes in a RGB image and outputs                                                                 
 * a Lab space image.                                                                               
 **/
Mat RGB2LAB(Mat const &imgRGB){
  Mat imgLab;
  cvtColor(imgRGB, imgLab, CV_BGR2Lab);
  return imgLab;
}


/**
 * Takes in image in the lab space and splits 
 * out the a stream. Returning it as a matrix,
 * to be saved out to file or manipulated
 **/
Mat aLAB(Mat const &imgLab){

  Mat aLab;
  cvtColor(imgLab, aLab, CV_Lab2RGB);
  
  Size s = aLab.size();
  int steps = imgLab.step;
  int channels = imgLab.channels();

  /* Create Black and White image of a space */
  for(int y = 0; y < s.height; y++){
    for(int x = 0; x < s.width; x++){
      (*aLab.ptr<Point3_<uchar> >(y,x)).x = imgLab.data[steps*y + channels*x + 1];
      (*aLab.ptr<Point3_<uchar> >(y,x)).y = imgLab.data[steps*y + channels*x + 1];
      (*aLab.ptr<Point3_<uchar> >(y,x)).z = imgLab.data[steps*y + channels*x + 1];
    }
  }
  return aLab;
}

/**
 * Takes in image in the lab space and splits 
 * out the b stream. Returning it as a matrix,
 * to be saved out to file or manipulated
 **/
Mat bLAB(Mat &imgLab){
  
  Mat bLab;
  cvtColor(imgLab, bLab, CV_Lab2RGB);
  
  Size s = bLab.size();
  int steps = imgLab.step;
  int channels = imgLab.channels();
  
  /* Create Black and White image of b space */
  for(int y = 0; y < s.height; y++){
    for(int x = 0; x < s.width; x++){
      (*bLab.ptr<Point3_<uchar> >(y,x)).x = imgLab.data[steps*y + channels*x + 2];
      (*bLab.ptr<Point3_<uchar> >(y,x)).y = imgLab.data[steps*y + channels*x + 2];
      (*bLab.ptr<Point3_<uchar> >(y,x)).z = imgLab.data[steps*y + channels*x + 2];
    }
  }
  return bLab;
}

/**
 * Finds the gradient via the sobel method,
 * the output is a Mat with the gradient edges (might be better as array).
 * It must be gray!
 **/
Mat gradientEdges(Mat &img){
  
  Mat gradImg;
  Mat gx, gy;
  Mat abs_gx, abs_gy;
  
  int scale = 1;
  int delta = 0;
  int ddepth = CV_16S;
  
  GaussianBlur(img, img, Size(3,3), 0, 0, BORDER_DEFAULT);
  
  // gradient in the x direction
  Sobel(img, gx, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
  convertScaleAbs(gx, abs_gx);
  
  // Gradient in the y direction
  Sobel( img, gy, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
  convertScaleAbs(gy, abs_gy);
  
  // Finds the combined Gradient
  addWeighted(abs_gx*1.5, 0.5, abs_gy*1.5, 0.5, 0, gradImg);
  
  return gradImg;
  
}

int main(int argc, const char* argv[]){
  
  if(argc < 2){
    std::cout << "Not enough input arguments" << std::endl;
    exit(1);
  }
  
  Mat imgLab =  RGB2LAB(imread(argv[1], CV_LOAD_IMAGE_COLOR));
  
  char filename[120];
  
  Mat a = aLAB(imgLab);
  Mat b = bLAB(imgLab);
  Mat g = gradientEdges(a);

  /* Write labs */
  imwrite("rgb_2_lab.jpg", imgLab);
  imwrite("aLab.jpg", a);
  imwrite("bLab.jpg", b);
  
  /* Write gradients */
  imwrite("aLabGradient.jpg", g);
  g = gradientEdges(b);
  imwrite("bLabGradient.jpg", g);
  
  return 0;
}
