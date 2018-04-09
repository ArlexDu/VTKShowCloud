#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include "QValueAxis"
#include <iostream>
#include <string>
#include "HS_Lidar.h"

using namespace std;

QT_CHARTS_USE_NAMESPACE

bool isHeaderRight(uint8_t header[8]) {

    uint8_t headerSign[] = {1, 35, 69, 103, 137, 171, 205, 239};
    bool returnVal = true;

    for (size_t i = 0; i < 8; i++) {
        header[i] != headerSign[i] ? returnVal = false : returnVal = true;
    }

    return returnVal;
}


void moveToOffset(int offset, FILE *fp) {
    fseek(fp, 0, SEEK_SET);
    for (int nHead_coll = 0; nHead_coll < offset; nHead_coll++) {
        uint8_t header[8];
        memset(header, 0, sizeof(uint8_t) * 8);
        fread(header, sizeof(uint8_t), 8, fp);

        if (isHeaderRight(header)) {
            fseek(fp, 2680, SEEK_CUR);
        } else {
            nHead_coll = nHead_coll - 1;
            fseek(fp, -7, SEEK_CUR);
        }
    }
}


MainWindow::MainWindow() {
    this->ui = new Ui_MainWindow;
    this->ui->setupUi(this);

    //VTK显示点云

    // Geometry
    unsigned int red[3] = {255,0,0};
    unsigned int green[3] = {0,255,0};
    unsigned int blue[3] = {0,0,255};
    //数据的导入
    std::ifstream ifs;
    ifs.open("/Users/arlex/Downloads/111.las",std::ios::in|std::ios::binary);
    liblas::ReaderFactory f;
    liblas::Reader reader = f.CreateWithStream(ifs);
    double arr[3];
    //空间结构的处理
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    //颜色值
    vtkSmartPointer<vtkUnsignedCharArray> colors = vtkSmartPointer<vtkUnsignedCharArray>::New();
    colors->SetNumberOfComponents(3);
    colors->SetName("Colors");
    int n=0;
    while(reader.ReadNextPoint()){
        liblas::Point const& p = reader.GetPoint();
        arr[0] = p.GetX();
        arr[1] = p.GetY();
        arr[2] = p.GetZ();
        points->InsertPoint(n,arr[0],arr[1],arr[2]);
        if(arr[2]<17){
            colors->InsertNextTuple3(blue[0],blue[1],blue[2]);
        }else{
            colors->InsertNextTuple3(green[0],green[1],green[2]);
        }
//        colors->InsertNextTuple3(red[0],red[1],red[2]);
        n++;
    }
//    printf("%d\n", n);

    //组织结构/拓扑结构的处理
    vtkSmartPointer<vtkPolyData> grid=vtkSmartPointer<vtkPolyData>::New();
    grid->SetPoints(points);
    grid->GetPointData()->AddArray(colors);
    vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    vertexFilter->SetInputData(grid);
    vertexFilter->Update();
    vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
//  printf("\n filter number is %d",vertexFilter->GetOutput()->GetNumberOfCells());
    mapper->SetInputData(vertexFilter->GetOutput());
    //设置点的颜色
    mapper->SetScalarModeToUsePointFieldData();
    mapper->SelectColorArray("Colors");

    // Actor in scene
    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);

    // VTK Renderer
    vtkNew<vtkRenderer> ren;

    // Add Actor to renderer
    ren->AddActor(actor);

    // VTK/Qt wedded
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    this->ui->Map->SetRenderWindow(renderWindow);
    this->ui->Map->GetRenderWindow()->AddRenderer(ren);

    //显示二维波形图部分

    char filename[500] = "/Users/arlex/Downloads/ocean_2017_12_24_15_43_24~2017_12_24_16_10_08.bin";
    FILE *fp;
    fp = fopen(filename, "rb+");
    fseek(fp, 0, SEEK_END);
    int filesize = ftell(fp) / 2688 - 2;
    cout << "filesize" << ":" << filesize << endl;
    fseek(fp, 0, SEEK_SET);


    HS_Lidar *HS = new HS_Lidar[filesize];

    int i1, i2, i3, i4 = 0;
    auto lines1 = new QLineSeries();
    auto lines2 = new QLineSeries();
    auto lines3 = new QLineSeries();
    auto lines4 = new QLineSeries();
    int j1 = 0;
    int j2 = 0;
    int j3 = 0;
    int j4 = 0;
    int maxY = 0;

    for (int i = 0; i < 1; i++) {
        cout << "Loading" << ":" << i << "/" << filesize << endl;
        uint8_t header[8];
        fseek(fp, 0, SEEK_SET);
        moveToOffset(0, fp);
        fread(header, sizeof(uint8_t), 8, fp);

        HS[i].initData(fp);

        for (int j = 0; j < HS[i].CH1.nL0; j++) {
            lines1->append(j1, (unsigned) HS[i].CH1.nD0[j]);
            if ((unsigned) HS[i].CH1.nD0[j] > maxY) {
                maxY = (unsigned) HS[i].CH1.nD0[j];
            }
            j1++;
        }
        for (int j = 0; j < HS[i].CH2.nL0; j++) {
            lines2->append(j2, (unsigned) HS[i].CH2.nD0[j]);
            if ((unsigned) HS[i].CH2.nD0[j] > maxY) {
                maxY = (unsigned) HS[i].CH2.nD0[j];
            }
            j2++;
        }
        for (int j = 0; j < HS[i].CH3.nL0; j++) {
            lines3->append(j3, (unsigned) HS[i].CH3.nD0[j]);
            if ((unsigned) HS[i].CH3.nD0[j] > maxY) {
                maxY = (unsigned) HS[i].CH3.nD0[j];
            }
            j3++;
        }
        for (int j = 0; j < HS[i].CH4.nL0; j++) {
            lines4->append(j4, (unsigned) HS[i].CH4.nD0[j]);

            if ((unsigned) HS[i].CH4.nD0[j] > maxY) {
                maxY = (unsigned) HS[i].CH4.nD0[j];
            }

            j4++;
        }

    }


    // 3d Chart
    auto chart3D = new QChart();
    chart3D->setTitle("2d 波形图");
    chart3D->legend()->setVisible(false);

    chart3D->createDefaultAxes();

    chart3D->addSeries(lines1);
    chart3D->addSeries(lines2);
    chart3D->addSeries(lines3);
    chart3D->addSeries(lines4);

    chart3D->setGeometry(10, 4, 300, 260); // the method of QGraphicsWidget, move && resize
    chart3D->createDefaultAxes();
    chart3D->setBackgroundVisible(true);
    chart3D->setBackgroundPen(QPen(Qt::lightGray)); // the frame
    chart3D->setBackgroundBrush(QBrush(QColor(240, 240, 240)));
    ui->View3d->setChart(chart3D);
    ui->View3d->setRenderHint(QPainter::Antialiasing);


    for (int i = 0; i < 100; i++) {
        lines4->append(j4, 1000);
    }

    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, j1);
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, maxY);

    cout << maxY << endl;

    chart3D->setAxisX(axisX, lines1);
    chart3D->setAxisY(axisY, lines1);
    chart3D->setAxisX(axisX, lines2);
    chart3D->setAxisY(axisY, lines2);
    chart3D->setAxisX(axisX, lines3);
    chart3D->setAxisY(axisY, lines3);
    chart3D->setAxisX(axisX, lines4);
    chart3D->setAxisY(axisY, lines4);
}

MainWindow::~MainWindow() {
//    delete ui;
}

void MainWindow::slotExit() {
    qApp->exit();
}

