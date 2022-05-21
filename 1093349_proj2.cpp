#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

struct table
{
	string history = "000";
	int state[8] = { 0 };
	string prediction = "N";	
};

int entrynum;
int current_entrynum;
vector<string> input;
map<string, int> reg;
bool entry = false;
bool taken = false;
bool branch = false;
string const_state[8] = { "SN","WN1","WN2","WN3","WT1","WT2","WT3","ST" };
vector <struct table >form;
string outcome;
string endstring;
bool endfunc = false;
int loopnum;
int endnum;

void update(vector <struct table > &form)
{
	int temp = stoi(form[current_entrynum].history);
	int num = 0; //算要改第幾個state
	for (int k = 0; k < 3; k++)
	{
		if (temp % 10 == 1)
			num += pow(2, k);
		temp /= 10;
	}

	if (taken == false)
	{
		form[current_entrynum].history += "0";
		if (form[current_entrynum].state[num] != 0)
			form[current_entrynum].state[num]--;
	}
	else if (taken == true)
	{
		form[current_entrynum].history += "1";
		if (form[current_entrynum].state[num] != 7)
			form[current_entrynum].state[num]++;
	}

	if (form[current_entrynum].state[num] < 4)
		form[current_entrynum].prediction = "N";
	else form[current_entrynum].prediction = "T";

	for (int j = 1; j < 4; j++)
	{
		form[current_entrynum].history[j - 1] = form[current_entrynum].history[j];
	}
	form[current_entrynum].history.erase(--form[current_entrynum].history.end());
}
void print(vector <struct table >form)
{
	for (int n = 0; n < entrynum; n++)
	{
		//cout 原本的
		cout << form[n].history << "  ";
		for (int i = 0; i < 8; i++)
		{
			cout << setw(3) << const_state[form[n].state[i]] << "  ";
		}
		if (endfunc == true)
		{
			cout << endl;
			continue;
		}
		if (n == current_entrynum)
			cout << form[current_entrynum].prediction << "  " << outcome;
		cout << endl;		
	}
	cout << endl;
	endfunc = false;
	
	int x;
	//更改裡面的值
	if (entry == false)
		x = 0;
	else x = 1;

	update(form);

}

void convert(vector<string> input_temp,int &i)
{
	if (input_temp[0] == "addi")
	{
		taken = false;
		outcome = "N";
		if (input_temp[1] == input_temp[2])
		{
			reg[input_temp[1]] += stoi(input_temp[3]);
		}
		else
		{
			reg[input_temp[1]] = reg[input_temp[2]] + stoi(input_temp[3]);
		}
		print(form);

	}

	else if (input_temp[0] == (endstring += ":"))
	{
		taken = false;
		outcome = "N";
		endfunc = true;
		print(form);
	}
	else if (input_temp[0] == "beq")
	{
		if (reg[input_temp[1]] != reg[input_temp[2]])
		{
			taken = false;
			outcome = "N";
		}
		else
		{
			taken = true;
			outcome = "T";
			endstring = input_temp[3];
		}
		print(form);
	}
	else
	{
		taken = false;
		outcome = "F";
	}
	
	if (taken == true)
	{	
		for (int j = 0; j < input.size(); j++)
		{
			bool find_branch = true;
			for (int k = 0; k < endstring.size(); k++)
			{
				if (input[j][k] != endstring[k])
				{
					find_branch = false;
					break;
				}					
			}
			if (find_branch == true)
			{
				i = j;
				if (j == input.size() - 1)
					i--;
				break;
			}
		}
	}
}

int main()
{
	table buffer;
	string temp;
	vector<string> input_temp;
	string input_buffer;
	string space_delimiter1 = " ";
	string space_delimiter2 = ",";
	                                        
	cout << "Please input entry:";
	cin >> entrynum;
	for (int k = 0; k < entrynum;k++)
	{
		form.push_back(buffer);
	}
	cout << endl << "please input code :" << endl;
	cin.ignore();
	while (getline(cin, temp) && temp != "")
	{
		input.push_back(temp);
	}
	int not_inst = 0;
	for (int i = 0; i < input.size(); i++)
	{
		int j = i;
		input_buffer = input[i];
		if (not_inst > 0)
			j -= not_inst;
		current_entrynum = j% entrynum;
		size_t pos = 0;
		while ((pos = input[i].find(space_delimiter1)) != string::npos)
		{
			input_temp.push_back(input[i].substr(0, pos));
			input[i].erase(0, pos + space_delimiter1.length());
		}
		while ((pos = input[i].find(space_delimiter2)) != string::npos)
		{
			input_temp.push_back(input[i].substr(0, pos));
			input[i].erase(0, pos + space_delimiter2.length());
		}
		input_temp.push_back(input[i]);
		if (input_temp.size() < 3)
			not_inst++;
		input[i] = input_buffer;
		cout << input[i] << endl;
		convert(input_temp,i);
		input_temp.clear();
	}
}