#pragma once

#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QDialog>

class SliderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SliderDialog(QWidget *parent = nullptr, QString title = "Select a value");
    int getValue() const;

private slots:
    void onSliderValueChanged(int value);
    void onOkButtonClicked();

private:
    QSlider *slider;
    QLabel *label;
    QPushButton *okButton;
    int result;
};