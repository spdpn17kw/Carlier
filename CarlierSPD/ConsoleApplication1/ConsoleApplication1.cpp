#include <iostream>
#include <fstream>
#include <queue>
#include <vector>

using namespace std;

class Task
{
	int task_number;
	int r;
	int p;
	int q;

public:

	void CreateTask(int a, int b, int c, int d)
	{
		task_number = a;
		r = b;
		p = c;
		q = d;
	}

	int GetTaskNr()
	{
		return task_number;
	}

	int GetR() const
	{
		return r;
	}

	void SetR(int noweR)
	{
		r = noweR;
	}

	int GetP()
	{
		return p;
	}

	void SetP(int noweP)
	{
		p = noweP;
	}

	int GetQ() const
	{
		return q;
	}

	void SetQ(int noweQ)
	{
		q = noweQ;
	}

	Task()
	{
		task_number = -1;
		r = 10000000;
		p = 10000000;
		q = 100000000;
	}

};


class Permutation
{
public:
	vector<Task> listOfTasks;   
	int n;                      
	int Cmax;                   
    int LB;                    
	int a, b;                  
	int c;                      

	void RewritePermutation(vector<Task>& DocelowaPerm) const;

	int Schrage();                  

	int SchragePmtn();               

	int SciezkaKryt();             

	bool ZnajdzZadReferencyjne(); 

	int ZnajdzRmin(int pocz, int kon);  

	int ZnajdzQmin(int pocz, int kon); 

	int SumaP(int pocz, int kon);      

	int KminR();               

	int KminQ();                  

	int KsumaP();                 

    int ZwrocRZadRef();

	void ZmienRZadRef(int noweR);

	int ZwrocQZadRef();

	void ZmienQZadRef(int noweQ);

	int ZwrocIDZadRef();

	void ZmienRZad(int IDzad, int noweR);

	void ZmienQZad(int IDzad, int noweQ);

	Permutation(const char*);    

	Permutation(const Permutation& Perm);  

	~Permutation()
	{
		listOfTasks.clear();
	}

};


struct CompareAvailable
{
	bool operator()(Task& lhs, Task& rhs)
	{
		return lhs.GetR() > rhs.GetR();
	}
};

struct CompareDelivery
{
	bool operator() (Task& lhs, Task& rhs)
	{
		return lhs.GetQ() < rhs.GetQ();
	}
};

int DoCarlier(const char*);

int Carlier(Permutation& Perm);

vector<Task> OptymPerm;
int depth = 0;
int UB;

int DoCarlier(const char* fileName)
{
	Permutation Perm(fileName);
	UB = 100000000;
	Carlier(Perm);
	cout << "\n Solution: " << UB << " \nDepth of recursion: " << depth << "\n";
	return 1;
}


int Carlier(Permutation& Perm)
{
	int stareR = 0;
	int rmin = 10000000;
	int stareQ = 0;
	int qmin = 10000000;
	int sumaP = 0;
	int IDzad = -1;

	int U = Perm.Schrage();
	if (U < UB)
	{
		UB = U;
		Perm.RewritePermutation(OptymPerm);
	}
	Perm.SciezkaKryt();
	if (!(Perm.ZnajdzZadReferencyjne()))
		return 1;

	rmin = Perm.KminR();
	qmin = Perm.KminQ();
	sumaP = Perm.KsumaP();

	IDzad = Perm.ZwrocIDZadRef();
	stareR = Perm.ZwrocRZadRef();

	Perm.ZmienRZadRef(rmin + sumaP);
	Perm.SchragePmtn();
	if (Perm.LB < UB)
	{
		depth++;
		Permutation PermPotomekR(Perm);
		Carlier(PermPotomekR);
	}
	Perm.ZmienRZadRef(stareR);
	Perm.ZmienQZadRef(qmin + sumaP);
	Perm.SchragePmtn();
	if (Perm.LB < UB)
	{
		Permutation PermPotomekQ(Perm);
		Carlier(PermPotomekQ);
	}
	Perm.ZmienQZadRef(stareQ);
	return 1;
}


