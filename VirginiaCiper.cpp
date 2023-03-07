// VirginiaCiper.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include <vector>
using namespace std;

//频率表 计算拟重合指数使用
double freq_table[] = {
8.167e-2,
1.492e-2,
2.782e-2,
4.253e-2,
12.702e-2,
2.228e-2,
2.015e-2,
6.094e-2,
6.966e-2,
0.153e-2,
0.772e-2,
4.025e-2,
2.406e-2,
6.749e-2,
7.507e-2,
1.929e-2,
0.095e-2,
5.987e-2,
6.327e-2,
9.056e-2,
2.758e-2,
0.978e-2,
2.360e-2,
0.150e-2,
1.974e-2,
0.074e-2,
};

//加密
string encrypt(string m,string key) {
    string c="";
    transform(key.begin(), key.end(), key.begin(), tolower);
    int key_length = key.size(), m_len = m.size();
    int j = 0;
    for (int i = 0; i < m_len; i++) {
        if (m[i] >= 'a' && m[i] <= 'z')
        {
            c += (m[i] - 'a' + key[j] - 'a') % 26 + 'a';
            j = (j + 1) % key_length;
        }
        else if (m[i] >= 'A' && m[i] <= 'Z')
        {
            c += (m[i] - 'A' + key[j] - 'a') % 26 + 'A';
            j = (j + 1) % key_length;
        }
        else
        {
            c += m[i];
        }
    }
    return c;
}


//解密
string decrypt(string c, string key) {
    string m = "";
    transform(key.begin(), key.end(), key.begin(), tolower);
    int key_length = key.size(), c_len = c.size();
    int j = 0;
    for (int i = 0; i < c_len; i++) {
        if (c[i] >= 'a' && c[i] <= 'z')
        {
            m += (c[i] - 'a' + 26 - (key[j] - 'a')) % 26 + 'a';
            j = (j + 1) % key_length;
            //c[i]-'a'+26-(key[j]-'a')
        }
        else if (c[i] >= 'A' && c[i] <= 'Z')
        {
            m += (c[i] - 'A' + 26 - (key[j] - 'a')) % 26 + 'A';
            j = (j + 1) % key_length;
        }
        else
        {
            m += c[i];
        }
    }
    return m;
}
//重合指数计算
double IndexCalculate(string ciper_text) {
    transform(ciper_text.begin(), ciper_text.end(), ciper_text.begin(), toupper);//字母全部大写
    double index = 0;
    double size = ciper_text.size();
    long sum = 0;
    int temp[26] = {};
    for (int i = 0; i < size; i++)
    {
        temp[ciper_text[i] - 'A']++;
    }
    for (int i = 0; i < 26; i++)
    {
        sum = sum + temp[i] * (temp[i] - 1);
    }

    index = (sum) / (size * (size - 1));
    return index;
}


/*
* 破解中确定密钥长度使用了弗里德曼检验
* 相关记录https://zhuanlan.zhihu.com/p/552942664?utm_id=0
* 我挺好奇使用长随机key这边能计算出什么
*/
double KeyLengthEstimate(string ciper_text) {
    double index = IndexCalculate(ciper_text);
    double keyLength = (0.0265 * ciper_text.size()) / ((0.065 - index) + ciper_text.size() * (index - 0.0385));
    /*
    *公式暂时没理解 直接拿来用的 或者通过计算分组的重合指数确定长度
    */
    return keyLength;
}

//拟重合指数计算
double FitIndexCalculate(string ciper_text) {
    transform(ciper_text.begin(), ciper_text.end(), ciper_text.begin(), toupper);//字母全部大写
    double index = 0;
    double size = ciper_text.size();
    double sum = 0;
    int temp[26] = {};
    for (int i = 0; i < size; i++)
    {
        temp[ciper_text[i] - 'A']++;
    }
    for (int i = 0; i < 26; i++)
    {
        sum = sum + temp[i] * freq_table[i];
    }

    index = sum / size;
    return index;
}

int maxIndex(double indexList[]) {
    double max=indexList[0];
    int valueReturn=0;
    for (int i = 1; i < 26; i++)
    {
        if (max < indexList[i])
        {
            valueReturn = i;
            max = indexList[i];
        }
    }
    return valueReturn;
}

