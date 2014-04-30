#include "mplayerqt.h"

MPlayerQt::MPlayerQt(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	
	CreateActions();
	CreateMenus();
	CreateStatusBar();
	CreateToolBars();

	SetCurrentFile("");
	setUnifiedTitleAndToolBarOnMac(true);
	m_pDSPlayer = new DShowPlayer();
}

MPlayerQt::~MPlayerQt()
{

}
void MPlayerQt::closeEvent(QCloseEvent *event)
{
	event->accept();
}

void MPlayerQt::OpenSlot()
{
	QString strFileName = QFileDialog::getOpenFileName(this);
	if(!strFileName.isEmpty())
	{//LoadFile(strFileName);
		m_strCurFile =strFileName ;
		m_pDSPlayer->OpenFile(strFileName.utf16());
	}
}
void MPlayerQt::SaveSlot()
{
	if(!m_strCurFile.isEmpty())	
		SaveFile();
}
void MPlayerQt::PlaySlot()
{
	m_pDSPlayer->Display(ui.labelVideo->winId());
	m_pDSPlayer->Play();
}
void MPlayerQt::StopSlot()
{
	m_pDSPlayer->Stop();
}
void MPlayerQt::PauseSlot()
{
	m_pDSPlayer->Pause();
}

bool MPlayerQt::SaveFile()
{
	QString strFileName = QFileDialog::getSaveFileName(this);
	if(strFileName.isEmpty())
		return false;
	return true;
	// savefile..
}
void MPlayerQt::CreateActions()
{
	m_pOpenAct = new QAction(QIcon("image/1.png"),tr("&Open"),this);
	m_pOpenAct->setStatusTip(tr("Open an existing file"));
	connect(m_pOpenAct,SIGNAL(triggered()),this,SLOT(OpenSlot()));

	m_pSaveAct = new QAction(QIcon("image/2.png"),tr("&Save"),this);
	m_pOpenAct->setStatusTip(tr("Save file"));
	connect(m_pSaveAct,SIGNAL(triggered()),this,SLOT(SaveSlot()));

	m_pQuitAct = new QAction(QIcon("image/2.png"),tr("&Quit"),this);
	m_pQuitAct->setStatusTip(tr("Open an existing file"));
	//connect(m_pQuitAct,SIGNAL(triggered()),this,SLOT(OpenSlot()));

	m_pPlayAct = new QAction(QIcon("image/3.png"),tr("Play"),this);
	connect(m_pPlayAct,SIGNAL(triggered()),this,SLOT(PlaySlot()));

	m_pStopAct = new QAction(QIcon("image/4.png"),tr("Stop"),this);
	connect(m_pStopAct,SIGNAL(triggered()),this,SLOT(StopSlot()));

	m_pPauseAct = new QAction(QIcon("image/5.png"),tr("Pause"),this);
	connect(m_pPauseAct,SIGNAL(triggered()),this,SLOT(PauseSlot()));
}
void MPlayerQt::CreateMenus()
{
	ui.menu_File->addAction(m_pOpenAct);
	ui.menu_File->addAction(m_pSaveAct);
	ui.menu_File->addAction(m_pQuitAct);

	ui.menu_Play->addAction(m_pPlayAct);
	ui.menu_Play->addAction(m_pStopAct);
	ui.menu_Play->addAction(m_pPauseAct);
}
void MPlayerQt::CreateStatusBar()
{

}
void MPlayerQt::CreateToolBars()
{

	m_pFileToolBar = addToolBar(tr("File"));
	m_pFileToolBar->addAction(m_pOpenAct);
	m_pFileToolBar->addAction(m_pSaveAct);
	//m_pFileToolBar->addAction(m_pOpenAct);
	m_pPlayToolBar = addToolBar(tr("Play"));
	m_pPlayToolBar->addAction(m_pPlayAct);
	m_pPlayToolBar->addAction(m_pStopAct);
	m_pPlayToolBar->addAction(m_pPauseAct);

}
void MPlayerQt::SetCurrentFile(const QString &strFileName)
{

}
void MPlayerQt::LoadFile(const QString &strFileName)
{
	//QFile file(strFileName);
	
}