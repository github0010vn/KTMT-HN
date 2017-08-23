#include <iostream>
#include <sstream>
#include <string>

using namespace std;

string div2(string value) {
	string res = "";
	int f = 0, d;

	for (int i = 0; i < value.length(); ++i) {
		d = int(value[i] - '0') + 10*f;
		res += char(d/2 + '0');
		f = d%2;
	}

	while (res[0] == '0' && res.length() != 1) {
		res = res.substr(1, res.length() - 1);
	}
	return res;
}
string mul2(string value) {
	string res = "";
	int f = 0, d;
	for (int i = value.length() - 1; i >= 0; --i) {
		d = int(value[i] - '0');
		res = char((d*2 + f)%10 + '0') + res;
		f = (d*2 + f)/10;
	}
	if (f) {
		res = '1' + res;
	}
	return res;
}

class QInt
{
private:
	int q[4];
public:
	QInt() {
		for (int i = 3; i >= 0; --i) {
			q[i] = 0;
		}
	}
	QInt(string value, int base) {
		int k = 0;
		char ch;
		bool f = false;

		switch(base) {
		case 2:
			for (int i = 3; i >= 0; --i) {
				q[i] = 0;
			}
			for (int i = 0; i < value.length(); ++i) {
				if (value[value.length() - i - 1] == '1') {
					q[3 - i/32] |= 1 << (i % 32);
				}
			}
			break;
		case 10:
			for (int i = 3; i >= 0; --i) {
				q[i] = 0;
			}

			if (value[0] == '-') {
				f = true;
				value = value.substr(1, value.length() - 1);
			}

			while (value != "0") {
				if (int(value[value.length() - 1] - '0') % 2 == 1) {
					q[3 - k/32] |= 1 << (k % 32); 
				}
				value = div2(value);
				++k;
			}

			if (f) {
				*this = ~*this;
				*this = this->add1();
			}

			break;
		case 16:
			for (int i = 3; i >= 0; --i) {
				q[i] = 0;
			}

			for (int i = 0; i < value.length(); ++i) {
				ch = value[value.length() - i - 1];
				if (ch == '1' || ch == '3' || ch == '5' || ch == '7' || ch == '9' || ch == 'B' || ch == 'D' || ch == 'F') {
					q[3 - (4*i)/32] |= 1 << ((4*i) % 32); 
				}
				if (ch == '2' || ch == '3' || ch == '6' || ch == '7' || ch == 'A' || ch == 'B' || ch == 'E' || ch == 'F') {
					q[3 - (4*i + 1)/32] |= 1 << ((4*i + 1) % 32);
				}
				if (ch == '4' || ch == '5' || ch == '6' || ch == '7' || ch == 'C' || ch == 'D' || ch == 'E' || ch == 'F') {
					q[3 - (4*i + 2)/32] |= 1 << ((4*i + 2) % 32);
				}
				if (ch == '8' || ch == '9' || ch == 'A' || ch == 'B' || ch == 'C' || ch == 'D' || ch == 'E' || ch == 'F') {
					q[3 - (4*i + 3)/32] |= 1 << ((4*i + 3) % 32);
				}
			}
			break;
		default:
			break;
		}
	}

	string show(int base) {
		string res;
		res = "";
		bool f, f2;
		string tmp;
		string bin[16] = {"0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"};
		char hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
		QInt pos;
		switch(base) {
		case 2:
			for (int i = 3; i >= 0; --i) {
				for (int j = 0; j < 32; ++j) {
					if (q[i] & (1 << j)) {
						res = '1' + res;
					}
					else {
						res = '0' + res;
					}
				}
			}
			while (res[0] == '0' && res.length() != 1) {
				res = res.substr(1, res.length() - 1);
			}
			break;
		case 10:
			if (q[0] & (1 << 31)) {
				pos = ~*this;
				pos = pos.add1();
				f = true;
			}
			else {
				pos = *this;
				f = false;
			}
			res = "0";
			for (int i = 0; i < 4; ++i) {
				for (int j = 31; j >= 0; --j) {
					res = mul2(res);
					if (pos.q[i] & (1 << j)) {
						f2 = true;
						for (int k = res.length() - 1; k >= 0; --k) {
							if (res[k] == '9') {
								res[k] = '0';
							}
							else {
								res[k] = char(int(res[k] - '0') + 1 + '0');
								f2 = false;
								break;
							}
						}
						if (f2) {
							res = '1' + res;
						}
					}
				}
			}
			if (f) {
				res = "-" + res;
			}
			break;
		case 16:
			for (int i = 0; i < 32; ++i) {
				tmp = "";
				for (int j = 0; j < 4; ++j) {
					if (q[3 - (4*i + j)/32] & (1 << ((4*i + j) % 32))) {
						tmp = '1' + tmp;
					}
					else {
						tmp = '0' + tmp;
					}
				}
				for (int j = 0; j < 16; ++j) {
					if (tmp == bin[j]) {
						res = hex[j] + res;
						break;
					}
				}
			}
			while (res[0] == '0' && res.length() != 1) {
				res = res.substr(1, res.length() - 1);
			}
			break;
		default:
			break;
		}

		return res;
	}

