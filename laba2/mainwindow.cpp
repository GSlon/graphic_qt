#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Laba 2");
    this->InitWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitWindow()
{
    this->InitMenu();

    this->resize(600, 400);
    widget = new QWidget(this);

    painter = new QCustomPlot(widget);
    painter->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom); // Чтобы менять масштаб мышкой и двигаться по графику
    // give the axes some labels:
    painter->xAxis->setLabel("X");
    painter->yAxis->setLabel("Y");
    painter->xAxis->setRange(0,10);
    painter->yAxis->setRange(0,10);

    lay = new QVBoxLayout(this);
    lay->addWidget(painter);

    widget->setLayout(lay);
    setCentralWidget(widget);
}

void MainWindow::InitMenu()
{
    fileMenu = new QMenu("&File");

    fileMenu->addAction("&Open"
                        ,this
                        ,SLOT(OpenFile())
                        ,Qt::CTRL + Qt::Key_O);

    fileMenu->addAction("&Exit"
                        ,this
                        ,SLOT(Quit())
                        ,Qt::CTRL + Qt::Key_E);


    aboutMenu = new QMenu("&About");

    aboutMenu->addAction("&About"
                         ,this
                         ,SLOT(About())
                         ,Qt::CTRL + Qt::Key_A);

    aboutMenu->addAction("&Help"
                         ,this
                         ,SLOT(Help())
                         ,Qt::CTRL + Qt::Key_H);


    ui->menuBar->addMenu(fileMenu);
    ui->menuBar->addMenu(aboutMenu);
    ui->menuBar->show();
}

// добавляем новую линию
void MainWindow::Draw(const QVector<double> &x, const QVector<double> &y)
{
    // create new graph
    painter->addGraph();
    painter->graph(painter->graphCount() - 1)->setData(x, y);    // graph(number) это номер очередной отдельной линии (обращаемся к новосозданной линии)

    //Задаем цвет точки
    painter->graph(painter->graphCount() - 1)->setPen(QColor(255, 0, 0, 255));
    //формируем вид точек
    painter->graph(painter->graphCount() - 1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));

    painter->replot();
}

void MainWindow::OpenFile()
{
    QString filePath = QFileDialog::getOpenFileName(0, "Open txt file", QString(), "*.txt");
    QFile readFile(filePath);
    QTextStream instream(&readFile);

    if (readFile.open(QIODevice::ReadOnly|QIODevice::Text))
            qDebug() << "File opened ";
    else
    {
            qDebug() << "File opening error ";
            return;
    }

    painter->clearGraphs();   // начинаем рисовать с 'чистого листа'
    QVector<double> x,y;
    int count_dots = 0;
    bool check;          // корректность ввода

    // считывание из файла
    while (!instream.atEnd())
    {
        // Считываем количество точек
        count_dots = instream.readLine().toInt(&check);
        if (!check)
        {
            qDebug() << "incorrect input ";
            readFile.close();
            return;
        }

        // начинаем заполнение
        x.clear();
        x.resize(count_dots);
        y.clear();
        y.resize(count_dots);

        int i = 0;
        while (i < count_dots && !instream.atEnd())
        {
            // Считываем в стринг лист строчки, разделённые
            QStringList list = instream.readLine().simplified().split(' '); // метод simplified() заменяет все пробелы на один

            if (list.length() != 2)
            {
                qDebug() << "incorrect input ";
                readFile.close();
                return;
            }

            // Добавляем координаты из листа
            x[i] = list.at(0).toDouble(&check);
            if (!check)
            {
                qDebug() << "incorrect input ";
                readFile.close();
                return;
            }

            y[i] = list.at(1).toDouble(&check);
            if (!check)
            {
                qDebug() << "incorrect input ";
                readFile.close();
                return;
            }

            ++i;
        }

        Draw(x, y);
    }

    readFile.close();
}

void MainWindow::Quit()
{
    this->close();
}

void MainWindow::About()
{
    QMessageBox::about(this, "Автор", "Прохватилов Станислав, группа М8О-213Б");
}

void MainWindow::Help()
{
    QString str;
    QDir dir;
    // str = QApplication::applicationFilePath();
    dir.setPath(dir.absolutePath());  // путь до сборки
    str = dir.absolutePath();
    qDebug() << str;

    QFile file(str + "/help.txt");

    //не откроется, значит была включена теневая сборка (двигаемся к pro-файлу)
    if (!file.isOpen())
    {
        dir.cdUp();  //к каталогу
        str = dir.absolutePath();
        file.setFileName(str + "/laba2/help.txt");
        file.open(QIODevice::ReadOnly);
        if (!file.isOpen())
        {
            qDebug() << "help.txt не найден";
            QMessageBox::information(this, "error", "файл help.txt не найден");
            return;
        }
    }

    QTextEdit *edit = new QTextEdit(this);
    edit->setReadOnly(true);

    QTextStream text(&file);
    str.clear();
    str = text.readAll();
    edit->setPlainText(str);

    HelpWidget = new QWidget(this, Qt::Window);
    HelpWidget->setGeometry(QRect(450, 250, 500, 300));
    HelpWidget->setWindowTitle("help");

    QHBoxLayout *lay = new QHBoxLayout(this);
    lay->addWidget(edit);
    HelpWidget->setLayout(lay);

    HelpWidget->activateWindow();
    HelpWidget->show();
}
