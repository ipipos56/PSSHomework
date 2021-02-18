
#include <bits/stdc++.h>

using namespace std;

set<string> st;

string alf = "qwertyuiopasdfghjklzxcvbnm";
string alf2 = "QWERTYUIOPASDFGHJKLZXCVBNM";


void dotDivision(string text)
{
    int n = 10;
    while(n != -1)
    {
        n = text.find('.');
        string sent = text.substr(0,n+1);
        while(sent[0] == ' ')
            sent.erase(0,1);
        st.insert(sent);
        text.erase(0,n+1);
    }
}

string uppercaseWord(string word)
{
    for(int i = 0;i<word.size();i++)
    {
        int n;
        n = alf.find(word[i]);
        word[i] = alf2[n];
    }
    return word;
}

void findWord(string word)
{
    int sum = 0;
    set<string> outMess;
    for(auto it : st)
    {
        int n;
        n = it.find(word);
        if(n!= -1) {
            sum++;
            it.erase(n, word.size());
            it.insert(n, uppercaseWord(word));
            outMess.insert(it);
        }
    }
    cout<<sum<<endl;
    if(!outMess.empty())
    {
        for(auto &it : outMess)
        {
            cout<<it<<endl;
        }
    }
}

int main() {
    freopen("in.txt","r",stdin);
    freopen("out.txt","w",stdout);
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);



    string a;
    getline(cin,a);
    dotDivision(a);
    getline(cin,a);
    int m = atoi(a.c_str());
    for(int col = 0;col<m;col++)
    {
        getline(cin,a);
        findWord(a);
    }
    return 0;
}
