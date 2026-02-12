#ifndef NEWSPROTOCOLPARSER_H
#define NEWSPROTOCOLPARSER_H

#include <QString>
#include <QtEndian>
#include <cstring>

/**
 * @brief Parser for Hotline news protocol data
 * 
 * Handles parsing of news articles, categories, and bundles from raw protocol data.
 * Supports parameter IDs 321 (news items) and 323 (categories/bundles).
 */
class NewsProtocolParser
{
public:
    /**
     * @brief Parsed news item data
     */
    struct NewsItem {
        QString title;
        QString poster;
        quint32 articleID;
        quint32 parentArticleID;
    };

    /**
     * @brief Parsed news category or bundle data
     */
    struct NewsCategory {
        unsigned char type;  // 2 = Bundle, 3 = Category
        QString name;
    };

    /**
     * @brief Parse news items from parameter data (ID 321)
     * @param data Raw parameter data
     * @param items Output vector for parsed items
     * @return true if parsing succeeded
     */
    static bool parseNewsItems(const char* data, std::vector<NewsItem>& items) {
        if (!data) return false;

        quint32 count;
        memcpy(&count, data + 4, 4);
        count = qFromBigEndian(count);

        char size;
        memcpy(&size, data + 8, 1);

        char size2;
        memcpy(&size2, data + 9 + size, 1);

        qint32 offset = 10 + size + size2;

        for (quint32 j = 0; j < count; j++) {
            NewsItem item;

            memcpy(&item.articleID, data + offset, 4);
            item.articleID = qFromBigEndian(item.articleID);

            memcpy(&item.parentArticleID, data + offset + 12, 4);
            item.parentArticleID = qFromBigEndian(item.parentArticleID);

            quint16 fcount;
            memcpy(&fcount, data + offset + 20, 2);
            fcount = qFromBigEndian(fcount);

            char tsize;
            memcpy(&tsize, data + offset + 22, 1);
            item.title = QString::fromUtf8(data + offset + 23, tsize);

            char psize;
            memcpy(&psize, data + offset + 23 + tsize, 1);
            item.poster = QString::fromUtf8(data + offset + 24 + tsize, psize);

            qint32 offset2 = offset + 24 + tsize + psize;

            // Skip over file attachments (not currently used)
            for (quint32 f = 0; f < fcount; f++) {
                char fsize;
                memcpy(&fsize, data + offset2, 1);
                offset2 += 1 + fsize;  // Skip mime type

                quint16 asize;
                memcpy(&asize, data + offset2, 2);
                asize = qFromBigEndian(asize);
                offset2 += 2;
            }

            offset = offset2;
            items.push_back(item);
        }

        return true;
    }

    /**
     * @brief Parse news category or bundle from parameter data (ID 323)
     * @param data Raw parameter data
     * @param category Output structure for parsed data
     * @return true if parsing succeeded
     */
    static bool parseNewsCategory(const char* data, NewsCategory& category) {
        if (!data) return false;

        quint16 typeshort;
        memcpy(&typeshort, data, 2);
        typeshort = qFromBigEndian(typeshort);
        category.type = static_cast<unsigned char>(typeshort);

        if (typeshort == 2) {  // Bundle
            unsigned char ns;
            memcpy(&ns, data + 4, 1);
            category.name = QString::fromUtf8(data + 5, ns);
            return true;
        }

        if (typeshort == 3) {  // Category
            unsigned char ns;
            memcpy(&ns, data + 28, 1);
            category.name = QString::fromUtf8(data + 29, ns);
            return true;
        }

        return false;
    }
};

#endif // NEWSPROTOCOLPARSER_H
