#include "credits.h"
#include "common.h"
#include "audit.h"
#include "i18n.h"
#include <fstream>
#include <mutex>

extern std::mutex file_mutex;

struct Credit {
    int id;
    int userId;
    double amount;
    int duration;
    std::string status;
    std::string ctype;
    int term;
};

static std::vector<Credit> readCredits() {
    std::lock_guard<std::mutex> lock(file_mutex);
    std::vector<Credit> credits;
    std::ifstream f("credits.txt");
    std::string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        auto t = split(line, ';');
        if (t.size() < 6) continue;
        Credit c;
        c.id = std::stoi(t[0]);
        c.userId = std::stoi(t[1]);
        c.amount = std::stod(t[2]);
        c.duration = std::stoi(t[3]);
        c.status = t[4];
        c.ctype = t[5];
        c.term = (t.size() > 6) ? std::stoi(t[6]) : 0;
        credits.push_back(c);
    }
    return credits;
}

static void writeCredits(const std::vector<Credit>& credits) {
    std::lock_guard<std::mutex> lock(file_mutex);
    std::ofstream f("credits.txt", std::ios::trunc);
    for (auto& c : credits) {
        f << c.id << ";" << c.userId << ";" << c.amount << ";" << c.duration << ";" << c.status << ";" << c.ctype << ";" << c.term << "\n";
    }
}

std::string requestCredit(int userId, double amount, int duration) {
    auto credits = readCredits();
    int newId = 1; for (auto& c : credits)if (c.id >= newId)newId = c.id + 1;
    Credit cr; cr.id = newId; cr.userId = userId; cr.amount = amount; cr.duration = duration; cr.status = "requested"; cr.ctype = "default"; cr.term = duration;
    credits.push_back(cr);
    writeCredits(credits);
    auditLog("User " + std::to_string(userId) + " requested credit " + std::to_string(newId));
    return "SUCCESS;Credit requested with ID: " + std::to_string(newId);
}

std::string approveCredit(int creditId) {
    auto credits = readCredits();
    for (auto& c : credits) {
        if (c.id == creditId) { c.status = "approved"; writeCredits(credits); auditLog("Credit " + std::to_string(creditId) + " approved"); return "SUCCESS;Credit approved."; }
    }
    return "ERROR;" + tr("ERROR_GENERIC");
}

std::string setCreditType(int creditId, const std::string& type) {
    auto credits = readCredits();
    for (auto& c : credits) {
        if (c.id == creditId) { c.ctype = type; writeCredits(credits); auditLog("Credit " + std::to_string(creditId) + " type set " + type); return "SUCCESS;Credit type set."; }
    }
    return "ERROR;" + tr("ERROR_GENERIC");
}

std::string setCreditTerm(int creditId, int term) {
    auto credits = readCredits();
    for (auto& c : credits) {
        if (c.id == creditId) { c.term = term; writeCredits(credits); auditLog("Credit " + std::to_string(creditId) + " term set " + std::to_string(term)); return "SUCCESS;Credit term set."; }
    }
    return "ERROR;" + tr("ERROR_GENERIC");
}
