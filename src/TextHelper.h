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
        QString encodingName = settings.value(QString::fromUtf8("Encoding"), QString::fromUtf8("Apple Roman")).toString();
        
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
        QString encodingName = settings.value(QString::fromUtf8("Encoding"), QString::fromUtf8("Apple Roman")).toString();
        
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
        QString encodingName = settings.value(QString::fromUtf8("Encoding"), QString::fromUtf8("Apple Roman")).toString();
        
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
        while(str.at(0) == QLatin1Char('\n') || str.at(0) == QLatin1Char('\r')) {
            str = str.right(str.length()-1);
        }
        while(str.at(str.length()-1) == QLatin1Char('\n') || str.at(str.length()-1) == QLatin1Char('\r')) {
            str = str.left(str.length()-1);
        }
        str.replace(QString::fromUtf8("\r"), QString::fromUtf8("\n"));
        str.replace(QString::fromUtf8("\n\n"), QString::fromUtf8("\n"));
        str.replace(QString::fromUtf8("\n"), QString::fromUtf8("<br>"));
        QStringList words = str.split(QString::fromUtf8(" "));
        QStringList newWords;
        for(qint32 i=0; i<words.size(); i++) {
            if(words.at(i).startsWith(QString::fromUtf8("www."))) {
                QString URL = QString::fromUtf8("<a href=\"http://")+words.at(i)+QString::fromUtf8("\">")+words.at(i)+QString::fromUtf8("</a>");
                newWords.append(URL);
            } else if(words.at(i).startsWith(QString::fromUtf8("http://")) || words.at(i).startsWith(QString::fromUtf8("https://"))) {
                    QString URL = QString::fromUtf8("<a href=\"")+words.at(i)+QString::fromUtf8("\">")+words.at(i)+QString::fromUtf8("</a>");
                    newWords.append(URL);
            } else if(words.at(i) == QString::fromUtf8("<"))
            {
                newWords.append(QString::fromUtf8("&lt;"));
            } else if(words.at(i).endsWith(QString::fromUtf8("<")))
            {
                newWords.append(words.at(i).left(words.at(i).length()-1) + QString::fromUtf8("&lt;"));
            } else if(words.at(i).startsWith(QString::fromUtf8("<-")))
            {
                newWords.append(QString::fromUtf8("&lt;-")+words.at(i).right(words.at(i).length()-2));
            }
            else
            {
                newWords.append(words.at(i));
            }
        }
        return newWords.join(QString::fromUtf8("&nbsp;"));
    }
};

#endif // TEXTHELPER_H
