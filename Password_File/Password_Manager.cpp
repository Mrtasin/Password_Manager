#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<stdlib.h>
using namespace std;

void red ()     {   cout<<"\033[1;31m";   }
void yellow()   {   cout<<"\033[1;33m";   }
void Blue()     {   cout<<"\033[0;34m";   }
void Green()    {   cout<<"\033[0;32m";   }
void Purple()   {   cout<<"\033[0;35m";   }
void White()    {   cout<<"\033[0;37m";   }
void Cyan()     {   cout<<"\033[0;36m";   }
void reset ()   {   cout<<"\033[0m";      }


struct user_Id  {
    string username;
    string password;
    string AccType;
    user_Id(string uname, string passw, string Acc) {
        username = uname;
        password = passw;
        AccType = Acc;
    }
    void Print_Data()   {
        Green();
        cout<<"\n|---------------------------------------\n";
        cout<<"| User Name    is : "<<username<<endl;
        cout<<"| Password     is : "<<password<<endl;
        cout<<"| Account Name is : "<<AccType<<endl;
        cout<<"|---------------------------------------\n";
        reset();
    }
};

string encrypt(const string& message, int shift, int dig = 0) {
    string encryptedMessage = "";
    if(shift<0)
        shift = -shift;
    if(dig==0)
        dig = shift;
    shift%=26;
    for (char c : message) {
        if (isalpha(c)) {
            char shiftedChar = (isupper(c)) ? 'A' + (c - 'A' + shift) % 26 : 'a' + (c - 'a' + shift) % 26;
            encryptedMessage += shiftedChar;
        }
        else if('0'<=c && '9'>=c)   {
                dig = dig%9;
                char ch = '0' + (c - '0' + dig) % 9;
            encryptedMessage += ch;
        }
        else if(c == '@' || c == '&' || c == '#' || c == '$' || c == '_' || c == '!')   {
            char sm;
            if(c == '@')
                sm = '!';
            else if(c == '&')
                sm = '_';
            else if(c == '#')
                sm = '$';
            else if(c == '$')
                sm = '#';
            else if(c == '_')
                sm = '&';
            else if(c == '!')
                sm = '@';
            encryptedMessage += sm;
        }
        else
            encryptedMessage += c;
    }
    return encryptedMessage;
}

string decrypt(const string& encryptedMessage, int shift) {
    return encrypt(encryptedMessage, 26 - ((shift =shift<0? -shift:shift)%26),9 - ((shift =shift<0? -shift:shift)%9));
}



class Password_Manager
{
    private:
        string User_Name;
        string Login_Password;
        vector<user_Id> List_of_user;
         bool userExists(const string& username)    {
            for (const user_Id &Id : List_of_user)
                if (Id.username == username)
                    return true;
            return false;
        }
    public:
        void Login(string&, string&);
        void New_Register(const string&, string&);
        void Add_Account(const string&, const string&, const string&);
        void search(const string&);
        void Save_Data_to_File();
        void del_userName(string&, string&);
        void Update_Password(const string&,string&, const string&);
        void Print_All_Data();
        void Main_Menu();

};

void Password_Manager::Login(string& uname,string& pass)  {
    string mpin;
    string userName,Password,AccType;
    int len;
    ifstream fin(uname);
    if(fin.is_open())   {
        List_of_user.clear();
        fin>>mpin;
        mpin = decrypt(mpin,uname.size());
        if(pass == mpin)    {
            User_Name = uname;
            Login_Password = mpin;
            while(fin>>userName>>Password>>AccType) {
                Password = decrypt(Password,userName.size());
                List_of_user.push_back(user_Id(userName,Password,AccType));
            }
            fin.close();
            cout << "\nFile Open successful...!\n";
            Main_Menu();
        }
        else    {
            cout<<"Password Wrong"<<endl;
            fin.close();
        }
    }
    else
        cout<<"User Name Wrong"<<endl;
}

void Password_Manager::New_Register(const string &uname,string &pass)
{
    User_Name = uname;
    Login_Password = pass;
    ofstream fout(uname);
    if(fout.is_open())  {
        pass = encrypt(pass,uname.size());
        fout<<pass<<'\n';
        fout.close();
        cout << "\nRegistration successful...!\n";
        Main_Menu();
    }
}


void Password_Manager::Add_Account(const string &uname,const string &pass, const string &AccountName)
{
    if(!userExists(uname))  {
        List_of_user.push_back(user_Id(uname,pass,AccountName));
        cout << "Add Account successful!\n";
    }
    else
        cout << "User name already exists. Try a different username.\n";
}

void Password_Manager::search(const string &AccountType)    {
    for(user_Id &Id : List_of_user)
        if(Id.AccType == AccountType)
            Id.Print_Data();
}

