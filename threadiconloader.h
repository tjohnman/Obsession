#ifndef THREADICONLOADER_H
#define THREADICONLOADER_H

#include <QListWidgetItem>
#include <QThread>

class ThreadIconLoader : public QThread
{
    Q_OBJECT

public:
    void run();

signals:
    void loadedItem(int, QListWidgetItem *);
    void startIconCount(int);
    
public slots:
    
};

#endif // THREADICONLOADER_H
