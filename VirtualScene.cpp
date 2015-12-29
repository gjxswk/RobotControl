

#include "stdafx.h"
//#include "monitor.h"
#include "VirtualScene.h"
#include "gl/GL.h"
#include "gl/GLU.H"

#define ANGLESTEP			3			// degree

static bool bAdjustView;   //静态全局变量 只能在该文件中使用，多个函数使用
static bool bColorScene;

static int old_x, old_y;
static GLfloat rot_angle_x, rot_angle_y, rot_angle_z;
static GLfloat offset_x, offset_y, offset_z;
static GLfloat trans_x, trans_y; //, trans_z;

static void InitView()
{
	trans_x = 0;
	trans_y = 0;
	//trans_z = 0;
	offset_x = 0;
	offset_y = 0;
	offset_z = 0;
	rot_angle_x = 0;
	rot_angle_y = 0;
	rot_angle_z = 0;
}//静态函数，该文件中使用

// CVirtualScene

IMPLEMENT_DYNAMIC(CVirtualScene, CWnd)

CVirtualScene::CVirtualScene()
{
	m_GLPixelIndex = 0;
	m_marker[0] = 0;	
	m_marker[1] = 0;
	m_marker[2] = 0;
	m_hGLContext = NULL;
	bAdjustView = false;
	bColorScene = true;
	InitView();	

//	m_pointClouds	= NULL;
//	m_rgbImage		= NULL;
	memset(m_xyz, 0, sizeof(m_xyz));
	memset(m_color, 0, sizeof(m_color));
	m_width = 0;
	m_height = 0;
	m_pointCloudType = 1;
}

CVirtualScene::~CVirtualScene()
{
	//if (m_pointClouds != NULL)
	//{
	//	cvReleaseImage(&m_pointClouds);
	//}
	//if (m_rgbImage != NULL)
	//{
	//	cvReleaseImage(&m_rgbImage);
	//}
}

bool CVirtualScene::UpdatePointCloud(const IplImage *pc, const IplImage *color, bool bUpdateWindow)
{
	// assert(pc->depth == IPL_DEPTH_64F && pc->nChannel == 3
	//			&& color->depth == IPL_DEPTH_8U && color->nChannel == 3);
	
	//if (m_pointClouds == NULL)
	//{
	//	m_pointClouds = cvCloneImage(pc);
	//}
	//else 
	//{
	//	cvCopyImage(pc, m_pointClouds);
	//}

	//if (m_rgbImage == NULL)
	//{
	//	m_rgbImage = cvCloneImage(color);
	//}
	//else
	//{
	//	cvCopyImage(color, m_rgbImage);
	//}
	if (pc->width != color->width || pc->height != color->height)
	{
		return false;
	}
	m_width = pc->width;
	m_height = pc->height;

	for (int r = 0; r < m_height; r++)
	{
		byte *pColor = reinterpret_cast<byte*>(color->imageData + r * color->widthStep);
		double *pCloud = reinterpret_cast<double*>(pc->imageData + r * pc->widthStep);
		for (int c = 0; c < m_width; c++)
		{
			m_color[r][c][0] = pColor[2];
			m_color[r][c][1] = pColor[1];
			m_color[r][c][2] = pColor[0];
			m_xyz[r][c][0] = pCloud[0];
			m_xyz[r][c][1] = pCloud[1];
			m_xyz[r][c][2] = pCloud[2];
			pColor += 3;
			pCloud += 3;
		}
	}

	m_pointCloudType = 2;
	if (bUpdateWindow)
	{
		Invalidate(FALSE);
	}
	return true;
}

void CVirtualScene::UpdatePointCloud(const std::vector<PointCloud>& vpc, bool bUpdateWindow)
{
	m_vpc = vpc;
	m_pointCloudType = 1;
	if (bUpdateWindow)
	{
		Invalidate(FALSE);
	}
}

BOOL CVirtualScene::SetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelDesc =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,
		0,0,0,0,0,0,
		0,
		0,
		0,
		0,0,0,0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0,0,0
	};

	m_GLPixelIndex = ChoosePixelFormat(hDC, &pixelDesc);
	if(m_GLPixelIndex == 0)
	{
		m_GLPixelIndex = 1;
		if(DescribePixelFormat(hDC, m_GLPixelIndex, sizeof(PIXELFORMATDESCRIPTOR), &pixelDesc) == 0)
		{
			return FALSE;
		}
	}

	return SetPixelFormat(hDC, m_GLPixelIndex, &pixelDesc);
}

