// RANDOMLY INFECT ONE PERSON WITHIN THE POPULATION
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
  int n = 0;  // sick day/status counter within class
  bool rec = false;  // initially not recovered
  int sick = 0;  // state of sickness
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
  vector<int> condition; // vector to hold condition of those in population
  int pop_size = 0;  // user input of population size
  int ran_person = 0;  // the first random person to be infected
  int day_num = 0;  // the day counter for each day in simulation
  SickPerson Joe;
public:
  Population(int in_people) : pop_size(in_people){
    condition = vector<int>(pop_size, 0);
  };
  void random_infection(){
    srand((float)time(NULL));
    ran_person = round((rand()/double(RAND_MAX))* pop_size); // choose a random person to infect
    if(ran_person == pop_size){
      ran_person--;
    }
    if(condition.at(ran_person) != -1){ // if person has already been infected pick a new random person
      condition.at(ran_person) = -1;
      int d = 1;
      cout << "step " << update() << " has " << count_infected() << " infected ";
      for (int pn = 0; pn < pop_size; pn++){
	cout << condition.at(pn) << " ";  // print out condition of each person in population
      }
      cout << endl;
      day_num++;
      while(d <=4){  // for all days of sickness, keep updating to the next day
	cout << "step " << update() << " has " << count_infected() << " infected ";
	for (int pn = 0; pn < pop_size; pn++){
	  cout << condition.at(pn) << " ";
	}
	if(d == 4){  // if sickness is over, person is recovered and break out of loop
	  condition.at(ran_person) = 1;
	  day_num++;
	  cout << endl;
	  cout << "step " << update() << " has " << count_infected() << " infected ";
	  for(int person = 0; person < pop_size; person++){
	    cout << condition.at(person) << " ";
	  }
	  break;
	}
	cout << endl;
	d++;
	day_num++;
      }
    }
    cout << endl;
    cout << "Disease ran its course by step " << (update() - 1) << " and there are " << count_infected() << " infected " << endl;
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
};

int main(){
  int npeople;
  cout << "Enter population size: ";
  cin >> npeople;
  Population population(npeople);
  population.random_infection();
  return 0;
}

