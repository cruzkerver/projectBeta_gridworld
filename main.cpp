//
//  main.cpp
//  ME493_ProjectBeta
//
//  Created by Cruz Kerver on 3/2/17.
//  Copyright © 2017 Autonomy. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <assert.h>
#include <string>
#include <random>
#include <ctime>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <limits>

#define CKRAND ((double)rand()/RAND_MAX)

using namespace std;

class domain{
public:
    int xmin;
    int xmax;
    int ymin;
    int ymax;
    int goalx;
    int goaly;
    
    void init_domain(int*,int*);
};

class agent{
public:
    int agentx;
    int agenty;
    int agent_lastx;
    int agent_lasty;
    int agent_startx;
    int agent_starty;
    
    void init_agent(int*,int*);
    void decide(int*,vector<vector<double>>*,int*,int*,int*,int*);
    void act(int*,int*,int*);
    void react(int*,int*,int*,int*,int*,int*,int*,vector<vector<double>>*);
};

void domain::init_domain(int* width , int* height){
    xmax = *width-1;
    ymax = *height-1;
    xmin = 0;
    ymin = 0;
    goalx = rand() % *width;
    goaly = rand() % *height;
    cout << "goal position: " << "(" << goalx << "," << goaly << ")" << endl;
};

void agent::init_agent(int* width,int* height){
    agent_startx = rand() % *width;
    agent_starty = rand() % *height;
    agentx = agent_startx;
    agenty = agent_starty;
    agent_lastx = agentx;
    agent_lasty = agenty;
    cout << "agent position: " << "(" << agentx << "," << agenty << ")" << endl;
};

void init_AVL(int* num_states, vector<vector<double>> *learner_vals , int* pxmax , int* pymax , int* pgoalx , int* pgoaly){
    for(int j=0; j<(* pymax+1); j++){
        vector<double> states;
        for(int i=0; i<(* pxmax+1); i++){
            double q_vals = CKRAND;
            states.push_back(q_vals);
        }
        learner_vals->push_back(states);
    }
//    learner_vals->at(*pgoaly)[*pgoalx] = 100;
};

