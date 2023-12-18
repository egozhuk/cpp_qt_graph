#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    m_plot = new Plot();
    isSurface = true;
    createUI();
}

MainWindow::~MainWindow()
{
    delete m_plot;
}

void MainWindow::createUI()
{
    languageMenu = menuBar()->addMenu("Язык");
    russianAction = languageMenu->addAction("Русский");
    englishAction = languageMenu->addAction("English");

    connect(englishAction, &QAction::triggered, this, &MainWindow::switchToEnglish);
    connect(russianAction, &QAction::triggered, this, &MainWindow::switchToRussian);

    saveSettingsAction = new QAction(style()->standardIcon(QStyle::SP_DialogSaveButton), "", this);
    loadSettingsAction = new QAction(style()->standardIcon(QStyle::SP_DialogResetButton), "", this);

    settingsMenu = menuBar()->addMenu("Настройки");
    settingsMenu->addAction(saveSettingsAction);
    settingsMenu->addAction(loadSettingsAction);

    typeMenu = menuBar()->addMenu("");
    surfaceAction = typeMenu->addAction("Surface");
    scatterAction = typeMenu->addAction("Scatter");

    connect(surfaceAction, &QAction::triggered, this, &MainWindow::switchToSurface);
    connect(scatterAction, &QAction::triggered, this, &MainWindow::switchToScatter);

    menuBar()->update();

    statusBar = new QStatusBar(this);
    setStatusBar(statusBar);

    settingsToolbar = addToolBar("");
    settingsToolbar->addAction(saveSettingsAction);
    settingsToolbar->addAction(loadSettingsAction);

    connect(saveSettingsAction, &QAction::triggered, this, &MainWindow::saveSettings);
    connect(loadSettingsAction, &QAction::triggered, this, &MainWindow::loadSettings);

    saveSettingsAction->setShortcut(QKeySequence::Save);
    loadSettingsAction->setShortcut(QKeySequence::Open);

    showGridCheckBox = new QCheckBox("");
    showLabelCheckBox = new QCheckBox("");
    showLabelBorderCheckBox = new QCheckBox("");

    showGridCheckBox->setChecked(true);
    showLabelBorderCheckBox->setEnabled(true);
    showLabelBorderCheckBox->setChecked(true);
    showLabelCheckBox->setChecked(true);

    connect(showGridCheckBox, &QCheckBox::toggled, this, &MainWindow::callToggleGrid);
    connect(showLabelCheckBox, &QCheckBox::toggled, this, &MainWindow::callAxisVisibility);
    connect(showLabelBorderCheckBox, &QCheckBox::toggled, this, &MainWindow::callLabelsVisibility);

    connect(showLabelCheckBox, &QCheckBox::toggled, showLabelBorderCheckBox, &QCheckBox::setEnabled);

    connect(showLabelCheckBox, &QCheckBox::toggled, this,
        [=]()
        {
            if (showLabelCheckBox->isChecked() && wasChecked)
            {
                showLabelBorderCheckBox->setChecked(true);
            }
        });

    tField = new QSpinBox(this);
    tField->setValue(stepsX);
    tField->setRange(0, 200);

    tField1 = new QSpinBox(this);
    tField1->setValue(stepsZ);
    tField1->setRange(0, 200);

    m_sincDistRadio = new QRadioButton("Sinc1");
    m_sincProdRadio = new QRadioButton("Sinc2");
    m_sincDistRadio->setChecked(true);

    connect(m_sincDistRadio, &QRadioButton::clicked, this, &MainWindow::updateGraph);
    connect(m_sincProdRadio, &QRadioButton::clicked, this, &MainWindow::updateGraph);

    m_noSelectionRadio = new QRadioButton("");
    m_selectionRadio = new QRadioButton("");
    m_selectionRadio->setChecked(true);

    connect(m_noSelectionRadio, &QRadioButton::clicked, this, &MainWindow::updateMode);
    connect(m_selectionRadio, &QRadioButton::clicked, this, &MainWindow::updateMode);

    QVBoxLayout *radioLayout = new QVBoxLayout;

    radioLayout->addWidget(m_sincDistRadio);
    radioLayout->addWidget(m_sincProdRadio);

    radioGroupBox = new QGroupBox("");
    radioGroupBox->setLayout(radioLayout);

    radioGroupBox->setFixedSize(300, 150);

    QVBoxLayout *radioSelectionLayout = new QVBoxLayout;
    radioSelectionLayout->addWidget(m_noSelectionRadio);
    radioSelectionLayout->addWidget(m_selectionRadio);

    radioSelectionGroupBox = new QGroupBox("");
    radioSelectionGroupBox->setLayout(radioSelectionLayout);

    radioSelectionGroupBox->setFixedSize(300, 150);

    colRange = new QLabel();
    colRange->setText("");

    rowRange = new QLabel();
    rowRange->setText("");

    xStepsLabel = new QLabel();
    xStepsLabel->setText("");

    zStepsLabel = new QLabel();
    zStepsLabel->setText("");

    xMinSlider = new QSlider;
    xMinSlider->setOrientation(Qt::Horizontal);
    xMinSlider->setMinimum(-10);
    xMinSlider->setMaximum(10);
    xMinSlider->setValue(-10);
    xMinSlider->setTickInterval(1);

    xMaxSlider = new QSlider;
    xMaxSlider->setOrientation(Qt::Horizontal);
    xMaxSlider->setMinimum(-10);
    xMaxSlider->setMaximum(10);
    xMaxSlider->setValue(10);
    xMaxSlider->setTickInterval(1);

    zMinSlider = new QSlider;
    zMinSlider->setOrientation(Qt::Horizontal);
    zMinSlider->setMinimum(-10);
    zMinSlider->setMaximum(10);
    zMinSlider->setValue(-10);
    zMinSlider->setTickInterval(1);

    zMaxSlider = new QSlider;
    zMaxSlider->setOrientation(Qt::Horizontal);
    zMaxSlider->setMinimum(-10);
    zMaxSlider->setMaximum(10);
    zMaxSlider->setValue(10);
    zMaxSlider->setTickInterval(1);

    connect(xMinSlider, &QSlider::valueChanged, this, [this](int value) { updateXMin(static_cast< float >(value)); });
    connect(xMaxSlider, &QSlider::valueChanged, this, [this](int value) { updateXMax(static_cast< float >(value)); });
    connect(zMinSlider, &QSlider::valueChanged, this, [this](int value) { updateZMin(static_cast< float >(value)); });
    connect(zMaxSlider, &QSlider::valueChanged, this, [this](int value) { updateZMax(static_cast< float >(value)); });
    connect(tField, &QSpinBox::textChanged, this, &MainWindow::onTextFieldTextChangedX);
    connect(tField1, &QSpinBox::textChanged, this, &MainWindow::onTextFieldTextChangedZ);
    connect(m_plot->getSeriesSurface(), &QSurface3DSeries::selectedPointChanged, this, &MainWindow::onPointSelected);

    colorGroupBox = new QGroupBox(QStringLiteral(""));

    QLinearGradient gradient1(0, 0, 1, 100);
    gradient1.setColorAt(0.0, QColor(253, 231, 37));
    gradient1.setColorAt(0.25, QColor(94, 201, 98));
    gradient1.setColorAt(0.5, QColor(33, 145, 140));
    gradient1.setColorAt(0.75, QColor(59, 82, 139));
    gradient1.setColorAt(1.0, QColor(68, 1, 84));
    QPixmap pm(24, 100);
    QPainter pmp(&pm);
    pmp.setBrush(QBrush(gradient1));
    pmp.setPen(Qt::NoPen);
    pmp.drawRect(0, 0, 24, 100);
    gradientBut = new QPushButton();
    gradientBut->setIcon(QIcon(pm));
    gradientBut->setIconSize(QSize(24, 100));

    QLinearGradient gradient2(0, 0, 1, 100);
    gradient2.setColorAt(0.0, QColor(240, 249, 33));
    gradient2.setColorAt(0.25, QColor(248, 149, 64));
    gradient2.setColorAt(0.5, QColor(204, 71, 120));
    gradient2.setColorAt(0.75, QColor(126, 3, 168));
    gradient2.setColorAt(1.0, QColor(13, 8, 135));
    pmp.setBrush(QBrush(gradient2));
    pmp.drawRect(0, 0, 24, 100);
    gradient2But = new QPushButton();
    gradient2But->setIcon(QIcon(pm));
    gradient2But->setIconSize(QSize(24, 100));

    connect(gradientBut, &QPushButton::clicked, this, &MainWindow::onGradientFirst);
    connect(gradient2But, &QPushButton::clicked, this, &MainWindow::onGradientSecond);

    QHBoxLayout *colorGroupBoxLayout = new QHBoxLayout;
    colorGroupBoxLayout->addWidget(gradientBut);
    colorGroupBoxLayout->addWidget(gradient2But);
    colorGroupBox->setLayout(colorGroupBoxLayout);

    QVBoxLayout *controlsLayout = new QVBoxLayout;
    controlsLayout->addWidget(showGridCheckBox);
    controlsLayout->addWidget(showLabelCheckBox);
    controlsLayout->addWidget(showLabelBorderCheckBox);

    controlsLayout->addWidget(radioGroupBox);
    controlsLayout->addWidget(radioSelectionGroupBox);

    controlsLayout->addWidget(colRange);
    controlsLayout->addWidget(xMinSlider);
    controlsLayout->addWidget(xMaxSlider);
    controlsLayout->addWidget(rowRange);
    controlsLayout->addWidget(zMinSlider);
    controlsLayout->addWidget(zMaxSlider);
    controlsLayout->addWidget(xStepsLabel);
    controlsLayout->addWidget(tField);
    controlsLayout->addWidget(zStepsLabel);
    controlsLayout->addWidget(tField1);
    controlsLayout->addWidget(colorGroupBox);
    controlsLayout->addStretch(1);

    mainLayout = new QHBoxLayout;

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    stackedWidget = new QStackedWidget;
    surfaceWidget = QWidget::createWindowContainer(m_plot->getGraphSurface(), this);
    scatterWidget = QWidget::createWindowContainer(m_plot->getGraphScatter(), this);
    stackedWidget->addWidget(surfaceWidget);
    stackedWidget->addWidget(scatterWidget);
    mainLayout->addWidget(stackedWidget, 1);

    mainLayout->addLayout(controlsLayout);

    loadSettings();
}