BOOL CVirtualScene::CreateViewGLContext(HDC hDC)
{
	m_hGLContext = wglCreateContext(hDC);
	if(m_hGLContext == NULL)
	{
		return FALSE;
	}

	return wglMakeCurrent(hDC, m_hGLContext);
}


BEGIN_MESSAGE_MAP(CVirtualScene, CWnd)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
//	ON_WM_KEYUP()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONDBLCLK()
ON_WM_MOUSELEAVE()
ON_WM_MOUSEWHEEL()
ON_WM_KEYDOWN()
ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()



// CVirtualScene 消息处理程序
void CVirtualScene::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	GLsizei width,height;
	GLdouble aspect;
	width = cx;
	height = cy;
	if(cy == 0)
	{
		aspect = (GLdouble)width;
	}
	else
	{
		aspect = (GLdouble)width / (GLdouble)height;
	}
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	gluPerspective(60, aspect, 0.1, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glDrawBuffer(GL_BACK);
	glEnable(GL_DEPTH_TEST);
}


int CVirtualScene::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
	HWND hWnd = GetSafeHwnd();    
	HDC hDC = ::GetDC(hWnd);
	
	if (!SetWindowPixelFormat(hDC))
	{
		return -1;
	}

	if (!CreateViewGLContext(hDC))
	{
		return -1;
	}

	return 0;
}


void CVirtualScene::OnDestroy()
{
	CWnd::OnDestroy();

	if(wglGetCurrentContext() != NULL)
	{
		wglMakeCurrent(NULL, NULL);
	}
	if(m_hGLContext != NULL)
	{
		wglDeleteContext(m_hGLContext);
		m_hGLContext = NULL;
	}
}

void ModelTransformation()
{
	glLoadIdentity();
	gluLookAt(0, 0, 0, 0, 0, 1, 0, 1, 0);

	glTranslatef(trans_x + offset_x, trans_y + offset_y, offset_z);
	glRotatef(rot_angle_x, 1, 0, 0);
	glRotatef(rot_angle_y, 0, 1, 0);
	glRotatef(rot_angle_z, 0, 0, 1);
}

inline byte CVirtualScene::GetColor(int r, int c, int i) const
{
	//return CV_IMAGE_ELEM(m_rgbImage, char, r, 3*c+2-i);
	return m_color[r][c][i];
}

inline double CVirtualScene::Get3DCoord(int r, int c, int i) const
{
	//return CV_IMAGE_ELEM(m_pointClouds, double, r, 3*c+i);
	return m_xyz[r][c][i];
}

void CVirtualScene::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	ModelTransformation();

	glPointSize(1);

	if (m_pointCloudType == 1)
	{
		glBegin(GL_POINTS);

		for (int i = 0; i < m_vpc.size(); i++)
		{
			PointCloud &pc = m_vpc[i];
			bColorScene ? glColor3b(pc.r, pc.g, pc.b) : glColor3f(1, 1, 1);
			glVertex3d(pc.x, pc.y, pc.z);
		}

		glEnd();
	}
	else
	{
		for (int r = 0; r < m_height; r++)
		{
			glBegin(GL_TRIANGLE_STRIP);
			for (int c = 0; c < m_width; c++)
			{
				glTexCoord2d(0.0, 0.0);
				glColor3b(GetColor(r,c,0), GetColor(r,c,1), GetColor(r,c,2));
				glVertex3d(Get3DCoord(r,c,0), Get3DCoord(r,c,1), Get3DCoord(r,c,2));

				if (c < m_width - 1)
				{
					glTexCoord2d(1.0, 0.0);
					glColor3b(GetColor(r,c+1,0), GetColor(r,c+1,1), GetColor(r,c+1,2));
					glVertex3d(Get3DCoord(r,c+1,0), Get3DCoord(r,c+1,1), Get3DCoord(r,c+1,2));
				}

				if (r < m_height - 1)
				{
					glTexCoord2d(0.0, 1.0);
					glColor3b(GetColor(r+1,c,0), GetColor(r+1,c,1), GetColor(r+1,c,2));
					glVertex3d(Get3DCoord(r+1,c,0), Get3DCoord(r+1,c,1), Get3DCoord(r+1,c,2));
				}

				if (c < m_width - 1 && r < m_height - 1)
				{
					glTexCoord2d(1.0, 1.0);
					glColor3b(GetColor(r+1,c+1,0), GetColor(r+1,c+1,1), GetColor(r+1,c+1,2));
					glVertex3d(Get3DCoord(r+1,c+1,0), Get3DCoord(r+1,c+1,1), Get3DCoord(r+1,c+1,2));
				}			
			}
			glEnd();
		}
	}

	glPointSize(5);
	glColor3b(255, 0, 0);
	glBegin(GL_POINTS);
	glVertex3dv(m_marker);
	glEnd();

	glEnable(GL_BLEND);  
	// enable read-only depth buffer  
	glDepthMask(GL_FALSE);  
	// set the blend function to what we use for transparency  
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);  
	// set back to normal depth buffer mode (writable)  
	glDepthMask(GL_TRUE);  
	// disable blending  
	glDisable(GL_BLEND);  

	glFlush();
	SwapBuffers(dc.GetSafeHdc());
}

