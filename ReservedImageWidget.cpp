#include "ReservedImageWidget.h"
#include <QFile>
#include <QDebug>

ReservedImageWidget::ReservedImageWidget(QWidget *parent)
    : QWidget(parent)
{
    // Устанавливаем фиксированный размер виджета
    setFixedSize(WIDGET_SIZE, WIDGET_SIZE);

    // Устанавливаем минимальный и максимальный размеры одинаковыми,
    // чтобы layout не мог изменить размер
    setMinimumSize(WIDGET_SIZE, WIDGET_SIZE);
    setMaximumSize(WIDGET_SIZE, WIDGET_SIZE);

    // Настраиваем стиль (опционально)
    setStyleSheet("QWidget { background: transparent; }");
}

void ReservedImageWidget::setImage(const QString &imagePath)
{
    if (imagePath.isEmpty()) {
        m_imageLoaded = false;
        emit imageLoaded(false);
        update();
        return;
    }

    // Проверяем существование файла
    if (!QFile::exists(imagePath)) {
        qWarning() << "Image file does not exist:" << imagePath;
        m_imageLoaded = false;
        emit imageLoaded(false);
        update();
        return;
    }

    loadAndScaleImage(imagePath);
    update(); // Обновляем отображение
}

void ReservedImageWidget::loadAndScaleImage(const QString &path)
{
    // Загружаем изображение
    QPixmap original(path);

    if (original.isNull()) {
        qWarning() << "Failed to load image:" << path;
        m_imageLoaded = false;
        emit imageLoaded(false);
        return;
    }

    // Создаем квадратное изображение с прозрачным фоном
    QPixmap squareImage(IMAGE_SIZE, IMAGE_SIZE);
    squareImage.fill(Qt::transparent);

    QPainter painter(&squareImage);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);

    // Рассчитываем размер и позицию для центрирования с сохранением пропорций
    QSize targetSize = original.size();
    targetSize.scale(IMAGE_SIZE, IMAGE_SIZE, Qt::KeepAspectRatio);

    int x = (IMAGE_SIZE - targetSize.width()) / 2;
    int y = (IMAGE_SIZE - targetSize.height()) / 2;

    // Рисуем изображение
    painter.drawPixmap(QRect(x, y, targetSize.width(), targetSize.height()),
                       original,
                       original.rect());

    m_pixmap = squareImage;
    m_imageLoaded = true;
    emit imageLoaded(true);
}

void ReservedImageWidget::enable()
{
    if (m_enabled) return;

    m_enabled = true;
    update(); // Перерисовываем виджет

    // Сигнал для возможных внешних обработчиков
    emit enabledChanged(true);
}

void ReservedImageWidget::disable()
{
    if (!m_enabled) return;

    m_enabled = false;
    update(); // Перерисовываем виджет

    // Сигнал для возможных внешних обработчиков
    emit enabledChanged(false);
}

void ReservedImageWidget::setBackgroundColor(const QColor &color)
{
    if (m_backgroundColor != color) {
        m_backgroundColor = color;
        update();
    }
}

void ReservedImageWidget::setBorderColor(const QColor &color)
{
    if (m_borderColor != color) {
        m_borderColor = color;
        m_showBorder = true;
        update();
    }
}

QSize ReservedImageWidget::sizeHint() const
{
    return QSize(WIDGET_SIZE, WIDGET_SIZE);
}

QSize ReservedImageWidget::minimumSizeHint() const
{
    return QSize(WIDGET_SIZE, WIDGET_SIZE);
}

void ReservedImageWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Вычисляем область для изображения (центрированную)
    int imageX = (width() - IMAGE_SIZE) / 2;
    int imageY = (height() - IMAGE_SIZE) / 2;
    QRect imageRect(imageX, imageY, IMAGE_SIZE, IMAGE_SIZE);

    // 1. Рисуем фон (если задан)
    if (m_backgroundColor.alpha() > 0) {
        painter.setBrush(m_backgroundColor);
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(imageRect, BORDER_RADIUS, BORDER_RADIUS);
    }

    // 2. Рисуем рамку (если нужно)
    if (m_showBorder) {
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(m_borderColor, 2));
        painter.drawRoundedRect(imageRect, BORDER_RADIUS, BORDER_RADIUS);
    }

    // 3. Рисуем изображение, если оно загружено и включено
    if (m_enabled && m_imageLoaded && !m_pixmap.isNull()) {
        painter.drawPixmap(imageRect, m_pixmap);
    }

    // 4. Если изображение отключено, можно нарисовать какой-то индикатор
    // (например, полупрозрачный слой или иконку "скрыто")
    if (!m_enabled && m_imageLoaded) {
        // Рисуем полупрозрачное изображение (раскомментировать при необходимости)
        // painter.setOpacity(0.3);
        // painter.drawPixmap(imageRect, m_pixmap);
        // painter.setOpacity(1.0);

        // Или рисуем иконку "скрыто"
        // painter.setPen(Qt::gray);
        // painter.drawText(rect(), Qt::AlignCenter, "✖");
    }
}
