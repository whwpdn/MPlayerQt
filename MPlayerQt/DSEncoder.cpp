// DSEncoder.cpp: implementation of the CDSEncoder class.
//
//////////////////////////////////////////////////////////////////////
#include "DSEncoder.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DSEncoder::DSEncoder()
{
	m_pGraphBuilder = NULL;
	BuildCodecArray();
}

DSEncoder::~DSEncoder()
{
	// Creal the codec collection
	QMap<QString, DSCodec*>::iterator it,end;
	for(it=mapCodec.begin(),end=mapCodec.end();it !=end;++it)
	{
		delete it.value();
	}
	mapCodec.clear();
	//int nNumberOfCodec = size();
	//for (int i=0; i<nNumberOfCodec; i++) {
	//	delete GetAt(i);
	//}
	//RemoveAll();
}

void DSEncoder::BuildCodecArray()
{
	HRESULT				hr;
	ICreateDevEnum*		pSysDevEnum = NULL;
	IEnumMoniker*		pEnum = NULL;
	IMoniker*			pMoniker = NULL;

	// System Device Enumerator
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void**) &pSysDevEnum);
	if (FAILED(hr)) {
		// ERROR HERE
		return;
	}

	// Moniker Enumerator
	//hr = pSysDevEnum->CreateClassEnumerator(CLSID_AudioCompressorCategory, &pEnum, 0);
	//if (FAILED(hr)) {
	//	// ERROR HERE
	//	pSysDevEnum->Release();
	//	return;
	//}
	hr = pSysDevEnum->CreateClassEnumerator(CLSID_VideoCompressorCategory, &pEnum, 0);
	if (FAILED(hr)) {
		// ERROR HERE
		pSysDevEnum->Release();
		return;
	}
	// Cycle throught IMoniker collection
	while (pEnum->Next(1, &pMoniker, NULL) == S_OK) {
		// New instance of CDSCodec
		DSCodec *pCodec = new DSCodec();
		pCodec->m_pMoniker = pMoniker;

		// Retrieve codec name
		IPropertyBag *pPropertyBag = NULL;
		hr = pMoniker->BindToStorage(NULL, NULL, IID_IPropertyBag, (void**) &pPropertyBag);
		if (SUCCEEDED(hr)) {
			VARIANT var;
			VariantInit(&var);
			pPropertyBag->Read(L"FriendlyName", &var, NULL);
			QString strTempName((QChar*)var.bstrVal,wcslen(var.bstrVal));
			pCodec->m_strCodecName = strTempName;
			VariantClear(&var);
			// Add new instance to collection
			mapCodec[pCodec->m_strCodecName] = pCodec;
			pCodec->BuildCodecFormatArray();
		}
		delete pCodec;
	}

	// Lib?ation des enumerators
	pEnum->Release();
	pSysDevEnum->Release();
}

HRESULT DSEncoder::AddFilterByClsid(IGraphBuilder *pGraph, LPCWSTR wszName, const GUID& clsid, IBaseFilter **ppF)
{
    *ppF = NULL;
    HRESULT hr = CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER,
       IID_IBaseFilter, (void**)ppF);
    if (SUCCEEDED(hr))
    {
        hr = pGraph->AddFilter((*ppF), wszName);
    }
    return hr;
}

