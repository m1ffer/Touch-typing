#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onModeChanged(int id);  // Слот для обработки переключения режимов

private:
    void applyButtonStyle();  // Добавляем объявление метода

    Ui::MainWindow *ui;
    QButtonGroup *modeButtonGroup;
};

#endif // MAINWINDOW_H
