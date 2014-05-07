#include <DShow.h>
#include <string>
#include <DSEncoder.h>

using namespace std;

class DShowPlayer
{
public:
	DShowPlayer();
	~DShowPlayer();

public:
	int OpenFile(LPCWSTR strFileName);
	int Play();
	void Stop();
	void Pause();
	void Display(HWND hWnd);


public:
	IGraphBuilder *m_pGraph;
	IMediaControl *m_pControl;
	IMediaEvent   *m_pEvent;
	IVideoWindow  *m_pVideoWindow;
	IMediaSeeking *m_pSeeking;

	IBaseFilter	*m_pSourceFilter;
	IBaseFilter *m_pDecoderFilter;
	IBaseFilter *m_pEncoderFilter;
	IBaseFilter	*m_pFileWriterFilter;


private:
	void ThrowIfError(HRESULT hr);

};