void MainWindow::switchToSurface()
{
    isSurface = true;
    m_plot->switchToSurface();
    stackedWidget->setCurrentWidget(surfaceWidget);

}

void MainWindow::switchToScatter()
{
    isSurface = false;
    m_plot->switchToScatter();
    stackedWidget->setCurrentWidget(scatterWidget);
}

void MainWindow::onPointSelected(const QPoint &position)
{
    statusBar->showMessage(m_plot->getCoord(position));
}

void MainWindow::onTextFieldTextChangedX(const QString &text)
{
    bool ok;
    stepsX = text.toInt(&ok);
    if (!ok)
    {
        stepsX = 50;
    }
    if (stepsX > 200)
    {
        stepsX = 200;
    }
    if (stepsX < 0)
    {
        stepsX = 0;
    }
    tField->setValue(stepsX);
    m_plot->setStepsX(stepsX);
}

void MainWindow::onTextFieldTextChangedZ(const QString &text)
{
    bool ok;
    stepsZ = text.toInt(&ok);
    if (!ok)
    {
        stepsZ = 50;
    }
    if (stepsZ > 200)
    {
        stepsZ = 200;
    }
    if (stepsZ < 0)
    {
        stepsZ = 0;
    }
    tField1->setValue(stepsZ);
    m_plot->setStepsZ(stepsZ);
}

