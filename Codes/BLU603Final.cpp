//
//  BLU603Final.cpp
//  
//
//  Created by Ahmed Bingol on 2/10/21.
//

#include "BLU603Final.hpp"

using namespace cv;
using namespace std;

#define FRAMEDIF 3

int main( int argc, char** argv )
{
    VideoCapture cap("/Users/ahmedbingol/Desktop/Robot1.mp4"); //capture the video from memory
    ofstream myfile;
    myfile.open("/Users/ahmedbingol/Desktop/BLU603Final/AKAZE_KAZE_Descriptor-Dif2_time.txt");
    if( !cap.isOpened() )  // if not success, exit program
    {
        cout << "Cannot open the web cam" << endl;
        return -1;
    }
    Mat framenew ,frameold;
    cap >> frameold;
    int framenum=0, pic=1 , total_match_size=0;
    
    for(;;)
    {
        double t = (double)getTickCount();  //START TIMER
        cap >> framenew;
        int descrip_size =0 , descript_channell=3 , nOctave = 4 , OctaveLayers = 4 ;
        float threshold = 0.01f;
        if(framenew.empty() == 1  ){
            
        }
        else {
            Ptr <cv::AKAZE> detector = cv::AKAZE::create(AKAZE::DESCRIPTOR_KAZE, descrip_size, descript_channell, threshold, nOctave , OctaveLayers , cv::KAZE::DIFF_CHARBONNIER);
            std::vector<KeyPoint> keypoints1, keypoints2;
            
            Mat descriptors1, descriptors2;
            
            detector->detectAndCompute(frameold, noArray(), keypoints1, descriptors1);
            detector->detectAndCompute(framenew, noArray(), keypoints2, descriptors2);
            
            Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE);
            std::vector<DMatch> matches;  //Matches array
            matcher->match( descriptors1, descriptors2, matches);
            
            Mat img_matches;
            drawMatches(frameold, keypoints1, framenew, keypoints2, matches, img_matches);
            
            // Show detected matches
            //imshow("Matches", img_matches );
            
            t= ((double)getTickCount()-t)/getTickFrequency();
            framenum++;
            
            if(framenum>100){
                std::ostringstream name;
                name << "/Users/ahmedbingol/Desktop/BLU603Final/samples/AKAZE_KAZE_Feat-" << pic <<".png";
                total_match_size += matches.size();
                imshow("Matches", img_matches );
                //imwrite(name.str(), img_matches);
                pic++;
                framenum=0;
                myfile << "Times passed in sec" << t <<" Frame number is = " << pic-1 << "\nNumber of matches for this frame is " << matches.size() << endl;
            }
            if(pic > 30)
                break;
            
            cout << "Frame Num= " << pic-1 <<" time passed in sec " << t <<endl;    //END TIMER
            frameold = framenew.clone();
            
            if(waitKey(30) >= 0) break;
        }
    }
    myfile.close();
    return 0;
}
