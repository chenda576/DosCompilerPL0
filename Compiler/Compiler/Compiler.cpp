#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include<sstream>
using namespace std;

const string codeTable[30] = { "", "beginsym","callsym","constsym","dosym", "endsym", "ifsym", "oddsym",
"proceduresym", "readsym", "thensym", "varsym", "whilesym", "writesym",//13 
"semicolon"	,"comma","period","Lparen","rparen","plus", "minus", "times",
"slash", "eql", "neq", "lss", "gtr","leq", "geq", "becomes" };
const string sysword[15] = { "BEGIN","CALL","CONST"/*3*/,"DO","END","IF","ODD","PROCEDURE"/*8*/,
"READ","THEN","VAR"/*11*/,"WHILE","WRITE" };
const char opsym[15] = { ';',',','.','(',')','+','-','*','/','=','#', '<','>' };
const string dbopsys[5] = { "<=", ">=",":=" };/*+14*/
											  /*数字为19 标识符为50*/

bool notIn(int a, int *b, int n)
{
	for (int i = 0;i < n;i++)
		if (a == b[i])
			return false;
	return true;
}

int inSysword(string s)
{
	for (int i = 0;i <13;i++)
	{
		if (s == sysword[i])
			return i + 1;
	}
	return 0;
}

int inSGOpSym(char s)
{
	for (int i = 0;i < 12;i++)
	{
		if (s == opsym[i])
			return i;
	}
	return -1;
}

int inDBOpSym(char s1, char s2)
{

	if (s2 == '=')
	{
		if (s1 == opsym[11])
			return 13;
		if (s1 == opsym[12])
			return 14;
		if (s1 == ':')
			return 15;
	}
	else
	{
		int i;
		i = inSGOpSym(s1);
		return i;
	}
	return -1;
}

int inS(string *s, string a, int n)
{
	for (int i = 0;i < n;i++)
	{
		if (s[i] == a)
			return i;
	}
	return -1;
}

bool isNumber(string s)
{
	for (int i = 0;i < int(s.length());i++)
	{
		if (!(s[i] >= 48 && s[i] <= 57))
			return false;
	}
	return true;
}

int findLast17(int *vacTag, int i, int *no, int n)
{
	for (int j = i - 1;j >= 0;j--)
	{
		if (vacTag[j] == 17)
			if (notIn(j, no, n))
				return j;
	}
	return -1;
}

int findLast17(string *s, int i)
{
	for (int j = i - 1;j >= 0;j--)
	{
		if (s[j] == "(")
			return j;
	}
	return -1;
}

int findNext18(int *vacTag, int i, int k, int *no, int n)
{
	for (int j = i + 1;j <k;j++)
		if (vacTag[j] == 18)
			if (notIn(j, no, n))
				return j;
	return k;
}

int findOP(string *s, int i, int j)
{
	for (int k = i + 1;k < j;k = k + 2)
		if (s[k + 1] == "*" || s[k + 1] == "/")
			return k + 1;
	for (int k = i + 1;k < j;k = k + 2)
		if (s[k + 1] == "+" || s[k + 1] == "-")
			return k + 1;
	return -1;
}

bool checkIn(int *vacTag, int i, int j, int *noUseTag, int no)
{
	for (int k = i + 1;k <= j - 1;k++)
	{
		if (vacTag[k] == 17)
		{
			if (k - 1 >= 0)
			{
				if (vacTag[k - 1] >= 17 && vacTag[k - 1] <= 22 && vacTag[k - 1] != 18)
				{
					k = findNext18(vacTag, k, j, noUseTag, no);
					if ((vacTag[k + 1] < 19 || vacTag[k + 1]>22) && vacTag[k + 1] != 0)
						return false;
					else
						continue;
				}
				else
					return false;
			}
		}
		else if (vacTag[k] == 50 || vacTag[k] == 80)
		{
			if ((vacTag[k + 1] >= 18 && vacTag[k + 1] <= 22) || k + 1 == j)
				k = k + 1;
			else
				return false;
		}
		else if (vacTag[k] >= 19 && vacTag[k] <= 22)
		{
			if (k - 1 >= 0)
			{
				if (vacTag[k - 1] != 18 && vacTag[k - 1] != 50 && vacTag[k - 1] != 80)
					return false;
			}
			else
				return false;
		}
		else
			return false;
	}
	return true;
}

