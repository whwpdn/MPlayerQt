#include <DShow.h>
#include <string>

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
	//IMediaEvent   *pEvent = NULL;
	IVideoWindow  *m_pVideoWindow;
	IMediaSeeking *m_pSeeking;

private:
	void ThrowIfError(HRESULT hr);
};