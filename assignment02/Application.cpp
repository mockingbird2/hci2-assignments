///////////////////////////////////////////////////////////////////////////
//
// Main class for HCI2 assignments
// Authors:
//		Stefan Neubert (2015)
//		Stephan Richter (2011)
//		Patrick L�hne (2012)
//
///////////////////////////////////////////////////////////////////////////

#include "Application.h"

#include <iostream>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "framework/DepthCamera.h"
#include "framework/KinectMotor.h"
#include "framework/SkeletonTracker.h"

#define BOOST_SIGNALS_NO_DEPRECATION_WARNING
#include <boost/thread.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include "uist-game/GameServer.h"
#include "uist-game/GameClient.h"
#include "uist-game/Game.h"
#include "uist-game/GameUnit.h"

const int Application::uist_level = 1;
const char* Application::uist_server = "127.0.0.1";

void Application::processFrame()
{	
	///////////////////////////////////////////////////////////////////////////
	//
	// To do:
	//
	// This method will be called every frame of the camera. Insert code here in
	// order to fulfill the assignment. These images will help you doing so:
	//
	// * m_bgrImage: The image of the Kinect's color camera
	// * m_depthImage: The image of the Kinects's depth sensor
	// * m_outputImage: The image in which you can draw the touch circles.
	// * m_gameImage: The image in which the UIST game is rendered.
	//
	///////////////////////////////////////////////////////////////////////////

	// Sample code brightening up the depth image to make the values visible
	m_depthImage *= 10;
}

void Application::processSkeleton(XnUserID userId)
{
	///////////////////////////////////////////////////////////////////////////
	//
	// To do:
	//
	// This method will be called every frame of the camera for each tracked user.
	// Insert code here in order to fulfill the assignment.
	//
	// Below is an example how to retrieve a skeleton joint position and
	// (if needed) how to convert its position into image space.
	//
	///////////////////////////////////////////////////////////////////////////

	m_skeletonTracker->drawSkeleton(m_bgrImage, userId);

	xn::SkeletonCapability skeletonCap = m_skeletonTracker->getSkeletonCapability();

	XnSkeletonJointPosition leftHand;
	skeletonCap.GetSkeletonJointPosition(userId, XN_SKEL_LEFT_HAND, leftHand);
	cv::Point2f imagePosition = m_skeletonTracker->getProjectedPosition(leftHand);
}

void Application::loop()
{
	int key = cv::waitKey(20);
	switch (key)
	{
	case 'q': // quit
		m_isFinished = true;
		break;
	case 'p': // screenshot
		makeScreenshots();
		break;
	// run the loaded level
	case 'r':
		if(m_gameServer)
			m_gameServer->startGame();
		break;
	// Move fist (0) game unit with wasd
	case 'w': // north
		if(m_gameClient && m_gameClient->game())
			m_gameClient->game()->moveUnit(0, (float)M_PI_2, 1.f);
		break;
	case 'a': // west
		if(m_gameClient && m_gameClient->game())
			m_gameClient->game()->moveUnit(0, (float)M_PI, 1.f);
		break;
	case 's': // south
		if(m_gameClient && m_gameClient->game())
			m_gameClient->game()->moveUnit(0, 3 * (float)M_PI_2, 1.f);
		break;
	case 'd': // east
		if(m_gameClient && m_gameClient->game())
			m_gameClient->game()->moveUnit(0, 0.f, 1.f);
		break;
	// stop the first (0) game unit
	case ' ':
		if(m_gameClient && m_gameClient->game())
			m_gameClient->game()->moveUnit(0, 0.f, 0.f);
		break;
	// highlight the first (0) game unit
	case 'h':
		if(m_gameClient && m_gameClient->game())
			m_gameClient->game()->highlightUnit(0, true);
		break;
	case 'u':
		if(m_gameClient && m_gameClient->game())
			m_gameClient->game()->highlightUnit(0, false);
		break;
	}
	
	if(m_isFinished) return;

	if(m_gameClient && m_gameClient->game())
		m_gameClient->game()->render(m_gameImage);

	if(m_depthCamera)
	{
		m_depthCamera->getFrame(m_bgrImage, m_depthImage);
		processFrame();
	}

	if(m_skeletonTracker)
	{
		std::set<XnUserID>& users = m_skeletonTracker->getTrackedUsers();
		for (std::set<XnUserID>::iterator i = users.begin(); i != users.end(); ++i)
			processSkeleton(*i);
	}

	cv::imshow("bgr", m_bgrImage);
	cv::imshow("depth", m_depthImage);
	cv::imshow("output", m_outputImage);
	cv::imshow("UIST game", m_gameImage);
}

void Application::makeScreenshots()
{
	cv::imwrite("color.png", m_bgrImage);
	cv::imwrite("depth.png", m_depthImage);
	cv::imwrite("output.png", m_outputImage);
}

Application::Application()
	: m_isFinished(false)
	, m_depthCamera(nullptr)
	, m_kinectMotor(nullptr)
	, m_skeletonTracker(nullptr)
	, m_gameClient(nullptr)
	, m_gameServer(nullptr)
{
	// If you want to control the motor / LED
	// m_kinectMotor = new KinectMotor;

	// Not used for UIST game demo, uncomment for skeleton assignment
	// m_depthCamera = new DepthCamera;
	// m_skeletonTracker = new SkeletonTracker(m_depthCamera);

	// open windows
	cv::namedWindow("output", CV_WINDOW_AUTOSIZE);
	cv::namedWindow("depth", CV_WINDOW_AUTOSIZE);
	cv::namedWindow("bgr", CV_WINDOW_AUTOSIZE);
	cv::namedWindow("UIST game", CV_WINDOW_AUTOSIZE);

	// create work buffer
	m_bgrImage = cv::Mat(480, 640, CV_8UC3);
	m_depthImage = cv::Mat(480, 640, CV_16UC1);
	m_outputImage = cv::Mat(480, 640, CV_8UC1);
	m_gameImage = cv::Mat(480, 480, CV_8UC3);

	//if(uist_server == "127.0.0.1") {
		m_gameServer = new GameServer;
		m_gameClient = new GameClient;
		m_gameServer->run();
		m_gameServer->loadGame(uist_level);
		boost::this_thread::sleep(boost::posix_time::milliseconds(100));
	//}
	m_gameClient->run();
	m_gameClient->connectToServer(uist_server);
	std::cout << "[Info] Connected to " << uist_server << std::endl;
}

Application::~Application()
{
	// Fails due to race conditions in UIST game
	/*
	if (m_gameClient)
	{
		m_gameClient->stop();
		delete m_gameClient;
	}
	if (m_gameServer)
	{
		m_gameServer->stop();
		delete m_gameServer;
	}*/

	if (m_skeletonTracker) delete m_skeletonTracker;
	if (m_depthCamera) delete m_depthCamera;
	if (m_kinectMotor) delete m_kinectMotor;
}

bool Application::isFinished()
{
	return m_isFinished;
}