int calIn(string *s, int i, int j, char *a)
{
	char filename[100];
	memset(filename, 0, 100);
	strcpy_s(filename, a);
	ofstream fout(filename, ios::app);
	int op1, op2, result = 0;
	int k = 0;
	while (true)
	{
		int k = findOP(s, i, j);
		if (k == -1)
		{
			break;
		}
		stringstream ss1, ss2;
		ss1 << s[k - 1];
		ss1 >> op1;
		ss2 << s[k + 1];
		ss2 >> op2;
		if (s[k] == "*")
			result = op1*op2;
		else if (s[k] == "/")
		{
			if (op2 == 0)
			{
				fout << "除数不能为0！" << endl;
				cout << "除数不能为0！" << endl;
				return 0;
			}
			else
				result = op1 / op2;
		}
		else if (s[k] == "+")
			result = op1 + op2;
		else
			result = op1 - op2;
		fout << "(" << s[k] << "\t" << op1 << "\t" << op2 << "\t" << result << ")" << endl;
		cout << "(" << s[k] << "\t" << op1 << "\t" << op2 << "\t" << result << ")" << endl;
		stringstream ss;
		ss << result;
		s[k - 1] = ss.str();
		s[k] = s[k - 1];
		s[k + 1] = s[k - 1];
	}
	return result;
}

int calIn(string *s, int i, int j)
{
	int op1, op2, result = 0;
	int k = 0;
	while (true)
	{
		int k = findOP(s, i, j);
		if (k == -1)
		{
			break;
		}
		stringstream ss1, ss2;
		ss1 << s[k - 1];
		ss1 >> op1;
		ss2 << s[k + 1];
		ss2 >> op2;
		if (s[k] == "*")
			result = op1*op2;
		else if (s[k] == "/")
		{
			if (op2 == 0)
			{
				cout << "除数不能为0！" << endl;
				return 0;
			}
			else
				result = op1 / op2;
		}
		else if (s[k] == "+")
			result = op1 + op2;
		else
			result = op1 - op2;
		stringstream ss;
		ss << result;
		s[k - 1] = ss.str();
		s[k] = s[k - 1];
		s[k + 1] = s[k - 1];
	}
	return result;
}

class receiver
{
protected:
	string *vac;
	string *sent;
	int sentNum;
	int vacNum;
	int *vacTag;

public:
	receiver(char *a, char *b);
	void input(char *a);
	void sentToVac();
	void anal(char *a);
	void show(char *a);
	friend class counter;
	friend class caculator;
};

class counter
{
private:
	string *aimVac;
	int *aimVacNum;

public:
	counter();
	int countIdent(receiver rec);
	void show(receiver rec, char *a);
};

class caculator
{
private:
	int *noUseTag;
	int result;
	bool correct;
	int *T;
	string *S;

public:
	caculator();
	bool check(receiver rec, char *a);
	bool checkCal(receiver rec);
	bool caculator::checkCal(receiver rec, int i, int j);
	int cal(receiver rec, int a, int b);
	int cal(receiver rec, char *filename);
	void cmbCal(receiver rec, char *a);
};

receiver::receiver(char *a, char *b)
{
	vac = new string[1000];
	memset(vac, 0, 1000);
	vacNum = 0;
	sent = new string[100];
	memset(sent, 0, 100);
	sentNum = 0;
	vacTag = new int[1000];
	memset(vacTag, 0, 1000);
	input(a);
	sentToVac();
	anal(b);
}

void receiver::input(char *a)
{
	char filename[100];
	memset(filename, 0, 100);
	strcpy_s(filename, a);
	ifstream fin(filename, ios::in);
	char str[100];
	while (!fin.eof())
	{
		fin.getline(str, 100);
		string buf;
		buf = str;
		transform(buf.begin(), buf.end(), buf.begin(), ::toupper);
		sent[sentNum] = buf;
		cout << sent[sentNum] << endl;
		sentNum++;
	}
	fin.close();
}

void receiver::sentToVac()
{
	int r;
	string s;
	for (int i = 0;i < sentNum;i++)
	{
		istringstream is(sent[i]);
		while (is >> s)
		{
			for (int j = 0;j <= int(s.length() - 1);j++)
			{
				if (j == s.length() - 1)
					r = inSGOpSym(s[j]);
				else
					r = inDBOpSym(s[j], s[j + 1]);
				if (r != -1)
				{
					if (j != 0)
					{
						vac[vacNum] = s.substr(0, j);
						++vacNum;
					}
					if (r > 12)
					{
						vac[vacNum] = s[j];
						vac[vacNum] += s[j + 1];
						s = s.substr(j + 2);
					}
					else
					{
						vac[vacNum] = s[j];
						s = s.substr(j + 1);
					}
					vacTag[vacNum] = r + 14;
					++vacNum;
					j = -1;
				}
				if ((j == s.length() - 1) && r == -1)
				{
					vac[vacNum] = s;
					++vacNum;
				}
			}
		}
	}
}

