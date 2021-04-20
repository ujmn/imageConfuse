#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class QPushButton;
class QLabel;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    void InitWidget();

    QSize sizeHint() const override{
        return QSize(480, 560);
    }

    void keyPressEvent(QKeyEvent *event) override;

    void display(const QString &fileName);

private:
    QPushButton *m_pBtn;
    QLabel		*m_pLable;
};
#endif // WIDGET_H
