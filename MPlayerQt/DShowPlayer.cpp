#include "DShowPlayer.h"

DShowPlayer::DShowPlayer()
{
	m_pGraph=NULL;
	m_pControl=NULL;
	m_pVideoWindow=NULL;
}
DShowPlayer::~DShowPlayer()
{
	if(m_pGraph)
	{
		m_pGraph->Release();
		m_pGraph = NULL;
	}
	if(m_pControl)
	{
		m_pControl->Release();
		m_pControl = NULL;
	}
	if(m_pVideoWindow)
	{
		m_pVideoWindow->Release();
		m_pVideoWindow = NULL;
	}
}
int DShowPlayer::OpenFile(LPCWSTR strFileName)
{
	HRESULT hr;
	try
	{
		hr = CoInitialize(NULL);
		ThrowIfError(hr);

		hr = CoCreateInstance(CLSID_FilterGraph,NULL,CLSCTX_INPROC_SERVER,IID_IGraphBuilder,(void **)&m_pGraph);

		ThrowIfError(hr);

		hr = m_pGraph->QueryInterface(IID_IMediaControl,(void**)&m_pControl);
		hr = m_pGraph->QueryInterface(IID_IVideoWindow,(void**)&m_pVideoWindow);
		hr = m_pGraph->QueryInterface(IID_IMediaSeeking,(void**)&m_pSeeking);
		ThrowIfError(hr);

		hr = m_pGraph->RenderFile(strFileName,NULL);
		ThrowIfError(hr);
	}
	catch (TCHAR szERR[])
	{ 
		MessageBox(NULL,(LPCWSTR)szERR,TEXT("DirectSHow App"),0);
	}

	CoUninitialize();
	return hr;
}

int DShowPlayer::Play()
{
	 m_pControl->Run();
	 //m_pSeeking->Run();
	return 0;
}
void DShowPlayer::Stop()
{
	m_pControl->Stop();
	LONGLONG pos =0;
	m_pSeeking->SetPositions(&pos,AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning);

}
void DShowPlayer::Pause()
{
	m_pControl->Pause();
}	

void DShowPlayer::Display(HWND hWnd)
{	
	//display
	m_pVideoWindow->put_Owner((OAHWND)hWnd);
	RECT rcDisplay;
	::GetClientRect(hWnd,&rcDisplay);
	m_pVideoWindow->put_WindowStyle(WS_CHILD|WS_CLIPSIBLINGS);
	m_pVideoWindow->SetWindowPosition(0,0,rcDisplay.right,rcDisplay.bottom);
}
void DShowPlayer::ThrowIfError(HRESULT hr)
{
	if(FAILED(hr))
	{
		TCHAR szERR[MAX_ERROR_TEXT_LEN]={0,};
		DWORD res = AMGetErrorText(hr,szERR,MAX_ERROR_TEXT_LEN);
		if(res ==0)
		{
			StringCchPrintf(szERR,MAX_ERROR_TEXT_LEN,TEXT("Unknown Error: 0x%2x"),hr);
		}
		throw szERR;
	}

}