#include "sliderdialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

SliderDialog::SliderDialog(QWidget *parent, QString title)
    : QDialog(parent), result(0) {
  slider = new QSlider(Qt::Horizontal, this);
  label = new QLabel("Value: 0", this);
  okButton = new QPushButton("OK", this);

  slider->setRange(-100, 100);

  connect(slider, &QSlider::valueChanged, this,
          &SliderDialog::onSliderValueChanged);
  connect(okButton, &QPushButton::clicked, this,
          &SliderDialog::onOkButtonClicked);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(label);
  layout->addWidget(slider);
  layout->addWidget(okButton);

  setLayout(layout);
  setWindowTitle(title);
}

void SliderDialog::onSliderValueChanged(int value) {
  label->setText(QString("Value: %1").arg(value));
}

void SliderDialog::onOkButtonClicked() {
  result = slider->value();
  accept();
}

int SliderDialog::getValue() const { return result; }