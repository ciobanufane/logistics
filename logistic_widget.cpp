#include <QString>
#include <QAction>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QTabWidget>
#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "logistic_widget.h"

LogisticWidget::LogisticWidget(QWidget* parent) : QWidget(parent)
{
    logs = readLogisticCSV(":/logistic/logistic.csv");
    cachedHour = -1;
    cachedMinute = -1;
    cachedResource = -1;
    createLogisticLayout();
}

/*
 *  Creates all child widgets for the logistic widget
 */
void LogisticWidget::createLogisticLayout()
{
    QVBoxLayout* mainLayout = new QVBoxLayout();

    chapterTabWidget = new QTabWidget;
    for (unsigned int i = 0; i < logs.size() / GROUP_SIZE; ++i) {
        QWidget* tab = new QWidget;
        QHBoxLayout* logLayout = new QHBoxLayout;
        for (unsigned int j = 0; j < GROUP_SIZE; ++j) {
            QLabel* tmp = createLogisticLabel(logs[i*GROUP_SIZE+j]);
            logLayout->addWidget(tmp);
        }
        tab->setLayout(logLayout);
        chapterTabWidget->addTab(tab, QString(tr("Chapter %1")).arg(i));
    }
    mainLayout->addWidget(chapterTabWidget);

    QHBoxLayout* hourLayout = new QHBoxLayout;
    QLabel* hourLabel = new QLabel(QString(tr("Enter hours:")));
    hourSpinBox = new QSpinBox;
    hourSpinBox->setRange(0, MAX_HOURS);
    connect(hourSpinBox, SIGNAL(valueChanged(int)), this, SLOT(modifyMinuteSpinBox()));
    hourLayout->addWidget(hourLabel);
    hourLayout->addWidget(hourSpinBox);
    mainLayout->addLayout(hourLayout);

    QHBoxLayout* minuteLayout = new QHBoxLayout;
    QLabel* minuteLabel = new QLabel(QString(tr("Enter minutes:")));
    minuteSpinBox = new QSpinBox;
    minuteSpinBox->setRange(0, MAX_MINUTES);
    minuteLayout->addWidget(minuteLabel);
    minuteLayout->addWidget(minuteSpinBox);
    mainLayout->addLayout(minuteLayout);

    QHBoxLayout* resourceLayout = new QHBoxLayout;
    QLabel* resourceLabel = new QLabel(QString(tr("Choose a resource:")));
    resourceComboBox = new QComboBox;
    resourceComboBox->addItem(QString(tr("Manpower")), Logistic::Manpower);
    resourceComboBox->addItem(QString(tr("Rations")), Logistic::Rations);
    resourceComboBox->addItem(QString(tr("Ammo")), Logistic::Ammo);
    resourceComboBox->addItem(QString(tr("Parts")), Logistic::Parts);
    resourceLayout->addWidget(resourceLabel);
    resourceLayout->addWidget(resourceComboBox);
    mainLayout->addLayout(resourceLayout);

    calcAction = new QAction(tr("&Calculate"), this);
    calcAction->setShortcut(tr("Control+C"));
    calcAction->setStatusTip(tr("Calculate Logistics"));
    connect(calcAction, SIGNAL(triggered()), this, SLOT(calculate()));

    calcPushButton = new QPushButton(tr("Calculate"));
    calcPushButton->addAction(calcAction);
    connect(calcPushButton, SIGNAL(clicked()), calcAction, SLOT(trigger()));
    mainLayout->addWidget(calcPushButton);

    resultFrame = new QFrame;
    QHBoxLayout* resultLayout = new QHBoxLayout;
    for (int i = 0; i < CAPACITY; ++i) {
        resultLabels[i] = new QLabel;
        resultLayout->addWidget(resultLabels[i]);
    }
    resultFrame->setLayout(resultLayout);
    resultFrame->setVisible(false);
    resultFrame->setFrameShape(QFrame::Panel);
    mainLayout->addWidget(resultFrame);

    setLayout(mainLayout);
    layout()->setSizeConstraint(QLayout::SetFixedSize);
}

QLabel* LogisticWidget::createLogisticLabel(const Logistic& logistic, QWidget* parent)
{
    QString logString = QString(tr(logistic.toString().c_str()));
    QLabel* tmp = new QLabel(logString, parent);
    tmp->setWordWrap(true);
    return tmp;
}

void LogisticWidget::modifyMinuteSpinBox()
{
    if (hourSpinBox->value() == MAX_HOURS) {
        minuteSpinBox->setMaximum(0);
        minuteSpinBox->setValue(0);
    } else {
        minuteSpinBox->setMaximum(MAX_MINUTES);
    }
}

void LogisticWidget::calculate()
{
    if (hourSpinBox->value() == cachedHour &&
            minuteSpinBox->value() == cachedMinute &&
            resourceComboBox->currentData().toInt() == cachedResource)
        return;

    cachedHour = hourSpinBox->value();
    cachedMinute = minuteSpinBox->value();
    cachedResource = resourceComboBox->currentData().toInt();

    double time = hourSpinBox->value() + static_cast<double>(minuteSpinBox->value()) / MAX_MINUTES;
    int resource = resourceComboBox->currentData().toInt();

    std::vector<Logistic> normLogs = normalizeLogistics(logs, time);
    LogisticGroup result = knapsack(normLogs, CAPACITY, static_cast<Logistic::Value>(resource));

    for (int i = 0; i < CAPACITY; ++i) {
        resultLabels[i]->setText(result[i].toString().c_str());
    }

    resultFrame->setVisible(true);
}
