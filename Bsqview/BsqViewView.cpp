// BsqViewView.cpp : implementation of the CBsqViewView class
//1

#include "stdafx.h"
#include "MainFrm.h"
#include "BsqView.h"
#include "BsqViewDoc.h"
#include "BsqViewView.h"
#include "math.h"
#include "BandMath.h"
#include "ClassSelectDlg.h"
#include "BigImageDlg.h"

#include <fstream> 
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBsqViewView

IMPLEMENT_DYNCREATE(CBsqViewView, CScrollView)

BEGIN_MESSAGE_MAP(CBsqViewView, CScrollView)
	//{{AFX_MSG_MAP(CBsqViewView)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_WM_MOUSEMOVE()
	ON_UPDATE_COMMAND_UI(ID_LEFT,OnUpdateXY)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_UPDATE_COMMAND_UI(ID_RIGHT,OnUpdateXY)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBsqViewView construction/destruction

CBsqViewView::CBsqViewView()
{
    Height = 0;  Width = 0;  Bands = 0;
	m_pathname = "";
	dataType = ByteType;

    m_classnum = 0;

	classnumber=0;
    bByteData=false;
    bStandardImage=false;
    bClassificationImage=false;
	m_bOpenProDlg = FALSE;
	m_bOpenFile = FALSE;

	byteflag=false;

	pBigPiData=NULL;
	m_ClassCount=0;
	m_fileopen = false;

	
}

CBsqViewView::~CBsqViewView()
{

}

BOOL CBsqViewView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	    cs.lpszClass=AfxRegisterWndClass
		(CS_VREDRAW|CS_HREDRAW,
		::LoadCursor(NULL,IDC_CROSS),
		(HBRUSH) ::GetStockObject(WHITE_BRUSH),
		0);

	return CScrollView::PreCreateWindow(cs);

}

/////////////////////////////////////////////////////////////////////////////
// CBsqViewView drawing

void CBsqViewView::OnDraw(CDC* pDC)
{
	CBsqViewDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here


}

void CBsqViewView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

}

/////////////////////////////////////////////////////////////////////////////
// CBsqViewView printing

BOOL CBsqViewView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBsqViewView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBsqViewView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CBsqViewView diagnostics

