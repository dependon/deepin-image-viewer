#include "viewpanel.h"
#include <dimagebutton.h>
#include <QBoxLayout>
#include <QLabel>
#include <QResizeEvent>
#include <QMenu>
#include <QDebug>
#include "controller/signalmanager.h"
#include "imageinfowidget.h"
#include <darrowrectangle.h>
using namespace Dtk::Widget;

ViewPanel::ViewPanel(QWidget *parent)
    : ModulePanel(parent)
{
    connect(SignalManager::instance(), &SignalManager::viewImage, [this](QString path) {
        openImage(path);
        DatabaseManager::ImageInfo info = DatabaseManager::instance()->getImageInfoByPath(path);
        m_infos = DatabaseManager::instance()->getImageInfoByTime(info.time);
        m_current = std::find_if(m_infos.cbegin(), m_infos.cend(), [&](const DatabaseManager::ImageInfo info){ return info.path == path;});
    });
    m_view = new ImageWidget();
    QHBoxLayout *hl = new QHBoxLayout();
    setLayout(hl);
    hl->addWidget(m_view);

    m_nav = new NavigationWidget(this);
    connect(m_view, &ImageWidget::transformChanged, [this](){
        // TODO: check user settings
        if (!m_nav->isAlwaysHidden())
            m_nav->setVisible(!m_view->isWholeImageVisible());
        m_nav->setRectInImage(m_view->visibleImageRect());
    });
    connect(m_view, &ImageWidget::doubleClicked, this, &ViewPanel::toggleFullScreen);
    connect(m_nav, &NavigationWidget::requestMove, [this](int x, int y){
        m_view->setImageMove(x, y);
    });
}

QWidget *ViewPanel::toolbarBottomContent()
{
    QWidget *w = new QWidget();
    QHBoxLayout *hb = new QHBoxLayout();
    hb->setContentsMargins(0, 0, 0, 0);
    hb->setSpacing(10);
    w->setLayout(hb);
    DImageButton *btn = new DImageButton();
    btn->setNormalPic(":/images/icons/resources/images/icons/info-normal.png");
    btn->setHoverPic(":/images/icons/resources/images/icons/info-hover.png");
    btn->setPressPic(":/images/icons/resources/images/icons/info-active.png");
    hb->addWidget(btn);
    hb->addStretch();
    connect(btn, &DImageButton::clicked, SignalManager::instance(), &SignalManager::showExtensionPanel);

    btn = new DImageButton();
    btn->setNormalPic(":/images/icons/resources/images/icons/collect-normal.png");
    btn->setHoverPic(":/images/icons/resources/images/icons/collect-hover.png");
    btn->setPressPic(":/images/icons/resources/images/icons/collect-active.png");
    hb->addWidget(btn);

    btn = new DImageButton();
    btn->setNormalPic(":/images/icons/resources/images/icons/previous-normal.png");
    btn->setHoverPic(":/images/icons/resources/images/icons/previous-hover.png");
    btn->setPressPic(":/images/icons/resources/images/icons/previous-press.png");
    hb->addWidget(btn);
    connect(btn, &DImageButton::clicked, [this]() {
        if (m_current == m_infos.cbegin())
            return;
        --m_current;
        openImage(m_current->path);
    });

    btn = new DImageButton();
    btn->setNormalPic(":/images/icons/resources/images/icons/slideshow-normal.png");
    btn->setHoverPic(":/images/icons/resources/images/icons/slideshow-hover.png");
    btn->setPressPic(":/images/icons/resources/images/icons/slideshow-press.png");
    hb->addWidget(btn);

    btn = new DImageButton();
    btn->setNormalPic(":/images/icons/resources/images/icons/next-normal.png");
    btn->setHoverPic(":/images/icons/resources/images/icons/next-hover.png");
    btn->setPressPic(":/images/icons/resources/images/icons/next-press.png");
    hb->addWidget(btn);
    connect(btn, &DImageButton::clicked, [this]() {
        if (m_current == m_infos.cend())
            return;
        ++m_current;
        if (m_current == m_infos.cend()) {
            --m_current;
            return;
        }
        openImage(m_current->path);
    });

    btn = new DImageButton();
    btn->setNormalPic(":/images/icons/resources/images/icons/edit-normal.png");
    btn->setHoverPic(":/images/icons/resources/images/icons/edit-hover.png");
    btn->setPressPic(":/images/icons/resources/images/icons/edit-press.png");
    hb->addWidget(btn);
    connect(btn, &DImageButton::clicked, [this](){
        Q_EMIT SignalManager::instance()->editImage(m_view->imagePath());
    });

    hb->addStretch();

    btn = new DImageButton();
    btn->setNormalPic(":/images/icons/resources/images/icons/delete-normal.png");
    btn->setHoverPic(":/images/icons/resources/images/icons/delete-hover.png");
    btn->setPressPic(":/images/icons/resources/images/icons/delete-press.png");
    hb->addWidget(btn);
    return w;
}

