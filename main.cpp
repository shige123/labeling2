#include <iostream>
#include <vector>
#include <algorithm>
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

int g_lbl = 0;
std::vector<int> srcLUT;
std::vector<int> dstLUT;
int LabelingPixel(cv::Mat img,int x, int y){
  int pxl[4];
  int min = 0;
  bool flg = false;
  pxl[0] = img.at<unsigned char>(x-1,y-1);
  pxl[1] = img.at<unsigned char>(x,y-1);
  pxl[2] = img.at<unsigned char>(x+1,y-1);
  pxl[3] = img.at<unsigned char>(x-1,y);
  
  for(int i = 0; i < 4; i++){
    if(pxl[i] != 0){
      if(min == 0 || min > pxl[i]){
	flg = true;
	min = pxl[i];
      }else if(min < pxl[i]){
	std::vector<int>::iterator itr = std::find(srcLUT.begin(),srcLUT.end(), pxl[i]);
	if(itr != srcLUT.end()){
	  if(*itr > min)  *itr = min;
	}else{
	  srcLUT.push_back(pxl[i]);
	  dstLUT.push_back(min);
	}
      }
    }
  }
  if(!(flg)){
    g_lbl++;
    return g_lbl;
  } else{
    return min;
  }
}

int label_convert(cv::Mat img){
  int cnt =0;
  std::cout << srcLUT.size() << std::endl;
  for(int i = 0; i < img.rows; i++){
    for(int j = 0; j < img.cols; j++){
      if(img.at<unsigned char>(i,j)){
	for(int k = 0; k < srcLUT.size(); k++){
	  if(img.at<unsigned char>(i,j) == srcLUT[k]){
	    img.at<unsigned char>(i,j) = dstLUT[k];
	    cnt++;
	    if(!(cnt %30)) std::cout << cnt << std::endl;
	  }
	}
      }
    }
  }
  return cnt;
}

int main (int argc, char *argv[]){
  cv::Mat img = cv::imread("img_0.png",0);
  int label[img.rows][img.cols];
  for(int i = 0; i < img.rows; i++){
    for(int j = 0; j < img.cols; j++){
      if(img.at<unsigned char>(i,j)){
	img.at<unsigned char>(i,j) = LabelingPixel(img, i, j);
      }
    }
  }
  
  std::cout << "debug" << std::endl;
  while(1){
    if(label_convert(img) == 0) break;
  }
  /* std::cout << "debug" << std::endl;
  std::sort(srcLUT.begin(),srcLUT.end());
  for(int i; i < srcLUT.size(); i++) std::cout << srcLUT[i] << " ";
  std::cout << std::endl;
  std::sort(dstLUT.begin(),dstLUT.end());
  for(int i; i < dstLUT.size(); i++) std::cout << dstLUT[i] << " ";
  std::cout << std::endl;*/
/*
  unsigned int a;
  for(int i = 0; i < img.rows;++i){
    for(int j = 0; j < img.cols; ++j){
      if(img.at<unsigned char>(i,j)){
      }
      //    std::cout << (int)img.at<unsigned char>(i,j) << " ";
    }
    //  std::cout << std::endl;
  }
  */
  cv::imwrite("output.png",img);
  
  return 0;
}