void receiver::anal(char *a)
{
	bool correct = true;
	int s = 0;
	int c = 0;
	char filename[100];
	memset(filename, 0, 100);
	strcpy_s(filename, a);
	ofstream fout(filename, ios::app);
	for (int i = 0;i < vacNum;i++)
	{
		if (vacTag[i] == 0)
		{
			s = inSysword(vac[i]);
			if (s != 0)
			{
				vacTag[i] = s;
				switch (vacTag[i])
				{
				case 3:
					while (correct == true)
					{
						if (vac[i + 1][0] >= 65 && vac[i + 1][0] <= 90 &&
							vacTag[i + 2] == 23 && isNumber(vac[i + 3]) &&
							(vacTag[i + 4] == 14 || vacTag[i + 4] == 15))
						{
							vacTag[i + 1] = 50;
							vacTag[i + 3] = 80;
							i += 4;
							c = c + 1;
							if (vacTag[i] == 14)
								break;
						}
						else
						{
							correct = false;
							for (int k = 1;k <= c;k++)
							{
								if (vacTag[i] == 3)
									vacTag[i] = 0;
								vacTag[i + 1 - 4 * k] = 0;
								vacTag[i + 3 - 4 * k] = 0;
							}
							fout << "error in " << i << "'s word!" << endl;
							cout << "error in " << i << "'s word!" << endl;
						}
					}
					c = 0;
					break;
				case 8:
					while (correct == true)
					{
						if (vac[i + 1][0] >= 65 && vac[i + 1][0] <= 90 && vacTag[i + 2] == 20)
						{
							vacTag[i + 1] = 50;
							i += 2;
							if (vacTag[i] == 20)
								break;
						}
						else
						{
							correct = false;
							fout << "error in " << i << "'s word!" << endl;
							cout << "error in " << i << "'s word!" << endl;
						}
					}
					break;
				case 11:
					while (correct == true)
					{
						if (vac[i + 1][0] >= 65 && vac[i + 1][0] <= 90
							&& (vacTag[i + 2] == 14 || vacTag[i + 2] == 15))
						{
							vacTag[i + 1] = 50;
							i += 2;
							c = c + 1;
							if (vacTag[i] == 14)
								continue;
							if (vacTag[i] == 15)
								break;
						}
						else
						{
							correct = false;
							for (int k = 1;k <= c;k++)
							{
								if (vacTag[i] == 11)
									vacTag[i] = 0;
								vacTag[i + 1 - 2 * k] = 0;
							}
							fout << "error in " << i << "'s word!" << endl;
							cout << "error in " << i << "'s word!" << endl;
						}
					}
					c = 0;
					break;
				}
			}
			else
			{
				if (isNumber(vac[i]))
					vacTag[i] = 80;
				else
					vacTag[i] = 50;
			}
		}
	}
}

void receiver::show(char *a)
{
	char filename[100];
	memset(filename, 0, 100);
	strcpy_s(filename, a);
	ofstream fout(filename, ios::app);
	for (int i = 0;i < vacNum;i++)
	{
		fout << vac[i] << ',' << '\t' << vacTag[i] << endl;
		cout << vac[i] << ',' << '\t' << vacTag[i] << endl;
	}
}

counter::counter()
{
	aimVac = new string[200];
	memset(aimVac, 0, 200);
	aimVacNum = new int[200];
	memset(aimVacNum, 0, 200);
}

int counter::countIdent(receiver rec)
{
	int identNum = 0;
	int r;
	for (int i = 0;i < rec.vacNum;i++)
	{
		if (rec.vacTag[i] == 50)
		{
			r = inS(aimVac, rec.vac[i], identNum);
			if (r != -1)
			{
				++aimVacNum[r];
			}
			else
			{
				aimVac[identNum] = rec.vac[i];
				++aimVacNum[identNum];
				++identNum;
			}
		}
	}
	return identNum;
}