int main()
{
	DoCarlier("in50.txt"); 

	return 0;
}



void Permutation::RewritePermutation(vector<Task>& DocelowaPerm) const
{
	DocelowaPerm = listOfTasks;
}

int Permutation::Schrage()
{
	priority_queue<Task, vector<Task>, CompareAvailable> FreeTasks;
	priority_queue<Task, vector<Task>, CompareDelivery> ReadyTasks;
	vector<Task> ScheduledTasks;
	Task bufor;
	int CmaxSchrage = 0;
	int Caktualnego = 0;
	int i = 0;
	int t = 0;
	while (i < n)
	{
		FreeTasks.push(listOfTasks[i]);
		i++;
	}

	while (FreeTasks.size() > 0 || ReadyTasks.size() > 0)
	{
		while (FreeTasks.size() > 0)  
		{
			if (FreeTasks.top().GetR() <= t)  
			{
				bufor = FreeTasks.top();
				ReadyTasks.push(bufor);       
				FreeTasks.pop();           
			}
			else
				break; 
		}
		if (ReadyTasks.size() == 0) 
		{
			t = FreeTasks.top().GetR(); 
		}
	
		else
		{
			bufor = ReadyTasks.top();  
			ScheduledTasks.push_back(bufor);
			ReadyTasks.pop();
			
			t = t + bufor.GetP(); 

			Caktualnego = t + bufor.GetQ();
			if (CmaxSchrage < Caktualnego)       
			{                          
				CmaxSchrage = Caktualnego;
			}
		}
	}
	i = 0;
	while (i < n)  
	{
		listOfTasks[i] = ScheduledTasks[i];
		i++;
	}

	Cmax = CmaxSchrage;
	return Cmax;
}


int Permutation::SchragePmtn()
{
	priority_queue<Task, vector<Task>, CompareAvailable> FreeTasks;
	priority_queue<Task, vector<Task>, CompareDelivery> ReadyTasks;
	vector<Task> ScheduledTasks;
	Task bufor;
	Task Actual;
	int CmaxSchrage = 0;
	int Caktualnego = 0;
	int i = 0;
	int t = 0;
	while (i < n)
	{
		FreeTasks.push(listOfTasks[i]);
		i++;
	}

	while (FreeTasks.size() > 0 || ReadyTasks.size() > 0)
	{
		while (FreeTasks.size() > 0)  
		{
			if (FreeTasks.top().GetR() <= t)  
			{
				bufor = FreeTasks.top();
				ReadyTasks.push(bufor);      
				FreeTasks.pop();           

				if (ReadyTasks.top().GetQ() > Actual.GetQ())
				{
					Actual.SetP(t - ReadyTasks.top().GetR());
					t = ReadyTasks.top().GetR();
					if (Actual.GetP() > 0)
					{
						ReadyTasks.push(Actual);
					}
				}
			}
			else
				break;   
		}
		if (ReadyTasks.size() == 0)
		{
			t = FreeTasks.top().GetR(); 
		}
		
		else
		{
			Actual = ReadyTasks.top();  
			ReadyTasks.pop();
		    t = t + Actual.GetP(); 
										
			Caktualnego = t + Actual.GetQ();
			if (CmaxSchrage < Caktualnego)       
			{                          
				CmaxSchrage = Caktualnego;
			}
		}
	}

	LB = CmaxSchrage;
	return CmaxSchrage;
}

int Permutation::SciezkaKryt()
{
	int aktMax = 0;
	int akt_t = 0;
	b = -1;
	
	for (int i = 0; i < n; i++)
	{
		if (akt_t < listOfTasks[i].GetR()) 
			akt_t = listOfTasks[i].GetR();
		if (akt_t + listOfTasks[i].GetP() + listOfTasks[i].GetQ() > aktMax)
		{
			b = i;
			akt_t = akt_t + listOfTasks[i].GetP(); 
			aktMax = akt_t + listOfTasks[i].GetQ();  
		}
	}

	a = b;  
	while (a >= 1 && (akt_t - listOfTasks[a].GetP() >= listOfTasks[a - 1].GetR()))
	{
		akt_t = akt_t - listOfTasks[a].GetP();
		--a;
	}
	//cout << "a= " << a << " b= " << b;

	return 0;
}