void Password_Manager::Save_Data_to_File()  {
    ofstream fout(User_Name);
    string Name,Pass,AccType;
    if(fout.is_open())  {
        fout<<encrypt(Login_Password,User_Name.size())<<'\n';
        for(const user_Id& Id : List_of_user)   {
            Name = Id.username;
            Pass = Id.password;
            AccType = Id.AccType;
            Pass = encrypt(Pass,Name.size());
            fout<<Name<<' '<<Pass<<' '<<AccType<<'\n';
        }
        fout.close();
    }

}

void Password_Manager::del_userName(string& uname,string& Pass)
{
    if(userExists(uname))
    {
        string Name, Local_Pass,AccType;
        vector<user_Id>::iterator it;
        for(it = List_of_user.begin(); it != List_of_user.end(); it++)
            if(it->username == uname && it->password == Pass)   {
                List_of_user.erase(it);
                cout<<"\n\nDeletion Successful...!\n\n";
                break;
            }

    }
    else
        cout<<"\nData Not Found\n";
}

void Password_Manager::Update_Password(const string &name,string &old_Pass,const string &New_Pass)
{
    vector<user_Id>::iterator it;
    for(it = List_of_user.begin(); it != List_of_user.end(); it++)
        if(it->username == name && it->password == old_Pass)    {
            it->password = New_Pass;
            cout<<"\nYour Password Update Successful..!\n";
            break;
        }
    if(it == List_of_user.end())
        cout<<"\nUsername & Password wrong\n";
}

void Password_Manager::Print_All_Data()
{
    for(user_Id &user : List_of_user)
        user.Print_Data();
}





void Password_Manager::Main_Menu()  {
    string Name,Pass,AccType;
    int choice;
    do  {
        cout<<"\n----------------------------------\n";
        cout<<"Enter 1 For Add New Account : "<<endl;
        cout<<"Enter 2 For Search Account  : "<<endl;
        cout<<"Enter 3 For Delete Username : "<<endl;
        cout<<"Enter 4 For Update Password : "<<endl;
        cout<<"Enter 5 For Print All Data  : "<<endl;
        cout<<"Enter 0 For Back Home Page  : "<<endl;
        cout<<"Enter your choice : ";
        cin>>choice;
        cout<<"\n----------------------------------\n";
        system("cls");
        if(choice == 0)
            Save_Data_to_File();
        switch(choice)  {
            case 1:
                cout<<"\n----------------------------------\n";
                cout<<"Enter Username     : ";
                cin>>Name;
                cout<<"Enter Password     : ";
                cin>>Pass;
                cout<<"Enter Account Name : ";
                cin>>AccType;
                cout<<"\n----------------------------------\n";
                Add_Account(Name,Pass,AccType);
                break;
            case 2:
                cout<<"\n----------------------------------\n";
                cout<<"Enter Account Name : ";
                cin>>AccType;
                cout<<"\n----------------------------------\n";
                search(AccType);
                break;
            case 3:
                cout<<"\n----------------------------------\n";
                cout<<"Enter Username     : ";
                cin>>Name;
                cout<<"Enter Password     : ";
                cin>>Pass;
                cout<<"\n----------------------------------\n";
                del_userName(Name,Pass);
                break;
            case 4: {
                string New_Pas;
                cout<<"\n----------------------------------\n";
                cout<<"Enter Username     : ";
                cin>>Name;
                cout<<"Enter old Password : ";
                cin>>Pass;
                cout<<"Enter New Password : ";
                cin>>New_Pas;
                cout<<"\n----------------------------------\n";
                Update_Password(Name,Pass,New_Pas);
                break;
            }
            case 5:
                Print_All_Data();
                break;
            default:
                cout<<"\n\nEnter valid Choice...!\n\n";

        }
    }while(choice != 0);
}




int main()
{
    Password_Manager P_user;
    string username;
    string Password;
    string AccountType;
    int choice;
    while("TasiNCoder")
    {
        cout<<"Enter 1 for Login :"<<endl;
        cout<<"Enter 2 for New Registration : "<<endl;
        cout<<"Enter 0 for Exit.....! : "<<endl;
        cout<<"Enter your choice : ";
        cin>>choice;
        system("cls");
        switch(choice)  {
            case 1:
                cout<<"Enter UserName : ";
                cin>>username;
                cout<<"Enter Password : ";
                cin>>Password;
                P_user.Login(username,Password);
                break;
            case 2:
                cout<<"Enter UserName : ";
                cin>>username;
                cout<<"Enter Password : ";
                cin>>Password;
                P_user.New_Register(username,Password);
                break;
            case 0:
                P_user.Save_Data_to_File();
                cout<<"Exit....!"<<endl;
                return 0;
            default:
                cout<<"\n\nEnter valid choice...!\n\n";
        }
    }
    return 0;
}