QWidget *ViewPanel::toolbarTopLeftContent()
{
    QWidget *w = new QWidget();
    QHBoxLayout *hb = new QHBoxLayout();
    hb->setContentsMargins(0, 0, 0, 0);
    hb->setSpacing(0);
    w->setLayout(hb);
    DImageButton *btn = new DImageButton();
    btn->setNormalPic(":/images/icons/resources/images/icons/album-normal.png");
    btn->setHoverPic(":/images/icons/resources/images/icons/album-hover.png");
    btn->setPressPic(":/images/icons/resources/images/icons/album-active.png");
    hb->addWidget(btn);
    connect(btn, &DImageButton::clicked, SignalManager::instance(), &SignalManager::backToMainWindow);
    return w;
}

QWidget *ViewPanel::toolbarTopMiddleContent()
{
    QWidget *w = new QWidget();
    QHBoxLayout *hb = new QHBoxLayout();
    hb->setContentsMargins(0, 0, 0, 0);
    hb->setSpacing(10);
    w->setLayout(hb);
    hb->addStretch();
    DImageButton *btn = new DImageButton();
    btn->setNormalPic(":/images/icons/resources/images/icons/contrarotate-normal.png");
    btn->setHoverPic(":/images/icons/resources/images/icons/contrarotate-hover.png");
    btn->setPressPic(":/images/icons/resources/images/icons/contrarotate-press.png");
    hb->addWidget(btn);
    connect(btn, &DImageButton::clicked, m_view, &ImageWidget::rotateAntiClockWise);

    btn = new DImageButton();
    btn->setNormalPic(":/images/icons/resources/images/icons/clockwise-rotation-normal.png");
    btn->setHoverPic(":/images/icons/resources/images/icons/clockwise-rotation-hover.png");
    btn->setPressPic(":/images/icons/resources/images/icons/clockwise-rotation-press.png");
    hb->addWidget(btn);
    connect(btn, &DImageButton::clicked, m_view, &ImageWidget::rotateClockWise);

    btn = new DImageButton();
    btn->setNormalPic(":/images/icons/resources/images/icons/adapt-image-normal.png");
    btn->setHoverPic(":/images/icons/resources/images/icons/adapt-image-hover.png");
    btn->setPressPic(":/images/icons/resources/images/icons/adapt-image-active.png");
    btn->setToolTip(tr("1:1 Size"));
    hb->addWidget(btn);
    connect(btn, &DImageButton::clicked, [this](){
        m_view->resetTransform();
        m_view->setScaleValue(1);
    });
    //
#if 0
    btn = new DImageButton();
    btn->setNormalPic(":/images/icons/resources/images/icons/share-normal.png");
    btn->setHoverPic(":/images/icons/resources/images/icons/share-hover.png");
    btn->setPressPic(":/images/icons/resources/images/icons/share-active.png");
    hb->addWidget(btn);
#endif
    hb->addStretch();
    return w;
}

QWidget *ViewPanel::extensionPanelContent()
{
    m_info = new ImageInfoWidget();
    m_info->setImagePath(m_view->imagePath());
    return m_info;
}

void ViewPanel::resizeEvent(QResizeEvent *e)
{
    m_nav->move(e->size().width() - m_nav->width() - 10, e->size().height() - m_nav->height() -10);
}

void ViewPanel::contextMenuEvent(QContextMenuEvent *e)
{
    QMenu m;
    m.addAction(window()->isFullScreen() ? tr("Exit fullscreen") : tr("Fullscreen"), this, SLOT(toggleFullScreen()));
    m.exec(e->globalPos());
}

void ViewPanel::toggleFullScreen()
{
    if (window()->isFullScreen()) {
        window()->showNormal();
        Q_EMIT SignalManager::instance()->showBottomToolbar();
        Q_EMIT SignalManager::instance()->showTopToolbar();
    } else {
        window()->showFullScreen(); //full screen then hide bars because hide animation depends on height()
        Q_EMIT SignalManager::instance()->hideBottomToolbar();
        Q_EMIT SignalManager::instance()->hideExtensionPanel();
        Q_EMIT SignalManager::instance()->hideTopToolbar();
    }
}

void ViewPanel::openImage(const QString &path)
{
    Q_EMIT SignalManager::instance()->gotoPanel(this);
    m_view->setImage(path);
    m_nav->setImage(m_view->image());
    qDebug() << "view path: " << m_view->imagePath();
    if (m_info)
        m_info->setImagePath(path);
}