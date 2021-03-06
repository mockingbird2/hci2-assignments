#pragma once

#include <opencv2/core/core.hpp>

class DepthCamera;
class KinectMotor;

class Application
{
public:
	Application();
	virtual ~Application();

	void loop();

	void processFrame();

	void makeScreenshots();
	void clearOutputImage();

	bool isFinished();
	void evaluateData();

	    static bool isFoot(std::vector<cv::Point> contour);
    
    void addToContacts(cv::RotatedRect);
    void drawLines();
    void drawLastLine();
    void drawEllipse(cv::RotatedRect);
    void handleNoContact();

protected:
	DepthCamera *m_depthCamera;
	KinectMotor *m_kinectMotor;

	cv::Mat m_bgrImage;
	cv::Mat m_depthImage;
	cv::Mat m_outputImage;
	cv::Mat m_backgroundImage;
	cv::Mat m_resultImage;
	cv::Mat m_resultSignImage;

	bool m_bBackgroundInitialized;

    std::vector<std::vector<cv::Point> > contacts;
    
    bool set_reference_image;
	bool m_isFinished;
};