#pragma once
#include <iostream>
#include <string>

using namespace std;

string get_login_from_id(int id)
{
	for (const auto& user : users)
	{
		if ((user.second).getID() == id)
		{
			return user.first;
		}
	}
}

void tokenize(string const& str, vector<string>& out)
{
    size_t start;
    size_t end = 0;

    while ((start = str.find_first_not_of(' ', end)) != string::npos)
    {
        end = str.find(' ', start);
        out.push_back(str.substr(start, end - start));
    }
}

uint string_to_uint(string str)
{
	uint Ans = 0;
	int q = 1;
	for (int i = str.length() - 1; i >= 0; i--)
	{
		Ans += (int(str[i]) - 48) * q;
		q *= 10;
	}
	return Ans;
}

void set_data_from_file()
{
	fstream file;
	char open_file[400];
	users.clear();
	vector<string> out;
	User user;

	file.open("AuthData.txt");															// ��������� ���� �� ����� ��������
	if (!file.is_open())																// �� ������ ������� � ������ ���
	{																					// ���� ��� ����� �, ��������������, �� ������� ��� �������
		file.close();
		ofstream{ "AuthData.txt" };														// �� ������� ���
		file.open("AuthData.txt");	
		// � ���������
	}

	file.seekg(0, ios_base::end);														// ���������� ��������� � ����� �����
	if (file.tellg())																	// ���������, �� ������ �� ����
	{
		file.seekg(0, ios_base::beg);													// ���������� ��������� � ������ �����
		while (!file.eof())																// ���� �� ������ �� ����� �����
		{
			file.getline(open_file, 400);												// ��������� �� ����� ������
			if (string(open_file) != "")												// ���� ������� �� ������
			{
				user.erase();
				out.clear();
				tokenize(open_file, out);

				user.setName(out[6]);													// ���������� �����
				uint* hash = new uint[5];
				hash[0] = string_to_uint(out[1]);
				hash[1] = string_to_uint(out[2]);
				hash[2] = string_to_uint(out[3]);
				hash[3] = string_to_uint(out[4]);
				hash[4] = string_to_uint(out[5]);
				user.setPassword(hash);

				for (int j = 0; j < 5; j++)
					hash[j] = 0;
				delete[] hash;
				user.setID(ID_Number);

				pair<string, User> myPair = make_pair((string)out[0], user);
				users.insert(myPair);													// ���������� � ��������� ������������
				user.erase();
				ID_Number++;
				myPair.second.erase();
			}
		}
	}
	file.close();

	file.open("Public_Chat.txt");														// ��������� ���� � ����������� ���������� ����
	if (!file.is_open())																// �� ������ ������� � ������ ��� (���������� � ��������)
	{
		file.close();
		ofstream{ "Public_Chat.txt" };
		file.open("Public_Chat.txt");
	}
	file.seekg(0, ios_base::end);														// ���������� ��������� � ����� �����
	if (file.tellg())																	// ���������, �� ������ �� ����
	{
		file.seekg(0, ios_base::beg);													// ���������� ��������� � ������ �����
		while (!file.eof())																// ���� �� ������ �� ����� �����
		{
			file.getline(open_file, 400);												// ��������� �� ����� ������
			public_chat.SetMessages(string(open_file));									// ���������� ��������� ������������� ���������� ���� � ���������� ������ PublicChat
		}
	}
	file.close();

	string name = "";
	Chat *chat;
	for (int i = 1; i < ID_Number; i++)
	{
		for (int j = i + 1; j < ID_Number; j++)
		{
			name = "Private_Chat_" + to_string(i) + "_" + to_string(j) + ".txt";
			file.open(name);
			if (!file.is_open())																// �� ������ ������� � ������ ��� (���������� � ��������)
			{
				chat = new Chat;
				file.close();
				ofstream{ name };
				file.open(name);
				users[get_login_from_id(i)].set_chat(chat, j);
				users[get_login_from_id(j)].set_chat(chat, i);
				chat = nullptr;
			}
			file.seekg(0, ios_base::end);

			if (file.tellg() && file.is_open())
			{
				chat = new Chat;
				file.seekg(ios_base::beg);
				while (!file.eof())
				{
					file.getline(open_file, 400);
					(*chat).SetMessages(string(open_file));
				}
				users[get_login_from_id(i)].set_chat(chat, j);
				users[get_login_from_id(j)].set_chat(chat, i);
				chat = nullptr;
			}
			file.close();
		}
	}
}

