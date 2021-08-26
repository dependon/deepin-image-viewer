/*
 * Copyright (C) 2020 ~ 2021 Uniontech Software Technology Co., Ltd.
 *
 * Author:     ZhangYong <zhangyong@uniontech.com>
 *
 * Maintainer: ZhangYong <ZhangYong@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <DWidget>
#include <DMainWindow>
#include <DTitlebar>
#include <DSearchEdit>
#include <DLabel>
#include <DTabBar>
#include <DSuggestButton>
#include <DProgressBar>

#include <QListWidget>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QStackedWidget>
#include <QStatusBar>
#include <QButtonGroup>

const QString SETTINGS_GROUP = "MAINWINDOW";
const QString SETTINGS_WINSIZE_W_KEY = "WindowWidth";
const QString SETTINGS_WINSIZE_H_KEY = "WindowHeight";
DWIDGET_USE_NAMESPACE
class HomePageWidget;
class ImageViewer;
class QSettings;
class MainWindow : public DWidget
{
    Q_OBJECT

public:
    //增加单例接口，方便closeFromMenu调用
    static MainWindow &instance()
    {
        static MainWindow w;
        return w;
    }
    explicit MainWindow();
    ~MainWindow() override;
    void setDMainWindow(DMainWindow *mainwidow);

    void setValue(const QString &group, const QString &key,
                  const QVariant &value);
    QVariant value(const QString &group, const QString &key,
                   const QVariant &defaultValue = QVariant());

private:

    void initUI();
protected:
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;

public slots:
    void slotOpenImg();
    void slotDrogImg(const QStringList &paths);
private:
    QStackedWidget   *m_centerWidget = nullptr;
    HomePageWidget   *m_homePageWidget = nullptr;
    ImageViewer      *m_imageViewer = nullptr;
    DMainWindow      *m_mainwidow = nullptr;
    QSettings        *m_settings = nullptr;
};

#endif // MAINWINDOW_H