BOOL CVirtualScene::PreCreateWindow(CREATESTRUCT& cs)
{
	//cs.style |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	return CWnd::PreCreateWindow(cs);
}

/*void CVirtualScene::OnLButtonDown(UINT nFlags, CPoint point)
{
	old_x = point.x;
	old_y = point.y;
	bAdjustView = true;

	CWnd::OnLButtonDown(nFlags, point);
}

void CVirtualScene::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (bAdjustView)
	{
		bAdjustView = false;	
		trans_x += offset_x;
		trans_y += offset_y;
		offset_x = 0;
		offset_y = 0;
	}	
}

void CVirtualScene::OnMouseMove(UINT nFlags, CPoint point)
{
	if (bAdjustView)
	{
		offset_x = static_cast<GLfloat>(old_x - point.x) / 500.0;
		offset_y = static_cast<GLfloat>(old_y - point.y) / 500.0;
		if (abs(offset_x) > 0.01 || abs(offset_y) > 0.01)
		{
			Invalidate(FALSE);
		}
	}	
}*/

void CVirtualScene::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	bAdjustView = false;
	InitView();
	Invalidate(FALSE);
}

void CVirtualScene::OnMouseLeave()
{
	bAdjustView = false;
}

BOOL CVirtualScene::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	offset_z += static_cast<GLfloat>(zDelta)/1200.0;
	if (abs(offset_z) > 0.01)
	{
		Invalidate(FALSE);
	}
	return TRUE;
}

void CVirtualScene::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_UP:
		rot_angle_x += ANGLESTEP;
		if (rot_angle_x >= 360)
		{
			rot_angle_x -= 360;
		}
		Invalidate(FALSE);
		break;

	case VK_DOWN:
		rot_angle_x -= ANGLESTEP;
		if (rot_angle_x < 0)
		{
			rot_angle_x += 360;
		}
		Invalidate(FALSE);
		break;

	case VK_LEFT:
		rot_angle_y += ANGLESTEP;
		if (rot_angle_y >= 360)
		{
			rot_angle_y -= 360;
		}
		Invalidate(FALSE);
		break;

	case VK_RIGHT:
		rot_angle_y -= ANGLESTEP;
		if (rot_angle_y < 0)
		{
			rot_angle_y += 360;
		}
		Invalidate(FALSE);
		break;

	case VK_SPACE:
		bColorScene = !bColorScene;
		break;

	default:
		break;
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CVirtualScene::OnRButtonDown(UINT nFlags, CPoint point)
{
	int viewport[4];
	double modelview[16];
	double projection[16];

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	ModelTransformation();
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glPopMatrix();

	int x = point.x;
	int y = viewport[3] - point.y;

	float z;
	glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
	gluUnProject(x, y, z, modelview, projection, viewport, m_marker, m_marker+1, m_marker+2);

	//CString pos_str;
	//pos_str.Format(_T("x=%.3f y=%.3f z=%.3f"), m_marker[0], m_marker[1], m_marker[2]);
	//MessageBox(pos_str);

	CWnd::OnRButtonDown(nFlags, point);
}
