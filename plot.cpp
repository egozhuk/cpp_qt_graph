#include "plot.h"

int numOfStepsX = 50;
int numOfStepsZ = 50;
int currFunction;
constexpr float minAx = -10.0f;
constexpr float maxAx = 10.0f;
constexpr float axisMinX = -10.0f;
constexpr float axisMaxX = 10.0f;
constexpr float axisMinZ = -10.0f;
constexpr float axisMaxZ = 10.0f;

Plot::Plot()
{
    isSurface = true;

    my_graph_surface = new Q3DSurface();
    proxy = new QSurfaceDataProxy();
    series = new QSurface3DSeries(proxy);

    my_graph_scatter = new Q3DScatter();
    proxy_scatter = new QScatterDataProxy();
    series_scatter = new QScatter3DSeries(proxy_scatter);
    series_scatter->setItemSize(0.2);
    series_scatter->setMesh(QAbstract3DSeries::MeshSphere);

    theme = new Q3DTheme;
    theme = my_graph_surface->activeTheme();

    createColorGradient();

    series->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
    series_scatter->setColorStyle(Q3DTheme::ColorStyleRangeGradient);

    my_graph_surface->addSeries(series);
    my_graph_scatter->addSeries(series_scatter);

    my_graph_surface->axisX()->setTitle(QStringLiteral("x"));
    my_graph_surface->axisY()->setTitle(QStringLiteral("y"));
    my_graph_surface->axisZ()->setTitle(QStringLiteral("z"));
    my_graph_surface->axisX()->setRange(axisMinX, axisMaxX);
    my_graph_surface->axisZ()->setRange(axisMinZ, axisMaxZ);

    my_graph_surface->axisX()->setTitleVisible(true);
    my_graph_surface->axisY()->setTitleVisible(true);
    my_graph_surface->axisZ()->setTitleVisible(true);

    my_graph_scatter->axisX()->setTitle(QStringLiteral("x"));
    my_graph_scatter->axisY()->setTitle(QStringLiteral("y"));
    my_graph_scatter->axisZ()->setTitle(QStringLiteral("z"));
    my_graph_scatter->axisX()->setRange(axisMinX, axisMaxX);
    my_graph_scatter->axisZ()->setRange(axisMinZ, axisMaxZ);

    my_graph_scatter->axisX()->setTitleVisible(true);
    my_graph_scatter->axisY()->setTitleVisible(true);
    my_graph_scatter->axisZ()->setTitleVisible(true);

    isSurface = false;
    updateData(0);
    isSurface = true;
    updateData(0);
}

Plot::~Plot()
{
    delete proxy_scatter;
    delete proxy;
}

Q3DSurface *Plot::getGraphSurface()
{
    return my_graph_surface;
}

Q3DScatter *Plot::getGraphScatter()
{
    return my_graph_scatter;
}

QSurface3DSeries *Plot::getSeriesSurface()
{
    return series;
}

void Plot::switchToSurface()
{
    if (isSurface)
    {
        return;
    }
    isSurface = true;
    bool grid = theme->isGridEnabled();
    bool label = theme->isLabelBorderEnabled();
    theme = my_graph_surface->activeTheme();
    toggleGrid(grid);
    setAxisVisibility(show);
    setAxisLabelsBorderVisibility(label);
    series->setBaseGradient(series_scatter->baseGradient());
    updateXMax(my_graph_scatter->axisX()->max());
    updateXMin(my_graph_scatter->axisX()->min());
    updateZMax(my_graph_scatter->axisZ()->max());
    updateZMin(my_graph_scatter->axisZ()->min());
    updateSelection(selectionMode);
    updateData(currFunction);
}

void Plot::switchToScatter()
{
    if (!isSurface)
    {
        return;
    }
    isSurface = false;
    bool grid = theme->isGridEnabled();
    bool label = theme->isLabelBorderEnabled();
    theme = my_graph_scatter->activeTheme();
    toggleGrid(grid);
    setAxisVisibility(show);
    setAxisLabelsBorderVisibility(label);
    series_scatter->setBaseGradient(series->baseGradient());
    updateXMax(my_graph_surface->axisX()->max());
    updateXMin(my_graph_surface->axisX()->min());
    updateZMax(my_graph_surface->axisZ()->max());
    updateZMin(my_graph_surface->axisZ()->min());
    updateSelection(selectionMode);
    updateData(currFunction);
}

QString Plot::getCoord(const QPoint &pos)
{
    float xValue = series->dataProxy()->itemAt(pos.x(), pos.y())->x();
    float yValue = series->dataProxy()->itemAt(pos.x(), pos.y())->y();
    float zValue = series->dataProxy()->itemAt(pos.x(), pos.y())->z();
    return QString(QString::number(xValue) + "; " + QString::number(yValue) + "; " + QString::number(zValue));
}

void Plot::setStepsX(int value)
{
    numOfStepsX = value;
    updateData(currFunction);
}

void Plot::setStepsZ(int value)
{
    numOfStepsZ = value;
    updateData(currFunction);
}

