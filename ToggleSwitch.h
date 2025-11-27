#ifndef TOGGLESWITCH_H
#define TOGGLESWITCH_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QMouseEvent>

class ToggleSwitch : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int indicatorPosition READ indicatorPosition WRITE setIndicatorPosition NOTIFY indicatorPositionChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)

public:
    explicit ToggleSwitch(QWidget *parent = nullptr);
    ~ToggleSwitch();

    bool isChecked() const;
    void setChecked(bool checked);

    QSize sizeHint() const override;

signals:
    void toggled(bool checked);
    void indicatorPositionChanged(int position);
    void backgroundColorChanged(QColor color);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    void updateAnimation();

    int indicatorPosition() const;
    void setIndicatorPosition(int position);

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &color);

    bool m_checked;
    int m_indicatorPosition;
    QColor m_backgroundColor;
    QPropertyAnimation *m_animation;
    QPropertyAnimation *m_colorAnimation;
};

#endif // TOGGLESWITCH_H