#ifdef _DEBUG
void CBsqViewView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CBsqViewView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CBsqViewDoc* CBsqViewView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBsqViewDoc)));
	return (CBsqViewDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBsqViewView message handlers

void CBsqViewView::OnFileOpen() 
{
	// TODO: Add your command handler code here
	CBsqViewDoc* pDoc = GetDocument();
    CFileDialog m_opendlg(TRUE);
    CStringArray strc;
	CString strt;
    CString str06;
	if(m_opendlg.DoModal()==IDOK)        //�����ļ����öԻ���
	{
		
		m_filename=m_opendlg.GetFileName();//�õ��ļ����ļ���
		m_pathname=m_opendlg.GetPathName();//�õ��ļ���·����
		
		header = m_pathname + ".txt";//����ͷ�ļ�������
		CFileFind finder;
        BOOL bfind = finder.FindFile(header);
		//�Ѹ��ļ���
		if(!m_file.Open(m_pathname,CStdioFile::modeRead|CStdioFile::typeBinary))  
		{
			MessageBeep(MB_ICONINFORMATION);
			AfxMessageBox("�㲻�ܴ�Դ�ļ�!");
			return;
		}
		m_fileopen = true;
		
		//���ͷ�ļ������ڣ��ͽ���һ���µ�ͷ�ļ�
		if(!bfind)
		{
			if(Imageinfo.DoModal()==IDOK)      //������ʾ��������Ի���        
			{
				//���ļ�����
				CString str01,str02,str03,str04,str05;
				CStdioFile headerfile;
				str01.Format("bands: %d\n", Imageinfo.m_bands);
				str02.Format("samples: %d\n",Imageinfo.m_samples);
				str03.Format("lines: %d\n", Imageinfo.m_lines);
			
				if(Imageinfo.DataResult == IDC_BYTE)
				{   
					bByteData = TRUE;
					byteflag=true;
					dataType = ByteType;
					//ͷ�ļ���Ϣ
					str04.Format("datatype: byte\n");
				}

				if(Imageinfo.FileResult==IDC_STANDARD_IMAGE)
				{
					//����ļ������Ǳ�׼ͼ��
					bStandardImage = TRUE;
					//ͷ�ļ���Ϣ
					str05.Format("filetype: Standard Image\n");
				}
                if(Imageinfo.FileResult == IDC_TRACE_IMAGE)
				{
					str05.Format("filetype:  TRACE Image\n");
				}
			

				//ͷ�ļ�д��
				headerfile.Open(header,CFile::modeCreate|CFile::modeWrite);
				headerfile.WriteString(str01);
				headerfile.WriteString(str02);
				headerfile.WriteString(str03);
				headerfile.WriteString(str04);
				headerfile.WriteString(str05);
			
		
				headerfile.Close();
				bfind = TRUE;
			}
		}
		//=================================================================================
		//��ͼ��Ĳ��֣���ȡͷ�ļ���

			int length;
			CFile headfile;
			headfile.Open(header,CFile::modeRead);//��ȡͷ�ļ�
			length = headfile.GetLength();
			headfile.Close();
			
			CString hbands,hsamples,hlines,hdatatype,hfiletype,hclassnum,hr,hg,hb,hsensor;
			int r,g,b;
			r = 0;
			g = 100;
			b = 100;

			char *pIn = new char[length];
			fstream fin2(header,ios::in);
			
			//����ѡ���εĶԻ��򣬰�ͷ�ļ�������Ӧ����Ϣ�������Ժ󴫸���Ӧ�ı���
			for(int input = 0; input < length; input++)
			{
				fin2>>pIn;
				if(input == 1)
				{
					hbands=pIn;
				}
				if(input == 3)
				{
					hsamples = pIn;
				}
				if(input == 5)
				{
					hlines = pIn;
				}
				if(input == 7)
				{
					hdatatype = pIn;
				}
				if(input == 9)
				{
					hfiletype = pIn;
				}
			}
			//��ѡ����
			
			if(hdatatype == "byte")
			{
				bByteData = TRUE;
				dataType = ByteType;
				byteflag=true;
			}

			if(hfiletype == "Standard")
			{
				m_bOpenFile = TRUE;
				bStandardImage = TRUE;
			}

			delete pIn;

			//Ӱ�����Ϣͨ��ͷ�ļ�ֱ�Ӹ���
			Bands=atoi(hbands);
			bi.biWidth=Width=atoi(hsamples);
			bi.biHeight=Height=atoi(hlines);
			m_classnum = atoi(hclassnum);
					
			//�������ù�����
			CSize sizeTotal;
			sizeTotal.cx = Width;
			sizeTotal.cy = Height;
			SetScrollSizes(MM_TEXT, sizeTotal);
			
			//�Ѷ������Ϣ��������ѡ��Ի���
			CString str,str1;
			CSelBandsDlg selBands;
			selBands.m_sarrBands.Add(m_filename);
			for(int i=1;i<=Bands;i++)
			{  
				str="     �� ��";
				str1.Format("%d",i);
				str=str+str1;
				fBandsString[i] = str;
				selBands.m_sarrBands.Add(str);
			}
			
			if(selBands.DoModal()==IDOK)  //����ѡ�񲨶ζԻ���
			{  
				//ȷ�������Ի����С
				if(Width>=400)    WBig=400;
				else  WBig=Width;
				if(Height>=400)   HBig=400;
				else  HBig=Height;
				
				ScanXPos=0;
				ScanYPos=HBig; 
	
				//������ʾͼ��ĶԻ���
				bigdlg=new CBigImageDlg;
				bigdlg->Create(IDD_BIG_IMAGE_DLG,this);
				bigdlg->SetWindowPos(&wndTop,40,60,WBig+9,HBig+28,NULL);
				bigdlg->ShowWindow(SW_SHOW);
							
				str="123456789";
				str1="";
				int nIndex;
				
				//����RGB����ͨ��ѡ�񣬸�֪ѡ�����ļ�������
				nIndex=selBands.m_static_r.FindOneOf(str);
				int fMaxN = nIndex + 6;
				while((nIndex!=-1)&&(nIndex < fMaxN))
				{
					str1+=selBands.m_static_r.GetAt(nIndex);
					selBands.m_static_r.Delete(nIndex,1);
					nIndex=selBands.m_static_r.FindOneOf(str);
				}
				red=atoi(str1);
				
				str1="";
				nIndex=selBands.m_static_g.FindOneOf(str);
				while((nIndex!=-1)&&(nIndex < fMaxN))
				{
					str1+=selBands.m_static_g.GetAt(nIndex);
					selBands.m_static_g.Delete(nIndex,1);
					nIndex=selBands.m_static_g.FindOneOf(str);
				}
				green=atoi(str1);
				
				str1="";
				nIndex=selBands.m_static_b.FindOneOf(str);
				while((nIndex!=-1)&&(nIndex < fMaxN))
				{
					str1+=selBands.m_static_b.GetAt(nIndex);
					selBands.m_static_b.Delete(nIndex,1);
					nIndex=selBands.m_static_b.FindOneOf(str);
				}
				blue=atoi(str1);
				
				//����ͼ����ʾ������Ĳ���
				bgbi.biWidth=WBig;
				bgbi.biHeight=HBig;
				bgbi.biSizeImage=bgbi.biWidth*bgbi.biHeight;
				bgbi.biSize =(DWORD)sizeof(BITMAPINFOHEADER);

				//����һЩ����������
				bgbi.biBitCount=(DWORD)24;
				bgbi.biPlanes=(DWORD)1;
				bgbi.biCompression=0;
				bgbi.biXPelsPerMeter=0;
				bgbi.biYPelsPerMeter=0;
				bgbi.biClrUsed=0;
				bgbi.biClrImportant=0; 
				
				pbi=(BITMAPINFO*)HeapAlloc(GetProcessHeap(),0,sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD));
				memcpy(pbi,&bi,sizeof(bi));
				m_NewWidth=Width;
				m_NewHeight=Height;

				//��������״̬Ϊ�ȴ�״̬
				SetCursor(LoadCursor(NULL,IDC_WAIT));
				m_file.SeekToBegin();

				//������ѡ�񲨶�
				BandsSelection();
				m_file.Close();
			//	pDoc->UpdateAllViews(NULL,0,NULL);
		}
	  }
	  
	else
    	return;
	//���ô��ļ��Ի���
}

