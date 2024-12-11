#pragma once
#pragma once
#include <string>
enum class Language { EN, RU, BE };
void setLanguage(Language lang);
Language getLanguage();
std::string tr(const std::string& key);
