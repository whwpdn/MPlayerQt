#ifndef MPLAYERQT_H
#define MPLAYERQT_H

#include <QtGui/QMainWindow>
#include "ui_mplayerqt.h"
#include <QCloseEvent>
#include <QFileDialog>

#include "DShowPlayer.h"
#include "DSCodec.h"
class MPlayerQt : public QMainWindow
{
	Q_OBJECT

public:
	MPlayerQt(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MPlayerQt();

protected:
	void closeEvent(QCloseEvent *event);
private:
	Ui::MPlayerQtClass ui;
private:
	void CreateActions();
	void CreateMenus();
	void CreateToolBars();
	void CreateStatusBar();
	void LoadFile(const QString &strFileName);
	//bool SaveFile(const QString &strFileName);
	bool SaveFile();
	void SetCurrentFile(const QString &strFileName);

private slots:
	void OpenSlot();
	void SaveSlot();
	void PlaySlot();
	void PauseSlot();
	void StopSlot();
private:
	QMenu *m_pFileMenu;
	QMenu *m_pFilterMenu;
	QMenu *m_pPlayMenu;
	QToolBar *m_pFileToolBar;
	QToolBar *m_pPlayToolBar;
	QAction *m_pOpenAct;
	QAction *m_pSaveAct;
	QAction *m_pQuitAct;
	QAction *m_pPlayAct;
	QAction *m_pStopAct;
	QAction *m_pPauseAct;

	QString m_strCurFile;
	DShowPlayer *m_pDSPlayer;
private:
	DSEncoder m_Encoder;

};

#endif // MPLAYERQT_H
