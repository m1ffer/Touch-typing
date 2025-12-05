#ifndef RESERVEDIMAGEWIDGET_H
#define RESERVEDIMAGEWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>

class ReservedImageWidget : public QWidget
{
    Q_OBJECT

public:
    // Константы размера
    static constexpr int IMAGE_SIZE = 200;          // Размер квадратного изображения
    static constexpr int WIDGET_SIZE = 210;         // Размер виджета (с учетом отступов)
    static constexpr int BORDER_RADIUS = 10;        // Радиус скругления углов

    explicit ReservedImageWidget(QWidget *parent = nullptr);

    // Основные методы
    void setImage(const QString &imagePath);        // Установка изображения из файла
    void enable();                                  // Включить отображение изображения
    void disable();                                 // Отключить отображение изображения

    // Дополнительные методы
    bool isEnabled() const { return m_enabled; }    // Проверка состояния
    void setBackgroundColor(const QColor &color);   // Установка цвета фона
    void setBorderColor(const QColor &color);       // Установка цвета рамки

    // Переопределенные методы для корректной работы в layout
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

signals:
    void enabledChanged(bool enabled);              // Сигнал при изменении состояния
    void imageLoaded(bool success);                 // Сигнал при загрузке изображения

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap m_pixmap;               // Загруженное изображение
    bool m_enabled = true;          // Флаг видимости изображения
    bool m_imageLoaded = false;     // Флаг загрузки изображения
    QColor m_backgroundColor = Qt::transparent;  // Цвет фона
    QColor m_borderColor = Qt::lightGray;        // Цвет рамки
    bool m_showBorder = false;      // Показывать рамку

    void loadAndScaleImage(const QString &path);  // Загрузка и масштабирование
};

#endif // RESERVEDIMAGEWIDGET_H
