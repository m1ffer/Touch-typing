#ifndef CONTROLBUTTON_H
#define CONTROLBUTTON_H

#include <QPushButton>
#include <QPropertyAnimation>
#include <QSvgRenderer>
#include <QPaintEvent>
#include <QEnterEvent>
#include <QPainter>
#include <QIcon>

class ControlButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(QColor iconColor READ getIconColor WRITE setIconColor NOTIFY iconColorChanged)
    Q_PROPERTY(qreal iconScale READ getIconScale WRITE setIconScale NOTIFY iconScaleChanged)

public:
    explicit ControlButton(QWidget *parent = nullptr);
    ~ControlButton();

    // Константы
    static const int BUTTON_SIZE = 60;
    static const int ICON_SIZE = 24;
    static const int HOVER_ICON_SIZE = 28;
    static const QColor NORMAL_COLOR;
    static const QColor HOVER_COLOR;
    static const QColor PRESSED_COLOR;

    // Переопределяем setIcon для обработки SVG из QIcon
    void setIcon(const QIcon &icon);

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void setIconColor(const QColor &color);
    QColor getIconColor() const { return m_iconColor; }

    void setIconScale(qreal scale);
    qreal getIconScale() const { return m_iconScale; }

signals:
    void iconColorChanged();
    void iconScaleChanged();

private:
    void initialize();
    void startHoverAnimation();
    void startLeaveAnimation();
    void cleanup();
    void extractSvgPathFromIcon(const QIcon &icon);

    QSvgRenderer *m_svgRenderer;
    QColor m_iconColor;
    qreal m_iconScale;

    QPropertyAnimation *m_colorAnimation;
    QPropertyAnimation *m_scaleAnimation;

    bool m_isHovered;
    bool m_isPressed;
};

#endif // CONTROLBUTTON_H
