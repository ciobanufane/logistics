#ifndef LOGISTICWIDGET_H
#define LOGISTICWIDGET_H

#include <QWidget>
#include <vector>
#include "knapsack.h"

class QLabel;
class QString;
class QPushButton;
class QSpinBox;
class QComboBox;
class QTabWidget;
class QFrame;

class LogisticWidget : public QWidget
{
    Q_OBJECT
public:
    LogisticWidget(QWidget* parent = nullptr);
private slots:
    void modifyMinuteSpinBox();
    void calculate();
private:
    enum { GROUP_SIZE = 4, CAPACITY = 4, MAX_HOURS = 24, MAX_MINUTES = 60 };

    QLabel* createLogisticLabel(const Logistic& logistic, QWidget* parent = nullptr);
    void createLogisticLayout();

    QTabWidget* chapterTabWidget;

    QPushButton* calcPushButton;
    QSpinBox* hourSpinBox;
    QSpinBox* minuteSpinBox;
    QComboBox* resourceComboBox;
    QLabel* resultLabels[CAPACITY];
    QFrame* resultFrame;

    QAction* calcAction;

    int cachedHour;
    int cachedMinute;
    int cachedResource;
    std::vector<Logistic> logs;
};


#endif
