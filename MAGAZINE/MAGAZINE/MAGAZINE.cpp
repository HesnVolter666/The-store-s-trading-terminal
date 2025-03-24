#include <iostream>
#include <vector>
#include <string>
#include <locale>
#include <codecvt>
#include <windows.h>

using namespace std;

// Структура для хранения информации о товаре
struct Product {
    string name;
    double price;
    int quantity; // Максимальное количество товара
};

// Структура для хранения информации о покупке
struct Purchase {
    string name;
    double price;
    int quantity; // Количество купленного товара
};

// Функция для добавления товара в каталог
void addProduct(vector<Product>& catalog) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "russian");
    Product newProduct;

    cout << "Введите название товара: ";
    cin.ignore(); // Игнорируем предыдущий символ новой строки
    getline(cin, newProduct.name); // Используем getline для считывания названия с пробелами

    cout << "Введите цену товара: ";
    cin >> newProduct.price;

    cout << "Введите максимальное количество товара: ";
    cin >> newProduct.quantity;

    catalog.push_back(newProduct);
    cout << "Товар добавлен в каталог!" << endl;
}

// Функция для отображения каталога товаров
void displayCatalog(const vector<Product>& catalog) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "russian");

    if (catalog.empty()) {
        cout << "Каталог пуст." << endl;
    }
    else {
        cout << "Каталог товаров:" << endl;
        for (size_t i = 0; i < catalog.size(); ++i) {
            cout << i + 1 << ". Название: " << catalog[i].name
                << ", Цена: " << catalog[i].price
                << " руб., Доступное количество: "
                << catalog[i].quantity << " штук." << endl;
        }
    }
}

// Функция для режима покупателя
void customerMode(vector<Product>& catalog) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "russian");

    vector<Purchase> cart;
    double wallet; // Кошелек покупателя
    cout << "Введите сумму в кошельке: ";
    cin >> wallet;

    int choice;

    while (true) {
        cout << "\nМеню покупателя:\n";
        cout << "1. Просмотреть каталог\n";
        cout << "2. Добавить товар в корзину\n";
        cout << "3. Показать корзину\n";
        cout << "4. Оплатить\n";
        cout << "5. Выйти\n";
        cout << "Выберите действие: ";
        cin >> choice;

        switch (choice) {
        case 1:
            displayCatalog(catalog);
            break;
        case 2: {
            int productIndex;
            cout << "Введите номер товара для добавления в корзину: ";
            cin >> productIndex;

            if (productIndex > 0 && productIndex <= catalog.size()) {
                int quantityToAdd;
                cout << "Введите количество товара для добавления в корзину: ";
                cin >> quantityToAdd;

                if (quantityToAdd > 0 && quantityToAdd <= catalog[productIndex - 1].quantity) {
                    cart.push_back({ catalog[productIndex - 1].name, catalog[productIndex - 1].price, quantityToAdd });
                    catalog[productIndex - 1].quantity -= quantityToAdd; // Уменьшаем доступное количество товара
                    cout << "Товар добавлен в корзину!" << endl;
                }
                else {
                    cout << "Ошибка: Недостаточно доступного количества товара. Доступно: "
                        << catalog[productIndex - 1].quantity << " штук." << endl;
                }
            }
            else {
                cout << "Неверный номер товара." << endl;
            }
            break;
        }
        case 3:
            if (cart.empty()) {
                cout << "Корзина пуста." << endl;
            }
            else {
                cout << "Корзина:" << endl;
                for (const auto& purchase : cart) {
                    cout << "Название: " << purchase.name
                        << ", Цена: " << purchase.price
                        << " руб., Количество: "
                        << purchase.quantity << " штук." << endl;
                }
            }
            break;
        case 4: {
            double total = 0;
            for (const auto& purchase : cart) {
                total += purchase.price * purchase.quantity; // Считаем общую сумму с учетом количества
            }

            if (total <= wallet) {
                cout << "Сумма к оплате: " << total << " руб." << endl;
                cout << "Оплата прошла успешно!" << endl;
                cart.clear(); // Очищаем корзину после оплаты
                cout << "До свидания, спасибо за покупки!" << endl;
                return; // Выход из режима покупателя
            }
            else {
                cout << "Недостаточно средств в кошельке! Сумма к оплате: "
                    << total << " руб., доступно: "
                    << wallet << " руб." << endl;

                // Предложение удалить товары из корзины
                while (total > wallet && !cart.empty()) {
                    int itemToRemove;
                    cout << "Выберите товар для удаления из корзины:" << endl;
                    for (size_t i = 0; i < cart.size(); ++i) {
                        cout << i + 1 << ". Название: " << cart[i].name
                            << ", Цена: " << cart[i].price
                            << ", Количество: "
                            << cart[i].quantity << " штук." << endl;
                    }
                    cout << "Введите номер товара для удаления (0 для отмены): ";
                    cin >> itemToRemove;

                    if (itemToRemove == 0) {
                        break; // Выход из цикла удаления
                    }

                    if (itemToRemove > 0 && itemToRemove <= cart.size()) {
                        total -= cart[itemToRemove - 1].price * cart[itemToRemove - 1].quantity; // Уменьшаем общую сумму
                        // Находим индекс удаляемого товара в каталоге
                        for (size_t j = 0; j < catalog.size(); ++j) {
                            if (catalog[j].name == cart[itemToRemove - 1].name) {
                                catalog[j].quantity += cart[itemToRemove - 1].quantity; // Возвращаем количество товара в каталог
                                break;
                            }
                        }
                        cart.erase(cart.begin() + itemToRemove - 1); // Удаляем товар из корзины
                        cout << "Товар удален из корзины." << endl;
                    }
                    else {
                        cout << "Неверный номер товара." << endl;
                    }
                }

                // Проверяем, достаточно ли средств после удаления товаров
                if (total <= wallet) {
                    cout << "Сумма к оплате теперь: " << total
                        << " руб. Оплата прошла успешно!" << endl;
                    cart.clear(); // Очищаем корзину после оплаты
                    cout << "До свидания, спасибо за покупки!" << endl;
                    return; // Выход из режима покупателя
                }
                else {
                    cout << "Вы все еще превышаете лимит кошелька."
                        << endl;
                }
            }
            break;
        }
        case 5:
            cout << "Выход из режима покупателя." << endl;
            return; // Выход из режима покупателя
        default:
            cout << "Неверный выбор. Попробуйте снова." << endl;
        }
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "russian");

    vector<Product> catalog;
    int choice;

    while (true) {
        cout << "\nМеню:\n";
        cout << "1. Добавить товар\n";
        cout << "2. Показать каталог\n";
        cout << "3. Режим покупателя\n";
        cout << "4. Выйти\n";
        cout << "Выберите действие: ";
        cin >> choice;

        switch (choice) {
        case 1:
            addProduct(catalog);
            break;
        case 2:
            displayCatalog(catalog);
            break;
        case 3:
            customerMode(catalog);
            break;
        case 4:
            cout << "Выход из программы." << endl;
            return 0;
        default:
            cout << "Неверный выбор. Попробуйте снова." << endl;
        }
    }

    return 0;
}
