#ifndef WIDGETCONSOLE_H
#define WIDGETCONSOLE_H

#include <QWidget>

namespace Ui {
    class WidgetConsole;
}

class WidgetConsole : public QWidget {
    Q_OBJECT
public:
    WidgetConsole(QWidget *parent = 0);
    ~WidgetConsole();
    void addText(QString);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::WidgetConsole *ui;
};

#endif // WIDGETCONSOLE_H
