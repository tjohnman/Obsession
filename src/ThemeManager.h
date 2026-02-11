#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QString>
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QSettings>
#include <QStyleHints>

enum class AppTheme {
    Light,
    Dark,
    Auto  // Follow system theme
};

class ThemeManager
{
public:
    // Apply the specified theme
    static void applyTheme(AppTheme theme)
    {
        switch (theme) {
            case AppTheme::Light:
                applyLightTheme();
                break;
            case AppTheme::Dark:
                applyDarkTheme();
                break;
            case AppTheme::Auto:
                applySystemTheme();
                break;
        }
    }
    
    // Detect and apply system theme (Qt 6.5+ feature)
    static void applySystemTheme()
    {
        // Qt 6.5+ provides colorScheme() to detect system theme
        auto colorScheme = QGuiApplication::styleHints()->colorScheme();
        
        if (colorScheme == Qt::ColorScheme::Dark) {
            applyDarkTheme();
        } else {
            applyLightTheme();
        }
    }
    
    // Apply dark theme from QSS file
    static void applyDarkTheme()
    {
        QFile styleFile(":/resources/dark-theme.qss");
        if (!styleFile.open(QFile::ReadOnly | QFile::Text)) {
            qWarning("Could not load dark theme stylesheet");
            return;
        }
        
        QTextStream stream(&styleFile);
        QString stylesheet = stream.readAll();
        qApp->setStyleSheet(stylesheet);
        styleFile.close();
    }
    
    // Apply light theme (default Qt style)
    static void applyLightTheme()
    {
        qApp->setStyleSheet("");  // Clear any custom stylesheet
    }
    
    // Save theme preference to settings
    static void saveThemePreference(AppTheme theme)
    {
        QSettings settings("mir", "Contra");
        settings.setValue("appTheme", static_cast<int>(theme));
    }
    
    // Load theme preference from settings
    static AppTheme loadThemePreference()
    {
        QSettings settings("mir", "Contra");
        int themeValue = settings.value("appTheme", static_cast<int>(AppTheme::Auto)).toInt();
        return static_cast<AppTheme>(themeValue);
    }
    
    // Get current theme setting
    static AppTheme currentTheme()
    {
        return loadThemePreference();
    }
    
    // Convert theme enum to string for UI
    static QString themeToString(AppTheme theme)
    {
        switch (theme) {
            case AppTheme::Light: return "Light";
            case AppTheme::Dark: return "Dark";
            case AppTheme::Auto: return "Auto (Follow System)";
            default: return "Auto (Follow System)";
        }
    }
    
    // Convert string to theme enum
    static AppTheme stringToTheme(const QString& str)
    {
        if (str == "Light") return AppTheme::Light;
        if (str == "Dark") return AppTheme::Dark;
        return AppTheme::Auto;
    }
};

#endif // THEMEMANAGER_H
