#ifndef TEXTHELPER_H
#define TEXTHELPER_H

#include <QSettings>
#include <QTextCodec>

class TextHelper
{
public:
    static QString DecodeText(char * str, int length)
    {
        QSettings settings(QString::fromUtf8("mir"), QString::fromUtf8("Contra"));
        QTextCodec * codec = QTextCodec::codecForName(settings.value(QString::fromUtf8("Encoding"), "macintosh").toString().toUtf8());
        if(!codec)
        {
            codec = QTextCodec::codecForName(QString::fromUtf8("macintosh").toUtf8());
        }
        return codec->toUnicode(str, length);
    }

    static QString DecodeTextAutoUTF8(char * str, int length)
    {
        QSettings settings(QString::fromUtf8("mir"), QString::fromUtf8("Contra"));
        QTextCodec * codec = QTextCodec::codecForName(settings.value(QString::fromUtf8("Encoding"), "macintosh").toString().toUtf8());
        if(!codec)
        {
            codec = QTextCodec::codecForName("macintosh");
        }
        QString utf8str = QTextCodec::codecForName("UTF-8")->toUnicode(str, length);
        QString regularStr = codec->toUnicode(str, length);
        return utf8str.length() < regularStr.length() ? utf8str : regularStr;
    }

    static QString DecodeText(char * str, int length, QString encoding)
    {
        QTextCodec * codec = QTextCodec::codecForName(encoding.toUtf8());
        if(!codec)
        {
            codec = QTextCodec::codecForName(QString::fromUtf8("macintosh").toUtf8());
        }
        return codec->toUnicode(str, length);
    }

    static QByteArray EncodeText(QString str)
    {
        QSettings settings(QString::fromUtf8("mir"), QString::fromUtf8("Contra"));
        QTextCodec * codec = QTextCodec::codecForName(settings.value(QString::fromUtf8("Encoding"), "macintosh").toString().toUtf8());
        if(!codec)
        {
            codec = QTextCodec::codecForName(QString::fromUtf8("macintosh").toUtf8());
        }
        return codec->fromUnicode(str);
    }

    static QByteArray EncodeText(QString str, QString encoding)
    {
        QTextCodec * codec = QTextCodec::codecForName(encoding.toUtf8());
        if(!codec)
        {
            codec = QTextCodec::codecForName(QString::fromUtf8("macintosh").toUtf8());
        }
        return codec->fromUnicode(str);
    }

    static QString FormatMessageToHTML(QString str)
    {
        while(str.at(0) == '\n' || str.at(0) == '\r') {
            str = str.right(str.length()-1);
        }
        while(str.at(str.length()-1) == '\n' || str.at(str.length()-1) == '\r') {
            str = str.left(str.length()-1);
        }
        str.replace("\r", "\n");
        str.replace("\n\n", "\n");
        str.replace("\n", "<br>");
        QStringList words = str.split(" ");
        QStringList newWords;
        for(qint32 i=0; i<words.size(); i++) {
            if(words.at(i).startsWith("www.")) {
                QString URL = "<a href=\"http://"+words.at(i)+"\">"+words.at(i)+"</a>";
                newWords.append(URL);
            } else if(words.at(i).startsWith("http://") || words.at(i).startsWith("https://")) {
                    QString URL = "<a href=\""+words.at(i)+"\">"+words.at(i)+"</a>";
                    newWords.append(URL);
            } else if(words.at(i) == "<")
            {
                newWords.append("&lt;");
            } else if(words.at(i).endsWith("<"))
            {
                newWords.append(words.at(i).left(words.at(i).length()-1) + "&lt;");
            } else if(words.at(i).startsWith("<-"))
            {
                newWords.append(QString("&lt;-")+words.at(i).right(words.at(i).length()-2));
            }
            else
            {
                newWords.append(words.at(i));
            }
        }
        return newWords.join("&nbsp;");
    }
};

#endif // TEXTHELPER_H
