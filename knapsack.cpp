#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>

#include "knapsack.h"

std::vector<std::string> NONQT::split(const std::string& line, char delimiter)
{
    std::vector<std::string> tokens;
	std::string substring{ line };
	auto index{ substring.find(delimiter) };

	if (index == std::string::npos) {
		tokens.push_back(substring);
	}

	while (index != std::string::npos) {
		tokens.push_back(substring.substr(0, index));
		substring = substring.substr(index+1);
		index = substring.find(delimiter);
	}
	tokens.push_back(substring);

	return tokens;
}

void NONQT::skipBOM(std::ifstream& in) {
	char test[3]{ 0 };
	in.read(test, 3);
	if ((unsigned char)test[0] == 0xEF &&
        (unsigned char)test[1] == 0xBB &&
        (unsigned char)test[2] == 0xBF) {
		return;
	}
	in.seekg(0);
}

std::vector<Logistic> NONQT::readLogisticCSV(const std::string& filename)
{
    std::string line;
    std::ifstream file(filename);
    std::vector<Logistic> result;

    if (file.is_open()) {
        NONQT::skipBOM(file);

        if (!std::getline(file, line)) {
            file.close();
            return result;
        }

        std::vector<std::string> columns;
        while (std::getline(file, line)) {
            columns = NONQT::split(line, ',');
            try {
                result.push_back({
                    columns[Logistic::Chapter],
                    static_cast<double>(std::stoi(columns[Logistic::Manpower])),
                    static_cast<double>(std::stoi(columns[Logistic::Ammo])),
                    static_cast<double>(std::stoi(columns[Logistic::Rations])),
                    static_cast<double>(std::stoi(columns[Logistic::Parts])),
                    std::stoi(columns[Logistic::Minutes])
                });
            } catch (...) {
                file.close();
                result.clear();
                return result;
            }
        }

        file.close();
    }

    return result;
}


void skipBOM(QFile* file)
{
    char buf[3];
    if (file->peek(buf, sizeof(buf)) == sizeof(buf))
        if ((unsigned char)buf[0] == 0xEF &&
            (unsigned char)buf[1] == 0xBB &&
            (unsigned char)buf[2] == 0xBF) {
                file->read(3);
            }
}

std::vector<Logistic> readLogisticCSV(const QString& filename)
{
    QFile file(filename);
    std::vector<Logistic> result;

    if (!file.open(QIODevice::ReadOnly))
        return result;

    skipBOM(&file);

    QString line;
    QTextStream stream(&file);
    QStringList columns;

    stream.readLineInto(&line);

    while (stream.readLineInto(&line)) {
        columns = line.split(',');

        bool manpower, rations, ammo, parts, minutes;
        columns[Logistic::Manpower].toDouble(&manpower);
        columns[Logistic::Rations].toDouble(&rations);
        columns[Logistic::Ammo].toDouble(&ammo);
        columns[Logistic::Parts].toDouble(&parts);
        columns[Logistic::Minutes].toInt(&minutes);

        if (!(manpower && rations && ammo && parts && minutes)) {
            result.clear();
            return result;
        }

        result.push_back({
            columns[Logistic::Chapter].toStdString(),
            columns[Logistic::Manpower].toDouble(),
            columns[Logistic::Rations].toDouble(),
            columns[Logistic::Ammo].toDouble(),
            columns[Logistic::Parts].toDouble(),
            columns[Logistic::Minutes].toInt()
        });
    }
    file.close();
    return result;
}

std::vector<Logistic> normalizeLogistics(const std::vector<Logistic>& logs, double hours)
{
    if (hours == 0)
        return logs;

    std::vector<Logistic> normalizeLogs;
    double minutes {hours*60};
    double newHours;

    for (const Logistic& log : logs) {
        /*
            round logistic time to the nearest multiple of "hours"
            IE, hours = 4
                3 hrs -> 4
                7 hrs -> 8
                9 hrs -> 12

            (n+h)/h returns ith multiple of "hours"
            cast to int to floor the number
        */
        newHours = hours * static_cast<int>(((log.minutes+minutes)/minutes));
        normalizeLogs.push_back({
            log.chapter,
            log.manpower/newHours,
            log.ammo    /newHours,
            log.rations /newHours,
            log.parts   /newHours,
            static_cast<int>(newHours*60)
        });
    }

    return normalizeLogs;
}

LogisticGroup knapsack(const std::vector<Logistic>& logs, int capacity, Logistic::Value resource)
{
    if (capacity <= 0 || logs.size() == 0) {
        return LogisticGroup();
    }

    std::vector<LogisticGroup> knapsack(capacity+1);
    for (int index = 0; index < capacity + 1; ++index) {
        knapsack[index] = LogisticGroup();
    }

    for (const Logistic& log : logs) {
        for (int index = capacity; index > 0; --index) {
            knapsack[index] = maxLogGroup(knapsack[index-1]+log, knapsack[index], resource);
        }
    }

    return knapsack[capacity];
}

const LogisticGroup& maxLogGroup(const LogisticGroup& logGroup1, const LogisticGroup& logGroup2, Logistic::Value resource)
{
    return logGroup1.getResource(resource)>=logGroup2.getResource(resource)?logGroup1:logGroup2;
}
