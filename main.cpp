#include <opencv2/opencv.hpp>

typedef struct algorithms {
    bool capture;
    bool gaussian;
    bool canny;
    bool sobel;
    bool brightness;
    bool contrast;
    bool negative;
    bool grayscale;
    bool halfSizeX;
    bool halfSizeY;
    int rotationsBy90;
    bool mirrorX;
    bool mirrorY;
} Algorithms;

void setToggles(Algorithms *toggles);
void applyProcessing(Algorithms toggles, cv::Mat *frame);

int main(int argc, char **argv) {
    int camera = 0;
    cv::VideoCapture cap;
    // open the default camera, use something different from 0 otherwise;
    // Check VideoCapture documentation.
    if (!cap.open(camera))
        return 0;

    Algorithms toggles = {true, false};
    while (toggles.capture) {
        cv::Mat frame;
        cap >> frame;
        if (frame.empty()) break; // end of video stream

        applyProcessing(toggles, &frame);

        imshow("This is you, smile! :)", frame);

        setToggles(&toggles);
    }
    cap.release();  // release the VideoCapture object
    return 0;
}

void setToggles(Algorithms *toggles) {
    switch (cv::waitKey(1)) {
        default: break;

        case 27: // stop capturing by pressing ESC
            toggles->capture = false;
            break;

        case 49: // 1 - Toggle Gaussian Blur
            toggles->gaussian = !toggles->gaussian;
            break;

        case 50: // 2 - Toggle Canny
            toggles->canny = !toggles->canny;
            break;

        case 51: // 3 - Toggle Sobel
            toggles->sobel = !toggles->sobel;
            break;

        case 52: // 4 - Toggle brightness adjustment
            toggles->brightness = !toggles->brightness;
            break;

        case 53: // 5 - Toggle contrast adjustment
            toggles->contrast = !toggles->contrast;
            break;

        case 54: // 6 - Toggle negative
            toggles->negative = !toggles->negative;
            break;

        case 55: // 7 - Toggle negative
            toggles->grayscale = !toggles->grayscale;
            break;

        case 56: // 8 - Toggle resize to half in x
            toggles->halfSizeX = !toggles->halfSizeX;
            break;

        case 57: // 9 - Toggle resize to half in y
            toggles->halfSizeY = !toggles->halfSizeY;
            break;

        case 65: // A - Rotate 90 degrees
            toggles->rotationsBy90 = (toggles->rotationsBy90 + 1) % 4;
            break;

        case 66: // B - Mirror in x
            toggles->mirrorX = !toggles->mirrorX;
            break;

        case 67: // C - Mirror in y
            toggles->mirrorY = !toggles->mirrorY;
            break;
    }
}

void applyProcessing(Algorithms toggles, cv::Mat *frame) {
    if (toggles.gaussian) {
        cv::Size size = cv::Size(25,25);
        cv::GaussianBlur(*frame, *frame, size, 0, cv::BORDER_DEFAULT);
    }

    if (toggles.canny) {
        cv::Mat edges;
        cv::Canny(*frame, edges, 10, 250, 3, false);
        *frame = edges;
    }

    if (toggles.sobel) {
        cv::Mat sobelX;
        cv::Mat sobelY;
        cv::Sobel(*frame, sobelX, frame->depth(), 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
        cv::Sobel(*frame, sobelY, frame->depth(), 0, 1, 3, 1, 0, cv::BORDER_DEFAULT);
        addWeighted(sobelX, 0.5, sobelY, 0.5, 0, *frame);
    }

    if (toggles.brightness) {
        frame->convertTo(*frame, frame->depth(), 1, 100);
    }

    if (toggles.contrast) {
        frame->convertTo(*frame, frame->depth(), 2, 0);
    }

    if (toggles.negative) {
        frame->convertTo(*frame, frame->depth(), -1, 255);
    }

    if (toggles.grayscale) {
        if (frame->channels() == 3) {
            cv::Mat gs;
            cv::cvtColor(*frame, gs, cv::COLOR_BGR2GRAY);
            *frame = gs;
        }
    }

    if (toggles.halfSizeX) {
        cv::Mat halved;
        cv::resize(*frame, halved, cv::Size(0,0), 0.5, 1, cv::INTER_LINEAR);
        *frame = halved;
    }

    if (toggles.halfSizeY) {
        cv::Mat halved;
        cv::resize(*frame, halved, cv::Size(0,0), 1, 0.5, cv::INTER_LINEAR);
        *frame = halved;
    }

    for (int rots = 0; rots < toggles.rotationsBy90; rots++) {
        cv::rotate(*frame, *frame, cv::ROTATE_90_CLOCKWISE);
    }

    if (toggles.mirrorX && toggles.mirrorY) {
        cv::Mat flipped;
        flip(*frame, flipped, -1 /* code for x and y axis */);
        *frame = flipped;
    } else {
        if (toggles.mirrorX) {
            cv::Mat flipped;
            flip(*frame, flipped, 0 /* code for x axis */);
            *frame = flipped;
        }

        if (toggles.mirrorY) {
            cv::Mat flipped;
            flip(*frame, flipped, 1 /* code for x axis */);
            *frame = flipped;
        }
    }
}