bool Authorization()
{
	string login, pass;
	bool access = true;
	User user;
	cout << "login: ";
	cin >> login;															// ������ �����

	if (users.find(login) == users.end())
	{
		cout << "Incorrect login." << endl;									// ������ �������� �����
		system("pause");
		return false;
	}

	cout << "password: ";
	cin >> pass;
	char* password = new char[pass.length()];
	for (int i = 0; i < pass.length(); i++)
		password[i] = pass[i];
	unordered_map<string, User>::iterator it = users.find(login);
	user = it->second;

	uint* hash1 = sha1(password, pass.length());
	
	uint* hash2 = user.getPassword();

	for (int j = 0; j < 5; j++)
	{
		if (hash1[j] != hash2[j])
			access = false;
	}

	if (access)
	{
		user.erase();
		//user = nullptr;
		current_user = login;
		return true;
	}
	else
	{
		cout << "Incorrect password." << endl;							// ������ �������� ������
		user.erase();
		//user = nullptr;
		system("pause");
		return false;
	}
}

bool Registration()
{
	string login, pass, name;
	User user;// = new User;
	user.erase();
	cout << "Set login: ";
	cin >> login;															// ������ �����

	fstream file;

	unordered_map<string, User>::iterator it = users.find(login);
	if (it != users.end())
	{
		cout << "This login is already taken." << endl;
		system("pause");
		return false;
	}
	user.setID(ID_Number++);												// ������������� ID ������������

	cout << "Set password: ";
	cin >> pass;
	user.setPassword(pass);

	cout << "Set your UserName: ";
	cin >> name;
	user.setName(name);

	cout << user << endl;
	//system("pause");

	pair<string, User> myPair = make_pair((string)login, user);
	users.insert(myPair);
	for (int i = 1; i < ID_Number - 1; i++)
	{
		string name = "Private_Chat_" + to_string(i) + "_" + to_string(ID_Number - 1) + ".txt";
		Chat* chat = new Chat;
		ofstream{ name };
		users[get_login_from_id(i)].set_chat(chat, ID_Number - 1);
		users[login].set_chat(chat, i);
		chat = nullptr;
	}
	//user = nullptr;

	uint* hash = user.getPassword();
	//cout << "hash: " << hash << " " << *hash << endl;
	file.open("AuthData.txt", ios::app);
	file << login << " ";
	current_user = login;
	//cout << name << endl;
	for (int j = 0; j < 5; j++)
	{
		//user._pass_hash[j];
		file << hash[j] << " ";
		cout << hash[j] << " ";
	}
	file << name << "\n";
	//delete[] hash;
	user.erase();
	myPair.second.erase();
	file.close();
	return true;
}

void Communication(int chat_num)
{
	fstream file;
	string message = "";
	if (!chat_num)
	{
		file.open("Public_Chat.txt", ios::app);								// ��������� ���� � ����������� ���������� ����
		while (message != "CLOSE")											// ���� ������������ �� ������� "CLOSE", ��� �� ���������
		{
			system("cls");													// ������� ������ �������
			cout << "CLOSE - close chat" << endl;							// ���������� ����� �����������, ����� ����� ����� ������ ��� ������ �� ����
			cout << "--------------------------------"
				<< "-------------------------------" << endl;
			cout << public_chat << endl;											// ����� ���� ��������� �� ������� ���� �� �������
			cout << "Message: ";
			getline(cin, message);											// ���� ���������, ���� "CLOSE", ����� ������� ������ ���
			while (message[0] == ' ')										// ������� ������� �� ������ ���������
				message.erase(0, 1);
			if (message != "CLOSE")
			{
				public_chat.addMessage(message, users[current_user].getName());		// �������� ��������� � ���, ����������� ��� � ������ ����
				if (message.size() > 0)
					file << users[current_user].getName() + ": " + message + "\n"; // ���������� ����� ��������� � ����
			}
		}
		file.close();
	}
	else
	{
		try
		{
			users.at(get_login_from_id(chat_num));
		}
		catch (out_of_range& e)
		{
			cout << "Incorrect data." << endl;
			message = "CLOSE";
			system("pause");
			return;
		}

		string name = "";
		if (users[current_user].getID() < chat_num)
			name = "Private_Chat_" + to_string(users[current_user].getID()) + "_" + to_string(chat_num) + ".txt";
		else
			name = "Private_Chat_" + to_string(chat_num) + "_" + to_string(users[current_user].getID()) + ".txt";
		file.open(name, ios::app);
		while (message != "CLOSE")
		{
			system("cls");										// ������� ������ �������
			cout << "CLOSE - close chat" << endl;				// ���������� ����� �����������, ����� ����� ����� ������ ��� ������ �� ����
			cout << "--------------------------------"
				<< "-------------------------------" << endl;
			users[current_user].show_chat(chat_num);
			cout << endl << "Message: ";
			getline(cin, message);
			while (message[0] == ' ')							// ������� ������� �� ������ ���������
				message.erase(0, 1);
			if (message != "CLOSE")
			{
				users[current_user].add_message(message, chat_num);	// �������� ��������� � ���, ����������� ��� � ������ ����
				if (message.size() > 0)
					file << users[current_user].getName() + ": " + message + "\n";	// ���������� ����� ��������� � ����
			}
		}
		file.close();
	}
}