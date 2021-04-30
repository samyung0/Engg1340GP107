#include <iostream>
#include <string>
#include <cstring>
#include <ctime>
#include <vector>
#include <stdlib.h>
#include <algorithm>

int maphori(int x, int y)
{
    int upper;
    int lower;
    int range = upper - lower + 1;
    int hori;

    srand(time(NULL));
    hori = rand() % range + lower;

    return hori;
}
int mapvert(int x, int y)
{
    int upper;
    int lower;
    int range = upper - lower + 1;
    int vert;

    srand(time(NULL));
    vert = rand() % range + lower;

    return vert;
}

void pointallocate(std::vector<std::vector<std::vector<std::string>>> &countries)
{
    int numberofcountries = countries.size();
    for (int i = 0; i < numberofcountries; ++i){
        int height = countries[i].size();
        int width = countries[i][0].size();
        std::vector<std::vector<int>> corner;
        for (int j = 0; j < height; ++j){
            for (int k = 0; k < width; ++k){
                if (j == 0){
                    
                    srand(time(NULL));
                    int tf = rand()% 2;
                    countries[i][j][k] = std::to_string(tf);
                    if (tf == 1){
                        std::vector <int> point = {j,k};
                        corner.push_back(point);
                        if (k > 0){
                            std::vector <int> leftcorner = {j,k-1};
                            for (int l = 0; l < corner.size(); l++){
                                if (corner[l] == leftcorner){
                                    corner.erase(corner.begin()+l);
                                }
                            }
                        }
                    }
                }
                else if (k == 0){
                    std::vector <int> topcorner = {j-1,k};
                    std::vector <int> point = {j,k};
                    srand(time(NULL));
                    int tf = rand() % 2;
                    int top, pos;
                    for (int l = 0; l < corner.size(); ++l){
                        if (corner[l] == topcorner){
                            top = 1;
                            pos = l;
                        }
                    }
                    if (top == 1){
                        countries[i][j][k] = "1";
                        corner.erase(corner.begin() + pos);
                        corner.push_back(point);
                        
                    }else{
                        countries[i][j][k] = std::to_string(tf);
                        if (tf == 1){
                            corner.push_back(point);
                        }
                    }
                }
                else{
                    std::vector <int> topcorner = {j-1,k};
                    std::vector <int> leftcorner = {j,k-1};
                    std::vector <int> point = {j,k};
                    int top, left;
                    int leftpos, toppos;
                    srand(time(NULL));
                    int tf = rand() % 2;
                    for (int l = 0; l < corner.size(); l++){
                        if (corner[l] == topcorner){
                            top = 1;
                            toppos = l;
                        }
                        if (corner[l] == leftcorner){
                            left = 1;
                            leftpos = l;
                        }
                    }
                    if (top == 1 && left == 1){
                        countries[i][j][k] = std::to_string(tf);
                        if (tf == 1){
                            corner.erase(corner.begin()+leftpos);
                            corner.erase(corner.begin()+toppos);
                            corner.push_back(point);
                        }
                    }else if (top == 1 && left != 1){
                        countries[i][j][k] = "1";
                        corner.erase(corner.begin()+toppos);
                        corner.push_back(point);
                    }else{
                        countries[i][j][k] = std::to_string(tf);
                        if (tf == 1){
                            corner.push_back(point);
                            for (int l = 0; l < corner.size(); l++){
                                if (corner[l] == leftcorner){
                                    corner.erase(corner.begin()+l);
                                }
                            }
                        }
                    }
                }
            }
        } 
    }
}

void points(std::vector<std::vector<std::vector<std::string>>> &countries){
    for (int i = 0; i < countries.size(); i++){
        int height, width;
        height = countries[i].size();
        width = countries[i][0].size();
        for (int j = 0; j < height; j++){
            for (int k = 0; k < width; k++){
                if (countries[i][j][k] == "1"){
                    std::string pos = std::to_string(k+1);
                    if (j == 0){
                        countries[i][j][k] = "A" + pos;
                    }
                    else{
                        if (countries[i][j-1][k] == "0"){
                            countries[i][j][k] = "A" + pos;
                        }else{
                            char level = 'A' + j;
                            std::string x;
                            x+=level;
                            countries[i][j][k] = x + pos;
                        }
                    }
                }
            }
        }
    }
}

int main()
{
    std::vector<std::string> NameofCountries = {"Pekoland", "Akukin", "MikoP", "Fubuki", "Atlantis", "Ninomae", "Okayu", "Ayame", "Takanashi", "Suisei"};
    srand(time(NULL));
    int NumofCountries;
    NumofCountries = rand() % 11 + 3;
    std::vector<std::vector<std::vector<std::string>>> countries(NumofCountries, std::vector<std::vector<std::string>>(3, std::vector<std::string>(3, "0")));
    for (int i = 0; i < NumofCountries; ++i)
    {
        int hori = maphori(20, 26);
        int vert = mapvert(20, 26);
        countries[i].resize(hori, std::vector<std::string>(vert, 0));
    }
    pointallocate(countries);
    for (int i = 0; i < countries[0].size(); ++i){
        for (int j = 0; j < countries[0][i].size(); ++j){
            std::cout << countries[0][i][j] << " ";
        }
        std::cout << std::endl;
    }
}