#include <iostream>
#include <string>
#include <vector>
#include <iomanip>    // Для форматування виводу
#include <algorithm>  // Для пошуку
#include <windows.h>  // Для кодування

using namespace std;

struct User {
    string username;
    string password;
    string role; // "admin" або "customer"
};

struct Book {
    int id;
    string title;
    string author;
    string description;
    double price;
    double rating;
};

//        МОДУЛЬ 1: АВТОРИЗАЦІЯ

void initializeUsers(vector<User>& users) {
    // Адміністратор (може додавати/видаляти книги)
    users.push_back({ "admin", "admin123", "admin" });
    // Звичайні покупці
    users.push_back({ "user", "111", "customer" });
    users.push_back({ "student", "knu", "customer" });
}

// Повертає вказівник на користувача або nullptr, якщо вхід невдалий
User* performLogin(vector<User>& userDb) {
    string inputUsername;
    string inputPassword;
    const int maxAttempts = 3;

    cout << "\n=== ВХІД У СИСТЕМУ ===" << endl;

    for (int attempt = 1; attempt <= maxAttempts; ++attempt) {
        cout << "Спроба " << attempt << " з " << maxAttempts << endl;
        cout << "Логін: ";
        cin >> inputUsername;
        cout << "Пароль: ";
        cin >> inputPassword;

        for (auto& user : userDb) {
            if (user.username == inputUsername && user.password == inputPassword) {
                cout << "\n[Успіх] Ласкаво просимо, " << user.username << "!" << endl;
                cout << "Ваша роль: " << (user.role == "admin" ? "Адміністратор" : "Покупець") << endl;
                return &user;
            }
        }
        cout << "[Помилка] Невірний логін або пароль.\n" << endl;
    }

    cout << "\n[Відмова] Перевищено ліміт спроб." << endl;
    return nullptr;
}

//        МОДУЛЬ 2: КАТАЛОГ ТА КОШИК

void initializeBooks(vector<Book>& db) {
    db.push_back({ 1, "Кобзар", "Тарас Шевченко", "Поезія", 350.00, 4.9 });
    db.push_back({ 2, "Захар Беркут", "Іван Франко", "Історична повість", 200.00, 4.7 });
    db.push_back({ 3, "Енеїда", "І. Котляревський", "Поема", 280.00, 4.8 });
}

void displayAllBooks(const vector<Book>& db) {
    cout << "\n--- АКТУАЛЬНИЙ КАТАЛОГ ---" << endl;
    if (db.empty()) {
        cout << "Каталог порожній." << endl;
        return;
    }
    cout << left << setw(5) << "ID" << setw(25) << "Назва" << setw(20) << "Автор" << "Ціна" << endl;
    cout << "------------------------------------------------------------" << endl;
    for (const auto& book : db) {
        cout << left << setw(5) << book.id << setw(25) << book.title << setw(20) << book.author << book.price << " грн" << endl;
    }
}

// --- ФУНКЦІЇ АДМІНІСТРАТОРА ---

void adminAddBook(vector<Book>& db) {
    Book newBook;
    cout << "\n[ADMIN] Додавання нової книги:" << endl;

    // Автогенерація ID
    int maxId = 0;
    for (const auto& b : db) if (b.id > maxId) maxId = b.id;
    newBook.id = maxId + 1;

    cout << "Введіть назву (без пробілів або з _): "; cin >> newBook.title;
    cout << "Введіть автора (прізвище): "; cin >> newBook.author;
    cout << "Введіть опис: "; cin >> newBook.description;
    cout << "Введіть ціну: "; cin >> newBook.price;
    cout << "Введіть рейтинг (0-5): "; cin >> newBook.rating;

    db.push_back(newBook);
    cout << "[Успіх] Книгу додано під ID " << newBook.id << endl;
}

void adminDeleteBook(vector<Book>& db) {
    int id;
    displayAllBooks(db);
    cout << "\n[ADMIN] Введіть ID книги для видалення: ";
    cin >> id;

    auto it = remove_if(db.begin(), db.end(), [id](const Book& b) { return b.id == id; });

    if (it != db.end()) {
        db.erase(it, db.end());
        cout << "[Успіх] Книгу видалено." << endl;
    }
    else {
        cout << "[Помилка] ID не знайдено." << endl;
    }
}

// --- ФУНКЦІЇ КОШИКА ---

void addToCart(const vector<Book>& db, vector<Book>& cart) {
    int id;
    cout << "\nВведіть ID книги, щоб додати в кошик: ";
    cin >> id;

    bool found = false;
    for (const auto& book : db) {
        if (book.id == id) {
            cart.push_back(book);
            cout << "[Кошик] \"" << book.title << "\" додано!" << endl;
            found = true;
            break;
        }
    }
    if (!found) cout << "Книгу з таким ID не знайдено." << endl;
}

