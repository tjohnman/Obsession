#ifndef FILETYPEMAPPER_H
#define FILETYPEMAPPER_H

#include <QString>
#include <QIcon>
#include <QMap>
#include <QSet>
#include <initializer_list>

/**
 * @brief Maps file extensions and type codes to appropriate icons
 * 
 * Replaces the repetitive if-statement chain in DialogFileBrowser with
 * a clean, maintainable mapping system using modern C++ containers.
 * 
 * Usage:
 *   FileTypeMapper::instance().getIcon("zip"); // Returns archive icon
 *   FileTypeMapper::instance().getIconByTypeCode("ZIP "); // Returns archive icon
 */
class FileTypeMapper
{
public:
    /**
     * @brief File type categories
     */
    enum class Category {
        Unknown,
        Archive,
        Executable,
        Image,
        Text,
        Document,
        Video,
        Audio,
        ISO,
        Partial
    };
    
    /**
     * @brief Get the singleton instance
     */
    static FileTypeMapper& instance()
    {
        static FileTypeMapper instance;
        return instance;
    }
    
    /**
     * @brief Get icon for a file extension
     * @param extension File extension (without dot, e.g., "zip")
     * @return QIcon for the file type
     */
    QIcon getIcon(const QString& extension) const
    {
        Category category = getCategory(extension);
        return getCategoryIcon(category);
    }
    
    /**
     * @brief Get icon by 4-character type code (Hotline protocol)
     * @param typeCode 4-character type code (e.g., "ZIP ")
     * @return QIcon for the file type
     */
    QIcon getIconByTypeCode(const char* typeCode) const
    {
        if (!typeCode) return getCategoryIcon(Category::Unknown);
        
        QString code = QString::fromLatin1(typeCode, 4);
        auto it = m_typeCodeToCategory.find(code);
        if (it != m_typeCodeToCategory.end()) {
            return getCategoryIcon(it.value());
        }
        return getCategoryIcon(Category::Unknown);
    }
    
    /**
     * @brief Get category for a file extension
     * @param extension File extension (without dot)
     * @return Category enum
     */
    Category getCategory(const QString& extension) const
    {
        auto it = m_extensionToCategory.find(extension.toLower());
        if (it != m_extensionToCategory.end()) {
            return it.value();
        }
        return Category::Unknown;
    }
    
    /**
     * @brief Get icon path for a category
     * @param category File category
     * @return Icon path string
     */
    QString getCategoryIconPath(Category category) const
    {
        switch (category) {
            case Category::Archive:
                return QString::fromUtf8(":/files/interfaceIcons/filesArchive.png");
            case Category::Executable:
                return QString::fromUtf8(":/files/interfaceIcons/filesExe.png");
            case Category::Image:
                return QString::fromUtf8(":/files/interfaceIcons/filesImage.png");
            case Category::Text:
                return QString::fromUtf8(":/files/interfaceIcons/filesText.png");
            case Category::Document:
                return QString::fromUtf8(":/files/interfaceIcons/filesDocument.png");
            case Category::Video:
                return QString::fromUtf8(":/files/interfaceIcons/filesVideo.png");
            case Category::Audio:
                return QString::fromUtf8(":/files/interfaceIcons/filesAudio.png");
            case Category::ISO:
                return QString::fromUtf8(":/files/interfaceIcons/filesIso.png");
            case Category::Partial:
                return QString::fromUtf8(":/files/interfaceIcons/filesPartial.png");
            case Category::Unknown:
            default:
                return QString::fromUtf8(":/files/interfaceIcons/filesGeneric.png");
        }
    }
    
    /**
     * @brief Get icon for a category
     * @param category File category
     * @return QIcon
     */
    QIcon getCategoryIcon(Category category) const
    {
        return QIcon(getCategoryIconPath(category));
    }

private:
    FileTypeMapper()
    {
        initializeMappings();
    }
    
    ~FileTypeMapper() = default;
    FileTypeMapper(const FileTypeMapper&) = delete;
    FileTypeMapper& operator=(const FileTypeMapper&) = delete;
    
    /**
     * @brief Initialize all file extension and type code mappings
     */
    void initializeMappings()
    {
        // Archive formats
        addExtensions(Category::Archive, {
            "sit", "sitx", "zip", "dmg", "rar", "7zip", "7z", "z", 
            "tar", "hqx", "gzip", "gz"
        });
        
        // Executable files
        addExtensions(Category::Executable, {"exe"});
        
        // Image formats
        addExtensions(Category::Image, {
            "jpg", "jpeg", "bmp", "png", "gif", "tif", "tiff"
        });
        
        // Text files
        addExtensions(Category::Text, {"txt", "srt", "lit"});
        
        // Document formats
        addExtensions(Category::Document, {"doc", "pdf", "rtf"});
        
        // Video formats
        addExtensions(Category::Video, {
            "mov", "avi", "wmv", "mp4", "mkv", "mpg", "mpeg", "ogm"
        });
        
        // Audio formats
        addExtensions(Category::Audio, {
            "mp3", "wav", "aif", "aiff", "wma", "ogg"
        });
        
        // ISO/Disk image formats
        addExtensions(Category::ISO, {"iso", "nrg"});
        
        // Partial downloads
        addExtensions(Category::Partial, {"hpf"});
        
        // Type codes (4-character Hotline protocol codes)
        m_typeCodeToCategory[QString::fromLatin1("JPEG")] = Category::Image;
        m_typeCodeToCategory[QString::fromLatin1("PNGf")] = Category::Image;
        m_typeCodeToCategory[QString::fromLatin1("BMP ")] = Category::Image;
        m_typeCodeToCategory[QString::fromLatin1("ZIP ")] = Category::Archive;
        m_typeCodeToCategory[QString::fromLatin1("SITD")] = Category::Archive;
        m_typeCodeToCategory[QString::fromLatin1("DEXE")] = Category::Executable;
    }
    
    /**
     * @brief Helper to add multiple extensions to a category
     */
    void addExtensions(Category category, std::initializer_list<const char*> extensions)
    {
        for (const char* ext : extensions) {
            m_extensionToCategory[QString::fromUtf8(ext)] = category;
        }
    }
    
    QMap<QString, Category> m_extensionToCategory;
    QMap<QString, Category> m_typeCodeToCategory;
};

#endif // FILETYPEMAPPER_H