BOOL DSEncoder::SetFilterFormat(AM_MEDIA_TYPE* pStreamFormat, IBaseFilter* pBaseFilter)
{
	HRESULT hr;
	BOOL retVal = FALSE;

	// Pin enumeration
	IEnumPins* pEnumPins = NULL;
	hr = pBaseFilter->EnumPins(&pEnumPins);
	if (FAILED(hr)) {
		// ERROR HERE
		return FALSE;
	}

	IPin* pPin = NULL;
	while (pEnumPins->Next(1, &pPin, 0) == S_OK) {
		PIN_DIRECTION sDirection;
		pPin->QueryDirection(&sDirection);
		// Output Pin ?
		if (sDirection == PINDIR_OUTPUT) {
			IAMStreamConfig* pStreamConfig = NULL;
			hr = pPin->QueryInterface(IID_IAMStreamConfig, (void**) &pStreamConfig);
			if (SUCCEEDED(hr)) {
				hr = pStreamConfig->SetFormat(pStreamFormat);
				if (SUCCEEDED(hr)) {
					retVal = TRUE;
				}
				pStreamConfig->Release();
			}
		}
		pPin->Release();
	}

	// Free memory
	pEnumPins->Release();

	return retVal;
}
/*
void DSEncoder::BuildGraph(QString strSrcFileName, QString strDestFileName,QString strCodecName, int nFormat)
{
	HRESULT hr;
//	IBaseFilter *pParser = NULL, *pCodec = NULL, *pMux = NULL, *pDest = NULL;
	IBaseFilter *pSplitter = NULL,*pColour = NULL, *pCodec = NULL, *pMux = NULL, *pDest = NULL;

	IFileSinkFilter* pSink = NULL;
	IFileSourceFilter* pSourceFilter = NULL;

	//GUID CLSID_WavParser;
	//UuidFromString((unsigned char*)"3C78B8E2-6C4D-11D1-ADE2-0000F8754B99", &CLSID_WavParser);
	//GUID CLSID_WavDest;
	//UuidFromString((unsigned char*)"D51BD5A1-7548-11CF-A520-0080C77EF58A", &CLSID_WavDest);

	GUID CLSID_Test_Mpeg4VideoSplitter;
	UuidFromString((RPC_WSTR)"D3D9D58B-45B5-48AB-B199-B8C40560AEC7", &CLSID_Test_Mpeg4VideoSplitter);

	GUID CLSID_Test_Mpeg4sDecoderDMO;
	UuidFromString((RPC_WSTR)"4A69B442-28BE-4991-969C-B500ADF5D8A8", &CLSID_Test_Mpeg4sDecoderDMO);

	GUID CLSID_Test_ffdshowEnc;
	UuidFromString((RPC_WSTR)"4DB2B5D9-4556-4340-B189-AD20110D953F", &CLSID_Test_ffdshowEnc);

	// GraphBuilder construction
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**) &m_pGraphBuilder);
	if (SUCCEEDED(hr)) {
		// Parse filter
		//hr = AddFilterByClsid(m_pGraphBuilder, L"Parser", CLSID_WavParser, &pParser);
		hr = AddFilterByClsid(m_pGraphBuilder, L"Avi Splitter", CLSID_AviSplitter, &pSplitter);
		hr = AddFilterByClsid(m_pGraphBuilder,L"Color Space Converter",CLSID_Colour,&pColour);
		// ACM codec filter
		IMoniker* pMoniker = mapCodec[strCodecName]->m_pMoniker;;
		pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter, (void**) &pCodec);
		hr = m_pGraphBuilder->AddFilter(pCodec, L"ffdshow video encoder");

		// Mux filter
		//hr = AddFilterByClsid(m_pGraphBuilder, L"WavDest", CLSID_WavDest, &pMux);

		// Output file filter
		hr = AddFilterByClsid(m_pGraphBuilder, L"File Writer", CLSID_FileWriter, &pDest);
		pDest->QueryInterface(IID_IFileSinkFilter, (void**) &pSink);
		pSink->SetFileName(strDestFileName.toStdWString().c_str(), NULL);
		pSink->Release();


		// Calculate output file size
		//CFileStatus fileStatus;
		//CFile::GetStatus(szSrcFileName, fileStatus);
		//int nInputFileSize = fileStatus.m_size;
		//// Assuming 44kHz, 16bits, stereo
		//int nMediaTime = (int) (nInputFileSize / (44000 * 2 * 2));
		//WAVEFORMATEX *pWav = (WAVEFORMATEX *) GetAt(nCodec)->GetAt(nFormat)->m_pMediaType->pbFormat;
		//int nOutputSize = nMediaTime * (pWav->nAvgBytesPerSec);

		// Check for output File
		try {
			QFile f(strDestFileName);
			if(f.exists())
				f.remove();
		} catch(...) {
			// nothing to do
		}

		// Render Graph
		//hr = m_pGraphBuilder->RenderFile(strSrcFileName.AllocSysString(), NULL);
		hr = m_pGraphBuilder->RenderFile(strSrcFileName.toStdWString().c_str(), NULL);
		if (SUCCEEDED(hr)) {
			// Set Codec property
			//hr = SetFilterFormat(GetAt(nCodec)->GetAt(nFormat)->m_pMediaType, pCodec);
			


			// Retrieve control interfaces
			IMediaControl* pMediaControl = NULL;
			hr = m_pGraphBuilder->QueryInterface(IID_IMediaControl, (void**) &pMediaControl);
			if (SUCCEEDED(hr)) {
				hr = pMediaControl->Run();
				// start encoding
				if (SUCCEEDED(hr)) {
					long nCode = 0;
					IMediaEvent *pMediaEvent = NULL;
					m_pGraphBuilder->QueryInterface(IID_IMediaEvent, (void**) &pMediaEvent);
					int nPercentComplete = 0;

					// Wait until job complete
					while (nCode != EC_COMPLETE) {
						pMediaEvent->WaitForCompletion(1000, &nCode);
						// Report Progress
//						CFile::GetStatus(szDestFileName, fileStatus);
						//QString strPercent;
//						szPercent.Format("%d %%", (fileStatus.m_size*100)/nOutputSize);
						//AfxGetMainWnd()->SetWindowText(strPercent);
					}

					pMediaControl->Stop();
					//AfxGetMainWnd()->SetWindowText("Complete");
					printf("Complete\n");
					pMediaEvent->Release();
				} else {
					//char szError[1024];
					//AMGetErrorText(hr, szError, 1024);
					//CString szDesc(szError);
					//AfxMessageBox(szDesc);
					printf("error\n");
				}

				pMediaControl->Release();
			}
		}

		// Free interfaces
		pCodec->Release();
		//pParser->Release();
		pSplitter->Release();
		pColour->Release();
		pMux->Release();
		pDest->Release();

		m_pGraphBuilder->Release();
		m_pGraphBuilder = NULL;
	}
}*/