void CBsqViewView::BandsSelection()
{
	//�����ֽ������ݵĻ�
	if(bByteData)
	{
		//���Ǳ�׼ͼ��
		if(bStandardImage)
		{
			lpbigshowbuf=(BYTE*)HeapAlloc(GetProcessHeap(),0,bgbi.biSizeImage*3);
			pBigPiDataHead=pBigPiData=(LPSTR)lpbigshowbuf;

			//��ͼ����м���	
			for(int i=bgbi.biHeight;i>0;i--)
			{ 
				//�ֱ��ȡRGB����ͨ����ֵ
				m_file.SeekToBegin();
				m_file.Seek(Width*Height*(blue-1),CFile::current);
				m_file.Seek(Width*(i-1),CFile::current);
				m_file.Read(lpshowbufB,bgbi.biWidth);
				
				m_file.SeekToBegin();
				m_file.Seek(Width*Height*(red-1),CFile::current);
				m_file.Seek(Width*(i-1),CFile::current);
				m_file.Read(lpshowbufR,bgbi.biWidth);
				
				m_file.SeekToBegin();
				m_file.Seek(Width*Height*(green-1),CFile::current);
				m_file.Seek(Width*(i-1),CFile::current);
				m_file.Read(lpshowbufG,bgbi.biWidth);
				

				//����һ���Ķ�Ӧ��ϵ����RGB��ͨ����ֵ����ת��������ֵ��pBigPiData
				for(int j=0;j<bgbi.biWidth;j++)
				{  			
					//����ɫ���µ���ʾӳ���ϵ
					if(lpshowbufB[j]<=63)
						*pBigPiData=0;
					else
					{
						if(lpshowbufB[j]==64)
							*pBigPiData=10;
						if(lpshowbufR[j]%3==0)
						{
							if((lpshowbufB[j]-63)*10+
								((lpshowbufB[j]-63)/3-1)*2+1>255)
								*pBigPiData=(unsigned char)255;
							else
								*pBigPiData=(lpshowbufR[j]-63)*10+
								((lpshowbufB[j]-63)/3-1)*2+1;
						}
						else 
						{
							if((lpshowbufB[j]-1)%3==0)
							{
								if((lpshowbufB[j]-1-63)*10+
									((lpshowbufR[j]-1-63)/3-1)*2+1+11>255)
									*pBigPiData=(unsigned char)255;
								else 
									*pBigPiData=(lpshowbufB[j]-1-63)*10+
									((lpshowbufB[j]-1-63)/3-1)*2+1+11;
							}
							
							if((lpshowbufB[j]+1)%3==0)
							{
								if((lpshowbufB[j]+1-63)*10+
									((lpshowbufB[j]+1-63)/3-1)*2+1-10>255)
									*pBigPiData=(unsigned char)255;
								else
									*pBigPiData=(lpshowbufB[j]+1-63)*10+
									((lpshowbufB[j]+1-63)/3-1)*2+1-10;
							}
						}
					}
					
					//����ɫ���µ���ʾӳ���ϵ
					if(lpshowbufG[j]<=26)
						*(pBigPiData+1)=0;
					else
					{
						if((lpshowbufG[j]-26)*15>255)
							*(pBigPiData+1)=(unsigned char)255;
						else
							*(pBigPiData+1)=(lpshowbufG[j]-26)*15;
					}
					
					//�Ժ�ɫ���µ���ʾӳ���ϵ
					if(lpshowbufR[j]<=24)
						*(pBigPiData+2)=0;
					else 
					{
						if((lpshowbufR[j]-24)*8-1>255)
							*(pBigPiData+2)=(unsigned char)255;
						else
							*(pBigPiData+2)=(lpshowbufR[j]-24)*8-1;
					}
					
					pBigPiData=pBigPiData+3;
					
				}
			}
			return ;
	   }
	}

}


void CBsqViewView::OnUpdateXY(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE); //ʹ�����ı��ܱ�����
    
}

void CBsqViewView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CScrollView::OnMouseMove(nFlags, point);
}


