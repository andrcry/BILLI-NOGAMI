#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <windows.h>
using namespace std;

//--------------------ТЕСТ----------------

struct User {
    string username;
    string password;
    string role; // admin àáî customer
};

struct Book {
    int id;
    string title;
    string author;
    string description;
    double price;
    double rating;
};

// ---------------------------- ÊÎÐÈÑÒÓÂÀ×² ----------------------------
void initializeUsers(vector<User>& users) {
    users.push_back({ "admin", "admin123", "admin" });
    users.push_back({ "user", "111", "customer" });
    users.push_back({ "student", "knu", "customer" });
}

User* performLogin(vector<User>& userDb) {
    string inputUsername, inputPassword;
    const int maxAttempts = 3;

    cout << "\n=== ÂÕ²Ä Ó ÑÈÑÒÅÌÓ ===" << endl;

    for (int attempt = 1; attempt <= maxAttempts; attempt++) {
        cout << "Ñïðîáà " << attempt << " ç " << maxAttempts << endl;
        cout << "Ëîã³í: ";
        cin >> inputUsername;
        cout << "Ïàðîëü: ";
        cin >> inputPassword;

        for (auto& user : userDb) {
            if (user.username == inputUsername && user.password == inputPassword) {
                cout << "\n[Óñï³õ] Â³òàþ, " << user.username << "!" << endl;
                return &user;
            }
        }
        cout << "[Ïîìèëêà] Íåâ³ðíèé ëîã³í àáî ïàðîëü.\n" << endl;
    }

    cout << "[Â³äìîâà] Ïåðåâèùåíî ê³ëüê³ñòü ñïðîá." << endl;
    return nullptr;
}

// ---------------------------- ÊÍÈÃÈ ----------------------------
void initializeBooks(vector<Book>& db) {
    db.push_back({ 1, "Êîáçàð", "Òàðàñ Øåâ÷åíêî", "Ïîåç³ÿ", 350, 4.9 });
    db.push_back({ 2, "Çàõàð Áåðêóò", "²âàí Ôðàíêî", "²ñòîðè÷íà ïîâ³ñòü", 200, 4.7 });
    db.push_back({ 3, "Åíå¿äà", "². Êîòëÿðåâñüêèé", "Ïîåìà", 280, 4.8 });
}

void displayAllBooks(const vector<Book>& db) {
    cout << "\n--- ÊÀÒÀËÎÃ ÊÍÈÃ ---" << endl;
    if (db.empty()) {
        cout << "Êàòàëîã ïîðîæí³é." << endl;
        return;
    }

    cout << left << setw(5) << "ID" << setw(25) << "Íàçâà" << setw(20) << "Àâòîð" << "Ö³íà" << endl;
    cout << "------------------------------------------------------------" << endl;

    for (const auto& b : db)
        cout << setw(5) << b.id << setw(25) << b.title << setw(20) << b.author << b.price << " ãðí\n";
}

void adminAddBook(vector<Book>& db) {
    Book newBook;

    cout << "\n[ADMIN] Äîäàòè êíèãó" << endl;

    int maxId = 0;
    for (const auto& b : db) if (b.id > maxId) maxId = b.id;
    newBook.id = maxId + 1;

    cout << "Íàçâà: "; cin >> newBook.title;
    cout << "Àâòîð: "; cin >> newBook.author;
    cout << "Îïèñ: "; cin >> newBook.description;
    cout << "Ö³íà: "; cin >> newBook.price;
    cout << "Ðåéòèíã: "; cin >> newBook.rating;

    db.push_back(newBook);
    cout << "[OK] Êíèãà äîäàíà." << endl;
}

void adminDeleteBook(vector<Book>& db) {
    int id;
    displayAllBooks(db);

    cout << "\nÂâåä³òü ID äëÿ âèäàëåííÿ: ";
    cin >> id;

    auto it = remove_if(db.begin(), db.end(), [id](const Book& b) { return b.id == id; });
    if (it != db.end()) {
        db.erase(it, db.end());
        cout << "[OK] Êíèãà âèäàëåíà." << endl;
    }
    else cout << "[Ïîìèëêà] ID íå çíàéäåíî." << endl;
}

