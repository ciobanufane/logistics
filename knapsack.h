#ifndef KNAPSACK_H
#define KNAPSACK

#include <vector>
#include <string>
#include <fstream>

#include "logistic.h"

class QFile;
class QString;

struct Logistic;
class LogisticGroup;

/*
 *  functions for reference if GUI is not used
 */
namespace NONQT {
    std::vector<Logistic> readLogisticCSV(const std::string& filename);
    std::vector<std::string> split(const std::string& line, char delimiter);
    void skipBOM(std::ifstream& in);
}

/*
 *  skip BOM if it exists in text files (mostly for files created by Powershell)
 */
void skipBOM(QFile* file);
/*
 *  reads logistic CSV file and converts to a vector of Logistics
 *  uses Qt classes in order to access Qt resource file
 *  call before normalizeLogistics(...)
 */
std::vector<Logistic> readLogisticCSV(const QString& filename);
/*
 *  copies vector of Logistics and adjust their rates accordingly to hours
 *  call after readLogisticCSV(...) but is not necessary
 */
std::vector<Logistic> normalizeLogistics(const std::vector<Logistic>& logs, double hours);
/*
 *  calculates the best combination of Logistics depending on resource
 */
LogisticGroup knapsack(const std::vector<Logistic>& logs, int capacity, Logistic::Value resource);
/*
 *  returns whichever LogisticGroup has more of a resource
 *  helper function used in knapsack(...)
 */
const LogisticGroup& maxLogGroup(const LogisticGroup& logGroup1, const LogisticGroup& logGroup2, Logistic::Value resource);


#endif
