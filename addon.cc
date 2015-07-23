// addon.cc
#include <node.h>
#include <node_buffer.h>
#include "opencv2/objdetect.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <iostream>

using namespace cv;
using namespace v8;

int hogdetect(Mat img);

void saveimage(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  // Import args from callback
  Local<Object> bufferObj    = args[0]->ToObject();
  char*         bufferData   = node::Buffer::Data(bufferObj);
  int           bufferLength = node::Buffer::Length(bufferObj);
  int           width        = args[1]->Uint32Value();
  int           height       = args[2]->Uint32Value();

#ifdef DEBUG
  printf("size_T = %d\n",bufferLength);
  std::cout << "Width = " << width << std::endl;
  std::cout << "Height = " << height << std::endl;
#endif

  // Create 2d Mat from char* buffer
  Mat mat = Mat(height, width, CV_8U, bufferData);

  // Decode the 1d matrix
  Mat img = imdecode(mat, 1);
  
  // Saving image
  std::string out = "test_cpp.jpg";
  
  if(img.empty()) {
    std::cout << "Decode failed"<< std::endl;
  }

  int detect = hogdetect(img);

#ifdef DEBUG
  if(detect > 0) {
    std::cout << "Detection found"<< std::endl;
  }
#endif
  // args.GetReturnValue().Set(success);
} 


//Based on peopledetect.cc OpenCV 2.4
int hogdetect(Mat img)
{
    int detect = 0;
    if(!img.data) return -1;

    HOGDescriptor hog;
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

    std::vector<Rect> found, found_filtered;
    double tk = (double)getTickCount();
    // run the detector with default parameters. to get a higher hit-rate
    // (and more false alarms, respectively), decrease the hitThreshold and
    // groupThreshold (set groupThreshold to 0 to turn off the grouping completely).
    hog.detectMultiScale(img, found, 0, Size(8,8), Size(32,32), 1.05, 2);
    double t = (double)getTickCount() - tk;

#ifdef DEBUG
    printf("Detection time = %gms\n", t*1000./cv::getTickFrequency());
#endif

    size_t i, j;

    // Traverse found objects
    for( i = 0; i < found.size(); i++ )
    {
        Rect r = found[i];
        for( j = 0; j < found.size(); j++ )
            if( j != i && (r & found[j]) == r)
                break;
        if( j == found.size() )
            found_filtered.push_back(r);
            detect += 1;
    }
    // Draw rectangles around detected objects
    for( i = 0; i < found_filtered.size(); i++ )
    {
        Rect r = found_filtered[i];
        // the HOG detector returns slightly larger rectangles than the real objects.
        // so we slightly shrink the rectangles to get a nicer output.
        r.x += cvRound(r.width*0.1);
        r.width = cvRound(r.width*0.8);
        r.y += cvRound(r.height*0.07);
        r.height = cvRound(r.height*0.8);
        rectangle(img, r.tl(), r.br(), cv::Scalar(0,255,0), 3);
    }

    // Concat timestamp + image name
    std::ostringstream strs;
    strs << tk;
    std::string str = strs.str();
    std::string out = str + "_hog_out.jpeg";
  
#ifdef DEBUG
    std::cout << "Output image:" << out << std::endl;
#endif

    imwrite(out, img);

    return detect;
}

void Init(Handle<Object> exports) {
  NODE_SET_METHOD(exports, "saveimage", saveimage);
}

NODE_MODULE(addon, Init)