void Plot::createColorGradient()
{
    QLinearGradient gradient(0, 0, 1, 0);
    gradient.setColorAt(0.0, QColor(253, 231, 37));
    gradient.setColorAt(0.25, QColor(94, 201, 98));
    gradient.setColorAt(0.5, QColor(33, 145, 140));
    gradient.setColorAt(0.75, QColor(59, 82, 139));
    gradient.setColorAt(1.0, QColor(68, 1, 84));
    if (isSurface)
    {
        series->setBaseGradient(gradient);
    }
    else
    {
        series_scatter->setBaseGradient(gradient);
    }
}

void Plot::createColorGradient2()
{
    QLinearGradient gradient(0, 0, 1, 0);
    gradient.setColorAt(0.0, QColor(240, 249, 33));
    gradient.setColorAt(0.25, QColor(248, 149, 64));
    gradient.setColorAt(0.5, QColor(204, 71, 120));
    gradient.setColorAt(0.75, QColor(126, 3, 168));
    gradient.setColorAt(1.0, QColor(13, 8, 135));
    if (isSurface)
    {
        series->setBaseGradient(gradient);
    }
    else
    {
        series_scatter->setBaseGradient(gradient);
    }
}

void Plot::updateXMin(float value)
{
    if (isSurface)
    {
        float minX = value;

        float maxX = my_graph_surface->axisX()->max();
        if (minX >= maxX)
        {
            maxX = minX;
        }

        my_graph_surface->axisX()->setRange(minX, maxX);
    }
    else
    {
        float minX = value;

        float maxX = my_graph_scatter->axisX()->max();
        if (minX >= maxX)
        {
            maxX = minX;
        }

        my_graph_scatter->axisX()->setRange(minX, maxX);
    }
}

void Plot::updateXMax(float value)
{
    if (isSurface)
    {
        float maxX = value;

        float minX = my_graph_surface->axisX()->min();
        if (maxX <= minX)
        {
            minX = maxX;
        }

        my_graph_surface->axisX()->setRange(minX, maxX);
    }
    else
    {
        float maxX = value;

        float minX = my_graph_scatter->axisX()->min();
        if (maxX <= minX)
        {
            minX = maxX;
        }

        my_graph_scatter->axisX()->setRange(minX, maxX);
    }
}

void Plot::updateZMin(float value)
{
    if (isSurface)
    {
        float minZ = value;

        float maxZ = my_graph_surface->axisZ()->max();
        if (minZ >= maxZ)
        {
            maxZ = minZ;
        }

        my_graph_surface->axisZ()->setRange(minZ, maxZ);
    }
    else
    {
        float minZ = value;

        float maxZ = my_graph_scatter->axisZ()->max();
        if (minZ >= maxZ)
        {
            maxZ = minZ;
        }

        my_graph_scatter->axisZ()->setRange(minZ, maxZ);
    }
}

void Plot::updateZMax(float value)
{
    if (isSurface)
    {
        float maxZ = value;

        float minZ = my_graph_surface->axisZ()->min();
        if (maxZ <= minZ)
        {
            minZ = maxZ;
        }

        my_graph_surface->axisZ()->setRange(minZ, maxZ);
    }
    else
    {
        float maxZ = value;

        float minZ = my_graph_scatter->axisZ()->min();
        if (maxZ <= minZ)
        {
            minZ = maxZ;
        }

        my_graph_scatter->axisZ()->setRange(minZ, maxZ);
    }
}

void Plot::toggleGrid(bool showGrid)
{
    if (theme)
    {
        theme->setGridEnabled(showGrid);
    }
}

void Plot::setAxisVisibility(bool showAxis)
{
    show = showAxis;
    if (showAxis)
    {
        theme->setLabelBackgroundColor(Qt::white);
        theme->setLabelBorderEnabled(true);
        theme->setLabelTextColor(Qt::black);
        if (isSurface)
        {
            my_graph_surface->axisX()->setLabelFormat("%.2f");
            my_graph_surface->axisY()->setLabelFormat("%.2f");
            my_graph_surface->axisZ()->setLabelFormat("%.2f");
        }
        else
        {
            my_graph_scatter->axisX()->setLabelFormat("%.2f");
            my_graph_scatter->axisY()->setLabelFormat("%.2f");
            my_graph_scatter->axisZ()->setLabelFormat("%.2f");
        }
    }
    else
    {
        theme->setLabelBackgroundColor(Qt::transparent);
        theme->setLabelBorderEnabled(false);
        theme->setLabelTextColor(Qt::transparent);
        if (isSurface)
        {
            my_graph_surface->axisX()->setLabelFormat("");
            my_graph_surface->axisY()->setLabelFormat("");
            my_graph_surface->axisZ()->setLabelFormat("");
        }
        else
        {
            my_graph_scatter->axisX()->setLabelFormat("");
            my_graph_scatter->axisY()->setLabelFormat("");
            my_graph_scatter->axisZ()->setLabelFormat("");
        }
    }
}

