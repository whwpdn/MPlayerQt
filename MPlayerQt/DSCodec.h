// DSCodec.h: interface for the CDSCodec class.
//
//////////////////////////////////////////////////////////////////////
#ifndef DSCODEC_H
#define DSCODEC_H
#include "dshow.h"
#include <QString>
#include <QMap>
#include <QFile>
class DSCodec
{
public:
	DSCodec();
	virtual ~DSCodec();

public:
	QString m_strCodecName;
	IMoniker	*m_pMoniker;
	AM_MEDIA_TYPE* m_pMediaType;

public:
	void BuildCodecFormatArray();
};
#endif // MPLAYERQT_H
