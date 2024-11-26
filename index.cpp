#include <iostream>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

void createUser(sql::Connection* conn, const std::string& name, const std::string& email) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("INSERT INTO users (name, email) VALUES (?, ?)"));
        pstmt->setString(1, name);
        pstmt->setString(2, email);
        pstmt->execute();
        std::cout << "User created successfully.\n";
    } catch (sql::SQLException& e) {
        std::cerr << "Error creating user: " << e.what() << "\n";
    }
}

void readUsers(sql::Connection* conn) {
    try {
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT id, name, email FROM users"));

        std::cout << "Users:\n";
        while (res->next()) {
            std::cout << "ID: " << res->getInt("id")
                      << ", Name: " << res->getString("name")
                      << ", Email: " << res->getString("email") << "\n";
        }
    } catch (sql::SQLException& e) {
        std::cerr << "Error reading users: " << e.what() << "\n";
    }
}

void updateUser(sql::Connection* conn, int id, const std::string& name, const std::string& email) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("UPDATE users SET name = ?, email = ? WHERE id = ?"));
        pstmt->setString(1, name);
        pstmt->setString(2, email);
        pstmt->setInt(3, id);
        pstmt->execute();
        std::cout << "User updated successfully.\n";
    } catch (sql::SQLException& e) {
        std::cerr << "Error updating user: " << e.what() << "\n";
    }
}

void deleteUser(sql::Connection* conn, int id) {
    try {
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement("DELETE FROM users WHERE id = ?"));
        pstmt->setInt(1, id);
        pstmt->execute();
        std::cout << "User deleted successfully.\n";
    } catch (sql::SQLException& e) {
        std::cerr << "Error deleting user: " << e.what() << "\n";
    }
}

int main() {
    try {
        sql::Driver* driver = get_driver_instance();
        std::unique_ptr<sql::Connection> conn(driver->connect("tcp://127.0.0.1:3306", "root", "your_password"));

        conn->setSchema("crud_example");

        int option;
        do {
            std::cout << "\nCRUD Menu:\n"
                      << "1. Create User\n"
                      << "2. Read Users\n"
                      << "3. Update User\n"
                      << "4. Delete User\n"
                      << "5. Exit\n"
                      << "Choose an option: ";
            std::cin >> option;

            switch (option) {
                case 1: {
                    std::string name, email;
                    std::cout << "Enter name: ";
                    std::cin >> name;
                    std::cout << "Enter email: ";
                    std::cin >> email;
                    createUser(conn.get(), name, email);
                    break;
                }
                case 2:
                    readUsers(conn.get());
                    break;
                case 3: {
                    int id;
                    std::string name, email;
                    std::cout << "Enter user ID to update: ";
                    std::cin >> id;
                    std::cout << "Enter new name: ";
                    std::cin >> name;
                    std::cout << "Enter new email: ";
                    std::cin >> email;
                    updateUser(conn.get(), id, name, email);
                    break;
                }
                case 4: {
                    int id;
                    std::cout << "Enter user ID to delete: ";
                    std::cin >> id;
                    deleteUser(conn.get(), id);
                    break;
                }
                case 5:
                    std::cout << "Exiting...\n";
                    break;
                default:
                    std::cout << "Invalid option. Try again.\n";
            }
        } while (option != 5);

    } catch (sql::SQLException& e) {
        std::cerr << "Database connection error: " << e.what() << "\n";
    }

    return 0;
}
