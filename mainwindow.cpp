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
    style->SetFileName(filepath.c_str());
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


