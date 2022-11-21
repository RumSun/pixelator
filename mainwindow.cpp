/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwindow.h"
#include "imagemodel.h"
#include "pixeldelegate.h"

#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#endif
#endif

//! [0]
MainWindow::MainWindow()
{
//! [0]
    currentPath = QDir::homePath();
    model = new ImageModel(this);

    QWidget *centralWidget = new QWidget;

//! [1]
    view = new QTableView;
  //  view->setShowGrid(false);
    view->horizontalHeader()->hide();
    view->verticalHeader()->hide();
    view->horizontalHeader()->setMinimumSectionSize(1);
    view->verticalHeader()->setMinimumSectionSize(1);
    view->setModel(model);
//! [1]
    codeOutput = new QPlainTextEdit;
    codeOutput->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    //codeOutput->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    codeOutput->setWordWrapMode(QTextOption::NoWrap);

    QString str = "starting...";//QString::number(8*2);
    codeOutput->document()->setPlainText(str);
//! [2]
    PixelDelegate *delegate = new PixelDelegate(this);
    view->setItemDelegate(delegate);
//! [2]

//! [3]
    QLabel *pixelSizeLabel = new QLabel(tr("Pixel size:"));
    QSpinBox *pixelSizeSpinBox = new QSpinBox;
    pixelSizeSpinBox->setMinimum(4);
    pixelSizeSpinBox->setMaximum(32);
    pixelSizeSpinBox->setValue(12);
    pixelSizeSpinBox->setSingleStep(2);
//! [3]
//! [4] // size of image width height
    QLabel *imageSizeWLabel = new QLabel(tr("Image size  W:"));
    QLabel *imageSizeHLabel = new QLabel(tr("H:"));
    QSpinBox *imageWidthSpinBox = new QSpinBox;
    QSpinBox *imageHeightSpinBox = new QSpinBox;
    imageWidthSpinBox->setMinimum(4);
    imageWidthSpinBox->setMaximum(32);
    imageWidthSpinBox->setValue(0);//19);
    imageHeightSpinBox->setMinimum(4);
    imageHeightSpinBox->setMaximum(32);
    imageHeightSpinBox->setValue(0);//12);
//! [4]
//!
// text box for code 0b00000000 :
//   QPlainTextEdit *codeOutput = new QPlainTextEdit(this);
//  // codeOutput->

//   QString str = QString::number(8*2);
//   codeOutput->document()->setPlainText(str);

//    QLabel *pixelSizeLabel = new QLabel(tr("Pixel size:"));
//    QSpinBox *pixelSizeSpinBox = new QSpinBox;
//    pixelSizeSpinBox->setMinimum(4);
//    pixelSizeSpinBox->setMaximum(32);
//    pixelSizeSpinBox->setValue(12);

    QMenu *fileMenu = new QMenu(tr("&File"), this);
    QAction *openAction = fileMenu->addAction(tr("&Open..."));
    openAction->setShortcuts(QKeySequence::Open);

    printAction = fileMenu->addAction(tr("&Print..."));
    printAction->setEnabled(false);
    printAction->setShortcut(QKeySequence::Print);

    QAction *quitAction = fileMenu->addAction(tr("E&xit"));
    quitAction->setShortcuts(QKeySequence::Quit);

    QMenu *helpMenu = new QMenu(tr("&Help"), this);
    QAction *aboutAction = helpMenu->addAction(tr("&About"));

    menuBar()->addMenu(fileMenu);
    menuBar()->addSeparator();
    menuBar()->addMenu(helpMenu);

    connect(openAction, &QAction::triggered, this, &MainWindow::chooseImage);
    connect(printAction, &QAction::triggered, this, &MainWindow::printImage);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAboutBox);
//! [4]
    connect(pixelSizeSpinBox, &QSpinBox::valueChanged,
            delegate, &PixelDelegate::setPixelSize);
    connect(pixelSizeSpinBox, &QSpinBox::valueChanged,
            this, &MainWindow::updateView);
