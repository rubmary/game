#include "Reader.h"
using namespace std;

void Reader::read_map ( Graph* &graph,
                        vector <DrawableObject*> &segments,
                        vector<Wall>* &W,
                        vector<Color> colors,
                        bool* &show_map)
{
    
    show_map = new bool;
    int N, ratio;
    ifstream file("MAP.txt");
    file >> N >> ratio;
    int x1, y1, x2, y2, c;
    segments.clear();
    vector <Edge> edges(N);
    for (int i = 0; i < N; i++) {
        file >> x1 >> y1 >> x2 >> y2 >> c;
        x1 = x1*ratio + 4;
        y1 = y1*ratio + 4;
        x2 = x2*ratio + 4;
        y2 = y2*ratio + 4;
        edges[i] = {{x1, y1}, {x2, y2}, c == 1 || c == 2, vector <int>(0), c};
        if (c == 1) {
            DrawableEdge *S =  new DrawableEdge(x1, y1, x2, y2, colors[c]);
            S -> visible = show_map;
            segments.push_back(S);
        } else {
            DrawableWall *S =  new DrawableWall(x1, y1, x2, y2, colors[c]);
            segments.push_back(S);
        }
    }
    graph = new Graph(edges);
    W = new vector<Wall>;
    for (int i = 0; i < edges.size(); i++) {
        if (!edges[i].valid)
            W -> push_back({ {edges[i].A.first, 0, edges[i].A.second},
                          {edges[i].B.first, 0, edges[i].B.second} });
    }
    file.close();
}

void Reader::read_agents(vector <DrawableObject*> &drawable_agents,
                         Player*    &player,
                         Object*    &coin,
                         Object*    &player_receiver,
                         Object*    &agent_receiver)
{
    int N;
    ifstream file("Agents.txt");
    file >> N;
    for(int i = 0; i < N; i++) {
        int type;
        file >> type;
        Agent* agent;
        DrawableObject* drawable_agent;
        double x, y, z, speed;
        int size;
        
        if (type == 0) {
            file >> x >> y >> z >> speed >> size;
            player = new Player(x, y, z, speed);
            drawable_agent = new DrawableAgent(player->character, Color::White, size);
            drawable_agents.push_back(drawable_agent);
        }else if(type == 1) {
            file >> size;
            coin = new Object();
            drawable_agent = new DrawableAgent(coin->character, Color::Yellow, size);
            drawable_agent -> visible = &(coin -> exists);
            drawable_agents.push_back(drawable_agent);
        }else if(type == 2) {
            file >> size;
            player_receiver = new Object();
            drawable_agent  = new DrawableAgent(player_receiver -> character, Color::Blue, size);
            drawable_agent -> visible = &(player_receiver -> exists);
            drawable_agents.push_back(drawable_agent);  
        } else if(type == 3) {
            file >> size;
            agent_receiver = new Object();
            drawable_agent = new DrawableAgent(agent_receiver -> character, Color::Green, size);
            drawable_agent -> visible = &(agent_receiver -> exists);
            drawable_agents.push_back(drawable_agent);
        }
    }
    file.close();
}

