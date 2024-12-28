#include <mutex>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>

struct Schedule {
    long scheduleID;
    long employeeID;
    long positionID;
    std::string scheduleType;
    std::string shiftDetails;
    std::string scheduleDates;
};

class ScheduleManager {
private:
    std::unordered_map<long, Schedule> schedulesMap;
    std::vector<Schedule> schedules;
    std::string scheduleFileName;
    std::mutex mtx;

    void loadSchedulesFromFile();  // Для загрузки данных из файла
    void saveSchedulesToFile();    // Для сохранения данных в файл
public:
    ScheduleManager(const std::string& filename);
    Schedule getSchedule(long scheduleID) const;
    bool addSchedule(const Schedule& schedule);
    bool updateSchedule(const Schedule& schedule);
    bool deleteSchedule(long scheduleID);
    std::vector<Schedule> getAllSchedules() const;
};