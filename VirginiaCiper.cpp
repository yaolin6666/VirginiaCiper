// VirginiaCiper.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
using namespace std;

string encrypt(string m,string key) {
    string c="";
    int key_length = key.size(), m_len = m.size();
    int j = 0;
    for (int i = 0; i < m_len; i++) {
        if (m[i] >= 'a' && m[i] <= 'z')
        {
            c += (m[i] - 'a' + key[j] - 'a') % 26 + 'a';
        }
        else if (m[i] >= 'A' && m[i] <= 'Z')
        {
            c += (m[i] - 'A' + key[j] - 'A') % 26 + 'A';
        }
        else
        {
            c += m[i];
        }
        j = (j+1) % key_length;
    }
    return c;
}
string decrypt(string c, string key) {
    string m = "";
    int key_length = key.size(), c_len = c.size();
    int j = 0;
    for (int i = 0; i < c_len; i++) {
        if (c[i] >= 'a' && c[i] <= 'z')
        {
            m += (c[i] +26- key[j] ) % 26 + 'a';
            //c[i]-'a'+26-(key[j]-'a')
        }
        else if (c[i] >= 'A' && c[i] <= 'Z')
        {
            m += (c[i] +26- key[j]) % 26 + 'A';
        }
        else
        {
            m += c[i];
        }
        j = (j + 1) % key_length;
    }
    return m;
}
int main()
{   
    string m = "trump donddddd";
    string key = "win";
    string c = encrypt(m, key);
    cout << c<<"\n";
    c = "pzhix zwazlqzl";
    m = decrypt(c, key);
    cout << m<<"\n";
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
