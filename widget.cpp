#include "widget.h"
#include "imageobfuscater.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QDebug>
#include <QLabel>
#include <QPicture>
#include <QKeyEvent>
#include <QMessageBox>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool is_jpeg( FILE* file ) {
    constexpr uint8_t JPEG_HEAD[] = {0xFF,0xD8};
    constexpr uint8_t JPEG_TAIL[] = {0xFF,0xD9};
    uint8_t bytes[2];
    fseek(file, 0, SEEK_SET);
    fread(bytes, 1, 2, file);
    if(memcmp(JPEG_HEAD, bytes, 2)!=0) {
        return false;
    }
    fseek(file, -2, SEEK_END);
    fread(bytes, 1, 2, file);
    if(memcmp(JPEG_TAIL, bytes, 2)!=0) {
        return false;
    }
    return true;
}

bool is_png( FILE* file) {
    const uint8_t PNG_HEADER[] = {0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A};
    uint8_t bytes[sizeof(PNG_HEADER)];
    fseek(file, 0, SEEK_SET);
    fread(bytes, 1, sizeof(PNG_HEADER), file);
    if(memcmp(PNG_HEADER, bytes, sizeof(PNG_HEADER))!=0) {
        return false;
        qDebug() << "isn't png";
    }
    return true;
}

enum class ImageType {
    PNG,
    JPEG,
    INVALID,
};

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , m_pBtn(new QPushButton)
    , m_pLable(new QLabel)
{
    InitWidget();
}

void Widget::InitWidget(){
    m_pBtn->setText("+");
    m_pBtn->setFixedSize(45, 45);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(m_pBtn);
    layout->addWidget(m_pLable);
    m_pLable->hide();

    connect(m_pBtn, &QPushButton::clicked, [&](){
       QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open Image"), "/home/", tr("Image Files (*.png *.jpg *.jpeg)"));
        if (fileName.isEmpty())
            return;

        uint32_t cell_size = 32;
        std::string input_path = fileName.toStdString();
        QStringList strList = fileName.split(".");
        QString strOutPath;
        if (strList.size() == 2){
            strOutPath = strList.at(0) + "-result." + strList.at(1);
        }else{
            for (int i = 0; i < strList.size() - 1; ++i){
                strOutPath += strList.at(i) + ".";
            }
            strOutPath += "-result." + strList.at(strList.size() - 2);
        }
        qDebug() << input_path.c_str();
        auto file = fopen(input_path.c_str(), "rb");
        if(!file) {
            printf("file not found!");
        }
        ImageType type = ImageType::INVALID;
        if( is_jpeg(file)) {
            type = ImageType::JPEG;
        } else if( is_png(file)) {
            type = ImageType::PNG;
        }
        if(type == ImageType::INVALID) {
            printf("unsupported format!");
            QMessageBox::critical(this, "error", "this format is not supported", QMessageBox::Ok, QMessageBox::NoButton);
            return;
        }
        int w,h,n;
        auto data = stbi_load(input_path.c_str(), &w, &h, &n, 4);
        ImageObfuscater obfs( (uint32_t*)data, w, h, cell_size);
        obfs.obfuscate();
        std::string output_path = strOutPath.toStdString();
        qDebug() << output_path.c_str();
        switch(type) {
            case ImageType::JPEG:{
                stbi_write_jpg(output_path.c_str(), w, h, 4, data, w*4);
                display(strOutPath);
        }
                break;
        case ImageType::PNG:{
                stbi_write_png(output_path.c_str(), w, h, 4, data, w*4);
                display(strOutPath);
                }
        }
    });
}

void Widget::display(const QString &fileName){
    QPixmap pixmap;
    if(pixmap.load(fileName)){
            m_pLable->setPixmap(pixmap);
            m_pBtn->hide();
            m_pLable->show();
    }else{
        QMessageBox::information(this, "information", "image confuse successful, but load image faile.", QMessageBox::Ok, QMessageBox::NoButton);
    }
}

void Widget::keyPressEvent(QKeyEvent *event){
        QKeyEvent *keyEvent = dynamic_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Escape){
            m_pLable->hide();
            m_pBtn->show();
        }
}

Widget::~Widget()
{
}

