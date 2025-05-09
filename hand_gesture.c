#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

// Constants for finger detection
#define MIN_AREA 1000
#define MAX_AREA 50000
#define DEFECTS_THRESHOLD 15000

// Function prototypes
void processFrame(IplImage* frame);
int countFingers(CvSeq* contour, CvMemStorage* storage, IplImage* frame);
void drawText(IplImage* frame, const char* text, CvPoint position);

int main(int argc, char** argv) {
    CvCapture* capture;
    IplImage* frame;
    int key;

    // Initialize camera
    if (argc > 1 && strcmp(argv[1], "--picam") == 0) {
        capture = cvCaptureFromCAM(-1); // Try Pi Camera
    } else {
        capture = cvCaptureFromCAM(0);  // Default USB camera
    }

    if (!capture) {
        fprintf(stderr, "Error: Could not initialize camera\n");
        return -1;
    }

    // Create window
    cvNamedWindow("Hand Gesture Counter", CV_WINDOW_AUTOSIZE);

    while(1) {
        // Get frame from camera
        frame = cvQueryFrame(capture);
        if (!frame) break;

        // Process the frame
        processFrame(frame);

        // Show the frame
        cvShowImage("Hand Gesture Counter", frame);

        // Check for 'q' key to quit
        key = cvWaitKey(10);
        if (key == 'q') break;
    }

    // Cleanup
    cvReleaseCapture(&capture);
    cvDestroyWindow("Hand Gesture Counter");

    return 0;
}

void processFrame(IplImage* frame) {
    // Create temporary images
    IplImage* gray = cvCreateImage(cvGetSize(frame), 8, 1);
    IplImage* binary = cvCreateImage(cvGetSize(frame), 8, 1);
    
    // Convert to grayscale
    cvCvtColor(frame, gray, CV_BGR2GRAY);
    
    // Apply Gaussian blur
    cvSmooth(gray, gray, CV_GAUSSIAN, 7, 7, 0, 0);
    
    // Binary threshold
    cvThreshold(gray, binary, 70, 255, CV_THRESH_BINARY_INV);
    
    // Find contours
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* contours = 0;
    cvFindContours(binary, storage, &contours, sizeof(CvContour),
                   CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
    
    // Process each contour
    CvSeq* maxContour = NULL;
    double maxArea = 0;
    
    CvSeq* contour = contours;
    while(contour) {
        double area = fabs(cvContourArea(contour, CV_WHOLE_SEQ));
        if (area > maxArea && area > MIN_AREA && area < MAX_AREA) {
            maxArea = area;
            maxContour = contour;
        }
        contour = contour->h_next;
    }
    
    // If we found a valid contour, process it
    if (maxContour) {
        int fingers = countFingers(maxContour, storage, frame);
        if (fingers >= 5 && fingers <= 10) {
            char text[50];
            sprintf(text, "Fingers: %d", fingers);
            drawText(frame, text, cvPoint(10, 30));
        }
    }
    
    // Cleanup
    cvReleaseImage(&gray);
    cvReleaseImage(&binary);
    cvReleaseMemStorage(&storage);
}

int countFingers(CvSeq* contour, CvMemStorage* storage, IplImage* frame) {
    CvSeq* hull = cvConvexHull2(contour, storage, CV_CLOCKWISE, 0);
    CvSeq* defects = cvConvexityDefects(contour, hull, storage);
    
    int defectCount = 0;
    
    // Count defects that could represent fingers
    for(int i = 0; i < defects->total; i++) {
        CvConvexityDefect* defect = (CvConvexityDefect*)cvGetSeqElem(defects, i);
        
        if (defect->depth > DEFECTS_THRESHOLD) {
            defectCount++;
            
            // Draw defect point
            cvCircle(frame, *(defect->depth_point), 5, CV_RGB(255,0,0), -1, 8, 0);
        }
    }
    
    // Draw contour and hull
    cvDrawContours(frame, contour, CV_RGB(0,255,0), CV_RGB(0,255,0), 0, 2, 8, cvPoint(0,0));
    
    // Number of fingers is typically defects + 1
    return defectCount + 1;
}

void drawText(IplImage* frame, const char* text, CvPoint position) {
    CvFont font;
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 2, CV_AA);
    cvPutText(frame, text, position, &font, CV_RGB(0, 255, 0));
}