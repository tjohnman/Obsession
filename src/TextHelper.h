#ifndef TEXTHELPER_H
#define TEXTHELPER_H

#include <QSettings>
#include <QStringConverter>
#include <optional>

class TextHelper
{
public:
    static QString DecodeText(const char * str, int length)
    {
        QSettings settings(QString::fromUtf8("mir"), QString::fromUtf8("Contra"));
        QString encodingName = settings.value(QString::fromUtf8("Encoding"), "Apple Roman").toString();
        
        // Try to create decoder with ICU support (provides Apple Roman, Shift-JIS, etc.)
        auto decoder = QStringDecoder(encodingName.toUtf8().constData());
        if (!decoder.isValid())
        {
            // Fallback to Apple Roman if encoding not available
            decoder = QStringDecoder("Apple Roman");
        }
        
        return decoder.decode(QByteArrayView(str, length));
    }

    static QString DecodeTextAutoUTF8(const char * str, int length)
    {
        QSettings settings(QString::fromUtf8("mir"), QString::fromUtf8("Contra"));
        QString encodingName = settings.value(QString::fromUtf8("Encoding"), "Apple Roman").toString();
        
        auto decoder = QStringDecoder(encodingName.toUtf8().constData());
        if (!decoder.isValid())
        {
            decoder = QStringDecoder("Apple Roman");
        }
        
        auto utf8Decoder = QStringDecoder(QStringConverter::Utf8);
        QString utf8str = utf8Decoder.decode(QByteArrayView(str, length));
        QString regularStr = decoder.decode(QByteArrayView(str, length));
        return utf8str.length() < regularStr.length() ? utf8str : regularStr;
    }

    static QString DecodeText(char * str, int length, QString encoding)
    {
        auto decoder = QStringDecoder(encoding.toUtf8().constData());
        if (!decoder.isValid())
        {
            // Fallback to Apple Roman if encoding not available
            decoder = QStringDecoder("Apple Roman");
        }
        return decoder.decode(QByteArrayView(str, length));
    }

    static QByteArray EncodeText(QString str)
    {
        QSettings settings(QString::fromUtf8("mir"), QString::fromUtf8("Contra"));
        QString encodingName = settings.value(QString::fromUtf8("Encoding"), "Apple Roman").toString();
        
        auto encoder = QStringEncoder(encodingName.toUtf8().constData());
        if (!encoder.isValid())
        {
            // Fallback to Apple Roman if encoding not available
            encoder = QStringEncoder("Apple Roman");
        }
        
        return encoder.encode(str);
    }

    static QByteArray EncodeText(QString str, QString encoding)
    {
        auto encoder = QStringEncoder(encoding.toUtf8().constData());
        if (!encoder.isValid())
        {
            // Fallback to Apple Roman if encoding not available
            encoder = QStringEncoder("Apple Roman");
        }
        return encoder.encode(str);
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
