#pragma once
#include <string>
#include <vector>

class Utils;

class DataAccess {
private:
    Utils& utils; // Теперь DataAccess зависит от Utils через ссылку
public:
    DataAccess(Utils& u);

    std::vector<std::string> LoadData(const std::string& fn);
    void SaveData(const std::string& fn, const std::vector<std::string>& v);
    std::string JoinLines(const std::vector<std::string>& v);
    std::string AddLine(const std::string& fn, const std::string& line);
    std::string DelLineByID(const std::string& fn, const std::string& id);
    std::string FindLinesContaining(const std::string& fn, const std::string& substr, int field_index = 2);
    bool UpdateUserRole(const std::string& user, const std::string& newrole);
    std::string UpdateLineByID(const std::string& fn, const std::string& id, int idfield, const std::vector<int>& fields, const std::vector<std::string>& vals);
    bool RoleExists(const std::string& role);
};