void runCatalogModule(vector<Book>& db, vector<Book>& cart, const User* user) {
    int choice = 0;
    do {
        cout << "\n--- МЕНЮ КАТАЛОГУ ---" << endl;
        cout << "1. Показати всі книги" << endl;
        cout << "2. Додати книгу в кошик (Купити)" << endl;

        // Додаткові опції для Адміна
        if (user->role == "admin") {
            cout << "-----------------------" << endl;
            cout << "8. [ADMIN] Додати нову книгу" << endl;
            cout << "9. [ADMIN] Видалити книгу" << endl;
            cout << "-----------------------" << endl;
        }

        cout << "0. Повернутися в головне меню" << endl;
        cout << "Ваш вибір: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear(); cin.ignore(1000, '\n'); continue;
        }

        switch (choice) {
        case 1: displayAllBooks(db); break;
        case 2: addToCart(db, cart); break;
        case 8:
            if (user->role == "admin") adminAddBook(db);
            else cout << "Доступ заборонено." << endl;
            break;
        case 9:
            if (user->role == "admin") adminDeleteBook(db);
            else cout << "Доступ заборонено." << endl;
            break;
        case 0: cout << "Вихід з каталогу..." << endl; break;
        default: cout << "Невірний вибір." << endl;
        }
    } while (choice != 0);
}

//          МОДУЛЬ 3: ОПЛАТА

bool processPaymentLogic(const string& method, double amount) {
    if (amount <= 0) return false;
    cout << "Обробка транзакції на суму " << amount << " грн..." << endl;

    if (method == "card") {
        string card;
        cout << "Введіть номер картки (16 цифр): ";
        cin >> card;
        if (card.length() != 16) {
            cout << "Помилка: номер картки має містити 16 цифр." << endl;
            return false;
        }
    }
    Sleep(1000); // Імітація затримки банківської операції
    return true;
}

void runPaymentModule(vector<Book>& cart) {
    cout << "\n--- КАСА (CHECKOUT) ---" << endl;

    if (cart.empty()) {
        cout << "Ваш кошик порожній. Оберіть книги в каталозі." << endl;
        return;
    }

    // 1. Розрахунок суми
    double totalAmount = 0;
    cout << "Ваше замовлення:" << endl;
    for (const auto& item : cart) {
        cout << "- " << item.title << " (" << item.price << " грн)" << endl;
        totalAmount += item.price;
    }
    cout << "-----------------------" << endl;
    cout << "ДО СПЛАТИ: " << totalAmount << " грн" << endl;

    // 2. Вибір методу
    string method, confirm;
    cout << "\nОберіть спосіб (card/cash): ";
    cin >> method;

    if (method != "card" && method != "cash") {
        cout << "Невідомий спосіб оплати." << endl;
        return;
    }

    cout << "Підтвердити оплату " << totalAmount << " грн? (y/n): ";
    cin >> confirm;
    if (confirm != "y" && confirm != "Y") {
        cout << "Оплата скасована." << endl;
        return;
    }

    // 3. Обробка
    if (processPaymentLogic(method, totalAmount)) {
        cout << "\n[УСПІХ] Оплату проведено! Квитанцію відправлено." << endl;
        cout << "Дякуємо за покупку!" << endl;
        cart.clear(); // Очищення кошика після успішної покупки
    }
    else {
        cout << "\n[ПОМИЛКА] Транзакцію відхилено." << endl;
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    vector<User> users;
    initializeUsers(users);

    vector<Book> books;
    initializeBooks(books);

    // Кошик для покупок (спільний для модулів)
    vector<Book> shoppingCart;

    // КРОК 1: Авторизація
    User* currentUser = performLogin(users);
    if (currentUser == nullptr) {
        system("pause");
        return 0;
    }

    // КРОК 2: Головне меню
    int mainChoice = 0;
    do {
        cout << "\n========================================" << endl;
        cout << "      ГОЛОВНЕ МЕНЮ (" << currentUser->username << ")" << endl;
        cout << "========================================" << endl;
        cout << "1. Каталог книг (Купівля / Редагування)" << endl;
        cout << "2. Перейти до Оплати (Кошик: " << shoppingCart.size() << " шт.)" << endl;
        cout << "3. Вихід з системи" << endl;
        cout << "Ваш вибір: ";
        cin >> mainChoice;

        if (cin.fail()) {
            cin.clear(); cin.ignore(1000, '\n');
            continue;
        }

        switch (mainChoice) {
        case 1:
            // Передаємо права користувача та кошик в модуль каталогу
            runCatalogModule(books, shoppingCart, currentUser);
            break;
        case 2:
            // Передаємо кошик на оплату
            runPaymentModule(shoppingCart);
            break;
        case 3:
            cout << "До побачення!" << endl;
            break;
        default:
            cout << "Невірний вибір." << endl;
        }

    } while (mainChoice != 3);

    return 0;
}