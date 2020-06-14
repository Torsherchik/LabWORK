#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "cstdlib"

using namespace std;

struct parameters {
    int id = 0;
    bool survival = false;
    int Pclass = 0;
    string name = " ";
    string sex;
    int age = 0;
    int sibsp = 0;
    int parch = 0;
    string ticket = " ";
    double fare = 0.0;
    string cabin = " ";
    string  embarked = "0";
};

parameters help(string str) {
    parameters param;
    string temp[13];
    int i = 0;

    for (int j = 0; j < 13; j++) {
        while (str[i] != ',' && str[i] != '\0') {
            temp[j] += str[i];
            i++;
        }
        i++;
    }

    param.id = atoi(temp[0].c_str());
    param.survival = atoi(temp[1].c_str());
    param.Pclass = atoi(temp[2].c_str());
    param.name = temp[3] + " " + temp[4];
    param.sex = temp[5];
    param.age = atoi(temp[6].c_str());
    param.sibsp = atoi(temp[7].c_str());
    param.parch = atoi(temp[8].c_str());
    param.ticket = temp[9];
    param.fare = atof(temp[10].c_str());
    param.cabin = temp[11];
    param.embarked = temp[12];
    return param;
}

int main()
{
    ifstream fin("train.csv");
    vector <string> temp;
    string str;
    while (getline(fin, str, '\r'))
    {
        if (str.size() > 0)
            temp.push_back(str);
    }
    fin.close();

    int SurvALL = 0, Cl1surv = 0, Cl2surv = 0, Cl3surv = 0, FemaleSurv = 0, MaleSurv = 0, FemaleALL = 0, MaleALL = 0, C = 0, Q = 0, S = 0;
    vector<int> underage;
    double FemaleCount = 0.0, MaleCount = 0.0, AgeALL = 0;
    for (int i = 1; i < temp.size(); i++) {
        parameters arg = help(temp[i]);
        if (arg.survival == 1) {
            SurvALL++;
            arg.sex == "male" ? MaleSurv++ : FemaleSurv++;
        
            switch (arg.Pclass) {
                case 1:
                    Cl1surv++;
                    break;
                case 2:
                    Cl2surv++;
                    break;
                case 3:
                    Cl3surv++;
                    break;
            }
        }

        if (arg.age < 18) underage.push_back(arg.id);
 
        if (arg.embarked == "C") C++;
    
        if (arg.embarked == "S") S++;
        
        if (arg.embarked == "Q") Q++;
        

        if (arg.sex == "male") {
            MaleALL++;
            MaleCount += arg.age;
        }
        else {
            FemaleALL++;
            FemaleCount += arg.age;
        }
        AgeALL += arg.age;
    }

    string embarked;
    if (C > S) {
        if (C > Q) {
            embarked = "Cherbourg";
        }
        else embarked = "Queenstown";
    }
    else
    {
        if (S > Q) {
            embarked = "Southampton";
        }
        else embarked = "Queenstown";
    }


    ofstream fout("Answer.txt");
    fout << "Общее число выживших: " << SurvALL << endl;
    fout << "Число выживших из 1 класса: " << Cl1surv << endl;
    fout << "Число выживших из 2 класса: " << Cl2surv << endl;
    fout << "Число выживших из 3 класса: " << Cl3surv << endl;
    fout << "Количество выживших женщин: " << FemaleSurv << endl;
    fout << "Количество выживших мужчин: " << MaleSurv << endl;
    fout << "Средний возраст пассажира: " << AgeALL / (temp.size() - 1) << endl;
    fout << "Средний женский возраст: " << FemaleCount / FemaleALL << endl;
    fout << "Средний мужской возраст: " << MaleCount / MaleALL << endl;
    fout << "Штат, в котором село больше всего пассажиров: " << embarked << endl;
    fout << "Cписок идентификаторов несовершеннолетних (младше 18) пассажиров: ";
    cout << FemaleALL << " " << MaleALL << " " << temp.size();

    for (int i = 0; i < underage.size(); i++) {
        if (i == underage.size() - 1) {
            fout << underage[i] << ".";
            fout.close();
            return 0;
        }
        fout << underage[i] << ", ";
    }
}