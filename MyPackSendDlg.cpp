// MyPackSendDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyPackSend.h"
#include "MyPackSendDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyPackSendDlg 对话框

pcap_if_t *alldevs;
char errbuf[PCAP_ERRBUF_SIZE];
UINT uiFileFlag=0;

CMyPackSendDlg::CMyPackSendDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyPackSendDlg::IDD, pParent)
	, csPathName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyPackSendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, CNetcardList);
	DDX_Text(pDX, IDC_EDIT1, csPathName);
}

BEGIN_MESSAGE_MAP(CMyPackSendDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CMyPackSendDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_FILEOPEN, &CMyPackSendDlg::OnBnClickedButtonFileopen)
	ON_BN_CLICKED(IDOK, &CMyPackSendDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_FILEOPENHEX, &CMyPackSendDlg::OnBnClickedButtonFileopenhex)
END_MESSAGE_MAP()


// CMyPackSendDlg 消息处理程序

BOOL CMyPackSendDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	pcap_if_t *d;

	int i=0;
	/* Retrieve the device list */
	if(pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		MessageBox(errbuf,errbuf);
		return FALSE;
	}

	/* Print the list */
	CString CTemp;
	for(d=alldevs; d; d=d->next)
	{
		CTemp.Format("%d. %s::%s", ++i, d->name,d->description);
		CNetcardList.AddString(CTemp);
	}
	/*



	printf("Enter the interface number (1-%d):",i);
	scanf("%d", &inum);

	/* Check if the user specified a valid adapter */
/*	if(inum < 1 || inum > i)
	{
		printf("\nAdapter number out of range.\n");

		/* Free the device list */
/*		pcap_freealldevs(alldevs);
		return -1;
	}

	/* Jump to the selected adapter */
/*	for(d=alldevs, i=0; i< inum-1 ;d=d->next, i++);*/

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyPackSendDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CMyPackSendDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMyPackSendDlg::OnBnClickedButtonFileopen()
{
	char szFilter[] = {"All Files(*.*)|*.*||"};
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter,NULL);
	if (dlg.DoModal()==IDOK)
	{
		csPathName=dlg.GetPathName();
		uiFileFlag=OPENED_FILE_TYPE_BIN;
		UpdateData(FALSE);
	}
	// TODO: 在此添加控件通知处理程序代码
}

void CMyPackSendDlg::OnBnClickedButtonSend()
{
	int inum=CNetcardList.GetCurSel();
	int i=0;
	if (inum==-1)
	{
		MessageBox("请选择网卡！");
		return;
	}
	pcap_if_t *d;
	pcap_t *fp;

	CString csTemp;
	
	
	for(d=alldevs, i=0; i< inum ;d=d->next, i++);


//	csTemp.Format("%s",d->name);
//	MessageBox(csTemp,csTemp);
	CFile cfSendFile;
	if (cfSendFile.Open(csPathName,CFile::modeRead)==0)
	{
		MessageBox("文件打开失败");
		return;
	}
	ULONGLONG iFileSize=cfSendFile.GetLength();
	if (iFileSize>1514)
	{
		MessageBox("文件过大，不能发送");
		cfSendFile.Close();
		return;
	}
	u_char* pCFileTempBuffer=new u_char[iFileSize];
	UINT iSendSize=0;
	u_char* pCFileBuffer=NULL;
	cfSendFile.Read(pCFileTempBuffer,iFileSize);
	if (uiFileFlag==OPENED_FILE_TYPE_BIN)
	{		
		pCFileBuffer=pCFileTempBuffer;
		iSendSize=iFileSize;
	}
	else if (uiFileFlag==OPENED_FILE_TYPE_HEX)
	{
		iSendSize=iFileSize/2;
		pCFileBuffer=new u_char[iSendSize];
		int m=0,n=0,f=0;
		for(m=0;m<iFileSize;m++)
		{
			if (pCFileTempBuffer[m]>='0'&&pCFileTempBuffer[m]<='9')
			{
				pCFileTempBuffer[m]-='0';
			}
			else if (pCFileTempBuffer[m]>='A'&&pCFileTempBuffer[m]<='Z')
			{
				pCFileTempBuffer[m]-='A'+10;
			}
			else if (pCFileTempBuffer[m]>='a'&&pCFileTempBuffer[m]<='z')
			{
				pCFileTempBuffer[m]=pCFileTempBuffer[m]-'a'+10;
			}
			else
			{
				continue;
			}
			f++;
			switch(f)
			{
			case 1:
				pCFileBuffer[n]=pCFileTempBuffer[m]*16;
				break;
			case 2:
				pCFileBuffer[n]+=pCFileTempBuffer[m];
				f=0;
				n++;
				break;
			}
		}
		iSendSize=n;
		delete[]pCFileTempBuffer;
	}
//	csTemp.Format("%d",cfSendFile.GetLength());
//	MessageBox(csTemp,csTemp);
	
	if ((fp = pcap_open_live(d->name,		// name of the device
		65536,			// portion of the packet to capture. It doesn't matter in this case 
		1,				// promiscuous mode (nonzero means promiscuous)
		1000,			// read timeout
		errbuf			// error buffer
		)) == NULL)
	{
		MessageBox("网卡打开失败");
		return;
	}
	/* Send down the packet */
	if (pcap_sendpacket(fp,	// Adapter
		pCFileBuffer,				// buffer with the packet
		iSendSize				// size
		) != 0)
	{
		MessageBox("报文发送失败！");
		cfSendFile.Close();
		return;
	}

	pcap_close(fp);	
	cfSendFile.Close();
	delete[] pCFileBuffer;
	MessageBox("发送完毕");
	return;

	// TODO: 在此添加控件通知处理程序代码
}



void CMyPackSendDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	pcap_freealldevs(alldevs);
	OnOK();
}

void CMyPackSendDlg::OnBnClickedButtonFileopenhex()
{
	char szFilter[] = {"All Files(*.*)|*.*||"};
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter,NULL);
	if (dlg.DoModal()==IDOK)
	{
		csPathName=dlg.GetPathName();
		uiFileFlag=OPENED_FILE_TYPE_HEX;
		UpdateData(FALSE);
	}

	// TODO: 在此添加控件通知处理程序代码
}
