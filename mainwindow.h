#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "plot.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QStyle>
#include <QtWidgets/QToolBar>

#include <QAction>
#include <QPainter>
#include <QSettings>
#include <QTranslator>
#include <QLocale>

class MainWindow : public QMainWindow
{
    Q_OBJECT;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static void setupLanguage(MainWindow &window);
    bool isSurface;
    int surface = 0;
    int scatter = 0;

private:
    void createUI();
    bool loadTranslator(const QString &locale);
    QString globalLanguage;
    bool firstGraphGradient;
    bool secondGraphGradient;
    int stepsX;
    int stepsZ;
    void updateRange();
    bool wasChecked;
    bool isNumeric(QString const str);
    void translate();
    void onTextFieldTextChangedX(const QString &text);
    void onTextFieldTextChangedZ(const QString &text);
    void onPointSelected(const QPoint &position);
    void changeLanguage(const QString &language);

    QString loadLanguage();
    Plot *m_plot;
    QRadioButton *m_sincDistRadio;
    QRadioButton *m_sincProdRadio;
    QRadioButton *m_noSelectionRadio;
    QRadioButton *m_selectionRadio;
    QPushButton *m_saveButton;
    QPushButton *m_loadButton;
    QCheckBox *showGridCheckBox;
    QCheckBox *showLabelCheckBox;
    QCheckBox *showLabelBorderCheckBox;
    QPushButton *gradientBut;
    QPushButton *gradient2But;
    QSlider *xMinSlider;
    QSlider *xMaxSlider;
    QSlider *zMinSlider;
    QSlider *zMaxSlider;
    QMenu *languageMenu;
    QMenu *typeMenu;
    QAction *loadSettingsAction;
    QAction *saveSettingsAction;
    QAction *russianAction;
    QAction *englishAction;
    QAction *surfaceAction;
    QAction *scatterAction;
    QMenu *settingsMenu;
    QToolBar *settingsToolbar;
    QGroupBox *radioSelectionGroupBox;
    QGroupBox *radioGroupBox;
    QGroupBox *colorGroupBox;
    QLabel *colRange;
    QLabel *rowRange;
    QLabel *xStepsLabel;
    QLabel *zStepsLabel;
    QSpinBox *tField;
    QSpinBox *tField1;
    QStatusBar *statusBar;
    QTranslator translator;
    QHBoxLayout *mainLayout;
    QStackedWidget *stackedWidget;
    QWidget *surfaceWidget;
    QWidget *scatterWidget;

private slots:
    void updateGraph();
    void updateMode();
    void saveSettings();
    void loadSettings();
    void onGradientFirst();
    void onGradientSecond();
    void switchToEnglish();
    void switchToRussian();
    void switchToSurface();
    void switchToScatter();
    void callToggleGrid(bool showGrid);
    void callAxisVisibility(bool showAxis);
    void callLabelsVisibility(bool showLabel);
    void updateXMin(float value);
    void updateXMax(float value);
    void updateZMin(float value);
    void updateZMax(float value);

signals:
    void gridToggled(bool showGrid);
    void languageChanged(const QString &language);
};

#endif	  // MAINWINDOW_H
