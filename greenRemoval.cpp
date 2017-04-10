#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

cv::Mat computeHSVHistogram(const cv::Mat &hsv) {
  int hbins = 30; // Quantize the hue to 30 levels
  int sbins = 32; // and the saturation to 32 levels
  int histSize[] = {hbins, sbins};

  float hranges[] = {0, 180}; // hue: 0 - 179 degrees, see cvtColor
  float sranges[] = {0, 256}; // saturation: gray to pure spectrum color
  const float *ranges[] = {hranges, sranges};

  int channels[] = {0, 1};
  cv::Mat rawHist, normHist;
  cv::calcHist(&hsv, 1, channels, cv::Mat(), rawHist, 2, histSize, ranges,
               true, // the histogram is uniform
               false);
  cv::normalize(rawHist, normHist, 1.0, 0.0, cv::NORM_L1);

  return normHist;
}

cv::Mat getGreenPatch(const cv::Mat &hsv, const int offset_x) {
  auto left_roi = hsv(cv::Rect(0, 0, offset_x, hsv.size().height));
  auto left_hist = computeHSVHistogram(left_roi);
  double left_minVal, left_maxVal;
  cv::minMaxLoc(left_hist, &left_minVal, &left_maxVal);

  auto right_roi = hsv(
      cv::Rect(hsv.size().width - offset_x, 0, offset_x, hsv.size().height));
  auto right_hist = computeHSVHistogram(right_roi);
  double right_minVal, right_maxVal;
  cv::minMaxLoc(right_hist, &right_minVal, &right_maxVal);

  return ((left_maxVal > right_maxVal) ? left_roi : right_roi);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << argv[0] << " <foreground>" << std::endl;
    return -1;
  }

  const int offset_x = 100;

  cv::Mat hsv, rgb = cv::imread(argv[1]);
  cv::cvtColor(rgb, hsv, CV_BGR2HSV);
  if (rgb.size().width < 4 * offset_x) {
    std::cerr
        << "Input image too small. Please, provide an image with at least "
        << 4 * offset_x << " width. Height doesn't matter." << std::endl;
    return -2;
  }

  auto greenPatch = getGreenPatch(hsv, offset_x);
  cv::Scalar mean, stddev;
  cv::meanStdDev(greenPatch, mean, stddev);
  std::cout << "Patch mean: " << mean << "; stddev: " << stddev << std::endl;

  return 0;
}