void MainWindow::onGradientFirst()
{
    if (m_sincDistRadio->isChecked())
    {
        firstGraphGradient = 0;
    }
    else
    {
        secondGraphGradient = 0;
    }
    m_plot->createColorGradient();
}

void MainWindow::onGradientSecond()
{
    if (m_sincDistRadio->isChecked())
    {
        firstGraphGradient = 1;
    }
    else
    {
        secondGraphGradient = 1;
    }
    m_plot->createColorGradient2();
}

void MainWindow::callToggleGrid(bool showGrid)
{
    m_plot->toggleGrid(showGrid);
}

void MainWindow::callAxisVisibility(bool showAxis)
{
    m_plot->setAxisVisibility(showAxis);
}

void MainWindow::callLabelsVisibility(bool showBorder)
{
    if (showLabelBorderCheckBox->isChecked() == false)
    {
        wasChecked = false;
    }
    else
    {
        wasChecked = true;
    }
    m_plot->setAxisLabelsBorderVisibility(showBorder);
}

void MainWindow::updateXMin(float value)
{
    updateRange();
    if (value >= xMaxSlider->value())
    {
        xMaxSlider->setValue(value);
    }
    m_plot->updateXMin(value);
}

void MainWindow::updateXMax(float value)
{
    updateRange();
    if (value <= xMinSlider->value())
    {
        xMinSlider->setValue(value);
    }
    m_plot->updateXMax(value);
}

