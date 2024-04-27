#include <iostream>
#include <unordered_map>
#include <vector>
#include "User.h"
#include <fstream>

int ID_Number;
unordered_map<string, User> users;
string current_user = "";
Chat public_chat;

#include "Functions.h"

using namespace std;

auto main() -> int
{
    ID_Number = 1;
	char input, input_2, proof;
	bool access = false;
	string message = "", enter = "";
	set_data_from_file();

	do
	{
		system("cls");																	// Очистка экрана консоли
		cout << "0 - log in" << endl;													// Авторизация
		cout << "1 - sign up" << endl;													// Регистрация
		cout << "q - close Console Chat" << endl;										// Выход

		cin >> enter;																	// Выбираем нужную опцию
		if (enter.size() == 1)
			input = enter[0];
		else
			input = '5';

		switch (input)
		{
		case '0':
			access = Authorization();
			break;

		case '1':
			access = Registration();
			break;

		case 'q':																	// ВЫХОД
			
			proof = 'q';
			while (proof != '0' && proof != '1')
			{
				cout << "Are you sure you want to close the Console Chat?" << endl;		// Убеждаемся, что пользователь хочет выйти из консольного чата
				cout << "0 - No" << endl;
				cout << "1 - Yes" << endl;

				enter = "";
				cin >> enter;
				if (enter.size() == 1)
					proof = enter[0];
				else
					proof = '5';

				if (proof != '0' && proof != '1')
				{
					cout << "Incorrect data." << endl;										// Выводим сообщение, если введенные данные не соответствуют предложенным опциям. Процесс входа начинается заново
					system("pause");
					system("cls");
				}
			}
			if (proof == '0')
				input = 'f';														// Меняем значение переменной input, чтобы не выйти из цикла															
			break;																	// Иначе, не меняем и выходим из чата

		default:
			cout << "Incorrect data." << endl;										// Выводим сообщение, если введенные данные не соответствуют предложенным опциям. Процесс входа начинается заново
			system("pause");
		}

		if (input != 'q' && access)
		{
			access = true;
			do
			{
				system("cls");															// Очистка экрана консоли
				cout << current_user << endl;
				cout << "0 - exit" << endl;												// Выход
				cout << "1 - to choose chat" << endl;									// Выбор существующих чатов

				cin >> enter;
				if (enter.size() == 1)
					input_2 = enter[0];
				else
					input_2 = '5';
				int chats_count = users[current_user].get_chats_count(), id;
				switch (input_2)
				{
				case '0':
					cout << "Are you sure you want to exit?" << endl;					// Убеждаемся, что пользователь хочет выйти из консольного чата
					cout << "0 - No" << endl;
					cout << "1 - Yes" << endl;
					cin >> proof;
					if (proof == 1)															// Меняем значение переменной access, чтобы выйти из цикла
						access = false;													// Иначе, не меняем и остаемся в чате
					break;

				case '1':
					cout << "0 - public chat" << endl;

					for (int i = 0; i < chats_count; i++)
					{
						id = users[current_user].get_receiver_id(i);
						cout << id << " - chat with user " <<
							users[get_login_from_id(id)].getName() << endl;
					}
					cout << "Choose chat: ";
					cin >> id;

					Communication(id);
					break;

				default:
					cout << "Incorrect data." << endl;
					system("pause");
				}
			} while (access);
			
		}
	} while (input != 'q');
	return 0;
}