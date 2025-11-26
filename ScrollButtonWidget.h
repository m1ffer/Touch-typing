// scrollbuttonwidget.h
#ifndef SCROLLBUTTONWIDGET_H
#define SCROLLBUTTONWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStringList>

class ScrollButtonWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ScrollButtonWidget(QWidget *parent = nullptr);

    void addButton(const QString &text, const QVariant &data = QVariant());
    void addButtons(const QStringList &texts);
    void clearButtons();
    void setButtonStyle(const QString &style);

signals:
    void buttonClicked(const QString &text, const QVariant &data);

private slots:
    void onButtonClicked();

private:
    QScrollArea *m_scrollArea;
    QWidget *m_contentWidget;
    QVBoxLayout *m_contentLayout;
    QList<QPushButton*> m_buttons;
};

#endif // SCROLLBUTTONWIDGET_H
