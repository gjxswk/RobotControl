// RobotControlDoc.cpp : implementation of the CRobotControlDoc class
//

#include "stdafx.h"
#include "RobotControl.h"
#include "afxwin.h"

#include "RobotControlMultiDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define  BACKGROUND     1
#define  OGL_AXIS_DLIST 2
/////////////////////////////////////////////////////////////////////////////
// CRobotControlDoc

IMPLEMENT_DYNCREATE(CRobotControlMultiDoc, CDocument)

BEGIN_MESSAGE_MAP(CRobotControlMultiDoc, CDocument)
	//{{AFX_MSG_MAP(CRobotControlDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CRobotControlMultiDoc::CRobotControlMultiDoc()   //---乐--模型读取函数
{
	// TODO: add one-time construction code here
	
	doc_delay = new CRobotControlDoc();
	doc_real = new CRobotControlDoc();
}

CRobotControlMultiDoc::~CRobotControlMultiDoc()  //---乐---析构函数
{
}

BOOL CRobotControlMultiDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	doc_delay->OnNewDocument();
	doc_real->OnNewDocument();
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CRobotControlDoc serialization

void CRobotControlMultiDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRobotControlDoc diagnostics

#ifdef _DEBUG
void CRobotControlMultiDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRobotControlMultiDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRobotControlDoc commands

void CRobotControlMultiDoc::RenderScene()
{
    
	/*POSITION pos = this->GetFirstViewPosition();
	CView *main_view;
	if (pos != NULL) {
		main_view = GetNextView(pos);
	} else {
		return;
	}
	// 获得窗口客户区大小
	int x_client, y_client;
	CRect client_rect = CRect(0, 0, 100, 100);
	GetClientRect(main_view->m_hWnd, &client_rect);
	x_client = client_rect.Width(), y_client = client_rect.Height();


	// 分别更新两个窗口子客户区
	glClearColor(1.0,1.0,1.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
    glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,doc_delay->aspect,0.1,200.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glViewport(0,0,x_client/2,y_client/2);
	doc_delay->RenderScene();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,doc_real->aspect,0.1,200.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0,0,x_client/2,y_client/2);
	doc_real->RenderScene();*/
	
}

void CRobotControlMultiDoc::PrintfBitmap( LPCTSTR lpszText, GLfloat x, GLfloat y, GLfloat z)
{
	CBitmap mfcBitmap;  //位图数据
	BITMAP bm;
	SIZE size;
	UCHAR* pBmpBits; //位图像素位
	HFONT hOldFont;  //旧字体指针
	HBITMAP hPrevBmp;
	
	HDC hdc = ::wglGetCurrentDC(); //获得当前设备场景
	
	//if(!m_hFont) m_hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);		// use default system gui font
	//if(!m_hFont) m_hFont = (HFONT)::GetStockObject(SYSTEM_FONT);		// use default system font
	
	// 选择新字体,保存旧字体
	//hOldFont = (HFONT)SelectObject(hdc, m_hFont); 
	
	::GetTextExtentPoint32(hdc, lpszText, strlen(lpszText), &size);
	
	// 创建字符位图对象(对应字符位图空间)
	mfcBitmap.CreateBitmap(size.cx, size.cy, 1, 1, NULL);
	
	// 创建与hdc兼容的暂存DC
	HDC hMemDC = ::CreateCompatibleDC(hdc);
	
	if(hMemDC)
	{   //选择字符位图到暂存DC
		hPrevBmp = (HBITMAP)SelectObject(hMemDC,mfcBitmap);
	}
	
	HFONT hPrevFont = (HFONT)SelectObject(hMemDC, m_hFont);
	
	SetBkColor(hMemDC, RGB(0, 0, 0));
	SetTextColor(hMemDC, RGB(255, 255, 255));
	SetBkMode(hMemDC, OPAQUE);
	
	// 输出文字到暂存DC
	TextOut(hMemDC, 0, 0, lpszText, strlen(lpszText));
	
	// copy GDI bitmap to DIB
	mfcBitmap.GetBitmap(&bm); //获得字符位图数据
	
	size.cx = (bm.bmWidth + 31) & (~31);
	size.cy = bm.bmHeight;
	
	int bufsize = size.cy * (size.cx /8);
	
	pBmpBits = new UCHAR[bufsize];
	memset(pBmpBits, 0, sizeof(UCHAR)*bufsize);
	
	struct
	{
		BITMAPINFOHEADER bih;
		RGBQUAD col[2];
	}bic;
	
	BITMAPINFO *binf = (BITMAPINFO *)&bic;
	binf->bmiHeader.biSize = sizeof(binf->bmiHeader);
	binf->bmiHeader.biWidth = bm.bmWidth;
	binf->bmiHeader.biHeight = bm.bmHeight;
	binf->bmiHeader.biPlanes = 1;
	binf->bmiHeader.biBitCount = 1;
	binf->bmiHeader.biCompression = BI_RGB;
	binf->bmiHeader.biSizeImage = bufsize;
	binf->bmiHeader.biXPelsPerMeter = 1;
	binf->bmiHeader.biYPelsPerMeter = 1;
	binf->bmiHeader.biClrUsed = 0;
	binf->bmiHeader.biClrImportant = 0;
	
	::GetDIBits(hdc, mfcBitmap, 0, bm.bmHeight, pBmpBits, binf,DIB_RGB_COLORS);
	::SelectObject(hMemDC, hPrevBmp);
	::DeleteDC(hMemDC);
	
	//::SelectObject(hdc, hOldFont);  // 设回旧字体
	
	::glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //控制像素存储
	::glRasterPos3f(x,y,z);                  //
	::glBitmap(size.cx, size.cy, 0.0, 0.0, size.cx, 0.0, pBmpBits); //写位图到缓存
	
	delete pBmpBits;
}