#include "histogram.h"
#include "sequential.h"
#include "parallel.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdexcept>
int main(){try{cv::Mat a(16,16,CV_8UC1);for(int y=0;y<16;y++)for(int x=0;x<16;x++)a.at<unsigned char>(y,x)=(x+y)*8;auto h=aie::calculateHistogram(a);unsigned long long n=0;for(auto x:h)n+=x;if(n!=a.total())throw std::runtime_error("histogram count");double t;auto s=aie::enhanceSequential(a,t);for(int k:{1,2,4,8}){auto p=aie::enhanceParallel(a,k,t);if(cv::norm(s,p,cv::NORM_INF)!=0)throw std::runtime_error("sequential/parallel mismatch");}cv::Mat c(10,10,CV_8UC1,cv::Scalar(77));auto o=aie::enhanceSequential(c,t);if(cv::norm(c,o,cv::NORM_INF)!=0)throw std::runtime_error("constant image mismatch");std::cout<<"All core tests passed\n";return 0;}catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 1;}}