void counter::show(receiver rec, char *a)
{
	char filename[100];
	memset(filename, 0, 100);
	strcpy_s(filename, a);
	ofstream fout(filename, ios::out);
	fout << "第一题：" << endl;
	cout << "第一题：" << endl;
	int s = 0;
	s = countIdent(rec);
	for (int i = 0;i < s;i++)
	{
		fout << '(' << aimVac[i] << ':' << '\t' << aimVacNum[i] << ')' << endl;
		cout << '(' << aimVac[i] << ':' << '\t' << aimVacNum[i] << ')' << endl;
	}
	/*fout << endl << "第二题：" << endl;
	cout << endl << "第二题：" << endl;
	for (int i = 0;i < rec.vacNum;i++)
	{
		cout << '(';
		if (rec.vacTag[i] == 50)
		{
			fout << "Ident";
			cout << "Ident";
		}
		else if (rec.vacTag[i] == 80)
		{
			fout << "number";
			cout << "number";
		}
		else
		{
			fout << codeTable[rec.vacTag[i]];
			cout << codeTable[rec.vacTag[i]];
		}
		fout << ',' << '\t' << rec.vac[i] << ')' << endl;
		cout << ',' << '\t' << rec.vac[i] << ')' << endl;
	}*/
	fout.close();
}

caculator::caculator()
{
	correct = true;
	result = 0;
	noUseTag = new int[50];
	memset(noUseTag, -1, 50);
	T = new int[100];
	memset(T, 0, 100);
	S = new string[100];
	memset(S, 0, 100);
}

bool caculator::check(receiver rec, char *a)
{
	char filename[100];
	memset(filename, 0, 100);
	strcpy_s(filename, a);
	ofstream fout(filename, ios::app);
	int j = 0;
	bool d = true;
	int No = 0;
	for (int i = 0;i < rec.vacNum;i++)
	{
		if (rec.vacTag[i] == 18)
		{
			noUseTag[No] = i;
			++No;
			j = findLast17(rec.vacTag, i, noUseTag, No);
			if (j == -1)
			{
				correct = false;
				fout << "语法错误！" << endl;
				fout << "第" << i + 1 << "个词出错" << endl;
				cout << "语法错误！" << endl;
				cout << "第" << i + 1 << "个词出错" << endl;
				return false;
			}
			else
			{
				noUseTag[No] = j;
				++No;
				correct = checkIn(rec.vacTag, j, i, noUseTag, No);
				if (correct == false)
				{
					fout << "语法错误！" << endl;
					fout << "第" << i + 1 << "个词出错" << endl;
					cout << "语法错误！" << endl;
					cout << "第" << i + 1 << "个词出错" << endl;
					return false;
				}
			}
		}
	}
	if (checkIn(rec.vacTag, -1, rec.vacNum, noUseTag, No))
		return true;
	else
	{
		fout << "语法错误！" << endl;
		fout << "第" << rec.vacNum << "个词出错" << endl;
		cout << "语法错误！" << endl;
		cout << "第" << rec.vacNum << "个词出错" << endl;
		return false;
	}
}

bool caculator::checkCal(receiver rec)
{
	for (int i = 0;i < rec.vacNum;i++)
		if (rec.vacTag[i] != 80 && (rec.vacTag[i] < 17 || rec.vacTag[i]>22))
			return false;
	return true;
}

bool caculator::checkCal(receiver rec, int i, int j)
{
	for (int k = i;k < j;k++)
		if (rec.vacTag[k] != 80 && (rec.vacTag[k] < 17 || rec.vacTag[k]>22))
			return false;
	return true;
}

int caculator::cal(receiver rec, char *filename)
{
	int ss = 0;
	int j = 0;
	int t = 0;
	string s2;
	for (int i = 0;i < rec.vacNum;i++)
	{
		if (rec.vacTag[i] != 18)
			S[ss] = rec.vac[i];
		else if (rec.vacTag[i] == 18)
		{
			j = findLast17(S, i);
			t = calIn(S, j, ss, filename);
			stringstream s1;
			s1 << t;
			s2 = s1.str();
			S[j] = s2;
			ss = j;
			for (int k = j + 1;k < i;k++)
				S[k] = "\0";
		}
		++ss;
	}
	result = calIn(S, -1, ss, filename);
	return result;
}

int caculator::cal(receiver rec, int a, int b)
{
	memset(S, 0, 100);
	int ss = 0;
	int j = 0;
	int t = 0;
	string s2;
	for (int i = a;i < b;i++)
	{
		if (rec.vacTag[i] != 18)
			S[ss] = rec.vac[i];
		else if (rec.vacTag[i] == 18)
		{
			j = findLast17(S, i);
			t = calIn(S, j, ss);
			stringstream s1;
			s1 << t;
			s2 = s1.str();
			S[j] = s2;
			ss = j;
			for (int k = j + 1;k < i;k++)
				S[k] = "\0";
		}
		++ss;
	}
	result = calIn(S, -1, ss);
	return result;
}