void Plot::setAxisLabelsBorderVisibility(bool showBorder)
{
    if (showBorder)
    {
        theme->setLabelBackgroundEnabled(true);
        theme->setLabelBorderEnabled(true);
    }
    else
    {
        theme->setLabelBackgroundEnabled(false);
        theme->setLabelBorderEnabled(false);
    }
}

void Plot::updateSelection(int selectionEnabled)
{
    selectionMode = selectionEnabled;
    if (isSurface)
    {
        if (selectionEnabled == 0)
        {
            my_graph_surface->setSelectionMode(QAbstract3DGraph::SelectionItem);
        }
        else
        {
            my_graph_surface->setSelectionMode(QAbstract3DGraph::SelectionNone);
        }
    }
    else
    {
        if (selectionEnabled == 0)
        {
            my_graph_scatter->setSelectionMode(QAbstract3DGraph::SelectionItem);
        }
        else
        {
            my_graph_scatter->setSelectionMode(QAbstract3DGraph::SelectionNone);
        }
    }
}

void Plot::updateData(int functionType)
{
    currFunction = functionType;

    if (isSurface)
    {
        if (functionType == 0)
        {
            generateSincData();
        }
        else
        {
            generateSincProductData();
        }
    }
    else
    {
        if (functionType == 0)
        {
            generateSincDataScatter();
        }
        else
        {
            generateSincProductDataScatter();
        }
    }
}

float sinc(float x)
{
    if (x == 0.0f)
    {
        return 1.0f;
    }
    return qSin(x) / x;
}

void Plot::generateSincData()
{
    float stepX = (maxAx - minAx) / float(numOfStepsX - 1);
    float stepZ = (maxAx - minAx) / float(numOfStepsZ - 1);

    QSurfaceDataArray *dataArray = new QSurfaceDataArray;

    for (int i = 0; i < numOfStepsZ; i++)
    {
        QSurfaceDataRow *newRow = new QSurfaceDataRow(numOfStepsX);
        float z = qMin(maxAx, (i * stepZ + minAx));
        int index = 0;
        for (int j = 0; j < numOfStepsX; j++)
        {
            float x = qMin(maxAx, (j * stepX + minAx));
            float R = qSqrt(z * z + x * x);
            float y = sinc(R);
            (*newRow)[index++].setPosition(QVector3D(x, y, z));
        }
        *dataArray << newRow;
    }
    proxy->resetArray(dataArray);
}

void Plot::generateSincProductData()
{
    float stepX = (maxAx - minAx) / float(numOfStepsX - 1);
    float stepZ = (maxAx - minAx) / float(numOfStepsZ - 1);

    QSurfaceDataArray *dataArray = new QSurfaceDataArray;
    for (int i = 0; i < numOfStepsZ; i++)
    {
        QSurfaceDataRow *newRow = new QSurfaceDataRow(numOfStepsX);
        float z = qMin(maxAx, (i * stepZ + minAx));
        int index = 0;
        for (int j = 0; j < numOfStepsX; j++)
        {
            float x = qMin(maxAx, (j * stepX + minAx));
            float y = sinc(x) * sinc(z);
            (*newRow)[index++].setPosition(QVector3D(x, y, z));
        }
        *dataArray << newRow;
    }
    proxy->resetArray(dataArray);
}

void Plot::generateSincDataScatter()
{
    float stepX = (maxAx - minAx) / float(numOfStepsX - 1);
    float stepZ = (maxAx - minAx) / float(numOfStepsZ - 1);

    QScatterDataArray *dataArrayScatter = new QScatterDataArray;
    dataArrayScatter->reserve(numOfStepsX * numOfStepsZ);

    for (int i = 0; i < numOfStepsZ; i++)
    {
        float z = qMin(maxAx, (i * stepZ + minAx));
        for (int j = 0; j < numOfStepsX; j++)
        {
            float x = qMin(maxAx, (j * stepX + minAx));
            float R = qSqrt(z * z + x * x);
            float y = sinc(R);
            dataArrayScatter->push_back(QScatterDataItem({x, y, z}));
        }
    }
    proxy_scatter->resetArray(dataArrayScatter);
}

void Plot::generateSincProductDataScatter()
{
    float stepX = (maxAx - minAx) / float(numOfStepsX - 1);
    float stepZ = (maxAx - minAx) / float(numOfStepsZ - 1);

    QScatterDataArray *dataArrayScatter = new QScatterDataArray;
    dataArrayScatter->reserve(numOfStepsX * numOfStepsZ);

    for (int i = 0; i < numOfStepsZ; i++)
    {
        float z = qMin(maxAx, (i * stepZ + minAx));
        for (int j = 0; j < numOfStepsX; j++)
        {
            float x = qMin(maxAx, (j * stepX + minAx));
            float y = sinc(x) * sinc(z);
            dataArrayScatter->push_back(QScatterDataItem({x, y, z}));
        }
    }
    proxy_scatter->resetArray(dataArrayScatter);
}
