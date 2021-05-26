//INFECTS NEIGHBOR WITH PROBABILITY, THE ENDS OF VECTOR LOOP AROUND, VACCINATION INCLUDED
#include <iostream>
#include <string>
#include <cmath>
#include <random>
#include <ctime>
#include <vector>
#include <time.h>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::rand;
using std::vector;

class SickPerson{
private:
  int n = 0; // sick day/status counter within class
  bool rec = false; // initially not recovered
  int sick = 0; // state of sickness
public:
  SickPerson() {};
  SickPerson(int in_x): n(in_x){};
  string status_string(){
    if(n == 1){
      sick = 0;
      string h = "susceptible";
      return h;
    }else if(sick == -1){
      string s = "sick";
      return s;
    }else if(sick == 0){
      string h = "susceptible";
      return h;
    }else if(sick == 1){
      string r = "recovered";
      return r;
    }else if(sick == 2){
      string v = "vaccinated";
      return v;
    }
  };
  int update(){ // update status to the next day
    n++;
    return n;
  };
  int infect(int j) { // infect for n days, n being the first day of infection
    j = j-1;
    sick = -1;
    cout << "On day " << n << ", friend is " << status_string() << endl;
    for (int i = 1; i <= j; i++){
      cout << "On day " << n + i << ", friend is " << status_string() << endl;
      if (i == j){
	sick = 1;
	n++;
	rec = true;
	return sick;
      }
    }
  };
  bool is_stable(){ // if (sick and recovered) return true
    if(rec == true){return true;}
    else{return false;}
  };
};

class Population{
private:
  vector<int> condition;  // vector to hold condition of those in population
  int pop_size = 0;  // user input of population size
  int ran_person = 0;  // the first random person to be infected
  int day_num = 0;  // the day counter for each day in simulation
  int p1, p2 = 0;  // p1 is person to the left in the vector and p2 is person to the right => the neighbors
  double probs;  // user input of probability
  double vac;  // user input of percent of population vaccinated
  int num_rec = 0;  // number of people recovered at any given time
  int num_sus = 0;  // number of susceptible people at any given time
  int num_vac = 0;  // number of vaccinated people at any given time
  SickPerson Joe;
public:
  Population(int in_people, double in_vac) : pop_size(in_people), vac(in_vac){
    condition = vector<int>(pop_size, 0);
    srand(time(NULL));
  };
  void random_infection(){
    day_num++;
    for( ; ; day_num++){
      if(day_num == 1){  // for the very first day
	srand(time(NULL));
	do{
	  ran_person = round((rand()/double(RAND_MAX))* pop_size);  // chose a random person to infect
	}while(condition.at(ran_person) == 2);  // make sure they are not previously vaccinated
	condition.at(ran_person) = -1;
	cout << "step " << update() << " has " << count_infected() << " has infected ";
	for (int pn = 0; pn < pop_size; pn++){
	  cout << condition.at(pn) << " ";  // print out condition of each person in the population
	}
	cout << endl;
      }else{  // for every day after the first day
	cout << "step " << day_num << " has ";
	for (int k = 0; k < pop_size; k++){  // for every person in the condition vector
	  if(condition.at(k) == 0){}
	  else if(condition.at(k) < 0){  // if the condition of the person is less than 0, then they are sick
	    if(condition.at(k) == -5){  // but if they are equal to -5, they have been sick for the sick period of 5 days
	      condition.at(k) = 1;  // now the person will recover
	    }else{
	      condition.at(k)--; // otherwise, subtract their condition by 1 if already sick to update their number of sick days within the condition vector
	    }
	  }
	  else if(condition.at(k) == 1){}
	  else if(condition.at(k) == 2){}
	}
	for(int k = 0; k < pop_size; k++){  // for every person in the condition vector
	  if(condition.at(k) <= -2){  // if they are not newly infected but previously infected on that day
	    if(k == 0){  // if at person in position zero of vector, connect person zero with person at the end of the vector to make a loop
	      p1 = pop_size - 1;
	      p2 = k + 1;
	    }else if(k == (pop_size - 1)){  // if at the last person in vector, connect them to the first person in vector
	      p1 = k - 1;
	      p2 = 0;
	    }else{
	      p1 = k-1;
	      p2 = k+1;
	    }
	    double chance_inf;  // chance of infection
	    if(p1 >= 0){
	      if(condition.at(p1) == 0){  // if the neighbor is susceptible
		chance_inf = (rand()/double(RAND_MAX));
		if(chance_inf < probs){  // and we can infect them
		  condition.at(p1) = -1;  // then infect the neighbor
		}
	      }
	    }
	    if(p2 <= (pop_size - 1)) {  // now check to see if we can infect the other neighbor
	      if(condition.at(p2) == 0){
		chance_inf = (rand()/double(RAND_MAX));
		if(chance_inf < probs){
		  condition.at(p2) = -1;
		}
	      }
	    }
	  }
	}
	cout << count_infected() << " has infected ";
	for(int k = 0; k < pop_size; k++){  // print out the condition of each person in the population
	  if(condition.at(k) == 0){
	    cout << " " << condition.at(k) << " ";
	  }else if(condition.at(k) < 0){
	    cout << condition.at(k) << " ";
	  }else if(condition.at(k) == 1){
	    cout << condition.at(k) << " ";
	  }else if(condition.at(k) == 2){
	    cout << condition.at(k) << " ";
	  }
	}
	cout << endl;
      }
      if(count_infected() == 0){
	int num_inf = 0;
	cout << "Disease ran its course by step " << (update() - 1) << endl;
	for(int a = 0; a < pop_size; a++){  // count the groups of people on the last day of simulation
	  if(condition.at(a) < 0){
	    num_inf++;
	  }
	  else if(condition.at(a) == 1){
	    num_rec++;
	  }else if(condition.at(a) == 0){
	    num_sus++;
	  }else if(condition.at(a) == 2){
	    num_vac++;	
	  }
	}
	cout << "number of infected: " << num_inf << endl;
	cout << "number of recovered: " << num_rec << endl;
	cout << "number of susceptible: " << num_sus << endl;
	cout << "number of vaccinated: " << num_vac << endl;
	break;
      }
    }
  };

  int count_infected(){  // count the number of infected people in the condition vector
    int num_infect = 0;
    for(int j = 0; j < pop_size; j++){
      if(condition.at(j) < 0){
	num_infect++;
      }
    }
    return num_infect;
  };
  int update(){
    return Joe.update();
  };
  void set_probability_of_transfer(double x){  // going to both vaccinate and read in the probability of transferring the disease
    probs = x;
    int vac_people = round(vac * pop_size);  // create an integer amount of people to randomly vaccinate
    for( int v = 1; v <= vac_people; v++){
      int y;
      do{
	y = round((rand()/double(RAND_MAX)) * pop_size);
	if(y == pop_size){
	  y--;
	}
      }while(condition.at(y) == 2);
      condition.at(y) = 2;
    }
    random_infection();
  };
};



int main(){
  int npeople;
  cout << "Enter population size: ";
  cin >> npeople;

  double vaccinated;
  do{
    cout << "Enter percentage of population vaccinated as a decimal between 0 and 1: ";
    cin >> vaccinated;
  }while(vaccinated < 0 || vaccinated > 1);

  Population population(npeople, vaccinated);

  double probability;
  do {
    cout << "Enter rate of infection as a decimal between 0 and 1: ";
    cin >> probability;
  }while(probability < 0 || probability > 1);

  population.set_probability_of_transfer(probability);

  return 0;
}