//Uses epsilon to decide whether it wants to be greedy or not
void agent::decide(int* move_direction , vector<vector<double>>* learner_vals , int* pagentx , int* pagenty , int* pxmax , int* pymax){
    //greedy of random??
    int greedy=0;
    int random=0;
    *move_direction=-1;
    double epsilon = .1;
    double decide_val = CKRAND;
    if(decide_val > epsilon){
        greedy=greedy+1;
    }
    else if(decide_val <= epsilon){
        random=random+1;
    }
    cout << "greedy: " << greedy << " " << "random: " << random << endl;
    
    //greedy pull
    double max_value = -10;
    if(greedy > random){
        if(*pagenty != *pymax){
            if(max_value < learner_vals->at(*pagenty+1)[*pagentx]){
                max_value = learner_vals->at(*pagenty+1)[*pagentx];
                *move_direction = 0;
            }
        }
        if(*pagenty != 0){
            if(max_value < learner_vals->at(*pagenty-1)[*pagentx]){
                max_value = learner_vals->at(*pagenty-1)[*pagentx];
                *move_direction=  1;
            }
        }
        if(*pagentx != *pxmax){
            if(max_value < learner_vals->at(*pagenty)[*pagentx+1]){
                max_value = learner_vals->at(*pagenty)[*pagentx+1];
                *move_direction = 2;
            }
        }
        if(*pagentx != 0){
            if(max_value < learner_vals->at(*pagenty)[*pagentx-1]){
                max_value = learner_vals->at(*pagenty)[*pagentx-1];
                *move_direction = 3;
            }
        }
        cout << *move_direction << endl;
    }
    
    //random pull
    
    else if(greedy < random){
        
        if(*pagenty == *pymax && *pagentx == *pxmax){
            while(*move_direction == 0 && *move_direction == 2){
                *move_direction = rand() % 4;
            }
        }
        else if(*pagenty == 0 && *pagentx == 0){
            while(*move_direction == 1 && *move_direction == 3){
                *move_direction = rand() % 4;
            }
        }
        else if(*pagenty == 0 && *pagentx == *pxmax){
            while(*move_direction == 1 && *move_direction == 2){
                *move_direction = rand() % 4;
            }
        }
        else if(*pagenty == *pymax && *pagentx == 0){
            while(*move_direction == 0 && *move_direction == 3){
                *move_direction = rand() % 4;
            }
        }
        else if(*pagenty == *pymax){
            while(*move_direction == 0){
                *move_direction = rand() % 4;
            }
        }
        else if(*pagentx == *pxmax){
            while(*move_direction == 2){
                *move_direction = rand() % 4;
            }
        }
        else if(*pagenty == 0){
            while(*move_direction == 1){
                *move_direction = rand() % 4;
            }
        }
        else if(*pagentx == 0){
            while(*move_direction == 3){
                *move_direction = rand() % 4;
            }
        }
        else{
            *move_direction = rand() % 4;
        }
    }
};
//        *move_direction = rand() % 4;
//        //cout << *move_direction << endl;
//        if(*move_direction == 0){
//            if(*pagenty == *pymax){
//                while(*move_direction == 0){
//                    *move_direction = rand() % 4;
//                }
//                if(*pagenty == *pymax && *pagentx == *pxmax){
//                    while(*move_direction == 0 && *move_direction == 2){
//                        *move_direction = rand() % 4;
//                    }
//                }
//            }
//        }
//        else if(*move_direction == 1){
//            if(*pagenty == 0){
//                while(*move_direction == 1){
//                    *move_direction = rand() % 4;
//                }
//                if(*pagenty == 0 && *pagentx == 0){
//                    while(*move_direction == 1 && *move_direction == 3){
//                        *move_direction = rand() % 4;
//                    }
//                }
//            }
//        }
//        else if(*move_direction == 2){
//            if(*pagentx == *pxmax){
//                while(*move_direction == 2){
//                    *move_direction = rand() % 4;
//                }
//                if(*pagentx == *pxmax && *pagenty == *pymax){
//                    while(*move_direction == 2 && *move_direction == 0){
//                        *move_direction = rand() % 4;
//                    }
//                }
//            }
//        }
//        else if(*move_direction == 3){
//            if(*pagentx == 0){
//                while(*move_direction == 3){
//                    *move_direction = rand() % 4;
//                }
//                if(*pagentx == 0 && *pagenty == 0){
//                    while(*move_direction == 1 && *move_direction == 3){
//                        *move_direction = rand() % 4;
//                    }
//                }
//            }
//        }
//        
//        cout << *move_direction << endl;
//    }

//This is where you move
void agent::act(int* pmove_direction , int* pagentx , int* pagenty){
    if(*pmove_direction == 0){
        *pagenty = *pagenty + 1;
    }
    else if(*pmove_direction == 1){
        *pagenty = *pagenty - 1;
    }
    else if(*pmove_direction == 2){
        *pagentx = *pagentx + 1;
    }
    else if(*pmove_direction == 3){
        *pagentx = *pagentx - 1;
    }
    cout << "agent new position: " << "(" << agentx << "," << agenty << ")" << endl;
};