void Reader::read_competitor(   Graph* &graph,
                                Character* &competitor,
                                vector<Wall>* &walls,
                                Player* &player,
                                Object* &coin,
                                double* &time,
                                vector<DrawableObject*> &drawable_agents)
{
    
    ifstream file("competitor.txt");
    time = new double();
    double epsilon;
    file >> epsilon;

    /**************** COMPETIDOR ************/
    competitor = new Character();
    double x, z, max_speed;
    file >> x >> z >> max_speed;
    (competitor -> character).position = {x, 0, z};
    (competitor -> character).max_speed = max_speed;

    cout << "Hice competidor" << endl;
    
    // Evitar paredes
    double lookahead, avoid_distance, max_acc_obstacle;
    file >> lookahead >> avoid_distance >> max_acc_obstacle;
    ObstacleAvoidance* obstacle_avoidance = new ObstacleAvoidance();
    obstacle_avoidance -> lookahead = lookahead;
    obstacle_avoidance -> avoid_distance = avoid_distance;
    obstacle_avoidance -> max_acceleration = max_acc_obstacle;
    (obstacle_avoidance -> collision_detector).walls = walls;


    cout << "Hice evitador de obstaculos" << endl;

    /*************************SEGUIR MONEDA ********************/
    //Seguir moneda
    double path_offset, max_acc_follow_path;
    file >> path_offset >> max_acc_follow_path;
    FollowPath* follow_path = new FollowPath();
    follow_path -> path_offset = path_offset;
    follow_path -> max_acceleration = max_acc_follow_path;
    follow_path -> character = &(competitor -> character);
    cout << "Hice seguir moneda" << endl;

    //Arrive
    double max_acc_arrive, max_speed_arrive, target_radius, slow_radius, time_to_target;
    file >> max_acc_arrive >> max_speed_arrive;
    file >> target_radius >> slow_radius >> time_to_target;
    Arrive *arrive = new Arrive();
    arrive -> target = &(coin -> character.position);
    arrive -> max_acceleration = max_acc_arrive;
    arrive -> max_speed = max_speed_arrive;
    arrive -> target_radius = target_radius;
    arrive -> slow_radius = slow_radius;
    arrive -> time_to_target = time_to_target;

    cout << "Hice arrive" << endl;
    // Crear los steering que se utilizaran en la accion finding_coin
    PrioritySteering *priority_steering_coin    = new PrioritySteering();
    (priority_steering_coin -> behaviours).resize(3);
    (priority_steering_coin -> behaviours)[0] = obstacle_avoidance;
    (priority_steering_coin -> behaviours)[1] = follow_path;
    (priority_steering_coin -> behaviours)[2] = arrive;
    priority_steering_coin -> character = &(competitor -> character);
    priority_steering_coin -> epsilon   = epsilon;

    cout << "Primer priority steering" << endl;


    /*********************** SEEK (TEMPORAL) *********************/

    // Seek
    double max_acc_seek;
    file >> max_acc_seek;
    Seek *seek = new Seek();
    seek -> target = &(player -> character.position);
    seek -> max_acceleration = max_acc_seek;

    PrioritySteering *priority_steering_seek = new PrioritySteering();
    (priority_steering_seek -> behaviours).resize(2);
    (priority_steering_seek -> behaviours)[0] = obstacle_avoidance;
    (priority_steering_seek -> behaviours)[1] = seek;
    priority_steering_seek -> character = &(competitor -> character);
    priority_steering_seek -> epsilon   = epsilon;

    cout << "Seek" << endl;

    /************************** ACCIONES DE ESTADOS *******************/
    SteeringBehaviorAction *finding_coin = new SteeringBehaviorAction();
    finding_coin -> steering_behavior   = priority_steering_coin;
    finding_coin -> time                = time;

    SteeringBehaviorAction *seeking_player = new SteeringBehaviorAction();
    seeking_player -> steering_behavior = priority_steering_seek;
    seeking_player -> time              = time;

    cout << "Acciones de estados" << endl;
    /************************ ACCIONES DE TRANSICIONES ***************/

    FindBestPath* calculate_path = new FindBestPath();
    calculate_path -> follow_path   = follow_path;
    calculate_path -> graph         = graph;
    calculate_path -> target        = &(coin -> character).position;

    Action *none = new Action();
    cout << "Acciones de transiciones" << endl;

    /*********************** CONDICIONES ****************************/
    BoolCondition *check_coin = new BoolCondition();
    NotCondition  *not_coin   = new NotCondition();
    check_coin -> condition = &(coin -> exists);
    not_coin ->   condition = check_coin;

    cout << "Condiciones" << endl;

    /******************************* MAQUINA DE ESTADOS ************/
    (competitor -> state_machine).states.resize(2);
    State &find_coin = (competitor -> state_machine).states[0];
    State &seek_player = (competitor -> state_machine).states[1];

    (competitor -> state_machine).initial_state = &seek_player;
    (competitor -> state_machine).current_state = &seek_player;


    /************************** ESTADOS *******************************/
    find_coin.action    = finding_coin;
    seek_player.action  = seeking_player;
    find_coin.transitions.push_back({&seek_player, not_coin, none});
    seek_player.transitions.push_back({&find_coin, check_coin, calculate_path});
    cout << "maquina de estados" << endl;

    /******************* OBJETO DIBUJABLE **************************/
    int size;
    file >> size;
    DrawableObject* drawable_agent;
    drawable_agent = new DrawableAgent(competitor->character, Color::Magenta, size);
    drawable_agents.push_back(drawable_agent);
}