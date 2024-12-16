#include "data_access.h"
#include "utils.h"
#include <fstream>

DataAccess::DataAccess(Utils& u)
    : utils(u)
{}

std::vector<std::string> DataAccess::LoadData(const std::string& fn) {
    std::vector<std::string> v;
    if (!utils.FileExists("data/" + fn)) {
        utils.Log("ERROR", "Файл не найден: data/" + fn);
        return v;
    }
    std::ifstream f("data/" + fn);
    std::string line;
    while (std::getline(f, line)) v.push_back(utils.Trimr(line));
    return v;
}

void DataAccess::SaveData(const std::string& fn, const std::vector<std::string>& v) {
    std::ofstream of("data/" + fn);
    for (auto& s : v)of << s << "\n";
}

std::string DataAccess::JoinLines(const std::vector<std::string>& v) {
    std::string r;
    for (size_t i = 0; i < v.size(); i++) {
        r += v[i];
        if (i < v.size() - 1)r += "\n";
    }
    return r;
}

std::string DataAccess::AddLine(const std::string& fn, const std::string& line) {
    if (!utils.FileExists("data/" + fn)) {
        utils.Log("ERROR", "AddLine: файл не найден data/" + fn);
        return "ERR";
    }
    if (line.empty()) {
        utils.Log("WARN", "AddLine: пустая строка для " + fn);
        return "ERR";
    }
    auto v = LoadData(fn);
    v.push_back(line);
    SaveData(fn, v);
    utils.Log("INFO", "Добавлена запись в " + fn + ": " + line);
    return "OK";
}

std::string DataAccess::DelLineByID(const std::string& fn, const std::string& id) {
    if (!utils.FileExists("data/" + fn)) {
        utils.Log("ERROR", "DelLineByID: файл не найден data/" + fn);
        return "ERR";
    }
    if (!utils.ValidateID(id)) {
        utils.Log("WARN", "DelLineByID: некорректный ID " + id + " для " + fn);
        return "ERR";
    }
    auto v = LoadData(fn);
    std::vector<std::string> nv;
    bool removed = false;
    for (auto& x : v) {
        auto p = x.find(';');
        if (p == std::string::npos) {
            nv.push_back(x);
            continue;
        }
        if (x.substr(0, p) != id) nv.push_back(x);
        else removed = true;
    }
    SaveData(fn, nv);
    if (removed) utils.Log("INFO", "Запись с ID=" + id + " удалена из " + fn);
    else utils.Log("WARN", "Запись с ID=" + id + " не найдена в " + fn);
    return removed ? "OK" : "ERR";
}

std::string DataAccess::FindLinesContaining(const std::string& fn, const std::string& substr, int field_index) {
    auto v = LoadData(fn);
    std::string r;
    for (auto& x : v) {
        auto parts = utils.Split(x, ';');
        if ((int)parts.size() > field_index) {
            if (parts[field_index].find(substr) != std::string::npos)r += x + "\n";
        }
    }
    return r;
}

bool DataAccess::UpdateUserRole(const std::string& user, const std::string& newrole) {
    auto us = LoadData("users.txt");
    bool changed = false;
    for (auto& x : us) {
        auto parts = utils.Split(x, ';');
        if (parts.size() < 3)continue;
        if (parts[0] == user) {
            parts[2] = newrole;
            x = parts[0] + ";" + parts[1] + ";" + parts[2];
            changed = true;
            break;
        }
    }
    if (changed) {
        SaveData("users.txt", us);
        utils.Log("INFO", "Роль пользователя " + user + " изменена на " + newrole);
    }
    else {
        utils.Log("WARN", "Пользователь " + user + " не найден для смены роли");
    }
    return changed;
}

std::string DataAccess::UpdateLineByID(const std::string& fn, const std::string& id, int idfield, const std::vector<int>& fields, const std::vector<std::string>& vals) {
    if (!utils.FileExists("data/" + fn)) {
        utils.Log("ERROR", "UpdateLineByID: файл не найден data/" + fn);
        return "ERR";
    }
    auto v = LoadData(fn);
    bool ok = false;
    for (auto& x : v) {
        auto parts = utils.Split(x, ';');
        if ((int)parts.size() <= idfield)continue;
        if (parts[idfield] == id) {
            for (size_t i = 0; i < fields.size() && i < vals.size(); i++) {
                int fi = fields[i];
                if (fi < (int)parts.size()) parts[fi] = vals[i];
            }
            x = "";
            for (size_t i = 0; i < parts.size(); i++) {
                x += parts[i];
                if (i < parts.size() - 1)x += ";";
            }
            ok = true;
            break;
        }
    }
    if (ok) {
        SaveData(fn, v);
        utils.Log("INFO", "Запись с id=" + id + " обновлена в " + fn);
    }
    else {
        utils.Log("WARN", "Запись с id=" + id + " не найдена в " + fn);
    }
    return ok ? "OK" : "ERR";
}

bool DataAccess::RoleExists(const std::string& role) {
    auto r = LoadData("roles.txt");
    for (auto& x : r) if (x == role)return true;
    return false;
}
