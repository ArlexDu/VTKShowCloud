#include "mainwindow.h"

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
    connect(ui->actionOpen,SIGNAL(triggered()), this, SLOT(openLas()));
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

//打开资源管理器选择文件
void MainWindow::openLas() {
//    选择文件
//    QString path=QFileDialog::getOpenFileName(this,"选择文件",".","las(*.las)");
//    std::string filepath = path.toLocal8Bit().constData();
    string filepath = "/Users/arlex/Downloads/111.las";
    vtkSmartPointer<vtkDataReader> reader = vtkSmartPointer<vtkDataReader>::New();
    reader->SetFileName(filepath.c_str());
    reader->Update();
    vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
//  printf("\n filter number is %d",vertexFilter->GetOutput()->GetNumberOfCells());
    mapper->SetInputData(reader->GetOutput());
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
//    增加交互
    vtkSmartPointer<vtkPointPicker> pointPicker = vtkSmartPointer<vtkPointPicker>::New();
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetPicker(pointPicker);
    renderWindowInteractor->SetRenderWindow(renderWindow);
    vtkSmartPointer<PointPickerInteractorStyle> style = vtkSmartPointer<PointPickerInteractorStyle>::New();
    renderWindowInteractor->SetInteractorStyle(style);

//    设置方位轴
    vtkSmartPointer<vtkAxesActor> Axes = vtkSmartPointer<vtkAxesActor>::New();
    vtkSmartPointer<vtkOrientationMarkerWidget> widget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    widget->SetInteractor(renderWindowInteractor);
    widget->SetOrientationMarker(Axes);
    widget->SetOutlineColor(1,1,1);
    widget->SetViewport(0,0,0.2,0.2);
    widget->SetEnabled(1);
    widget->InteractiveOn();

    renderWindow->Render();
    renderWindowInteractor->Start();
    renderWindow->AddRenderer(ren);
    this->ui->Map->SetRenderWindow(renderWindow);
//    this->ui->Map->GetRenderWindow()->AddRenderer(ren);
};

