#ifndef PLOT_H
#define PLOT_H

#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/Q3DScatter>
#include <QtDataVisualization/QAbstract3DGraph>
#include <QtDataVisualization/Q3DTheme>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtDataVisualization/QValue3DAxis>
#include <QtMath>

#include <QLinearGradient>

using namespace QtDataVisualization;

class Plot
{
public:
    Plot();
    ~Plot();
    Q3DSurface *getGraphSurface();
    Q3DScatter *getGraphScatter();
    void updateData(int functionType);
    void updateSelection(int selectionEnabled);
    QString getCoord(const QPoint &pos);
    QSurface3DSeries *getSeriesSurface();
    QScatter3DSeries *getSeriesScatter();
    bool isSurface;

public slots:
    void toggleGrid(bool showGrid);
    void setAxisVisibility(bool showAxis);
    void setAxisLabelsBorderVisibility(bool showBorder);

    void updateXMin(float value);
    void updateXMax(float value);
    void updateZMin(float value);
    void updateZMax(float value);
    void createColorGradient();
    void createColorGradient2();
    void setStepsX(int value);
    void setStepsZ(int value);
    void switchToSurface();
    void switchToScatter();

private:
    void generateSincData();
    void generateSincProductData();
    void generateSincDataScatter();
    void generateSincProductDataScatter();

    Q3DSurface *my_graph_surface;
    QSurfaceDataProxy *proxy;
    QSurface3DSeries *series;

    Q3DScatter *my_graph_scatter;
    QScatterDataProxy *proxy_scatter;
    QScatter3DSeries *series_scatter;

    Q3DTheme *theme;

    int selectionMode;
    bool show;
};

#endif	  // PLOT_H
