#include "landmark_detector.h"

class LandmarkDetectorDlib : public LandmarkDetector {
public:
    explicit LandmarkDetectorDlib(const char *fld_model_path);

    std::array<cv::Point, 68> detect_landmarks(const cv::Mat &frame, const cv::Rect &face) override;

private:
    dlib::shape_predictor fld_model;
    dlib::full_object_detection landmarks;
};


LandmarkDetectorDlib::LandmarkDetectorDlib(const char *fld_model_path)
{
    dlib::deserialize(fld_model_path) >> fld_model;
}

std::array<cv::Point, 68> LandmarkDetectorDlib::detect_landmarks(const cv::Mat &frame, const cv::Rect &face)
{
    cv::Mat gray_img;
    cv::cvtColor(frame, gray_img, CV_BGR2GRAY);

    dlib::cv_image<unsigned char> img(gray_img);
    dlib::rectangle face_region = dlib::rectangle(face.x, face.y, face.x+face.width, face.y+face.height);
    landmarks = fld_model(img, face_region);

    std::array<cv::Point, 68> cv_landmarks;
    for(int i = 0; i < 68; i++) {
        cv_landmarks[i] = cv::Point(landmarks.part(i).x(), landmarks.part(i).y());
    }

    return cv_landmarks;
}


std::unique_ptr<LandmarkDetector> make_dlib_landmark_detector(const char *source_filepath)
{
    return std::make_unique<LandmarkDetectorDlib>(source_filepath);
}