//! [4]

    QHBoxLayout *controlsLayout = new QHBoxLayout;
    controlsLayout->addWidget(pixelSizeLabel);
    controlsLayout->addWidget(pixelSizeSpinBox);
   // controlsLayout->addStretch(1);
    controlsLayout->addWidget(imageSizeWLabel);
    controlsLayout->addWidget(imageWidthSpinBox);
    controlsLayout->addWidget(imageSizeHLabel);
    controlsLayout->addWidget(imageHeightSpinBox);

    connect(imageWidthSpinBox, &QSpinBox::valueChanged,
            this, &MainWindow::updateCode);
    connect(imageHeightSpinBox, &QSpinBox::valueChanged,
            this, &MainWindow::updateCode);

  //  controlsLayout->addStretch(1);
    controlsLayout->addWidget(codeOutput);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(view);
    mainLayout->addLayout(controlsLayout);
    centralWidget->setLayout(mainLayout);
    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(onTableClicked(QModelIndex)));

    setCentralWidget(centralWidget);

    setWindowTitle(tr("Pixelator"));
    resize(640, 480);
//! [5]
}
//! [5]

void MainWindow::chooseImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Choose an image"), currentPath, "*");

    if (!fileName.isEmpty())
        openImage(fileName);
}

void MainWindow::openImage(const QString &fileName)
{
    QImage image,img2;
   // QString str;
    if (image.load(fileName)) {
        image.convertTo(QImage::Format_Mono);
        image.setColorCount(2);
        if(image.size().width() > 128){
            image= image.scaledToWidth(128);
        }
        if(image.size().height() > 64){
            image= image.scaledToHeight(64);
        }

        model->setImage(image);
        //image.loadFromData()
        if (!fileName.startsWith(":/")) {
            currentPath = fileName;
            setWindowTitle(tr("%1 - Pixelator").arg(currentPath));
        }

       // model->setData()
   //     printAction->setEnabled(true);
        updateView();
        updateCode();
        //str = QString::number(18*2);
       // QPlainTextEdit *codeOutput = new QPlainTextEdit(this->codeOutput);
       // codeOutput->

   //     QString str = QString::number(image.height() * image.width());
        //codeOutput->document()->setPlainText(QString::number(view->model()->rowCount() * view->model()->columnCount()));//QString::number(model->rowCount() * model->columnCount()));
   //     codeOutput->document()->setPlainText(str);
    }
}

void MainWindow::printImage()
{
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printdialog)
    if (model->rowCount(QModelIndex())*model->columnCount(QModelIndex()) > 90000) {
        QMessageBox::StandardButton answer;
        answer = QMessageBox::question(this, tr("Large Image Size"),
                tr("The printed image may be very large. Are you sure that "
                   "you want to print it?"),
        QMessageBox::Yes | QMessageBox::No);
        if (answer == QMessageBox::No)
            return;
    }

    QPrinter printer(QPrinter::HighResolution);

    QPrintDialog dlg(&printer, this);
    dlg.setWindowTitle(tr("Print Image"));

    if (dlg.exec() != QDialog::Accepted) {
        return;
    }

    QPainter painter;
    painter.begin(&printer);

    int rows = model->rowCount(QModelIndex());
    int columns = model->columnCount(QModelIndex());
    int sourceWidth = (columns + 1) * ItemSize;
    int sourceHeight = (rows + 1) * ItemSize;

    painter.save();

    double xscale = printer.pageRect(QPrinter::DevicePixel).width() / double(sourceWidth);
    double yscale = printer.pageRect(QPrinter::DevicePixel).height() / double(sourceHeight);
    double scale = qMin(xscale, yscale);

    painter.translate(printer.paperRect(QPrinter::DevicePixel).x() + printer.pageRect(QPrinter::DevicePixel).width() / 2,
                      printer.paperRect(QPrinter::DevicePixel).y() + printer.pageRect(QPrinter::DevicePixel).height() / 2);
    painter.scale(scale, scale);
    painter.translate(-sourceWidth / 2, -sourceHeight / 2);

    QStyleOptionViewItem option;
    QModelIndex parent = QModelIndex();

    QProgressDialog progress(tr("Printing..."), tr("Cancel"), 0, rows, this);
    progress.setWindowModality(Qt::ApplicationModal);
    float y = ItemSize / 2;

    for (int row = 0; row < rows; ++row) {
        progress.setValue(row);
        qApp->processEvents();
        if (progress.wasCanceled())
            break;

        float x = ItemSize / 2;



        for (int column = 0; column < columns; ++column) {
            option.rect = QRect(int(x), int(y), ItemSize, ItemSize);
            view->itemDelegate()->paint(&painter, option,
                                        model->index(row, column, parent));
            x = x + ItemSize;
        }
        y = y + ItemSize;
    }
    progress.setValue(rows);

    painter.restore();
    painter.end();

    if (progress.wasCanceled()) {
        QMessageBox::information(this, tr("Printing canceled"),
            tr("The printing process was canceled."), QMessageBox::Cancel);
    }