	QInt operator=(QInt other) {
		for (int i = 3; i >= 0; --i) {
			q[i] = other.q[i];
		}
		return *this;
	}
	QInt operator+(QInt other) {
		QInt res;
		int mask;
		int flag = 0;

		for (int i = 3; i >= 0; --i) {
			for (int j = 0; j < 32; ++j) {
				mask = 1 << j;
				if (flag) {
					flag = (q[i] & mask) | (other.q[i] & mask);
					res.q[i] |= ((q[i] & mask) ^ (other.q[i] & mask)) ^ mask;
				}
				else {
					flag = (q[i] & mask) & (other.q[i] & mask);
					res.q[i] |= (q[i] & mask) ^ (other.q[i] & mask);
				}
			}
		}
			
		return res;
	}
	QInt operator-(QInt other) {
		QInt res;
		int mask;
		int flag = 0;

		for (int i = 3; i >= 0; --i) {
			for (int j = 0; j < 32; ++j) {
				mask = 1 << j;
				if (flag) {
					flag = (((q[i] & mask) ^ (other.q[i] & mask)) && (q[i] & mask)) ? 0 : 1;
					res.q[i] |= ((q[i] & mask) ^ (other.q[i] & mask)) ^ mask;
				}
				else {
					flag = (((q[i] & mask) ^ (other.q[i] & mask)) && (other.q[i] & mask)) ? 1 : 0;
					res.q[i] |= (q[i] & mask) ^ (other.q[i] & mask);
				}
			}
		}
			
		return res;
	}
	QInt operator*(QInt other) {
		QInt res;
		int mask;
			
		for (int i = 3; i >= 0; --i) {
			for (int j = 0; j < 32; ++j) {
				mask = 1 << j;
				if (other.q[i] & mask) {
					res = res + (*this << ((3 - i)*32 + j));
				}
			}
		}
			
		return res;
	}
	QInt operator/(QInt other) {
		QInt res, tmp = *this, tmp2 = other;
		int mask, cnt = 0;
		bool f1 = false, f2 = false;

		if (tmp.q[0] & (1 << 31)) {
			tmp = ~tmp;
			tmp = tmp.add1();
			f1 = true;
		}

		if (tmp2.q[0] & (1 << 31)) {
			tmp2 = ~tmp2;
			tmp2 = tmp2.add1();
			f2 = true;
		}

		while (!(tmp2.q[0] & (1 << 30))) {
			tmp2 = tmp2 << 1;
			++cnt;
		}

		while (cnt-- >= 0) {
			res = res << 1;
			if (!((tmp - tmp2).q[0] & (1 << 31))) {
				tmp = tmp - tmp2;
				res.q[3] |= 1;
			}
			tmp2 = tmp2 >> 1;
		}

		if (!((f1 && f2) || (!f1 && !f2))) {
			res = res.sub1();
			res = ~res;
		}
			
		return res;
	}

	QInt operator&(QInt& other) {
		QInt res;

		for (int i = 3; i >= 0; --i) {
			res.q[i] = q[i] & other.q[i];
		}

		return res;
	}
	QInt operator|(QInt& other) {
		QInt res;

		for (int i = 3; i >= 0; --i) {
			res.q[i] = q[i] | other.q[i];
		}

		return res;
	}
	QInt operator^(QInt& other) {
		QInt res;

		for (int i = 3; i >= 0; --i) {
			res.q[i] = q[i] ^ other.q[i];
		}

		return res;
	}
	QInt operator~() {
		QInt res;

		for (int i = 3; i >= 0; --i) {
			res.q[i] = ~q[i];
		}

		return res;
	}

