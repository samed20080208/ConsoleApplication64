#include <iostream>
#include <windows.h>
#include <cassert>
#include <string>
#include <ctime>
using namespace std;

class Exception : public exception {
public:
	Exception(const string& message) : exception(message.c_str()) {}
};

class DatabaseException : public Exception {
public:
	DatabaseException() : Exception("Database error") {}
};

class InvalidArgumentException : public Exception {
public:
	InvalidArgumentException(const string& message) : Exception(message) {}
};
class User {
	string _username;
	string _password;
	string _name;
	string _surname;

public:
	User()
	{
		_username = "";
		_password = "";
		_name = "";
		_surname = "";
	}
	User(string username, string password, string name, string surname)
	{
		SetUsername(username);
		SetPassword(password);
		SetName(name);
		SetSurname(surname);
	}
	User(const User& other)
	{
		this->SetName(other._name);
		this->SetSurname(other._surname);
		this->SetUsername(other._username);
		this->SetPassword(other._password);
	}
	User operator=(const User& other)
	{
		this->SetName(other._name);
		this->SetSurname(other._surname);
		this->SetUsername(other._username);
		this->SetPassword(other._password);

	}
	string GetUsername()const { return _username; }
	string GetPassword()const { return _password; }
	string GetName() const { return _name; }
	string GetSurname() const { return _surname; }

	void SetUsername(string username)
	{
		int len = size(username);
		if (len < 6 || username.front() > 65 && username.front() > 92) assert(!"Username duzgun deyil");
		this->_username = username;

	}
	void SetPassword(string password)
	{
		int len = size(password);
		if (len < 6) assert(!"Password error verir");
		this->_password = password;
	}
	void SetName(string name)
	{
		int len = size(name);
		if (len < 4) assert(!"Name error verir");
		this->_name = name;

	}
	void SetSurname(string surname)
	{
		int len = size(surname);

		if (len < 4) assert(!"SurName error verir");
		this->_surname = surname;


	}
	void show()
	{

		cout << "Username: " << GetUsername() << endl;
		cout << "Password: " << GetPassword() << endl;
		cout << "Name: " << GetName() << endl;
		cout << "Surname: " << GetSurname() << endl;


	};
};

class Database {
	User** users;
	int user_count = 0;

public:
	Database() : users(nullptr), user_count(0) {}

	void addUser(const User& user) {
		User** newUsers = new User * [user_count + 1];

		for (int i = 0; i < user_count; ++i) {
			newUsers[i] = users[i];
		}

		newUsers[user_count] = new User(user);

		++user_count;

		delete[] users;
		users = newUsers;
	}

	User& getUserByUsername(const string& username) {
		for (int i = 0; i < user_count; ++i) {
			if (users[i]->GetUsername() == username) {
				return *users[i];
			}
		}
		assert(!"");


	}

	void updateUser(const User& olduser, const User& newuser) {
		for (int i = 0; i < user_count; ++i) {
			if (users[i]->GetUsername() == olduser.GetUsername()) {
				delete users[i];
				users[i] = new User(newuser);
				return;
			}
		}


	}
	int GetUserCount() { return user_count; }
	User** Getusers() { return users; }
	~Database() {
		for (int i = 0; i < user_count; ++i) {
			delete users[i];
		}

		delete[] users;
	}
	bool isUsernameExists(const string& username) const {
		for (int i = 0; i < user_count; ++i) {
			if (users[i]->GetUsername() == username) {
				return true;
			}
		}
		return false;
	}
};



class Registration {
	Database _database;
public:
	Registration(const Database& database) {
		_database = database;
	}

	void signIn(string username, string password)
	{
		for (size_t i = 0; i < _database.GetUserCount(); i++)
		{
			if (_database.Getusers()[i]->GetUsername() == username)
			{
				if (_database.Getusers()[i]->GetPassword() == password)
					cout << "Ugurlu giris";
				else
					assert(!"Password yalnisdir");
				return;
			}
			assert(!"Username yalnisdir");
		}

	}

	void signUp(string username, string password, string name, string surname) {
		if (_database.isUsernameExists(username)) {
			throw DatabaseException();
		}

		if (username.length() < 6 || !isupper(username[0])) {
			throw InvalidArgumentException("Invalid username");
		}

		if (password.length() < 8) {
			throw InvalidArgumentException("Invalid password");
		}

		if (name.length() < 3) {
			throw InvalidArgumentException("Invalid name");
		}

		if (surname.length() < 3) {
			throw InvalidArgumentException("Invalid surname");
		}

		User newUser(username, password, name, surname);
		_database.addUser(newUser);
	}
	void AllUser()
	{
		for (size_t i = 0; i < _database.GetUserCount(); i++)
		{
			_database.Getusers()[i]->show();
		}

	}
};


class System {
public:
	static void Control() {
		Database db;
		Registration twitter(db);

		int choose;

		while (true) {
			cout << "Secim edin\n";
			cout << "1. All users\n";
			cout << "2. Sign in\n";
			cout << "3. Sign up\n";
			cout << "0. Exit\n";

			cin >> choose;

			if (choose == 1) {
				twitter.AllUser();
			}
			else if (choose == 2) {
				system("cls");
				string username;
				string password;

				cout << "Enter username: ";
				cin >> username;
				cout << "Enter password: ";
				cin >> password;

				twitter.signIn(username, password);
			}
			else if (choose == 3) {
				system("cls");


				string username;
				string password;
				string name;
				string surname;

				cout << "Enter username: ";
				cin >> username;
				cout << "Enter password: ";
				cin >> password;
				cout << "Enter name: ";
				cin >> name;
				cout << "Enter surname: ";
				cin >> surname;


				twitter.signUp(username, password, name, surname);
			}
			else if (choose == 0) {

				cout << "Sagolun\n";
				break;
			}
			else {
				cout << "Invalid option. Please try again.\n";
			}
		}

	}
};
