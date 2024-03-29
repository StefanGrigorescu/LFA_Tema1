#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <deque>

std::ifstream f("lambda.in");
std::ofstream g("lambda.out");

// consideram sigma = literele mici ale alfabetului englez;
// vom folosi simbolul + in loc de lambda in program;   

class stare
{
#pragma region Typedefuri
    typedef std::unordered_map <char, std::vector<int>> umstari;
    typedef std::deque <int> dqstari;
    typedef std::unordered_set <int> setstari;
#pragma endregion

    int ind;
    bool finala;
    umstari t;

public:
    void set_ind(int i) { ind = i; }
    void set_finala(bool t) { finala = t; }
    void set_tranz(char lit, const stare& surm)
    {
        if (t.count(lit) > 0)
            t[lit].push_back(surm.ind);
        else
        {
            std::vector <int> vaux;
            vaux.push_back(surm.ind);
            std::pair <char, std::vector<int>> aux(lit, vaux);
            t.insert(aux);
        }
    }
    int get_ind(){ return ind; }
    void afis_t()
    {
        std::cout << "\n  " << ind << ": ";
        for (auto it = t.begin(); it != t.end(); ++it)
        {
            if(it->first !='+')
                std::cout << "\n      " << it->first << " -> { ";
            else
                std::cout << "\n    lambda -> { ";
            for (unsigned int i = 0; i < it->second.size(); i++)
                std::cout <<  it->second[i] << " ";
            std::cout << "}";
        }
            
    }

    void casual_check(std::string, dqstari&, setstari&, bool&);
    void lambda_check(std::string, dqstari&, setstari&, bool&);
    void apartine(std::string, dqstari&, setstari&, bool&);
    stare()
    {
        ind = 0;
        finala = 0;
    }
};

#pragma region Typedefuri
typedef std::vector <stare> umglobal;
typedef std::vector <int> vstari;
typedef std::deque <int> dqstari;
typedef std::unordered_set <int> setstari;
#pragma endregion

umglobal stari;

void construire_stari(umglobal& stari, int nr_stari, int& nr_stari_finale)
{
    for (int i = 0; i < nr_stari; i++)
    {
        bool end;
        f >> end;
        stare aux;
        aux.set_ind(i);
        if (end == 1)
        {
            aux.set_finala(end);
            nr_stari_finale++;
        }
        stari.push_back(aux);
    }
}
void construire_tranz(umglobal& stari, int nr_tranz)
{
    for (int i = 0; i < nr_tranz; i++)
    {
        int si, surm;       // starile initiala si urmatoare
        char litera;
        f >> si;
        f >> surm;
        f >> litera;
        stari[si].set_tranz(litera, stari[surm]);
    }
}
void afis_tranz(umglobal stari, int nr_stari)
{
    std::cout << "\n Tranzitiile:";
    for (int i = 0; i < nr_stari; i++)
        stari[i].afis_t();
    std::cout << "\n";
}

void stare::casual_check(std::string cuv, dqstari& drum, setstari& lambda_set, bool& gata)
{
    if (cuv.size() and t.count(cuv[0]))
        for (int i = 0; i < t[cuv[0]].size(); i++)
        {
            if (gata == 1)
                break;
            std::string cuv2 = cuv.substr(1, cuv.size() - 1);
            setstari lambda_set_chior;
            stari[t[cuv[0]][i]].apartine(cuv2, drum, lambda_set_chior, gata);
            if (gata == 1)
                break;
        }
}
void stare::lambda_check(std::string cuv, dqstari& drum, setstari& lambda_set, bool& gata)
{
    if (t.count('+'))
        for (int i = 0; i < t['+'].size(); i++)
        {
            if (lambda_set.count(ind) == 0 and gata == 0)
            {
                lambda_set.emplace(ind);
                stari[t['+'][i]].apartine(cuv, drum, lambda_set, gata);
                lambda_set.erase(ind);
            }
            if (gata == 1)
                break;
        }
}
void stare::apartine(std::string cuv , dqstari& drum, setstari& lambda_set, bool& gata)
{
    std::cout << "\n  | stare = " << ind << ", cuv = " << cuv << ", cuv[0] = " << cuv[0] << ", gata = " << gata;
    drum.push_back(ind);
    if (cuv.size() == 0 and finala == 1 and gata == 0)
    {
        gata = 1;
        std::cout << "\n  | ~ gata a fost setat la " << gata << " ~";
        std::cout << "\n  | ~ acceptat ~\n";
    }
    else if (cuv.size() == 0 and finala != 1 and gata == 0)
        lambda_check(cuv, drum, lambda_set, gata);
    else if(gata == 0)
    {
        casual_check(cuv, drum, lambda_set, gata);
        lambda_check(cuv, drum, lambda_set, gata);
    }
    if (gata == 0)
    {
        std::cout << "\n  |  drum.size() = " << drum.size() << " ";
        drum.pop_back();
    }
}

void verificare_cuvinte(umglobal& stari, int nr_cuv, int stare_inceput)
{
    std::cout << "\n\n Verificarea cuvintelor:";
    for (int i = 0; i < nr_cuv; i++)
    {
        std::string cuv;
        std::getline(f, cuv, '\n');
        std::deque <int> drum;
        setstari lambda_set;
        bool gata = 0;
        std::cout << "\n  " << cuv << ": ";
        stari[stare_inceput].apartine(cuv, drum, lambda_set, gata);
        if(gata == 0)
            std::cout << "\n  | ~ respins ~\n";
        g << "\n  Cuvantul ''" << cuv << "''";
        if (gata == 0)
            g << " NU";
        g << " apartine limbajului acceptat de lambda-NFA";
        if (gata == 1)
        {
            g << ", iar un drum viabil prin automat este: ";
            for (int i = 0; i < drum.size(); i++)
                g << " " << drum[i];
        }
    }
    std::cout << "\n\n    ~    Verificarea cuvintelor a luat sfarsit! Puteti vedea rezultatele in fisierul ''lambda.out''    ~\n\n";
}

int main()
{
    int nr_stari, nr_stari_finale = 0, nr_tranz, nr_cuv, stare_inceput;
    f >> nr_stari >> nr_tranz >> nr_cuv >> stare_inceput;
    
    construire_stari(stari, nr_stari, nr_stari_finale);
    construire_tranz(stari, nr_tranz);
    afis_tranz(stari, nr_stari);

    std::string gunoi;
    getline(f, gunoi);
    verificare_cuvinte(stari, nr_cuv, stare_inceput);

    return 0;
}

