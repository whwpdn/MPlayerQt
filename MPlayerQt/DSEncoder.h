// DSEncoder.h: interface for the CDSEncoder class.
//
//////////////////////////////////////////////////////////////////////
#ifndef DSENCODER_H
#define DSENCODER_H
#include "dshow.h"
#include "DSCodec.h"
#include <QString>
class DSEncoder
{
public:
	DSEncoder();
	virtual ~DSEncoder();
	//void BuildGraph(QString strSrcFileName, QString strDestFileName, QString strCodecName, int nFormat);

public:
	QMap<QString, DSCodec*> mapCodec;

private:
	void BuildCodecArray();
	HRESULT AddFilterByClsid(IGraphBuilder *pGraph, LPCWSTR wszName, const GUID& clsid, IBaseFilter **ppF);
	BOOL SetFilterFormat(AM_MEDIA_TYPE* pStreamFormat, IBaseFilter* pBaseFilter);

	IGraphBuilder *m_pGraphBuilder;
};
#endif
