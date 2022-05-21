# 目錄
- [題目](#題目)
- [輸入輸出範例](#輸入輸出範例)
- [說明文件](#說明文件)
	- [全域變數](#全域變數)
	- [struct](https://github.com/hlyliang/1093349_proj2/edit/main/README.md#%E5%BB%BA%E7%AB%8B%E4%B8%80%E5%80%8Bstruct%E5%8E%BB%E5%AD%983-bit-hostory--state-%E5%92%8C-prediction)
	- [main function](https://github.com/hlyliang/1093349_proj2/edit/main/README.md#%E7%94%A8%E4%BD%BF%E7%94%A8%E8%80%85%E8%BC%B8%E5%85%A5entry%E5%80%8B%E6%95%B8%E4%BB%A5%E5%8F%8A%E7%A8%8B%E5%BC%8F%E7%A2%BC%E4%B8%A6%E5%B0%87%E5%AD%97%E4%B8%B2%E5%81%9A%E5%88%86%E5%89%B2)
	- [convert function](https://github.com/hlyliang/1093349_proj2/edit/main/README.md#%E5%88%A4%E6%96%B7%E4%BD%BF%E7%94%A8%E8%80%85%E8%BC%B8%E5%85%A5%E7%9A%84code%E6%98%AF%E5%93%AA%E4%B8%80%E7%A8%AE%E4%B8%A6%E6%8A%8Areg%E7%9A%84%E5%80%BC%E4%B9%9F%E5%AD%98%E5%A5%BD)
	- [print function](https://github.com/hlyliang/1093349_proj2/edit/main/README.md#%E6%8A%8A%E6%89%80%E6%9C%89%E7%9A%84%E7%B5%90%E6%9E%9C%E9%83%BD%E5%8D%B0%E5%87%BA%E4%BE%86%E4%BB%A5%E5%8F%8A%E7%8F%BE%E5%9C%A8%E6%98%AF%E4%BD%BF%E7%94%A8%E5%93%AA%E5%80%8Bentry)
	- [update function](https://github.com/hlyliang/1093349_proj2/edit/main/README.md#%E6%9B%B4%E6%96%B0predictor%E8%A3%A1%E9%9D%A2%E7%9A%84%E5%80%BC)
***

# 題目
做一個3-bit Histroy Predictor , 並且可以指定entry個數。

***

# 輸入輸出範例
#### input
![1](https://user-images.githubusercontent.com/103658997/169656179-5cdc3adb-26a8-437e-8a46-852b65a6155a.png)

#### output
![2](https://user-images.githubusercontent.com/103658997/169660390-1ab7a7e9-4bbe-4fec-8d83-93b1385ac17f.png)



***

# 說明文件

- ##### 全域變數
```c++
int entrynum; //使用者輸入entry個數
int current_entrynum; //當前在使用的entry
vector<string> input; //使用者輸入的code
map<string, int> reg; //存reg的值
bool taken = false; //判斷是否taken
string const_state[8] = { "SN","WN1","WN2","WN3","WT1","WT2","WT3","ST" }; //存他的狀態是如何變化
vector <struct table >form; //存每一個entry的狀態
string outcome; //實際的結果是如何
string endstring; //branch到哪
bool endfunc = false; //是否結束程式了
```
***
- ##### 建立一個struct去存3-bit hostory 、 state 和 prediction 
```c++
struct table
{
	string history = "000";
	int state[8] = { 0 };
	string prediction = "N";	
};
```
***
- ##### 用使用者輸入entry個數以及程式碼，並將字串做分割。
```c++
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
		input_buffer = input[i]; //先將要被分割的字串存入buffer裡
		if (not_inst > 0)
			j -= not_inst; //如果有不是addi或beq的行數就要減掉
		current_entrynum = j% entrynum; //計算要用哪個entry
		size_t pos = 0;
		while ((pos = input[i].find(space_delimiter1)) != string::npos) //遇到空白就分割
		{
			input_temp.push_back(input[i].substr(0, pos));
			input[i].erase(0, pos + space_delimiter1.length());
		}
		while ((pos = input[i].find(space_delimiter2)) != string::npos) //遇到逗號就分割
		{
			input_temp.push_back(input[i].substr(0, pos));
			input[i].erase(0, pos + space_delimiter2.length());
		}
		input_temp.push_back(input[i]); //把剩下的也存入
		if (input_temp.size() < 3)
			not_inst++; //分割完的數量如果<3就代表他可能是loop:或是end:之類的
		input[i] = input_buffer;
		cout << input[i] << endl;
		convert(input_temp,i);
		input_temp.clear();
	}
}
```
***
- ##### 判斷使用者輸入的code是哪一種，並把reg的值也存好
- ##### 判斷branch是否會taken
- ##### 如果branch會發生的話，找到要跳去哪一行執行
```c++
void convert(vector<string> input_temp,int &i)
{
	if (input_temp[0] == "addi") //使用者輸入addi的case
	{
		taken = false; 
		outcome = "N";
		if (input_temp[1] == input_temp[2])
		{
			reg[input_temp[1]] += stoi(input_temp[3]); //把值放入reg裡
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
	else if (input_temp[0] == "beq") //beq的case
	{
		if (reg[input_temp[1]] != reg[input_temp[2]]) //判斷reg內的值是否相等，如果不等於的話就不會做
		{
			taken = false;
			outcome = "N";
		}
		else //相等的話代表taken
		{
			taken = true;
			outcome = "T";
			endstring = input_temp[3]; //把要跳去的地方存起來
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
			if (find_branch == true) //找到要跳去的地方並把該位置記錄在i
			{
				i = j;
				if (j == input.size() - 1) //如果要跳去的地方是最後一行的話，會回傳該行的前一行，才能把predictor的內容印出來
					i--;
				break;
			}
		}
	}
}
```
***
- ##### 把所有的結果都印出來，以及現在是使用哪個entry
```c++
void print(vector <struct table >&form)
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

	update(form);

}
```
***
- ##### 更新predictor裡面的值
```c++
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
```
