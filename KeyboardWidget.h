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

    void enable();
    void disable();
    bool isEnabled() const;

private:
    void setLayoutType(const QString &layout);
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
    QString detectLayoutFromSymbol(const QString &symbol); // Новый метод
};

#endif // KEYBOARDWIDGET_H