void MainWindow::updateZMin(float value)
{
    updateRange();
    if (value >= zMaxSlider->value())
    {
        zMaxSlider->setValue(value);
    }
    m_plot->updateZMin(value);
}

void MainWindow::updateZMax(float value)
{
    updateRange();
    if (value <= zMinSlider->value())
    {
        zMinSlider->setValue(value);
    }
    m_plot->updateZMax(value);
}

void MainWindow::updateGraph()
{
    if (m_sincDistRadio->isChecked())
    {
        if (firstGraphGradient == 0)
        {
            onGradientFirst();
        }
        else
        {
            onGradientSecond();
        }
        m_plot->updateData(0);
    }
    else
    {
        if (secondGraphGradient == 0)
        {
            onGradientFirst();
        }
        else
        {
            onGradientSecond();
        }
        m_plot->updateData(1);
    }
}

void MainWindow::updateMode()
{
    if (m_selectionRadio->isChecked())
    {
        m_plot->updateSelection(0);
    }
    else
    {
        m_plot->updateSelection(1);
    }
}

void MainWindow::saveSettings()
{
    QSettings settings("settings.ini", QSettings::IniFormat);

    settings.beginGroup("Graph");
    settings.setValue("sinc1", m_sincDistRadio->isChecked() ? 1 : 0);
    settings.setValue("sinc2", m_sincProdRadio->isChecked() ? 1 : 0);
    settings.endGroup();

    settings.beginGroup("Selection");
    settings.setValue("NoSelection", m_noSelectionRadio->isChecked() ? 1 : 0);
    settings.setValue("ItemSelection", m_selectionRadio->isChecked() ? 1 : 0);
    settings.endGroup();

    settings.beginGroup("Checkboxes");
    settings.setValue("showGrid", showGridCheckBox->isChecked());
    settings.setValue("showLabel", showLabelCheckBox->isChecked());
    settings.setValue("showLabelBorder", showLabelBorderCheckBox->isChecked());
    settings.endGroup();

    settings.beginGroup("Sliders");
    settings.setValue("xMin", xMinSlider->value());
    settings.setValue("xMax", xMaxSlider->value());
    settings.setValue("zMin", zMinSlider->value());
    settings.setValue("zMax", zMaxSlider->value());
    settings.endGroup();

    settings.beginGroup("Steps");
    settings.setValue("stepsX", stepsX);
    settings.setValue("stepsZ", stepsZ);
    settings.endGroup();

    settings.beginGroup("Gradient");
    settings.setValue("firstGraphGradient", firstGraphGradient);
    settings.setValue("secondGraphGradient", secondGraphGradient);
    settings.endGroup();
}

bool MainWindow::isNumeric(QString const str)
{
    bool isNumber = false;
    str.toInt(&isNumber);
    return isNumber;
}

