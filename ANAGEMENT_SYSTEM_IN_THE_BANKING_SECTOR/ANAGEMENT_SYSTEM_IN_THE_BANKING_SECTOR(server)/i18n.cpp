#include "i18n.h"
#include <unordered_map>

static Language currentLanguage = Language::EN;

struct Translation {
    std::unordered_map<std::string, std::string> en;
    std::unordered_map<std::string, std::string> ru;
    std::unordered_map<std::string, std::string> be;
};

static Translation T = {
    // Английский
    {
        {"WELCOME", "Welcome"},
        {"USERNAME_PROMPT", "Username"},
        {"PASSWORD_PROMPT", "Password"},
        {"BLOCK_SELF_ERROR", "You cannot block your own account"},
        {"MENU_USER", "User Menu"},
        {"MENU_WORKER", "Worker Menu"},
        {"DEPOSIT", "Deposit"},
        {"WITHDRAW", "Withdraw"},
        {"TRANSFER", "Transfer"},
        {"VIEW_BALANCE", "View Balance"},
        {"VIEW_HISTORY", "View Transaction History"},
        {"CREATE_ACCOUNT", "Create Account"},
        {"DELETE_ACCOUNT", "Delete Account"},
        {"BLOCK_ACCOUNT", "Block Account"},
        {"UNBLOCK_ACCOUNT", "Unblock Account"},
        {"APPLY_INTEREST", "Apply Interest to Accounts"},
        {"SEARCH_BALANCE", "Search Accounts by Balance"},
        {"GENERATE_REPORT", "Generate Report"},
        {"CHOOSE_STATUS", "Choose user status"},
        {"ACTIVE", "Active"},
        {"BLOCKED", "Blocked"},
        {"LOGIN_FAILED", "Login failed. Try again."},
        {"LOGIN_SUCCESS", "Login successful!"},
        {"ATTEMPTS_LEFT", "attempts left"},
        {"TOO_MANY_ATTEMPTS", "Too many failed attempts. Exiting..."},
        {"LOGOUT", "Logout"},
        {"ERROR_GENERIC", "An error occurred"},
        {"USER_BLOCKED", "User successfully blocked"},
        {"NOT_AUTHORIZED", "Not authorized"},
        {"ROLE_UNKNOWN", "Unknown role"}
    },
    // Русский
    {
        {"WELCOME", u8"Добро пожаловать"},
        {"USERNAME_PROMPT", u8"Имя пользователя"},
        {"PASSWORD_PROMPT", u8"Пароль"},
        {"BLOCK_SELF_ERROR", u8"Вы не можете заблокировать свой собственный аккаунт"},
        {"MENU_USER", u8"Меню пользователя"},
        {"MENU_WORKER", u8"Меню работника"},
        {"DEPOSIT", u8"Пополнить счет"},
        {"WITHDRAW", u8"Снять средства"},
        {"TRANSFER", u8"Перевести средства"},
        {"VIEW_BALANCE", u8"Просмотреть баланс"},
        {"VIEW_HISTORY", u8"Просмотреть историю транзакций"},
        {"CREATE_ACCOUNT", u8"Создать аккаунт"},
        {"DELETE_ACCOUNT", u8"Удалить аккаунт"},
        {"BLOCK_ACCOUNT", u8"Заблокировать аккаунт"},
        {"UNBLOCK_ACCOUNT", u8"Разблокировать аккаунт"},
        {"APPLY_INTEREST", u8"Начислить проценты"},
        {"SEARCH_BALANCE", u8"Поиск аккаунтов по балансу"},
        {"GENERATE_REPORT", u8"Сформировать отчет"},
        {"CHOOSE_STATUS", u8"Выберите статус пользователя"},
        {"ACTIVE", u8"Активен"},
        {"BLOCKED", u8"Заблокирован"},
        {"LOGIN_FAILED", u8"Ошибка входа. Попробуйте снова."},
        {"LOGIN_SUCCESS", u8"Успешный вход!"},
        {"ATTEMPTS_LEFT", u8"попыток осталось"},
        {"TOO_MANY_ATTEMPTS", u8"Слишком много неудачных попыток. Выход..."},
        {"LOGOUT", u8"Выход"},
        {"ERROR_GENERIC", u8"Произошла ошибка"},
        {"USER_BLOCKED", u8"Пользователь успешно заблокирован"},
        {"NOT_AUTHORIZED", u8"Не авторизированы"},
        {"ROLE_UNKNOWN", u8"Роль не определена"}
    },
    // Белорусский
    {
        {"WELCOME", u8"Сардэчна запрашаем"},
        {"USERNAME_PROMPT", u8"Імя карыстальніка"},
        {"PASSWORD_PROMPT", u8"Пароль"},
        {"BLOCK_SELF_ERROR", u8"Вы не можаце блакаваць уласны рахунак"},
        {"MENU_USER", u8"Меню карыстальніка"},
        {"MENU_WORKER", u8"Меню работніка"},
        {"DEPOSIT", u8"Папоўніць рахунак"},
        {"WITHDRAW", u8"Зняць сродкі"},
        {"TRANSFER", u8"Перакласці сродкі"},
        {"VIEW_BALANCE", u8"Праглядзець баланс"},
        {"VIEW_HISTORY", u8"Праглядзець гісторыю транзакцый"},
        {"CREATE_ACCOUNT", u8"Стварыць рахунак"},
        {"DELETE_ACCOUNT", u8"Выдаліць рахунак"},
        {"BLOCK_ACCOUNT", u8"Заблакаваць рахунак"},
        {"UNBLOCK_ACCOUNT", u8"Разблакаваць рахунак"},
        {"APPLY_INTEREST", u8"Налічыць працэнты"},
        {"SEARCH_BALANCE", u8"Пошук рахункаў па балансе"},
        {"GENERATE_REPORT", u8"Сфармаваць справаздачу"},
        {"CHOOSE_STATUS", u8"Абярыце статус карыстальніка"},
        {"ACTIVE", u8"Актыўны"},
        {"BLOCKED", u8"Заблакаваны"},
        {"LOGIN_FAILED", u8"Памылка ўваходу. Паспрабуйце зноў."},
        {"LOGIN_SUCCESS", u8"Паспяховы ўваход!"},
        {"ATTEMPTS_LEFT", u8"спроб засталося"},
        {"TOO_MANY_ATTEMPTS", u8"Занадта шмат няўдалых спробаў. Выхад..."},
        {"LOGOUT", u8"Выхад"},
        {"ERROR_GENERIC", u8"Адбылася памылка"},
        {"USER_BLOCKED", u8"Карыстальнік паспяхова заблакаваны"},
        {"NOT_AUTHORIZED", u8"Не аўтарызаваны"},
        {"ROLE_UNKNOWN", u8"Роля не вызначана"}
    }
};



void setLanguage(Language lang) { currentLanguage = lang; }
Language getLanguage() { return currentLanguage; }
std::string tr(const std::string& key) {
    switch (currentLanguage) {
    case Language::EN:if (T.en.count(key))return T.en.at(key); break;
    case Language::RU:if (T.ru.count(key))return T.ru.at(key); break;
    case Language::BE:if (T.be.count(key))return T.be.at(key); break;
    }
    return key;
}
