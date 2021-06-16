#include <QApplication>
#include <assert.h>

#include "knapsack.h"
#include "logistic_widget.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    LogisticWidget* m = new LogisticWidget;
    m->show();
    return app.exec();
}