bool Permutation::ZnajdzZadReferencyjne()
{
	c = b;
	for (int i = b - 1; i >= a; i--)
	{
		if (listOfTasks[i].GetQ() < listOfTasks[b].GetQ())
		{
			c = i;
		//  cout << " c= " << c << "\n";
			return true;
		}
	}
	return false;
}

int Permutation::ZnajdzRmin(int pocz, int kon)
{
	int rmin = 1000000;

	for (int i = pocz; i <= kon; i++)
	{
		if (listOfTasks[i].GetR() < rmin)
		{
			rmin = listOfTasks[i].GetR();
		}
	}
	return rmin;
}

int Permutation::ZnajdzQmin(int pocz, int kon)
{
	int qmin = 1000000;
	
	for (int i = pocz; i <= kon; i++)
	{
		if (listOfTasks[i].GetQ() < qmin)
		{
			qmin = listOfTasks[i].GetQ();
		}
	}
	return qmin;
}

int Permutation::SumaP(int pocz, int kon)
{
	int sumap = 0;
	for (int i = pocz; i <= kon; i++)
		sumap = sumap + listOfTasks[i].GetP();
	return sumap;
}

int Permutation::KminR()                   
{
	return ZnajdzRmin(c + 1, b);
}

int Permutation::KminQ()              
{
	return ZnajdzQmin(c + 1, b);
}

int Permutation::KsumaP()                
{
	return SumaP(c + 1, b);
}


int Permutation::ZwrocRZadRef()
{
	return listOfTasks[c].GetR();
}

void Permutation::ZmienRZadRef(int noweR)
{
	listOfTasks[c].SetR(noweR);
}


int Permutation::ZwrocQZadRef()
{
	return listOfTasks[c].GetQ();
}

void Permutation::ZmienQZadRef(int noweQ)
{
	listOfTasks[c].SetQ(noweQ);
}


int Permutation::ZwrocIDZadRef()
{
	return listOfTasks[c].GetTaskNr();
}

void Permutation::ZmienRZad(int IDzad, int noweR)
{
	for (int i = n - 1; i > 0; i--)
	{
		if (listOfTasks[i].GetTaskNr() == IDzad)
		{
			listOfTasks[i].SetR(noweR);
			break;
		}
	}
}

void Permutation::ZmienQZad(int IDzad, int noweQ)
{
	for (int i = n - 1; i > 0; i--)
	{
		if (listOfTasks[i].GetTaskNr() == IDzad)
		{
			listOfTasks[i].SetQ(noweQ);
			break;
		}
	}
}


Permutation::Permutation(const char* nazwa_fileu)
{
	ifstream file;
	string line;
	int bufor[3];
	int i = 0;
	int k = 0;
	n = 0;

	file.open(nazwa_fileu, ios::in);
	if (!file.is_open())
	{
		cout << "\nOdczyt z fileu nie powiodl sie!\n";
	}

	if (!(file >> n))     
		cout << "\nfile jest pusty!!! Nie udalo sie stworzyc listy zadan\n";
	else
	{
		Task ReadTask;
		while (file >> bufor[k])
		{
			if (k == 2)   
			{
				ReadTask.CreateTask(i, bufor[0], bufor[1], bufor[2]);   
				listOfTasks.push_back(ReadTask);                     
				i++;    
				k = 0;   
			}
			else
				k++;
		}
	}
	file.close();
}

Permutation::Permutation(const Permutation & Perm)
{
	Perm.RewritePermutation(listOfTasks);   
	n = Perm.n;                     
	Cmax = Perm.Cmax;                  
	LB = Perm.LB;                   
	a = Perm.a;
	b = Perm.b;                 
	c = Perm.c;                  
}