	QInt operator<<(int step) {
		QInt res;
		int mask;

		if (step < 0 || step >= 128) return res;

		for (int i = 0; i < 128 - step; ++i) {
			if (q[3 - i/32] & (1 << (i % 32))) {
				res.q[3 - (i + step)/32] |= 1 << ((i + step) % 32);
			}
		}

		return res;
	}
	QInt operator>>(int step) {
		QInt res;
		int mask;

		if (step < 0 || step >= 128) return res;

		for (int i = step; i < 128; ++i) {
			if (q[3 - i/32] & (1 << (i % 32))) {
				res.q[3 - (i - step)/32] |= 1 << ((i - step) % 32);
			}
		}

		return res;
	}

	QInt rol() {
		int f = q[0] & (1 << 31);
		for (int i = 127; i > 0; --i) {
			if (q[3 - (i - 1)/32] & (1 << ((i - 1)%32))) {
				q[3 - i/32] |= 1 << (i%32);
			}
			else {
				q[3 - i/32] &= ~(1 << (i%32));
			}
		}
		if (f) {
			q[3] |= 1;
		}
		else {
			q[3] &= -2;
		}

		return *this;
	}
	QInt ror() {
		int f = q[3] & 1;
		for (int i = 0; i < 127; ++i) {
			if (q[3 - (i + 1)/32] & (1 << ((i + 1)%32))) {
				q[3 - i/32] |= 1 << (i%32);
			}
			else {
				q[3 - i/32] &= ~(1 << (i%32));
			}
		}
		if (f) {
			q[0] |= 1 << 31;
		}
		else {
			q[0] &= ~(1 << 31);
		}

		return *this;
	}

	QInt add1() {
		QInt res = *this;
		int mask;

		for (int i = 3; i >= 0; --i) {
			for (int j = 0; j < 32; ++j) {
				mask = 1 << j;
				if (res.q[i] & mask) {
					res.q[i] &= ~mask;
				}
				else {
					res.q[i] |= mask;
					return res;
				}
			}
		}
			
		return res;
	}
	QInt sub1() {
		QInt res = *this;
		int mask;

		for (int i = 3; i >= 0; --i) {
			for (int j = 0; j < 32; ++j) {
				mask = 1 << j;
				if (res.q[i] & mask) {
					res.q[i] &= ~mask;
					return res;
				}
				else {
					res.q[i] |= mask;
				}
			}
		}
		return res;
	}
};

void solve(int base, string opt, string value, string value2) {
	if (opt.length() == 1) {
		QInt x(value, base);
		QInt y(value2, base);
		if (opt == "+") {
			cout << (x + y).show(base) << endl;
		}
		else if (opt == "-") {
			cout << (x - y).show(base) << endl;
		}
		else if (opt == "*") {
			cout << (x * y).show(base) << endl;
		}
		else if (opt == "/") {
			cout << (x / y).show(base) << endl;
		}
		else if (opt == "&") {
			cout << (x & y).show(base) << endl;
		}
		else if (opt == "|") {
			cout << (x | y).show(base) << endl;
		}
		else if (opt == "^") {
			cout << (x ^ y).show(base) << endl;
		}
	}
	else {
		QInt x(value, base);
		int step = stoi(value2);
		if (opt == "<<") {
			cout << (x << step).show(base) << endl;
		}
		else {
			cout << (x >> step).show(base) << endl;
		}
	}
}
void solve2(string base, string type, string value) {
	QInt x(value, stoi(base));
	if (type == "~") {
		cout << (~x).show(stoi(base)) << endl;
	}
	else if (type == "rol") {
		cout << (x.rol()).show(stoi(base)) << endl;
	}
	else if (type == "ror") {
		cout << (x.ror()).show(stoi(base)) << endl;
	}
	else {	
		cout << x.show(stoi(type)) << endl;
	}
}

int main() {
	string str;
	while(getline(cin, str)) {
		string a, b, c, d;
		stringstream ss(str);
		ss >> a >> b >> c >> d;
		if (c == "+" || c == "-" || c == "*" || c == "/" || c == "&" || c == "|" || c == "^" || c == "<<" || c == ">>") {
			solve(stoi(a), c, b, d);
		}
		else {
			solve2(a, b, c);
		}
	}
	return 0;
}