void MainWindow::loadSettings()
{
    QSettings settings("settings.ini", QSettings::IniFormat);

    loadLanguage();

    settings.beginGroup("Graph");
    bool sinc1Checked = true;
    if (settings.contains("sinc1") && (settings.value("sinc1") == "1" || settings.value("sinc1") == "0"))
    {
        sinc1Checked = settings.value("sinc1").toBool();
    }
    else
    {
        sinc1Checked = true;
    }

    settings.endGroup();

    if (sinc1Checked)
    {
        m_sincDistRadio->setChecked(true);
    }
    else
    {
        m_sincProdRadio->setChecked(true);
    }

    settings.beginGroup("Selection");
    bool noSelectionChecked = true;
    if (settings.contains("NoSelection") && (settings.value("NoSelection") == "1" || settings.value("NoSelection") == "0"))
    {
        noSelectionChecked = settings.value("NoSelection").toBool();
    }
    else
    {
        noSelectionChecked = true;
    }
    settings.endGroup();

    if (noSelectionChecked)
    {
        m_noSelectionRadio->setChecked(true);
    }
    else
    {
        m_selectionRadio->setChecked(true);
    }

    settings.beginGroup("Gradient");
    if (settings.contains("firstGraphGradient") && (settings.value("firstGraphGradient") == "false" || settings.value("firstGraphGradient") == "true"))
    {
        firstGraphGradient = settings.value("firstGraphGradient").toBool();
    }
    else
    {
        firstGraphGradient = true;
    }

    if (settings.contains("secondGraphGradient") && (settings.value("secondGraphGradient") == "false" || settings.value("secondGraphGradient") == "true"))
    {
        secondGraphGradient = settings.value("secondGraphGradient").toBool();
    }
    else
    {
        secondGraphGradient = true;
    }
    settings.endGroup();

    updateMode();
    updateGraph();

    settings.beginGroup("Steps");
    if (settings.contains("stepsX") && isNumeric(settings.value("stepsX").toString()))
    {
        if (settings.value("stepsX").toInt() > 0 && settings.value("stepsX").toInt() < 201)
        {
            stepsX = settings.value("stepsX").toInt();
        }
        else
        {
            stepsX = 50;
        }
    }
    else
    {
        stepsX = 50;
    }

    if (settings.contains("stepsZ") && isNumeric(settings.value("stepsZ").toString()))
    {
        if (settings.value("stepsZ").toInt() > 0 && settings.value("stepsZ").toInt() < 201)
        {
            stepsZ = settings.value("stepsZ").toInt();
        }
        else
        {
            stepsZ = 50;
        }
    }
    else
    {
        stepsZ = 50;
    }
    settings.endGroup();

    tField->setValue(stepsX);
    tField1->setValue(stepsZ);

    settings.beginGroup("Sliders");
    int xMin;
    int xMax;
    int zMin;
    int zMax;
    if (settings.contains("xMin") && isNumeric(settings.value("xMin").toString()))
    {
        if (settings.value("xMin").toInt() >= -10)
        {
            xMin = settings.value("xMin").toInt();
        }
        else
        {
            xMin = -10;
        }
    }
    else
    {
        xMin = -10;
    }

    if (settings.contains("xMax") && isNumeric(settings.value("xMax").toString()))
    {
        if (settings.value("xMax").toInt() <= 10)
        {
            xMax = settings.value("xMax").toInt();
        }
        else
        {
            xMax = 10;
        }
    }
    else
    {
        xMax = 10;
    }

    if (settings.contains("zMin") && isNumeric(settings.value("zMin").toString()))
    {
        if (settings.value("zMin").toInt() >= -10)
        {
            zMin = settings.value("zMin").toInt();
        }
        else
        {
            zMin = -10;
        }
    }
    else
    {
        zMin = -10;
    }

    if (settings.contains("zMax") && isNumeric(settings.value("zMax").toString()))
    {
        if (settings.value("zMax").toInt() <= 10)
        {
            zMax = settings.value("zMax").toInt();
        }
        else
        {
            zMax = 10;
        }
    }
    else
    {
        zMax = 10;
    }
    settings.endGroup();

    xMinSlider->setValue(xMin);
    xMaxSlider->setValue(xMax);
    zMinSlider->setValue(zMin);
    zMaxSlider->setValue(zMax);

    updateXMin(static_cast< float >(xMin));
    updateXMax(static_cast< float >(xMax));
    updateZMin(static_cast< float >(zMin));
    updateZMax(static_cast< float >(zMax));

    settings.beginGroup("Checkboxes");
    bool showGridChecked;
    bool showLabelChecked;
    bool showLabelBorderChecked;
    if (settings.contains("showGrid") && (settings.value("showGrid") == "true" || settings.value("showGrid") == "false"))
    {
        showGridChecked = settings.value("showGrid").toBool();
    }
    else
    {
        showGridChecked = true;
    }

    if (settings.contains("showLabel") && (settings.value("showLabel") == "true" || settings.value("showLabel") == "false"))
    {
        showLabelChecked = settings.value("showLabel").toBool();
    }
    else
    {
        showLabelChecked = true;
    }

    if (settings.contains("showLabelBorder") && (settings.value("showLabelBorder") == "true" || settings.value("showLabelBorder") == "false"))
    {
        showLabelBorderChecked = settings.value("showLabelBorder").toBool();
    }
    else
    {
        showLabelBorderChecked = true;
    }
    settings.endGroup();

    showGridCheckBox->setChecked(showGridChecked);
    showLabelCheckBox->setChecked(showLabelChecked);
    showLabelBorderCheckBox->setChecked(showLabelBorderChecked);

    callToggleGrid(showGridChecked);
    callAxisVisibility(showLabelChecked);
    callLabelsVisibility(showLabelBorderChecked);

    settings.beginGroup("Language");
    if (settings.contains("language"))
    {
        if (settings.value("language") == "en_GB")
        {
            switchToEnglish();
        }
        else
        {
            switchToRussian();
        }
    }
    else
    {
        QLocale systemLocale = QLocale::system();
        QString languageCode = systemLocale.name();
        if (languageCode == "en_GB")
        {
            switchToEnglish();
        }
        else
        {
            switchToRussian();
        }
    }
    settings.endGroup();
}

