#pragma once
#ifndef ROLE_VALIDATOR_H
#define ROLE_VALIDATOR_H

#include <string>
#include <unordered_map>
#include <vector>

class RoleValidator {
private:
    std::unordered_map<std::string, std::vector<std::string>> rolePermissions; // ���� � ����������� �������

    void initializePermissions(); // ������������� ���������� ��� �����

public:
    RoleValidator();
    bool isAllowed(const std::string& role, const std::string& command); // �������� ����������
};

#endif // ROLE_VALIDATOR_H
