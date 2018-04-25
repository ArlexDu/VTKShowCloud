#include <geos/platform.h>
#include "mainwindow.h"

using namespace std;

QT_CHARTS_USE_NAMESPACE

MainWindow::MainWindow() {
    this->ui = new Ui_MainWindow;
    this->ui->setupUi(this);
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openLas()));
    //显示二维波形图部分
    chart = new Chart();
    chartView = new ChartView(chart->chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->mainChartLayout->addWidget(chartView);
    ui->propertyTable->setItem(0, 0,  new QTableWidgetItem(QString("GPS_Time")));
    ui->propertyTable->setItem(1, 0,  new QTableWidgetItem(QString("X")));
    ui->propertyTable->setItem(2, 0,  new QTableWidgetItem(QString("Y")));
    ui->propertyTable->setItem(3, 0,  new QTableWidgetItem(QString("Z")));
    ui->propertyTable->setItem(4, 0,  new QTableWidgetItem(QString("Intensity")));
    ui->propertyTable->setItem(5, 0,  new QTableWidgetItem(QString("Type")));
    ui->propertyTable->setItem(6, 0,  new QTableWidgetItem(QString("ShotNum")));
    ui->propertyTable->setItem(7, 0,  new QTableWidgetItem(QString("Begian")));
    ui->propertyTable->setItem(8, 0,  new QTableWidgetItem(QString("PeakLoc")));
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
//    QString path=QFileDialog::getOpenFileName(this,"选择文件",".","las(*.las);;txt(*.txt)");
//    std::string filepath = path.toLocal8Bit().constData();
//    string filepath = "/Users/arlex/Downloads/111.las";
    string filepath = "/Users/arlex/Documents/课程/激光点云/2015_12_15_07_31_01_117.txt";
    vtkSmartPointer<nvtkDataReader> reader = vtkSmartPointer<nvtkDataReader>::New();
    reader->SetFileName(filepath.c_str());

    vtkSmartPointer<nvtkDataSimplify> processing = vtkSmartPointer<nvtkDataSimplify>::New();
    processing->SetInputConnection(reader->GetOutputPort());
    processing->Update();

    vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();

    mapper->SetInputData(processing->GetOutput());
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
    ren->SetBackground(0,0,0);

    // VTK/Qt wedded
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->Map->SetRenderWindow(renderWindow);
    ui->Map->GetRenderWindow()->AddRenderer(ren);
//    增加交互
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = ui->Map->GetInteractor();
    renderWindowInteractor->SetRenderWindow(renderWindow);

    vtkSmartPointer<nvtkPointPickerInteractorStyle> style = vtkSmartPointer<nvtkPointPickerInteractorStyle>::New();
    style->SetFileName(filepath);
    style->table = ui->propertyTable;
    renderWindowInteractor->SetInteractorStyle(style);

//    设置方位轴
//    vtkSmartPointer<vtkAxesActor> Axes = vtkSmartPointer<vtkAxesActor>::New();
//    vtkSmartPointer<vtkOrientationMarkerWidget> widget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
//    widget->SetInteractor(renderWindowInteractor);
//    widget->SetOrientationMarker(Axes);
//    widget->SetOutlineColor(1, 1, 1);
//    widget->SetViewport(0, 0, 0.2, 0.2);
//    widget->SetEnabled(1);
//    widget->InteractiveOn();

    renderWindow->Render();
    renderWindowInteractor->Start();

    ui->Map->update();
};

void MainWindow::onDrawChanged(DrawData info) {

    chart->newFrame(info);

    chartView->getToZoomLevel();

}


void MainWindow::on_actionMoveRight_triggered() {
//    qDebug()<< chart->filename;
}

void MainWindow::on_actionMoveLeft_triggered() {

}

void MainWindow::on_actionOceanStart_triggered() {

    if (chart->isPlay) {
        drawChartThread->requestInterruption();
        QIcon icon;
        icon.addFile(tr(":/images/images/play.png"));
        ui->actionOceanStart->setIcon(icon);

    } else {
        QIcon icon;
        icon.addFile(tr(":/images/images/pause.png"));
        ui->actionOceanStart->setIcon(icon);
        drawChartThread = new DrawChartThread;
        drawChartThread->chart = chart;
        chart->chartType = 1;
        drawChartThread->i = chart->offset;
        connect(drawChartThread, SIGNAL(draw(DrawData)),
                SLOT(onDrawChanged(DrawData)));
        connect(drawChartThread, SIGNAL(finished()),
                drawChartThread, SLOT(deleteLater()));
        drawChartThread->start();
    }
    chart->isPlay = !chart->isPlay;


}

