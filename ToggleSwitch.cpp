#include "ToggleSwitch.h"
#include <QMouseEvent>
#include <QPainter>
#include <QEasingCurve>

ToggleSwitch::ToggleSwitch(QWidget *parent)
    : QWidget(parent)
    , m_checked(false)
    , m_indicatorPosition(2)
    , m_backgroundColor(QColor("#d5d5d5"))
{
    setFixedSize(60, 30);

    m_animation = new QPropertyAnimation(this, "indicatorPosition", this);
    m_animation->setDuration(300);
    m_animation->setEasingCurve(QEasingCurve::OutCubic);

    m_colorAnimation = new QPropertyAnimation(this, "backgroundColor", this);
    m_colorAnimation->setDuration(300);
    m_colorAnimation->setEasingCurve(QEasingCurve::OutCubic);

    updateAnimation();
}

ToggleSwitch::~ToggleSwitch()
{
}

bool ToggleSwitch::isChecked() const
{
    return m_checked;
}

void ToggleSwitch::setChecked(bool checked)
{
    if (m_checked == checked)
        return;

    m_checked = checked;
    updateAnimation();
    emit toggled(m_checked);
}

QSize ToggleSwitch::sizeHint() const
{
    return QSize(60, 30);
}

void ToggleSwitch::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        setChecked(!m_checked);
        event->accept();
    } else {
        event->ignore();
    }
}

void ToggleSwitch::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Рисуем фон
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_backgroundColor);
    painter.drawRoundedRect(rect(), 15, 15);

    // Рисуем индикатор
    painter.setBrush(Qt::white);
    painter.drawEllipse(m_indicatorPosition, 2, 26, 26);
}

void ToggleSwitch::updateAnimation()
{
    if (m_checked) {
        m_animation->setStartValue(m_indicatorPosition);
        m_animation->setEndValue(32);
        m_colorAnimation->setStartValue(m_backgroundColor);
        m_colorAnimation->setEndValue(QColor("#FFD700"));
    } else {
        m_animation->setStartValue(m_indicatorPosition);
        m_animation->setEndValue(2);
        m_colorAnimation->setStartValue(m_backgroundColor);
        m_colorAnimation->setEndValue(QColor("#d5d5d5"));
    }

    m_animation->start();
    m_colorAnimation->start();
}

int ToggleSwitch::indicatorPosition() const
{
    return m_indicatorPosition;
}

void ToggleSwitch::setIndicatorPosition(int position)
{
    if (m_indicatorPosition != position) {
        m_indicatorPosition = position;
        update();
        emit indicatorPositionChanged(position);
    }
}

QColor ToggleSwitch::backgroundColor() const
{
    return m_backgroundColor;
}

void ToggleSwitch::setBackgroundColor(const QColor &color)
{
    if (m_backgroundColor != color) {
        m_backgroundColor = color;
        update();
        emit backgroundColorChanged(color);
    }
}
