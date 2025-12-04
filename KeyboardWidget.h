#ifndef KEYBOARDWIDGET_H
#define KEYBOARDWIDGET_H

#include <QWidget>
#include <QMap>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSet>

class KeyboardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit KeyboardWidget(QWidget *parent = nullptr);
    ~KeyboardWidget();

    void highlightKey(const QString &key);
    void resetHighlights();
    void setLayoutType(const QString &layout);

    // Новые методы для включения/выключения отображения
    void enable();
    void disable();
    bool isEnabled() const;

private:
    QVBoxLayout *mainLayout;
    QMap<QString, QPushButton*> buttons;
    QString currentLayout = "english";
    bool shiftPressed = false;
    bool keyboardEnabled = true;

    QSet<QString> leftHandKeys;
    QSet<QString> rightHandKeys;

    void createLayout();
    void updateDisplay();
    void initHandKeys();
    void setButtonColors();
};

#endif // KEYBOARDWIDGET_H