// ---------------------------- ÊÎØÈÊ ----------------------------
void addToCart(const vector<Book>& db, vector<Book>& cart) {
    int id;
    cout << "Ââåä³òü ID êíèãè: ";
    cin >> id;

    for (const auto& b : db) {
        if (b.id == id) {
            cart.push_back(b);
            cout << "[Êîøèê] Äîäàíî: " << b.title << endl;
            return;
        }
    }
    cout << "Êíèãó íå çíàéäåíî." << endl;
}

void runCatalogModule(vector<Book>& db, vector<Book>& cart, const User* user) {
    int choice = 0;

    do {
        cout << "\n--- ÌÅÍÞ ÊÀÒÀËÎÃÓ ---\n";
        cout << "1. Ïîêàçàòè âñ³ êíèãè\n";
        cout << "2. Äîäàòè êíèãó â êîøèê\n";

        if (user->role == "admin") {
            cout << "8. [ADMIN] Äîäàòè êíèãó\n";
            cout << "9. [ADMIN] Âèäàëèòè êíèãó\n";
        }

        cout << "0. Íàçàä\nÂàø âèá³ð: ";
        cin >> choice;

        switch (choice) {
        case 1: displayAllBooks(db); break;
        case 2: addToCart(db, cart); break;

        case 8:
            if (user->role == "admin") adminAddBook(db);
            else cout << "Äîñòóï çàáîðîíåíî.\n";
            break;

        case 9:
            if (user->role == "admin") adminDeleteBook(db);
            else cout << "Äîñòóï çàáîðîíåíî.\n";
            break;

        case 0: break;

        default: cout << "Íåâ³ðíèé âèá³ð.\n"; break;
        }

    } while (choice != 0);
}

// ---------------------------- ÎÏËÀÒÀ ----------------------------

// Ïåðåâ³ðêà ùî ââåäåíî ëèøå öèôðè
bool isDigitsOnly(const string& s) {
    for (char c : s)
        if (!isdigit(c)) return false;
    return true;
}

bool processPaymentLogic(const string& method, double amount) {
    if (amount <= 0) return false;

    cout << "Îáðîáêà ïëàòåæó íà " << amount << " ãðí..." << endl;

    if (method == "card") {
        string card;

        while (true) {
            cout << "Ââåä³òü íîìåð êàðòêè (16 öèôð): ";
            cin >> card;

            if (card.length() == 16 && isDigitsOnly(card)) break;

            cout << "[Ïîìèëêà] Ìîæíà ââîäèòè Ò²ËÜÊÈ öèôðè (16 ñèìâîë³â).\n";
        }
    }

    Sleep(1000);
    return true;
}

void runPaymentModule(vector<Book>& cart) {
    if (cart.empty()) {
        cout << "Êîøèê ïîðîæí³é." << endl;
        return;
    }

    double total = 0;
    cout << "\n--- ÊÀÑÀ ---\nÂàøå çàìîâëåííÿ:\n";

    for (const auto& b : cart) {
        cout << "- " << b.title << " (" << b.price << " ãðí)\n";
        total += b.price;
    }

    cout << "Ðàçîì: " << total << " ãðí\n";

    string method;
    cout << "Îáåð³òü ñïîñ³á îïëàòè (card/cash): ";
    cin >> method;

    string confirm;
    cout << "Ï³äòâåðäèòè îïëàòó (y/n): ";
    cin >> confirm;

    if (confirm != "y" && confirm != "Y") {
        cout << "Îïëàòó ñêàñîâàíî.\n";
        return;
    }

    if (processPaymentLogic(method, total)) {
        cout << "[ÓÑÏ²Õ] Îïëàòà ïðîâåäåíà!\n";
        cart.clear();
    }
    else {
        cout << "[ÏÎÌÈËÊÀ] Òðàíçàêö³ÿ â³äõèëåíà.\n";
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
        cout << "\n===== ÃÎËÎÂÍÅ ÌÅÍÞ =====" << endl;
        cout << "1. Êàòàëîã êíèã\n";
        cout << "2. Îïëàòà (òîâàð³â ó êîøèêó: " << cart.size() << ")\n";
        cout << "3. Âèõ³ä\nÂàø âèá³ð: ";
        cin >> choice;

        switch (choice) {
        case 1: runCatalogModule(books, cart, currentUser); break;
        case 2: runPaymentModule(cart); break;
        case 3: cout << "Äî ïîáà÷åííÿ!\n"; break;
        default: cout << "Ïîìèëêà ââîäó.\n"; break;
        }

    } while (choice != 3);

    return 0;
}