//Uses Q-Learning eqatuion to update the learner values of each state
void agent::react(int* pmove_direction , int* pagent_lastx , int* pagent_lasty , int* pagentx , int* pagenty , int* pgoalx , int* pgoaly , vector<vector<double>> *learner_vals){
    double alpha = .3;
    double gamma = .2;
    int R = 0;
    if(*pagentx == *pgoalx && *pagenty == *pgoaly){
        R = 100;
    }
    else{
        R = -1;
    };
    
    if(*pmove_direction == 0 || *pmove_direction == 1){
        learner_vals->at(*pagent_lasty)[*pagentx] = learner_vals->at(*pagent_lasty)[*pagentx] + alpha * (R + gamma * learner_vals->at(*pagenty)[*pagentx] - learner_vals->at(*pagent_lasty)[*pagentx]);
    }
    else if(*pmove_direction == 2 || *pmove_direction == 3){
        learner_vals->at(*pagenty)[*pagent_lastx] = learner_vals->at(*pagenty)[*pagent_lastx] + alpha * (R + gamma * learner_vals->at(*pagenty)[*pagentx] - learner_vals->at(*pagenty)[*pagent_lastx]);
    }
    
    if(*pagentx == *pgoalx && *pagenty == *pgoaly){
        learner_vals->at(*pagenty)[*pagentx] = learner_vals->at(*pagenty)[*pagentx] + alpha * (R + gamma * learner_vals->at(*pagenty)[*pagentx] - learner_vals->at(*pagenty)[*pagentx]);
    }
    
    *pagent_lastx = *pagentx;
    *pagent_lasty = *pagenty;

};

void plot_position(int* height , int* width , int* pagentx , int* pagenty , int* pgoalx , int* pgoaly){
    for(int i=(*height-1); i>-1; i--){
        for(int j=0; j<*width; j++){
            if(*pagentx == j && *pagenty == i){
                cout << "A ";
            }
            else if(*pgoalx == j && *pgoaly == i){
                cout << "G ";
            }
            else{
                cout << ". ";
            }
        }
        cout << endl;
    }
};

void plot_learner_vals(int* height , int* width , vector<vector<double>> *learner_vals){
    for(int j=(*height-1); j>(-1); j--){
        for(int i=0; i<(*width); i++){
            cout << learner_vals->at(j)[i] << " ";
        }
        cout << endl;
    }
    cout << "\n";
};
int main() {
    
    srand(time(NULL));
    int width = 0;
    int height = 0;
    cout << "How many squares wide is your grid? ";
    cin >> width;
    cout << "How many squares tall is your grid? ";
    cin >> height;
    
    domain grid;
    grid.init_domain(&width , &height);
    cout << grid.xmax << "x" << grid.ymax << " grid" << endl;
    agent Z;
    Z.init_agent(&width , &height);
    while((Z.agentx == grid.goalx) && (Z.agenty == grid.goaly)){
        Z.init_agent(&width , &height);
    };
    
    int num_states = height * width;
    vector<vector<double>> learner_vals;
    init_AVL(&num_states, &learner_vals , &grid.xmax , &grid.ymax , &grid.goalx , &grid.goaly);
    
    int move_direction;
    int counter = 0;
    for(int i=0; i<10500; i++){
        Z.decide(&move_direction , &learner_vals , &Z.agentx , &Z.agenty , &grid.xmax , &grid.ymax);
        Z.act(&move_direction , &Z.agentx , &Z.agenty);
        Z.react(&move_direction , &Z.agent_lastx , &Z.agent_lasty , &Z.agentx , &Z.agenty , &grid.goalx , &grid.goaly , &learner_vals);
        counter = counter + 1;
        plot_position(&height , &width , &Z.agentx , &Z.agenty , &grid.goalx , &grid.goaly);
        //plot_learner_vals(&height , &width , &learner_vals);
        if(Z.agentx == grid.goalx && Z.agenty == grid.goaly){
            Z.agentx = Z.agent_startx;
            Z.agenty = Z.agent_starty;
            cout << "Moves to complete:                                    " << counter << endl;
            counter = 0;
        };
    };
    
     // Normalizing q values
    
    double max_q_value = -10;
    for(int k=0; k<grid.ymax; k++){
        for(int h=0; h<grid.xmax; h++){
            if(max_q_value < learner_vals.at(k)[h]){
                max_q_value = learner_vals.at(k)[h];
            }
        }
    };
    for(int k=0; k<grid.ymax; k++){
        for(int h=0; h<grid.xmax; h++){
            learner_vals.at(k)[h] = (learner_vals.at(k)[h]) / max_q_value * 100;
        }
    };

    plot_learner_vals(&height , &width , &learner_vals);

    return 0;
}
