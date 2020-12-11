#include <utility>
#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include <list>
using namespace std;
struct Item
{
    string name = "";
    ;
    int idNumber = 0;
    ;
    int inStock = 0;
    ;

    Item(string name, int idNumber, int inStock) {
        this->name = name;
        this->idNumber = idNumber;
        this->inStock = inStock;
    }
};

struct hash_map {

    vector<list<pair<string, Item>>>hash_structure_str;
    int size=0;
    int total_values=0;
    const double multiplier = 1.5;
    hash<string> hash_int;

    hash_map(int size_) {
        this->size = size_*multiplier;
        for (int i = 0; i < size; i++) {
            list<pair<string, Item>> empty_vec;
            hash_structure_str.push_back(empty_vec);

        }

    }

    void search(string key) {
        long hashed = hash_int(key);
        auto temp = hash_structure_str[abs(hashed) % size];
        for (auto element : temp) {
            if (element.first == key) {
                printItem(element.second);
                return;
            }
               
        }
        cout << "Item not found!";
    }

    void printItem(Item item) {

        cout << "Name: " <<item.name<< endl;
        cout << "Item number: " << item.idNumber << endl;
        cout << "Number in stock: " << item.inStock << endl;
    }

    void edit(string name,int newStock) {

        for (auto iter = hash_structure_str.begin(); iter != hash_structure_str.end(); iter++) {
            for (auto iter2 = (*iter).begin(); iter2 != (*iter).end(); iter2++) {
                iter2->second.inStock = newStock;
            }
        }


    }


   void insert(Item item) {
       
       long hashed = hash_int(item.name);
       auto temp = hash_structure_str[abs(hashed) % size];
       auto pair = make_pair(item.name, item);
       if (!found(temp, item.name)) {
           hash_structure_str[abs(hashed) % size].push_back(pair);
           total_values++;
       }
      
    }

   void delete_(string name) {
       long hashed = hash_int(name);
       auto iter = hash_structure_str[abs(hashed) % size].begin();
       for (; iter != hash_structure_str[abs(hashed) % size].end(); iter++) {
           if (iter->first == name) {
               hash_structure_str[abs(hashed) % size].erase(iter);
               return;
           }
               
       }
      
       
       
   }

   void reconfigure() {
       
       vector<list<pair<string, Item>>>temp;
       this->size = size * multiplier;

       for (int i = size; i < size; i++) {
           list<pair<string, Item>> empty_vec = {};
           temp.push_back(empty_vec);

       }
      
       for (auto iter = hash_structure_str.begin(); iter != hash_structure_str.end(); iter++) {
           for (auto iter2 = (*iter).begin(); iter2 != (*iter).end(); iter2++) {
               long hashed = hash_int(iter2->first);
               temp[hashed % size].push_back(*iter2);
           }
       }
       hash_structure_str = temp;
       

   }

   bool found(list<pair<string, Item>> list,string name) {
       bool found = false;
       for (auto element : list) {
           if (element.first == name)
               return true;
       }
       return false;

   }










};




























string printRandomString(int n);

int main() {
    hash_map temp(100000);
    
    


     for (int i = 0; i < 100000; i++) {
         string random_string = printRandomString(6);
         Item tempItem(random_string, i, 3);
         hash<string> hash_int;
         temp.insert(tempItem);
         



        

     }

     temp.delete_("meayln");


}














string printRandomString(int n)
{
    const int MAX = 26;
    char alphabet[MAX] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g',
                          'h', 'i', 'j', 'k', 'l', 'm', 'n',
                          'o', 'p', 'q', 'r', 's', 't', 'u',
                          'v', 'w', 'x', 'y', 'z' };
    string res = "";
    for (int i = 0; i < n; i++)
        res = res + alphabet[rand() % MAX];
    return res;
}