void MainWindow::on_actionOpenOceanFile_triggered() {

    QString fileName = QFileDialog::getOpenFileName(
            this, tr("open  file"),
            "./", tr("Ocean files(*.ocean);;All files (*.*)"));

    if (fileName.isEmpty()) {
        QMessageBox mesg;
        mesg.warning(this, "警告", "打开文件失败!");
        return;
    }
    chart->filename = fileName;

}

void MainWindow::on_showWave_clicked()
{
    if(chart->isPlay){
        drawChartThread->requestInterruption();
        QIcon icon;
        icon.addFile(tr(":/images/images/play.png"));
        ui->actionOceanStart->setIcon(icon);
        chart->isPlay = !chart->isPlay;
    }

    string filename = chart->filename.toStdString();
    FILE *fp;
    fp = fopen(filename.c_str(), "rb");

    int x1 = 0;
    int x2 = 0;
    int x3 = 0;
    int x4 = 0;
    DrawData *drawData = new DrawData;



    //读取4个波段数据

    //每帧数据长6500
    int nLen = 6500;
    short *sData_CH0 = new short [nLen];
    short *sData_CH1 = new short [nLen];
    short *sData_CH2 = new short [nLen];
    short *sData_CH3 = new short [nLen];
    memset(sData_CH0, 0, sizeof(short)*nLen);
    memset(sData_CH1, 0, sizeof(short)*nLen);
    memset(sData_CH2, 0, sizeof(short)*nLen);
    memset(sData_CH3, 0, sizeof(short)*nLen);

    //LADM2-激光数据头大小，固定值
    static const int nLen_Header = 160;



    //每个Binshot长度是固定的
    double dLen_Binshot = 2*4*nLen;

    int nTragetShot = ui->propertyTable->item(6,1)->text().toInt();

    //指针跳的长度 nTragetShot是想要读取的帧号
    int64 nSeekL = nLen_Header + dLen_Binshot*nTragetShot;

    //常规fseek第二个参数为long 最大值为2147483647, 激光数据的字节更大，这里用_fseeki64
    fseek(fp, nSeekL, SEEK_SET);

    fread(sData_CH0, sizeof(short), nLen, fp);
    fread(sData_CH1, sizeof(short), nLen, fp);
    fread(sData_CH2, sizeof(short), nLen, fp);
    fread(sData_CH3, sizeof(short), nLen, fp);



    drawData->maxY = 0;
    drawData->minY = 0;


    drawData->size1 = nLen;
    drawData->size2 = nLen;
    drawData->size3 = nLen;
    drawData->size4 = nLen;

    for (int j=0;j < nLen;j++) {
        if(sData_CH0[j]<(-2000)){
            sData_CH0[j]=0;
        }
        if(sData_CH0[j]<(-2000)){
            sData_CH0[j]=0;
        }
        if(sData_CH0[j]<drawData->minY){
            drawData->minY= sData_CH0[j];
        }
        if(sData_CH1[j]>drawData->maxY){
            drawData->maxY= sData_CH0[j];
        }
        drawData->line2[j]= sData_CH0[j];
        x1++;
    }
    for (int j=0;j < nLen;j++) {
        if(sData_CH1[j]<(-2000)){
            sData_CH1[j]=0;
        }
        if(sData_CH1[j]<(-2000)){
            sData_CH1[j]=0;
        }
        if(sData_CH1[j]<drawData->minY){
            drawData->minY= sData_CH1[j];
        }
        if(sData_CH1[j]>drawData->maxY){
            drawData->maxY= sData_CH1[j];
        }
        drawData->line2[j]= sData_CH1[j];
        x2++;
    }

    for (int j=0;j < nLen;j++) {
        if(sData_CH2[j]<(-2000)){
            sData_CH2[j]=0;
        }
        if(sData_CH2[j]<(-2000)){
            sData_CH2[j]=0;
        }
        if(sData_CH2[j]<drawData->minY){
            drawData-> minY= sData_CH1[j];
        }
        if(sData_CH2[j]>drawData->maxY){
            drawData->maxY= sData_CH2[j];
        }
        drawData->line3[j]= sData_CH2[j];
        x3++;
    }

    for (int j=0;j < nLen;j++) {
        if(sData_CH3[j]<(-2000)){
            sData_CH3[j]=0;
        }
        if(sData_CH3[j]<(-2000)){
            sData_CH3[j]=0;
        }
        if(sData_CH3[j]<drawData->minY){
            drawData->minY= sData_CH1[j];
        }
        if(sData_CH3[j]>drawData->maxY){
            drawData->maxY= sData_CH3[j];
        }
        drawData->line4[j]= sData_CH3[j];
        x4++;
    }

    drawData->x1 = x1;
    drawData->x2 = x2;
    drawData->x3 = x3;
    drawData->x4 = x4;
    nTragetShot++;
    drawData->i = nTragetShot;
    drawChartThread->i = nTragetShot;
    chart->newFrame(*drawData);
    fclose(fp);
}