QString MainWindow::loadLanguage()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.beginGroup("Language");
    QString language = settings.value("language", "en_GB").toString();
    settings.endGroup();
    return language;
}

void MainWindow::switchToEnglish()
{
    globalLanguage = "en_GB";

    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.beginGroup("Language");
    settings.setValue("language", globalLanguage);
    settings.endGroup();

    changeLanguage(globalLanguage);
}

void MainWindow::switchToRussian()
{
    globalLanguage = "ru_RU";

    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.beginGroup("Language");
    settings.setValue("language", globalLanguage);
    settings.endGroup();

    changeLanguage(globalLanguage);
}

void MainWindow::changeLanguage(const QString &language)
{
    translator.load("../lab4ppa2en/lab4ppa2en_" + language);
    qApp->installTranslator(&translator);
    translate();
}

void MainWindow::updateRange()
{
    colRange->setText(
        tr("Диапазон колонн") + ": [" + QString::number(xMinSlider->value()) + "; " + QString::number(xMaxSlider->value()) + "]");
    rowRange->setText(
        tr("Диапазон рядов") + ": [" + QString::number(zMinSlider->value()) + "; " + QString::number(zMaxSlider->value()) + "]");
}

void MainWindow::translate()
{
    languageMenu->setTitle(tr("Язык"));
    saveSettingsAction->setText(tr("Сохранить настройки"));
    loadSettingsAction->setText(tr("Загрузить настройки"));
    settingsMenu->setTitle(tr("Настройки"));
    typeMenu->setTitle(tr("Тип графика"));
    showGridCheckBox->setText(tr("Показать сетку"));
    showLabelCheckBox->setText(tr("Подписать ось"));
    showLabelBorderCheckBox->setText(tr("Показать границу вокруг надписей"));
    m_noSelectionRadio->setText(tr("Выбор запрещен"));
    m_selectionRadio->setText(tr("Выбор разрешен"));
    radioSelectionGroupBox->setTitle(tr("Выбор точек"));
    radioGroupBox->setTitle(tr("График"));
    colorGroupBox->setTitle(tr("Градиент"));
    colRange->setText(tr("Диапазон колонн") + ": [" + QString::number(xMinSlider->value()) + "; " + QString::number(xMaxSlider->value()) + "]");
    rowRange->setText(tr("Диапазон рядов") + ": [" + QString::number(zMinSlider->value()) + "; " + QString::number(zMaxSlider->value()) + "]");
    xStepsLabel->setText(tr("Шаги по Х: "));
    zStepsLabel->setText(tr("Шаги по Z: "));
    russianAction->setStatusTip(tr("Русский язык"));
    englishAction->setStatusTip(tr("Английский язык"));
    surfaceAction->setStatusTip(tr("Сделать график Surface"));
    scatterAction->setStatusTip(tr("Сделать график Scatter"));
    saveSettingsAction->setStatusTip(tr("Сохранить настройки"));
    loadSettingsAction->setStatusTip(tr("Загрузить настройки"));
}
