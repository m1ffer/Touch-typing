#ifndef MESSAGEHELPER_H
#define MESSAGEHELPER_H

#include <QWidget>
#include <QString>

class MessageHelper
{
public:
    static void showInfo(QWidget* parent, const QString& title, const QString& text);
    static void showWarning(QWidget* parent, const QString& title, const QString& text);
    static void showError(QWidget* parent, const QString& title, const QString& text);

private:
    static void createAndShowDialog(QWidget* parent, const QString& title,
                                    const QString& text, const QString& color);
    // УДАЛЕН: static QString darkenColor(const QString& color);
};

#endif // MESSAGEHELPER_H
