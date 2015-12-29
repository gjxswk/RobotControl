#pragma once
//#include "KinectInterface.h"
#include "windows.h"
#include <vector>
//#include "cv.h"
#include <opencv/cv.h>

#include <XnCppWrapper.h> // kinect.h
#include <fstream>


struct PointCloud
{ 
float  x;  
float  y; 
float  z; 
float  r;  
float  g;  
float  b;  
PointCloud( XnPoint3D pos, XnRGB24Pixel color )  
{    x = pos.X*0.001;  
y = pos.Y*0.001;  
z = pos.Z*0.001;   
r =color.nRed;//(float);//color.nRed / 255;  
g = color.nGreen;//(float);//color.nGreen / 255;  
b = color.nBlue;//(float)//color.nBlue / 255;
}  
};

class CVirtualScene : public CWnd
{
	DECLARE_DYNAMIC(CVirtualScene)

public:
	CVirtualScene();
	virtual ~CVirtualScene();

	bool UpdatePointCloud(const IplImage *pc, const IplImage *color, bool bUpdateWindow = true);
	void UpdatePointCloud(const std::vector<PointCloud> &vpc, bool bUpdateWindow = true);
	double GetMarkerPos(int idx) { return m_marker[idx]; }

private:
	std::vector<PointCloud> m_vpc;
	double m_marker[3];
	double m_xyz[480][640][3];
	byte m_color[480][640][3];
	int m_height;
	int m_width;
	int m_pointCloudType;
	//IplImage *m_pointClouds;
	//IplImage *m_rgbImage;

private:
	byte CVirtualScene::GetColor(int r, int c, int i) const;
	double CVirtualScene::Get3DCoord(int r, int c, int i) const;

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();

protected:
	int m_GLPixelIndex;
	HGLRC m_hGLContext;

	BOOL SetWindowPixelFormat(HDC hDC);
	BOOL CreateViewGLContext(HDC hDC);

public:
	afx_msg void OnPaint();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	/*afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);*/

	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};


