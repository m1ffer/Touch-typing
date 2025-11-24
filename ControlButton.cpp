#include "ControlButton.h"
#include <QMouseEvent>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

// Определение констант
const QColor ControlButton::NORMAL_COLOR = QColor(100, 100, 100);
const QColor ControlButton::HOVER_COLOR = QColor(200, 200, 200);
const QColor ControlButton::PRESSED_COLOR = QColor(50, 150, 250);

ControlButton::ControlButton(QWidget *parent)
    : QPushButton(parent)
    , m_svgRenderer(nullptr)
    , m_iconColor(NORMAL_COLOR)
    , m_iconScale(1.0)
    , m_colorAnimation(nullptr)
    , m_scaleAnimation(nullptr)
    , m_isHovered(false)
    , m_isPressed(false)
{
    initialize();
}

ControlButton::~ControlButton()
{
    cleanup();
}

void ControlButton::initialize()
{
    // Устанавливаем фиксированный размер кнопки
    setFixedSize(BUTTON_SIZE, BUTTON_SIZE);

    // Базовые стили - прозрачный фон, но не перезаписываем полностью
    QString baseStyle =
        "QPushButton {"
        "    background: transparent;"
        "    border: none;"
        "}"
        "QPushButton:hover {"
        "    background: transparent;"
        "}"
        "QPushButton:pressed {"
        "    background: transparent;"
        "}";

    setStyleSheet(baseStyle);

    // Инициализация анимаций с увеличенной длительностью для плавности
    m_colorAnimation = new QPropertyAnimation(this, "iconColor", this);
    m_colorAnimation->setDuration(300); // Увеличили с 200 до 300 мс
    m_colorAnimation->setEasingCurve(QEasingCurve::OutCubic); // Добавили плавную кривую

    m_scaleAnimation = new QPropertyAnimation(this, "iconScale", this);
    m_scaleAnimation->setDuration(250); // Увеличили с 150 до 250 мс
    m_scaleAnimation->setEasingCurve(QEasingCurve::OutCubic); // Добавили плавную кривую

    setFocusPolicy(Qt::NoFocus);
}

void ControlButton::cleanup()
{
    if (m_colorAnimation) {
        m_colorAnimation->stop();
        m_colorAnimation->deleteLater();
        m_colorAnimation = nullptr;
    }

    if (m_scaleAnimation) {
        m_scaleAnimation->stop();
        m_scaleAnimation->deleteLater();
        m_scaleAnimation = nullptr;
    }

    if (m_svgRenderer) {
        delete m_svgRenderer;
        m_svgRenderer = nullptr;
    }
}

void ControlButton::setIcon(const QIcon &icon)
{
    // Вызываем базовый метод
    QPushButton::setIcon(icon);

    // Пытаемся извлечь SVG путь из иконки
    extractSvgPathFromIcon(icon);
    update();
}

void ControlButton::extractSvgPathFromIcon(const QIcon &icon)
{
    // Эта функция оставлена для будущего расширения
    // В текущей реализации мы работаем напрямую с QIcon в paintEvent
}

void ControlButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    // Получаем иконку, установленную через setIcon
    QIcon icon = this->icon();

    if (!icon.isNull()) {
        // Вычисляем размер иконки с учетом масштаба
        int currentIconSize = qRound(ICON_SIZE * m_iconScale);

        // Точное центрирование - вычисляем координаты как вещественные числа
        qreal x = (width() - currentIconSize) / 2.0;
        qreal y = (height() - currentIconSize) / 2.0;

        QRectF iconRect(x, y, currentIconSize, currentIconSize);

        // Получаем pixmap иконки
        QPixmap pixmap = icon.pixmap(currentIconSize, currentIconSize);

        if (!pixmap.isNull()) {
            // Создаем изображение для применения цвета
            QImage image = pixmap.toImage();
            image = image.convertToFormat(QImage::Format_ARGB32);

            // Применяем наш цвет ко всем непрозрачным пикселям
            for (int imgY = 0; imgY < image.height(); ++imgY) {
                QRgb *scanLine = (QRgb*)image.scanLine(imgY);
                for (int imgX = 0; imgX < image.width(); ++imgX) {
                    QColor pixelColor = QColor::fromRgba(scanLine[imgX]);
                    if (pixelColor.alpha() > 10) {
                        // Сохраняем альфа-канал, но меняем цвет
                        m_iconColor.setAlpha(pixelColor.alpha());
                        scanLine[imgX] = m_iconColor.rgba();
                    }
                }
            }

            // Рисуем обработанное изображение с точным позиционированием
            painter.drawImage(iconRect, image);
        }
    } else {
        // Если иконка не установлена, рисуем отладочную форму
        painter.setPen(QPen(m_iconColor, 2));
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(rect().adjusted(5, 5, -5, -5));

        painter.drawText(rect(), Qt::AlignCenter, "No\nIcon");
    }
}

void ControlButton::enterEvent(QEnterEvent *event)
{
    m_isHovered = true;
    if (!m_isPressed) {
        startHoverAnimation();
    }
    QPushButton::enterEvent(event);
}

void ControlButton::leaveEvent(QEvent *event)
{
    m_isHovered = false;
    if (!m_isPressed) {
        startLeaveAnimation();
    }
    QPushButton::leaveEvent(event);
}

void ControlButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isPressed = true;
        setIconColor(PRESSED_COLOR);
    }
    QPushButton::mousePressEvent(event);
}

void ControlButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isPressed = false;
        if (m_isHovered) {
            startHoverAnimation();
        } else {
            startLeaveAnimation();
        }
    }
    QPushButton::mouseReleaseEvent(event);
}

void ControlButton::setIconColor(const QColor &color)
{
    if (m_iconColor != color) {
        m_iconColor = color;
        update();
        emit iconColorChanged();
    }
}

void ControlButton::setIconScale(qreal scale)
{
    if (m_iconScale != scale) {
        m_iconScale = scale;
        update();
        emit iconScaleChanged();
    }
}

void ControlButton::startHoverAnimation()
{
    if (!m_colorAnimation || !m_scaleAnimation) return;

    m_colorAnimation->stop();
    m_colorAnimation->setStartValue(m_iconColor);
    m_colorAnimation->setEndValue(HOVER_COLOR);
    m_colorAnimation->start();

    m_scaleAnimation->stop();
    m_scaleAnimation->setStartValue(m_iconScale);
    m_scaleAnimation->setEndValue(static_cast<qreal>(HOVER_ICON_SIZE) / ICON_SIZE);
    m_scaleAnimation->start();
}

void ControlButton::startLeaveAnimation()
{
    if (!m_colorAnimation || !m_scaleAnimation) return;

    m_colorAnimation->stop();
    m_colorAnimation->setStartValue(m_iconColor);
    m_colorAnimation->setEndValue(NORMAL_COLOR);
    m_colorAnimation->start();

    m_scaleAnimation->stop();
    m_scaleAnimation->setStartValue(m_iconScale);
    m_scaleAnimation->setEndValue(1.0);
    m_scaleAnimation->start();
}
