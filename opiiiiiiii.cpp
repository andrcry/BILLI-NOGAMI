#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <windows.h>
using namespace std;

// ---------------------------- СТРУКТУРИ ----------------------------
struct User {
    string username;
    string password;
    string role; // admin або customer
};

struct Book {
    int id;
    string title;
    string author;
    string description;
    double price;
    double rating;
};

// ---------------------------- КОРИСТУВАЧІ ----------------------------
void initializeUsers(vector<User>& users) {
    users.push_back({ "admin", "admin123", "admin" });
    users.push_back({ "user", "111", "customer" });
    users.push_back({ "student", "knu", "customer" });
}

User* performLogin(vector<User>& userDb) {
    string inputUsername, inputPassword;
    const int maxAttempts = 3;

    cout << "\n=== ВХІД У СИСТЕМУ ===" << endl;

    for (int attempt = 1; attempt <= maxAttempts; attempt++) {
        cout << "Спроба " << attempt << " з " << maxAttempts << endl;
        cout << "Логін: ";
        cin >> inputUsername;
        cout << "Пароль: ";
        cin >> inputPassword;

        for (auto& user : userDb) {
            if (user.username == inputUsername && user.password == inputPassword) {
                cout << "\n[Успіх] Вітаю, " << user.username << "!" << endl;
                return &user;
            }
        }
        cout << "[Помилка] Невірний логін або пароль.\n" << endl;
    }

    cout << "[Відмова] Перевищено кількість спроб." << endl;
    return nullptr;
}

// ---------------------------- КНИГИ ----------------------------
void initializeBooks(vector<Book>& db) {
    db.push_back({ 1, "Кобзар", "Тарас Шевченко", "Поезія", 350, 4.9 });
    db.push_back({ 2, "Захар Беркут", "Іван Франко", "Історична повість", 200, 4.7 });
    db.push_back({ 3, "Енеїда", "І. Котляревський", "Поема", 280, 4.8 });
}

void displayAllBooks(const vector<Book>& db) {
    cout << "\n--- КАТАЛОГ КНИГ ---" << endl;
    if (db.empty()) {
        cout << "Каталог порожній." << endl;
        return;
    }

    cout << left << setw(5) << "ID" << setw(25) << "Назва" << setw(20) << "Автор" << "Ціна" << endl;
    cout << "------------------------------------------------------------" << endl;

    for (const auto& b : db)
        cout << setw(5) << b.id << setw(25) << b.title << setw(20) << b.author << b.price << " грн\n";
}

void adminAddBook(vector<Book>& db) {
    Book newBook;

    cout << "\n[ADMIN] Додати книгу" << endl;

    int maxId = 0;
    for (const auto& b : db) if (b.id > maxId) maxId = b.id;
    newBook.id = maxId + 1;

    cout << "Назва: "; cin >> newBook.title;
    cout << "Автор: "; cin >> newBook.author;
    cout << "Опис: "; cin >> newBook.description;
    cout << "Ціна: "; cin >> newBook.price;
    cout << "Рейтинг: "; cin >> newBook.rating;

    db.push_back(newBook);
    cout << "[OK] Книга додана." << endl;
}

void adminDeleteBook(vector<Book>& db) {
    int id;
    displayAllBooks(db);

    cout << "\nВведіть ID для видалення: ";
    cin >> id;

    auto it = remove_if(db.begin(), db.end(), [id](const Book& b) { return b.id == id; });
    if (it != db.end()) {
        db.erase(it, db.end());
        cout << "[OK] Книга видалена." << endl;
    }
    else cout << "[Помилка] ID не знайдено." << endl;
}

// ---------------------------- КОШИК ----------------------------
void addToCart(const vector<Book>& db, vector<Book>& cart) {
    int id;
    cout << "Введіть ID книги: ";
    cin >> id;

    for (const auto& b : db) {
        if (b.id == id) {
            cart.push_back(b);
            cout << "[Кошик] Додано: " << b.title << endl;
            return;
        }
    }
    cout << "Книгу не знайдено." << endl;
}

void runCatalogModule(vector<Book>& db, vector<Book>& cart, const User* user) {
    int choice = 0;

    do {
        cout << "\n--- МЕНЮ КАТАЛОГУ ---\n";
        cout << "1. Показати всі книги\n";
        cout << "2. Додати книгу в кошик\n";

        if (user->role == "admin") {
            cout << "8. [ADMIN] Додати книгу\n";
            cout << "9. [ADMIN] Видалити книгу\n";
        }

        cout << "0. Назад\nВаш вибір: ";
        cin >> choice;

        switch (choice) {
        case 1: displayAllBooks(db); break;
        case 2: addToCart(db, cart); break;

        case 8:
            if (user->role == "admin") adminAddBook(db);
            else cout << "Доступ заборонено.\n";
            break;

        case 9:
            if (user->role == "admin") adminDeleteBook(db);
            else cout << "Доступ заборонено.\n";
            break;

        case 0: break;

        default: cout << "Невірний вибір.\n"; break;
        }

    } while (choice != 0);
}

// ---------------------------- ОПЛАТА ----------------------------

// Перевірка що введено лише цифри
bool isDigitsOnly(const string& s) {
    for (char c : s)
        if (!isdigit(c)) return false;
    return true;
}

bool processPaymentLogic(const string& method, double amount) {
    if (amount <= 0) return false;

    cout << "Обробка платежу на " << amount << " грн..." << endl;

    if (method == "card") {
        string card;

        while (true) {
            cout << "Введіть номер картки (16 цифр): ";
            cin >> card;

            if (card.length() == 16 && isDigitsOnly(card)) break;

            cout << "[Помилка] Можна вводити ТІЛЬКИ цифри (16 символів).\n";
        }
    }

    Sleep(1000);
    return true;
}

void runPaymentModule(vector<Book>& cart) {
    if (cart.empty()) {
        cout << "Кошик порожній." << endl;
        return;
    }

    double total = 0;
    cout << "\n--- КАСА ---\nВаше замовлення:\n";

    for (const auto& b : cart) {
        cout << "- " << b.title << " (" << b.price << " грн)\n";
        total += b.price;
    }

    cout << "Разом: " << total << " грн\n";

    string method;
    cout << "Оберіть спосіб оплати (card/cash): ";
    cin >> method;

    string confirm;
    cout << "Підтвердити оплату (y/n): ";
    cin >> confirm;

    if (confirm != "y" && confirm != "Y") {
        cout << "Оплату скасовано.\n";
        return;
    }

    if (processPaymentLogic(method, total)) {
        cout << "[УСПІХ] Оплата проведена!\n";
        cart.clear();
    }
    else {
        cout << "[ПОМИЛКА] Транзакція відхилена.\n";
    }
}

// ---------------------------- MAIN ----------------------------
int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    vector<User> users;
    vector<Book> books;
    vector<Book> cart;

    initializeUsers(users);
    initializeBooks(books);

    User* currentUser = performLogin(users);
    if (!currentUser) return 0;

    int choice;
    do {
        cout << "\n===== ГОЛОВНЕ МЕНЮ =====" << endl;
        cout << "1. Каталог книг\n";
        cout << "2. Оплата (товарів у кошику: " << cart.size() << ")\n";
        cout << "3. Вихід\nВаш вибір: ";
        cin >> choice;

        switch (choice) {
        case 1: runCatalogModule(books, cart, currentUser); break;
        case 2: runPaymentModule(cart); break;
        case 3: cout << "До побачення!\n"; break;
        default: cout << "Помилка вводу.\n"; break;
        }

    } while (choice != 3);

    return 0;
}
