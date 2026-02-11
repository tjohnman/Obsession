#ifndef USERICONDELEGATE_H
#define USERICONDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QPixmap>

class UserIconDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    
public:
    explicit UserIconDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}
    
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        // Get the background brush from the item
        QVariant bg = index.data(Qt::BackgroundRole);
        
        if (bg.canConvert<QBrush>()) {
            QBrush brush = qvariant_cast<QBrush>(bg);
            
            // If it's a texture pattern (image), paint it
            if (brush.style() == Qt::TexturePattern) {
                QPixmap pixmap = brush.texture();
                if (!pixmap.isNull()) {
                    painter->drawPixmap(option.rect, pixmap);
                }
            }
        }
        
        // Call base implementation to paint text and other elements
        QStyledItemDelegate::paint(painter, option, index);
    }
};

#endif // USERICONDELEGATE_H
