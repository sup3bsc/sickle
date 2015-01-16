#ifndef TESTMAIN_H
#define TESTMAIN_H

#include <QMainWindow>
#include <QListWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_centralwidget_childAdded(QObject *obj);

    void on_listWidget_currentItemChanged(QListWidgetItem *current);

    void on_actionOpen_triggered();

    void on_actionSave_as_triggered();

    void on_action_Save_triggered();

private:
    QWidget* widgetForVariant(QObject* obj, const char* name);
    Ui::MainWindow *ui;
};

#endif // TESTMAIN_H