//分组确定key
//弗里德曼检验确定的长度为实数 长度并非四舍五入 双边都有可能
void generateKey(string ciper_text) {
    int key_length = 0;
    key_length = (int)KeyLengthEstimate(ciper_text);
    vector<string> lowSubString;
    vector<string> upSubString;
    int n_low = ciper_text.size() / key_length;
    int l_low = (int)(ciper_text.size() % key_length);

    for (int i = 0; i < key_length; i++) {
        string temp = "";
        for (int j = 0; j < n_low; j++)
        {
            temp += ciper_text[key_length * j + i];
        }
        lowSubString.push_back(temp);
    }
    for (int i = 0; i < l_low; i++) {
        lowSubString[i] += ciper_text[key_length * n_low + i];
    }
    //低位分组

    key_length++;
    int n_up = ciper_text.size() / key_length;
    int l_up = (int)(ciper_text.size() % key_length);
    for (int i = 0; i < key_length; i++) {
        string temp = "";
        for (int j = 0; j < n_up; j++)
        {
            temp += ciper_text[key_length * j + i];
        }
        upSubString.push_back(temp);
    }
    for (int i = 0; i < l_up; i++) {
        upSubString[i] += ciper_text[key_length * n_up + i];
    }

    //高位分组
    

    //低位密码确定
    string finalKey_low = "";
    int offset_low = 0;
    for (int i = 0; i < lowSubString.size(); i++)
    {
        string tempCiper;
        double fitIndexList[26] = {};
        string key = "a";
        for (int j = 0; j < 26; j++)
        {
            tempCiper = decrypt(lowSubString[i], key);//解密后确定字符串 计算拟重合指数
            fitIndexList[j] = FitIndexCalculate(tempCiper);
            key[0] ++;
        }
        offset_low = maxIndex(fitIndexList);            //根据拟重合指数确定密钥
        finalKey_low += ('a' + offset_low);
    }

    //高位密钥确定
    string finalKey_up = "";
    int offset_up = 0;
    for (int i = 0; i < upSubString.size(); i++)
    {
        string tempCiper;
        double fitIndexList[26] = {};
        string key = "a";
        for (int j = 0; j < 26; j++)
        {
            tempCiper = decrypt(upSubString[i], key);//解密后确定字符串 计算拟重合指数
            fitIndexList[j] = FitIndexCalculate(tempCiper);
            key[0] ++;
        }
        offset_up = maxIndex(fitIndexList);            //根据拟重合指数确定密钥
        finalKey_up += ('a' + offset_up);
    }
    cout << "低位密钥:" << finalKey_low << "\n";
    cout << "高位密钥:" << finalKey_up << "\n";
}

int main()
{   
    string ciper_text = "CQKATIYRCZXEGVXPCTEWDCBJVCIDTWFRJMUYUQVAQUIMPMGRUWYUKVZQHCLCNIZCUBTGNCGGVATLFKHLVZHJUCKDCKXQVPXPGIKCXIKGCBBMPABLRIKRKKNJCZTGTKKYHBYMTMQYOXECCLXJVIPGPOTGTKKYHBPMWTWLQBGCEMLQCZBJAXHQUMLQCPHPKHHLVIERCQEYNBAMWOARJQLGUXKCUMGRKVTACVTPFKHLHQZSTIMGQVLSEPTQVPTRQNMFGMNPQNBEJBXPGIVFEWFNQVXLVPTQQVXMTUHPGAICEQYGENNLEBBMPATLFUNQVJXBGABEPMWRQMGQWZXRJIMGVKTLEIKPAWNRVPXQGNNLEBBMPALYHMEWKVMFKAVFCXMCTEXBGAVPKJXRJMOYTQHSUTHYFAMMYPBAJIBPEZTDVKHKRWGCPBLYTMLSDRXAVMWRJMBPHCGAVQHLCVWDCJKGEIMGQVTLFBACFMLGIVHDEWGLGKMGQVLQRIVCEZTDVIIYTBYPQUMFGAIYEMLFWBMJGEAGEPAYFIFMTMHPNMLQEWGTGVMGQVTJNIRMWBVMPABQVOXLGZTJNGHDCTHLIVTPTWPRWJXAQVMYKVBLIBACVPKSUBLRTCVRWZXDWMERCVDQCVWNCGEMCLPCUPTJNMQYOQGCUCVFUBKSEBNPGABLUWFCFMMYKTTJVPHSIPTAQUIPGPXLUQOCUBNBAWYQRIVCEZTDVLXQKOGGUWNRUQWCVPXQEWICQNMFKAUMQSYMTANAJAMSFQXQTMYCTMGAGAAMWTWZGUTBGBHYPGHDVPXQGDXPCTMCZBLYXIBJCJECVGIGEIEMHEAGEPBQVPTRDGPGLSXP";
    string m = "Aircraft are generally built up from the basic components of wings, fuselages, tail units, and control surfaces. There are variations in particular aircraft: for example, a delta wing aircraft would not necessarily possess a horizontal tail, although this is present in a canard configuration, such as that of the Eurofighter. Each component has one or more specific functions and must be designed to ensure that it can carry out these functions safely. In this chapter, we describe the various loads to which aircraft components are subjected, their function and fabrication, and the design of connections. Spacecraft, apart from the Space Shuttle, which had a more or less conventional layout, consist generally of a long narrow tube containing the thrust structure, fuel tanks, and payload. We shall examine such structures in some detail, although a comprehensive study of spacecraft design is outside the scope of this book. For such studies, reference should be made to any of the several texts available, typical of which is that by Wijker.";
    string key = "test";
    string ciper_text_output = encrypt(m, key);
    cout << "ciper_text:\n" << ciper_text_output << endl;
    string decrept_text_output = decrypt(ciper_text_output, key);
    cout << "decrept_text_output:\n" << decrept_text_output << endl;
    generateKey(ciper_text);
}
