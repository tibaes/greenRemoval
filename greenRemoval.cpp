#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

uchar find_best_threshold(const cv::Mat & img) {
    /* finds the best threhsold for binarization using otsuo's method */
	uchar best = 0;
	double best_sb = 0;

    /* compute the probability of each gray level and total mean u */
    double p[256];
	for (int i = 0; i < 256; i++)
		p[i] = 0;
	for (int i = 0; i < img.rows; i++)
		for (int j = 0; j < img.cols; j++)
			p[img.at<uchar>(i, j)] += 1;
	double ut = 0;
	for (int i = 0; i < 256; i++) {
		p[i] /= img.rows*img.cols;
		ut += i*p[i];
	}

    /* compute best k */
    double w = 0, u = 0;
	for (int k = 0; k < 256; k++) {
		w += p[k];
		u += k*p[k];
		double sb = pow(ut*w - u, 2.0) / (w * (1 - w));
		if (sb > best_sb) {
			best_sb = sb;
			best = k;
		}
	}

	return best;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << argv[0] << " <foreground>" << std::endl;
        return -1;
    }

    cv::Mat lab, rgb = cv::imread(argv[1]);
    cv::cvtColor(rgb, lab, CV_BGR2Lab);

    std::vector<cv::Mat> labs;
    cv::split(lab, labs);

    cv::Mat mask = labs[1] > find_best_threshold(labs[1]);

    cv::Mat combined;
    rgb.copyTo(combined, mask);

    cv::imshow("input", rgb);
    cv::imshow("mask", mask * 255);
    cv::imshow("combined", combined);
    cv::waitKey(0);

    return 0;
}
