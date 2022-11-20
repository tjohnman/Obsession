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
    void loadedItem(qint32, QListWidgetItem *);
    void startIconCount(qint32);
    
public slots:
    
};

#endif // THREADICONLOADER_H