#else
    QMessageBox::information(this, tr("Printing canceled"),
        tr("Printing is not supported on this Qt build"), QMessageBox::Cancel);
#endif
}

void MainWindow::showAboutBox()
{
    QMessageBox::about(this, tr("About the Pixelator example"),
        tr("This example demonstrates how a standard view and a custom\n"
           "delegate can be used to produce a specialized representation\n"
           "of data in a simple custom model."));
}

//! [6]
void MainWindow::updateView()
{
    view->resizeColumnsToContents();
    view->resizeRowsToContents();
    //   codeOutput->document()->setPlainText(QString("hi"));
}

void MainWindow::updateCode()
{
       //QString str = QString::number(8*2);
    QString str = "";
    QString str_out = "";
    codeOutput->clear();
       int a = 0;
       int cols = model->columnCount();
       int rows = model->rowCount();
       codeOutput->insertPlainText(QString::asprintf("%d, ",cols));
       codeOutput->insertPlainText(QString::asprintf("%d, ",rows));
       int number = 0;
       //int ddata = 0;
       //codeOutput->setPlainText(str);
       //codeOutput->document()->setPlainText(str);
       if(rows*cols <= 128*64)
       for(int i=0;i<cols;i++){
           for(int j=0;j< rows;j++){
               //number = number << 1;
               //const QPoint &pt, uint index_or_rgb) { setPixel(pt.x(), pt.y(), index_or_rgb);

               //this->model->setPixel(QPoint(1,1),255);
//               beginResetModel();
//               modelImage = image;
//               modelImage.setPixel(1,1,255);
//               endResetModel();
          //    ddata = view->model()->index(j,i).data().toInt();
              // view->model()->index(j,i).data()="1";
               if(view->model()->index(j,i).data().toBool()){
                   view->model()->setData(view->model()->index(j,i),0/*"0"*/,Qt::DisplayRole/*Qt::EditRole*/);
                   view->update(view->model()->index(j,i));

                   //ddata = view->model()->index(j,i)=0;//.data().toInt();
                  // view->model()->setItemData(view->model()->index(j,i),)
                   //        view->model().set
                   //codeOutput->insertPlainText("0");//QString::number(a));
                   //number += 1;
               }
               else{
        //           view->model()->index(j,i).data()="1";
                   //codeOutput->insertPlainText("1");//QString::number(a));
                   number += (1 << a);
               }
               if(a<7){
                   a++;
                   if((i==cols-1) && ( j== rows-1)){
                       //codeOutput->insertPlainText(QString::asprintf("0x%02X ",number));
                       str = QString::asprintf("0x%02X ",number);
                       str_out+=str;
                       //codeOutput->insertPlainText(str);
                   //    codeOutput->insertPlainText(" ");
                       //codeOutput->insertPlainText("\n");
                   }
               }
               else{
                   a=0;
                   if((i==cols-1) && ( j== rows-1)){
                       //codeOutput->insertPlainText(QString::asprintf("0x%02X ",number));
                       str = QString::asprintf("0x%02X ",number);
                       str_out+=str;
                       //codeOutput->insertPlainText(str);
                   //    codeOutput->insertPlainText(" ");
                       //codeOutput->insertPlainText("\n");
                   }
                   else{
                       //codeOutput->insertPlainText(QString::asprintf("0x%02X, ",number));
                       str = QString::asprintf("0x%02X, ",number);
                       str_out+=str;
                       number = 0;
                       //codeOutput->insertPlainText(str);
                    //   codeOutput->insertPlainText(" ");
                       //codeOutput->insertPlainText("\n");
                   }
               }
               //codeOutput->insertPlainText(str);
           }
       }
       codeOutput->insertPlainText(str_out);

       //codeOutput->document()->setPlainText(QString::number(view->model()->rowCount() * view->model()->columnCount()));//QString::number(model->rowCount() * model->columnCount()));

}
//! [6]
void MainWindow::onTableClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        //QString cellText = index.data().toString();
    //this->view->model()->setData(this->view->model()->index(5,5),/*index,*/1);
       // this->view->model()->setData(index,"1");

        this->model->setPixel(QPoint(index.column(),index.row()),1);
       // this->model->setPixel(QPoint(index.column()+1,index.row()+1),1);
        //QVariant value = QVariant("1");
        //this->view->model()->setData(index,value);
            //index
        //index
       // view->model()->index(j,i).data()
        updateView();
        updateCode();
    }
}