void caculator::cmbCal(receiver rec, char *a)
{
	char filename[100];
	memset(filename, 0, 100);
	strcpy_s(filename, a);
	ofstream fout(filename, ios::app);
	int start = 0, end;
	int *s_op0;
	s_op0 = new int[100];
	int k = 0;
	int result;
	string *s_out;
	s_out = new string[100];
	for (int i = 0;i < rec.vacNum;i++)
	{
		s_out[i] = rec.vac[i];
		if (rec.vacTag[i] == 29)
		{
			s_op0[k] = i;
			++k;
		}
		if (rec.vacTag[i] == 14)
		{
			if (((i - s_op0[k - 1]) == 4))
			{
				if (checkCal(rec, s_op0[k - 1] + 1, i) == true)
				{
					result = cal(rec, s_op0[k - 1] + 1, i);
					stringstream ss;
					ss << result;
					ss >> s_out[s_op0[k - 1] + 1];
					s_out[s_op0[k - 1] + 2] = "\0";
					s_out[s_op0[k - 1] + 3] = "\0";
				}
				else
				{
					for (int j = k - 1;j < 0;j++)
					{
						if (rec.vacTag[s_op0[j] + 2] == rec.vacTag[i - 2])
							if (((rec.vac[s_op0[j] + 1] == rec.vac[i - 3]) &&
								(rec.vac[s_op0[j] + 3] == rec.vac[i - 1])) ||
								((rec.vac[s_op0[j] + 3] == rec.vac[i - 3]) &&
									(rec.vac[s_op0[j] + 1] == rec.vac[i - 1])))
							{
								s_out[s_op0[k - 1] + 1] = s_out[s_op0[j] - 1];
								s_out[s_op0[k - 1] + 2] = "\0";
								s_out[s_op0[k - 1] + 3] = "\0";
								break;
							}
					}
				}
			}
			end = i + 1;
			if (((i - s_op0[k - 1]) == 4) && s_out[s_op0[k - 1] + 2] != "\0")
			{
				fout << "(" << s_out[s_op0[k - 1] + 2] << "," << s_out[s_op0[k - 1] + 1] << "," << s_out[s_op0[k - 1] + 3]
					<< "," << s_out[s_op0[k - 1] - 1] << ")" << endl;
				cout << "(" << s_out[s_op0[k - 1] + 2] << "," << s_out[s_op0[k - 1] + 1] << "," << s_out[s_op0[k - 1] + 3]
					<< "," << s_out[s_op0[k - 1] - 1] << ")" << endl;
			}
			else
			{
				fout << "(" << s_out[s_op0[k - 1]] << "," << s_out[s_op0[k - 1] + 1] << "," << " "
					<< "," << s_out[s_op0[k - 1] - 1] << ")" << endl;
				cout << "(" << s_out[s_op0[k - 1]] << "," << s_out[s_op0[k - 1] + 1] << "," << " "
					<< "," << s_out[s_op0[k - 1] - 1] << ")" << endl;
			}
		}
	}
}

int main()
{
	char filename3[100] = ".\\text\\PL_output.txt";
	char filename1[100] = ".\\text\\PL1_input.txt";
	char filename4[100] = ".\\text\\PL3_input.txt";
	receiver rec1(filename1, filename3);
	counter con;
	con.show(rec1, filename3);
	ofstream fout(filename3, ios::app);
	char filename2[100] = ".\\text.\\PL2_input.txt";
	receiver rec2(filename2, filename3);
	caculator calt, calt2;
	fout << endl << "第三题，第四题和第五题：" << endl;
	cout << endl << "第三题，第四题和第五题：" << endl;
	if (calt.check(rec2, filename3))
	{
		if (calt.checkCal(rec2))
		{
			fout << calt.cal(rec2, filename3) << endl;
			cout << calt.cal(rec2, filename3) << endl;
		}
		else
		{
			fout << "语法正确，但有未知变量存在，无法计算" << endl;
			cout << "语法正确，但有未知变量存在，无法计算" << endl;
		}
	}
	else
	{
		fout << "语法错误,无法计算" << endl;
		cout << "语法错误,无法计算" << endl;
	}
	fout << endl << "第六题：" << endl;
	cout << endl << "第六题：" << endl;
	receiver rec3(filename4, filename3);
	calt2.cmbCal(rec3, filename3);
	system("Pause");
}
